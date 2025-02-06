#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "BehaviorTreeNode.h"
#include "ActionNode.h"
#include "Demon_Dying.h"
#include "Character.h"
#include "Enemy.h"
#include "Demon.h"
#include "EnemyManager.h"
#include "DemonBehaviorTree.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Demon_Dying::Demon_Dying()
	: isInitialize		(false)
{
	auto& json = Singleton<JsonManager>::GetInstance();
	this->animationType		= static_cast<int>(Demon::AnimationType::DYING);
	this->animationPlayTime = json.GetJson(JsonManager::FileType::DEMON_PANDA)["ANIMATION_PLAY_TIME"][this->animationType];
	this->actionType		= static_cast<short>(DemonBehaviorTree::ActionType::DYING);
	this->maxSpeed			= 0.0f;
	this->accel				= json.GetJson(JsonManager::FileType::DEMON_PANDA)["ACCEL"];
	this->decel				= json.GetJson(JsonManager::FileType::DEMON_PANDA)["DECEL"];
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Demon_Dying::~Demon_Dying()
{

}

/// <summary>
/// ������
/// </summary>
void Demon_Dying::Initialize()
{
	this->isInitialize = false;
}

/// <summary>
/// �X�V����
/// </summary>
Demon_Dying::NodeState Demon_Dying::Update()
{
	auto& enemyManager = Singleton<EnemyManager>::GetInstance();
	auto& enemy = dynamic_cast<Demon&>(enemyManager.GetCharacter());

	/*�����t���O������Ă�����ȉ��̏����͍s��Ȃ�*/
	if (!enemy.GetIsAlive())return ActionNode::NodeState::SUCCESS;

	auto& rootNode = Singleton<DemonBehaviorTree>::GetInstance();
	/*�o�^����Ă���A�N�V�����Ǝ��ۂ̃A�N�V�������قȂ��Ă�����*/
	if (!this->isInitialize)
	{
		//�A�j���[�V�����̎�ނ�ݒ�
		enemy.SetNowAnimation(this->animationType);
		//�A�j���[�V�����Đ����Ԃ̐ݒ�
		enemy.SetAnimationPlayTime(this->animationPlayTime);
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
		enemy.OffIsAlive();
		return ActionNode::NodeState::SUCCESS;
	}
	//����ȊO�͎��s����Ԃ�
	else
	{
		return ActionNode::NodeState::RUNNING;
	}
}
