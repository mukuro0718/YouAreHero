#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "Character.h"
#include "BehaviorTreeNode.h"
#include "BehaviorTree.h"
#include "ActionNode.h"
#include "Beast_TurnLeft.h"
#include "Enemy.h"
#include "Beast.h"
#include "EnemyManager.h"
#include "BeastBehaviorTree.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Beast_TurnLeft::Beast_TurnLeft()
	: frameCount	(0)
	, maxFrameCount	(0)
{
	auto& json = Singleton<JsonManager>::GetInstance();
	this->animationPlayTime = json.GetJson(JsonManager::FileType::BEAST)["ANIMATION_PLAY_TIME"][this->animationType];
	this->animationType		= static_cast<int>(Beast::AnimationType::TURN_LEFT);
	this->actionType		= static_cast<int>(BeastBehaviorTree::ActionType::TURN_LEFT);
	this->interval			= json.GetJson(JsonManager::FileType::BEAST)["ACTION_INTERVAL"][this->actionType];
	this->maxSpeed			= 0.0f;
	this->accel				= json.GetJson(JsonManager::FileType::BEAST)["ACCEL"];
	this->decel				= json.GetJson(JsonManager::FileType::BEAST)["DECEL"];
	this->maxFrameCount		= json.GetJson(JsonManager::FileType::BEAST)["TURN_WAIT_COUNT"];

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Beast_TurnLeft::~Beast_TurnLeft()
{

}

/// <summary>
/// �X�V����
/// </summary>
Beast_TurnLeft::NodeState Beast_TurnLeft::Update(BehaviorTree& _tree, Character& _chara)
{
	auto& enemy = dynamic_cast<Beast&>(_chara);

	/*�o�^����Ă���A�N�V�����Ǝ��ۂ̃A�N�V�������قȂ��Ă�����*/
	if (_tree.GetNowSelectAction() != this->actionType)
	{
		//�A�j���[�V�����̎�ނ�ݒ�
		enemy.SetNowAnimation(this->animationType);
		//�A�j���[�V�����Đ����Ԃ̐ݒ�
		enemy.SetAnimationPlayTime(this->animationPlayTime);
		//�A�N�V�����̐ݒ�
		_tree.SetNowSelectAction(this->actionType);
		//������Root�ɓo�^
		_tree.EntryCurrentBattleAction(*this);
	}

	/*�A�j���[�V�����̍Đ�*/
	enemy.PlayAnimation();

	/*�t���[���J�E���g���ő�ȏ�łȂ���Α������^�[��*/
	this->frameCount++;
	if (this->frameCount < this->maxFrameCount)return ActionNode::NodeState::RUNNING;


	/*�ړ�*/
	enemy.Move(this->maxSpeed, this->accel, this->decel, false);

	/*��Ԃ�Ԃ�*/
	//�A�j���[�V�������I�����Ă�����
	if (enemy.GetIsChangeAnimation())
	{
		//�o�^������
		_tree.ExitCurrentBattleAction();
		this->frameCount = 0;
		return ActionNode::NodeState::SUCCESS;
	}
	//����ȊO�͎��s����Ԃ�
	else
	{
		return ActionNode::NodeState::RUNNING;
	}
}
