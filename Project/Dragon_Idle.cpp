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
	: RAMPAGE_STANDBY_TIME(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::DRAGON)["RAMPAGE_STANDBY_TIME"])
	, FURY_STANDBY_TIME(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::DRAGON)["FURY_STANDBY_TIME"])
	, AWAKENING_STANDBY_TIME(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::DRAGON)["AWAKENING_STANDBY_TIME"])
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
	}

	/*�A�N�V�����̏�Ԃ��Z�b�g*/
	if (prevAction != this->actionType)
	{
		rootNode.SetCurrentAction(this->actionType);
		enemy.ChangeTiredColor();
		switch (rootNode.GetDragonStage())
		{
		case static_cast<short>(DragonBehaviorTree::DragonStage::AWAKENING):
			this->standbyTime = this->AWAKENING_STANDBY_TIME;
			break;
		case static_cast<short>(DragonBehaviorTree::DragonStage::FURY):
			this->standbyTime = this->FURY_STANDBY_TIME;
			break;
		case static_cast<short>(DragonBehaviorTree::DragonStage::RAMPAGE):
			this->standbyTime = this->RAMPAGE_STANDBY_TIME;
			break;
		default:
			break;
		}
		rootNode.SetDragonState(DragonBehaviorTree::DragonState::TIRED);
	}

	/*�A�j���[�V�����̍Đ�*/
	enemy.PlayAnimation(this->animationType, this->animationPlayTime);

	/*�ҋ@���Ԃ��v��*/
	this->currentStandbyTime++;

	/*��Ԃ�Ԃ�*/
	//�w��̑ҋ@���Ԃ𒴂��Ă�����SUCCESS��Ԃ�
	if (this->currentStandbyTime >= this->standbyTime)
	{
		rootNode.SetDragonState(DragonBehaviorTree::DragonState::NORMAL);
		switch (rootNode.GetDragonStage())
		{
		case static_cast<short>(DragonBehaviorTree::DragonStage::RAMPAGE):
			enemy.ChangeAngryColor();
			this->standbyTime = this->RAMPAGE_STANDBY_TIME;
			break;
		default:
			enemy.ChangeNormalColor();
			break;
		}
		rootNode.SetAttackCount();
		this->currentStandbyTime = 0;
		return ActionNode::NodeState::SUCCESS;
	}
	return ActionNode::NodeState::RUNNING;
}
