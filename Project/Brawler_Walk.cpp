#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "Character.h"
#include "BehaviorTreeNode.h"
#include "BehaviorTree.h"
#include "ActionNode.h"
#include "Brawler_Walk.h"
#include "Enemy.h"
#include "BrawlerEnemy.h"
#include "EnemyManager.h"
#include "BrawlerEnemyBehaviorTree.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Brawler_Walk::Brawler_Walk()
	: frameCount(0)
	, walkSoundCount(0)
{
	auto& json = Singleton<JsonManager>::GetInstance();
	this->animationType		= static_cast<int>(BrawlerEnemy::AnimationType::WALK_FRONT);
	this->animationPlayTime = json.GetJson(JsonManager::FileType::BRAWLER_ENEMY)["ANIMATION_PLAY_TIME"][this->animationType];
	this->maxSpeed			= json.GetJson(JsonManager::FileType::BRAWLER_ENEMY)["WALK_SPEED"];;
	this->accel				= json.GetJson(JsonManager::FileType::BRAWLER_ENEMY)["ACCEL"];
	this->walkSoundCount	= json.GetJson(JsonManager::FileType::BRAWLER_ENEMY)["WALK_SOUND_COUNT"];
	this->decel				= json.GetJson(JsonManager::FileType::BRAWLER_ENEMY)["DECEL"];
	this->actionType		= static_cast<short>(BrawlerEnemyBehaviorTree::ActionType::WALK);
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Brawler_Walk::~Brawler_Walk()
{

}

/// <summary>
/// �X�V����
/// </summary>
Brawler_Walk::NodeState Brawler_Walk::Update(BehaviorTree& _tree, Character& _chara)
{
	auto& enemy = dynamic_cast<BrawlerEnemy&>(_chara);

	/*�����t���O������Ă�����ȉ��̏����͍s��Ȃ�*/
	if (!enemy.GetIsAlive())return ActionNode::NodeState::SUCCESS;

	//auto& sound = Singleton<SoundManager>::GetInstance();
	//this->frameCount++;
	//if (this->frameCount == this->walkSoundCount)
	//{
	//	sound.OnIsPlayEffect(SoundManager::EffectType::MONSTER_FOOTSTEPS);
	//	this->frameCount = 0;
	//}

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
