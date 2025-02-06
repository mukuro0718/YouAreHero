#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "BehaviorTreeNode.h"
#include "ActionNode.h"
#include "Beast_Down.h"
#include "Character.h"
#include "Enemy.h"
#include "Beast.h"
#include "EnemyManager.h"
#include "BeastBehaviorTree.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Beast_Down::Beast_Down()
	: isInitialize(false)
{
	this->animationSet.emplace(AnimationStage::START, static_cast<int>(Beast::AnimationType::SHORT_FRIGHT_START));
	this->animationSet.emplace(AnimationStage::LOOP, static_cast<int>(Beast::AnimationType::SHORT_FRIGHT_LOOP));
	this->animationSet.emplace(AnimationStage::END, static_cast<int>(Beast::AnimationType::SHORT_FRIGHT_END));
	this->nextStageSet.emplace(AnimationStage::START, AnimationStage::LOOP);
	this->nextStageSet.emplace(AnimationStage::LOOP, AnimationStage::END);
	this->nextStageSet.emplace(AnimationStage::END, AnimationStage::START);

	auto& json = Singleton<JsonManager>::GetInstance();
	this->animationPlayTime = json.GetJson(JsonManager::FileType::BEAST)["ANIMATION_PLAY_TIME"][this->animationType];
	this->actionType		= static_cast<short>(BeastBehaviorTree::ActionType::DOWN);
	this->maxSpeed			= 0.0f;
	this->accel				= json.GetJson(JsonManager::FileType::BEAST)["ACCEL"];
	this->decel				= json.GetJson(JsonManager::FileType::BEAST)["DECEL"];

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Beast_Down::~Beast_Down()
{

}

/// <summary>
/// �X�V����
/// </summary>
Beast_Down::NodeState Beast_Down::Update()
{
	auto& rootNode = Singleton<BeastBehaviorTree>::GetInstance();
	auto& enemyManager = Singleton<EnemyManager>::GetInstance();
	auto& enemy = dynamic_cast<Beast&>(enemyManager.GetCharacter());

	/*�o�^����Ă���A�N�V�����Ǝ��ۂ̃A�N�V�������قȂ��Ă�����*/
	if (rootNode.GetNowSelectAction() != this->actionType)
	{
		//�A�N�V�����̐ݒ�
		rootNode.SetSelectAction(this->actionType);
		//�A�N�V�����̓o�^
		rootNode.EntryCurrentReaction(*this);
		enemy.ChangeTiredColor();
		enemy.UpdateSpeed(this->maxSpeed, this->accel, this->decel);
		enemy.UpdateVelocity(false);
	}

	/*�A�j���[�V����*/
	//�A�j���[�V�����̎�ނ�ݒ�
	int nowAnimationType = this->animationSet[this->stage];
	if (this->animationType != nowAnimationType)
	{
		//�Đ�����A�j���[�V������ݒ�
		this->animationType = nowAnimationType;
		enemy.SetNowAnimation(this->animationType);
		//�A�j���[�V�����Đ����Ԃ̐ݒ�
		auto& json = Singleton<JsonManager>::GetInstance();
		enemy.SetAnimationPlayTime(json.GetJson(JsonManager::FileType::BEAST)["ANIMATION_PLAY_TIME"][this->animationType]);
	}
	//�A�j���[�V�����̍Đ�
	enemy.PlayAnimation();

	/*�ړ�*/
	if (enemy.GetSpeed() != 0.0f)
	{
		enemy.UpdateSpeed(this->maxSpeed, this->accel, this->decel);
		enemy.UpdateVelocity(false);
	}

	/*��Ԃ�Ԃ�*/
	if (enemy.GetIsChangeAnimation())
	{
		/*��Ԃ����[�v�̎��Ƀ_�E���l���c���Ă�����stage��ς��Ȃ�*/
		if (this->stage == AnimationStage::LOOP && rootNode.GetDownValue() > 0)
		{
			return ActionNode::NodeState::RUNNING;
		}
		this->stage = this->nextStageSet[this->stage];
		if (this->stage == AnimationStage::START)
		{
			//�A�N�V�����̉���
			rootNode.ExitCurrentReaction();
			//��Ԃ�ʏ�ɖ߂�
			rootNode.SetBeastState(BeastBehaviorTree::BeastState::NORMAL);
			rootNode.ExitCurrentBattleAction();
			enemy.ChangeNormalColor();
			return ActionNode::NodeState::SUCCESS;
		}
		return ActionNode::NodeState::RUNNING;
	}
	return ActionNode::NodeState::RUNNING;
}
