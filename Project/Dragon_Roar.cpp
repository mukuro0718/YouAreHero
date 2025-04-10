#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "Character.h"
#include "BehaviorTreeNode.h"
#include "BehaviorTree.h"
#include "ActionNode.h"
#include "Dragon_Roar.h"
#include "Enemy.h"
#include "Dragon.h"
#include "EnemyManager.h"
#include "DragonBehaviorTree.h"
#include "SoundManager.h"
#include "EffectManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Dragon_Roar::Dragon_Roar()
	: frameCount(0)
	, roarFrame (0)
{
	auto& json = Singleton<JsonManager>::GetInstance();
	this->animationType		= static_cast<int>(Dragon::AnimationType::ROAR);
	this->animationPlayTime	= json.GetJson(JsonManager::FileType::DRAGON)["ANIMATION_PLAY_TIME"][this->animationType];
	this->actionType		= static_cast<short>(DragonBehaviorTree::ActionType::ROAR);
	this->maxSpeed			= 0.0f;
	this->accel				= json.GetJson(JsonManager::FileType::DRAGON)["ACCEL"];
	this->decel				= json.GetJson(JsonManager::FileType::DRAGON)["DECEL"];
	this->roarFrame			= json.GetJson(JsonManager::FileType::DRAGON)["ROAR_SOUND_COUNT"];
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Dragon_Roar::~Dragon_Roar()
{

}

/// <summary>
/// �X�V����
/// </summary>
Dragon_Roar::NodeState Dragon_Roar::Update(BehaviorTree& _tree, Character& _chara)
{
	/*���K��炷*/
	this->frameCount++;
	if (this->frameCount == this->roarFrame)
	{
		//����炷
		auto& sound = Singleton<SoundManager>::GetInstance();
		sound.OnIsPlayEffect(SoundManager::EffectType::MONSTER_ROAR);
		//�G�t�F�N�g�̍Đ�
		//auto& effect = Singleton<EffectManager>  ::GetInstance();
		//effect.OnIsEffect(EffectManager::EffectType::BOSS_ROAR);
	}

	/*���x���O�ȏ�܂��͍ŏ��ɂ��̃A�N�V�����ɂȂ������ړ��������s��*/
	auto& enemy = dynamic_cast<Dragon&>(_chara);
	int prevAction = _tree.GetNowSelectAction();
	if (enemy.GetSpeed() != 0.0f || prevAction != this->actionType)
	{
		enemy.UpdateSpeed(this->maxSpeed, this->accel, this->decel);
		enemy.UpdateVelocity(false);
	}

	/*�A�N�V�����̏�Ԃ��Z�b�g*/
	if (prevAction != this->actionType)
	{
		//�A�N�V�����̐ݒ�
		_tree.SetNowSelectAction(this->actionType);
		//�A�N�V�����̓o�^
		_tree.EntryCurrentBattleAction(*this);
	}

	/*�A�j���[�V�����̍Đ�*/
	enemy.PlayAnimation(this->animationType, this->animationPlayTime);

	/*��Ԃ�Ԃ�*/
	//�A�j���[�V�������I�����Ă�����
	if (enemy.GetIsChangeAnimation())
	{
		//�C���^�[�o���̐ݒ�
		_tree.ExitCurrentBattleAction();
		_tree.SetInterval(this->actionType);
		this->frameCount = 0;
		return ActionNode::NodeState::SUCCESS;
	}
	//����ȊO�͎��s����Ԃ�
	else
	{
		return ActionNode::NodeState::RUNNING;
	}
}
