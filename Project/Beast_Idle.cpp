#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "Character.h"
#include "BehaviorTreeNode.h"
#include "BehaviorTree.h"
#include "ActionNode.h"
#include "Beast_Idle.h"
#include "Enemy.h"
#include "Beast.h"
#include "EnemyManager.h"
#include "BeastBehaviorTree.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Beast_Idle::Beast_Idle()
{
	auto& json = Singleton<JsonManager>::GetInstance();
	this->animationType		= static_cast<int>(Beast::AnimationType::IDLE);
	this->animationPlayTime	= json.GetJson(JsonManager::FileType::BEAST)["ANIMATION_PLAY_TIME"][this->animationType];
	this->actionType		= static_cast<short>(BeastBehaviorTree::ActionType::IDLE);
	this->maxSpeed			= 0.0f;
	this->accel				= json.GetJson(JsonManager::FileType::BEAST)["ACCEL"];
	this->decel				= json.GetJson(JsonManager::FileType::BEAST)["DECEL"];
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Beast_Idle::~Beast_Idle()
{

}

/// <summary>
/// �X�V����
/// </summary>
Beast_Idle::NodeState Beast_Idle::Update(BehaviorTree& _tree, Character& _chara)
{
	/*�A�j���[�V����*/
	auto& enemy = dynamic_cast<Beast&>(_chara);
	/*�A�N�V�����̏�Ԃ��Z�b�g*/
	if (_tree.GetNowSelectAction() != this->actionType)
	{
		_tree.SetNowSelectAction(this->actionType);
		//�A�j���[�V�����̎�ނ�ݒ�
		enemy.SetNowAnimation(this->animationType);
		//�A�j���[�V�����Đ����Ԃ̐ݒ�
		enemy.SetAnimationPlayTime(this->animationPlayTime);
		_tree.EntryCurrentBattleAction(*this);
	}

	/*�A�j���[�V�����̍Đ�*/
	enemy.PlayAnimation();

	/*�ړ�*/
	if (enemy.GetSpeed() != 0.0f)
	{
		enemy.UpdateSpeed(this->maxSpeed, this->accel, this->decel);
		enemy.UpdateVelocity(false);

	}

	/*��Ԃ�Ԃ�*/
	//�A�j���[�V�������I�����Ă�����
	if (enemy.GetIsChangeAnimation())
	{
		//�A�N�V�����̉���
		_tree.ExitCurrentBattleAction();
		return ActionNode::NodeState::SUCCESS;
	}
	//����ȊO�͎��s����Ԃ�
	else
	{
		return ActionNode::NodeState::RUNNING;
	}
}
