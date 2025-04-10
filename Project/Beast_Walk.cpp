#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "Character.h"
#include "BehaviorTreeNode.h"
#include "BehaviorTree.h"
#include "ActionNode.h"
#include "Beast_Walk.h"
#include "Enemy.h"
#include "Beast.h"
#include "EnemyManager.h"
#include "BeastBehaviorTree.h"
#include "SoundManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Beast_Walk::Beast_Walk()
	: frameCount	(0)
	, walkSoundCount(0)
{
	auto& json = Singleton<JsonManager>::GetInstance();
	this->animationType		= static_cast<int>(Beast::AnimationType::WALK);
	this->animationPlayTime = json.GetJson(JsonManager::FileType::BEAST)["ANIMATION_PLAY_TIME"][this->animationType];
	this->maxSpeed			= json.GetJson(JsonManager::FileType::BEAST)["WALK_SPEED"];
	this->accel				= json.GetJson(JsonManager::FileType::BEAST)["ACCEL"];
	this->decel				= json.GetJson(JsonManager::FileType::BEAST)["DECEL"];
	this->walkSoundCount	= json.GetJson(JsonManager::FileType::BEAST)["WALK_SOUND_COUNT"];
	this->actionType		= static_cast<short>(BeastBehaviorTree::ActionType::WALK);
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Beast_Walk::~Beast_Walk()
{

}

/// <summary>
/// �X�V����
/// </summary>
Beast_Walk::NodeState Beast_Walk::Update(BehaviorTree& _tree, Character& _chara)
{
	auto& enemy = dynamic_cast<Beast&>(_chara);

	auto& sound = Singleton<SoundManager>::GetInstance();
	this->frameCount++;
	if (this->frameCount == this->walkSoundCount)
	{
		sound.OnIsPlayEffect(SoundManager::EffectType::MONSTER_FOOTSTEPS);
		this->frameCount = 0;
	}


	/*�o�^����Ă���A�N�V�����Ǝ��ۂ̃A�N�V�������قȂ��Ă�����*/
	if (_tree.GetNowSelectAction() != this->actionType)
	{
		//�A�j���[�V�����̎�ނ�ݒ�
		enemy.SetNowAnimation(this->animationType);
		//�A�j���[�V�����Đ����Ԃ̐ݒ�
		enemy.SetAnimationPlayTime(this->animationPlayTime);
		//�A�N�V�����̐ݒ�
		_tree.SetNowSelectAction(this->actionType);
		this->frameCount = 0;
	}

	/*�A�j���[�V�����̍Đ�*/
	enemy.PlayAnimation();

	/*�ړ�*/
	enemy.Move(this->maxSpeed, this->accel, this->decel, false);

	/*��Ԃ�Ԃ�*/
	{
		return ActionNode::NodeState::SUCCESS;
	}
}
