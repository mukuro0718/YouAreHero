#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "BehaviorTreeNode.h"
#include "ActionNode.h"
#include "Beast_Roar.h"
#include "Character.h"
#include "Enemy.h"
#include "Beast.h"
#include "EnemyManager.h"
#include "BeastBehaviorTree.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Beast_Roar::Beast_Roar()
	: isInitialize(false)
{
	auto& json = Singleton<JsonManager>::GetInstance();
	this->animationType		= static_cast<int>(Beast::AnimationType::ROAR);
	this->animationPlayTime = json.GetJson(JsonManager::FileType::BEAST)["ANIMATION_PLAY_TIME"][this->animationType];
	this->actionType		= static_cast<short>(BeastBehaviorTree::ActionType::ROAR);
	this->maxSpeed			= 0.0f;
	this->accel				= json.GetJson(JsonManager::FileType::BEAST)["ACCEL"];
	this->decel				= json.GetJson(JsonManager::FileType::BEAST)["DECEL"];

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Beast_Roar::~Beast_Roar()
{

}

/// <summary>
/// �X�V����
/// </summary>
Beast_Roar::NodeState Beast_Roar::Update()
{
	/*�A�j���[�V����*/
	auto& enemyManager = Singleton<EnemyManager>::GetInstance();
	auto& enemy = dynamic_cast<Beast&>(enemyManager.GetCharacter());
	//����������Ă��Ȃ����
	if (!this->isInitialize)
	{
		//�A�j���[�V�����̎�ނ�ݒ�
		enemy.SetNowAnimation(this->animationType);
		//�A�j���[�V�����Đ����Ԃ̐ݒ�
		enemy.SetAnimationPlayTime(this->animationPlayTime);
		//�A�N�V�����̏�Ԃ��Z�b�g
		auto& rootNode = Singleton<BeastBehaviorTree>::GetInstance();
		rootNode.SetSelectAction(this->actionType);
		this->isInitialize = true;
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
	{
		//�A�j���[�V�������I�����Ă�����
		if (enemy.GetIsChangeAnimation())
		{
			auto& rootNode = Singleton<BeastBehaviorTree>::GetInstance();
			rootNode.SetBeastState(BeastBehaviorTree::BeastState::ANGRY);
			this->isInitialize = false;
			return ActionNode::NodeState::SUCCESS;
		}
		//����ȊO�͎��s����Ԃ�
		else
		{
			return ActionNode::NodeState::RUNNING;
		}
	}
}