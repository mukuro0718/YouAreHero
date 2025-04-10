#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "Character.h"
#include "BehaviorTreeNode.h"
#include "BehaviorTree.h"
#include "ActionNode.h"
#include "Demon_Idle.h"
#include "Enemy.h"
#include "Demon.h"
#include "EnemyManager.h"
#include "DemonBehaviorTree.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Demon_Idle::Demon_Idle()
	: isInitialize		(false)
{
	auto& json = Singleton<JsonManager>::GetInstance();
	this->animationType		= static_cast<int>(Demon::AnimationType::IDLE);
	this->animationPlayTime = json.GetJson(JsonManager::FileType::DEMON_PANDA)["ANIMATION_PLAY_TIME"][this->animationType];
	this->actionType		= static_cast<short>(DemonBehaviorTree::ActionType::IDLE);
	this->maxSpeed			= 0.0f;
	this->accel				= json.GetJson(JsonManager::FileType::DEMON_PANDA)["ACCEL"];
	this->decel				= json.GetJson(JsonManager::FileType::DEMON_PANDA)["DECEL"];
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Demon_Idle::~Demon_Idle()
{

}

/// <summary>
/// ������
/// </summary>
void Demon_Idle::Initialize()
{
	this->isInitialize = false;
}

/// <summary>
/// �X�V����
/// </summary>
Demon_Idle::NodeState Demon_Idle::Update(BehaviorTree& _tree, Character& _chara)
{
	auto& enemy = dynamic_cast<Demon&>(_chara);

	if (!this->isInitialize)
	{
		/*�o�^����Ă���A�N�V�����Ǝ��ۂ̃A�N�V�������قȂ��Ă�����*/
		//�A�j���[�V�����̎�ނ�ݒ�
		enemy.SetNowAnimation(this->animationType);
		//�A�j���[�V�����Đ����Ԃ̐ݒ�
		enemy.SetAnimationPlayTime(this->animationPlayTime);
		enemy.UpdateSpeed(this->maxSpeed, this->accel, this->decel);
		enemy.UpdateVelocity(false);
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
		return ActionNode::NodeState::SUCCESS;
	}
	//����ȊO�͎��s����Ԃ�
	else
	{
		return ActionNode::NodeState::RUNNING;
	}
}
