#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "BehaviorTreeNode.h"
#include "ActionNode.h"
#include "Dragon_Idle.h"
#include "Character.h"
#include "Enemy.h"
#include "Dragon.h"
#include "EnemyManager.h"
#include "DragonBehaviorTree.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Dragon_Idle::Dragon_Idle()
	: STANDBY_TIME		(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::DRAGON)["STANDBY_TIME"])
	, currentStandbyTime(0)
{
	auto& json = Singleton<JsonManager>::GetInstance();
	this->animationType		= static_cast<int>(Dragon::AnimationType::IDLE);
	this->animationPlayTime	= json.GetJson(JsonManager::FileType::DRAGON)["ANIMATION_PLAY_TIME"][this->animationType];
	this->actionType		= static_cast<short>(DragonBehaviorTree::ActionType::IDLE);
	this->maxSpeed			= 0.0f;
	this->accel				= json.GetJson(JsonManager::FileType::DRAGON)["ACCEL"];
	this->decel				= json.GetJson(JsonManager::FileType::DRAGON)["DECEL"];
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Dragon_Idle::~Dragon_Idle()
{

}

/// <summary>
/// �X�V����
/// </summary>
Dragon_Idle::NodeState Dragon_Idle::Update()
{
	/*���x���O�ȏ�܂��͍ŏ��ɂ��̃A�N�V�����ɂȂ������ړ��������s��*/
	auto& enemyManager = Singleton<EnemyManager>::GetInstance();
	auto& enemy = dynamic_cast<Dragon&>(enemyManager.GetCharacter());
	auto& rootNode = Singleton<DragonBehaviorTree>::GetInstance();
	int prevAction = rootNode.GetCurrentAction();
	if (enemy.GetSpeed() != 0.0f || prevAction != this->actionType)
	{
		enemy.UpdateSpeed(this->maxSpeed, this->accel, this->decel);
		enemy.UpdateVelocity(false);
		rootNode.SetCurrentAction(this->actionType);
		this->standbyTime = this->STANDBY_TIME;
	}

	/*�A�j���[�V�����̍Đ�*/
	enemy.PlayAnimation(this->animationType, this->animationPlayTime);

	/*�ҋ@���Ԃ��v��*/
	this->currentStandbyTime++;

	/*��Ԃ�Ԃ�*/
	//�w��̑ҋ@���Ԃ𒴂��Ă�����SUCCESS��Ԃ�
	if (this->currentStandbyTime >= this->standbyTime)
	{
		this->currentStandbyTime = 0;
		enemy.SetAttackCount();
		return ActionNode::NodeState::SUCCESS;
	}
	return ActionNode::NodeState::RUNNING;
}
