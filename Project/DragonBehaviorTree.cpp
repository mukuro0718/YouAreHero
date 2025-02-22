#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "DragonBehaviorTreeHeader.h"
#include "DeleteInstance.h"
#include "Rigidbody.h"
#include "EnemyManager.h"
#include "Character.h"
#include "Enemy.h"
#include "Dragon.h"
#include "Player.h"
#include "PlayerManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
DragonBehaviorTree::DragonBehaviorTree()
	: Selector_DyingOrBattleOrIdle	 (nullptr)
	, debugActionNode				 (nullptr)
	, currentBattleAction			 (nullptr)
	, prevNodeState					 (BehaviorTreeNode::NodeState::NONE_ACTIVE)
	, toTargetDistance				 (0.0f)
	, innerProductOfDirectionToTarget(0.0f)
	, currentAction					 (0)
	, attackCount					 (0)
	, isSelectedBattleAction		 (false)
{

	this->debugActionNode = new Dragon_Sweep();

	/*������*/
	Initialize();
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
DragonBehaviorTree::~DragonBehaviorTree()
{
	DeleteMemberInstance(this->Selector_DyingOrBattleOrIdle);
	DeleteMemberInstance(this->debugActionNode);
	DeleteMemberInstance(this->currentBattleAction);
}

/// <summary>
/// ������
/// </summary>
void DragonBehaviorTree::Initialize()
{
	CreateBehaviorTree();
	this->prevNodeState					  = BehaviorTreeNode::NodeState::NONE_ACTIVE;
	this->toTargetDistance				  = 0.0f;
	this->innerProductOfDirectionToTarget = 0.0f;
	this->currentAction					  = -1;
	this->attackCount					  = 0;
	this->isSelectedBattleAction		  = false;
	this->isCancelAction				  = false;
	this->currentBattleAction			  = nullptr;
	/*if (this->currentBattleAction != nullptr)
	{
		this->currentBattleAction->Initialize();
	}*/
	SetInterval(static_cast<int>(DragonBehaviorTree::ActionType::ROAR), 1);
}

/// <summary>
/// �r�w�C�r�A�c���[���쐬
/// </summary>
void DragonBehaviorTree::CreateBehaviorTree()
{
	DeleteMemberInstance(this->Selector_DyingOrBattleOrIdle);
	/*�匳�̃c���[�̍쐬*/
	this->Selector_DyingOrBattleOrIdle = new SelectorNode();

	auto& json = Singleton<JsonManager>::GetInstance();
	/*�f�X�m�[�h*/
	BehaviorTreeNode* Sequencer_DyingIfHpIsLessThanZero = new SequencerNode();
	{
		Sequencer_DyingIfHpIsLessThanZero->AddChild(*new Condition_IsDragonHpIsLessThanConstant(0));
		Sequencer_DyingIfHpIsLessThanZero->AddChild(*new Dragon_Dying());
	}

	/*�o�g���A�N�V�����T�u�c���[*/
	BehaviorTreeNode* Sequencer_BattleActionIfAttackCountLeft = new SequencerNode();
	//�A�N�V�����I���T�u�c���[
	{
		//�������U���T�u�c���[
		BehaviorTreeNode* Sequencer_LongRangeAttackIfToTargetOutOfRange = new SequencerNode();
		{
			BehaviorTreeNode* Selector_LongRangeAttack = new SelectorNode();
			//�������U���͈͊O�U���T�u�c���[
			BehaviorTreeNode* Sequencer_OutOfLongRangeActionIfToTargetOutOfRange = new SequencerNode();
			Sequencer_OutOfLongRangeActionIfToTargetOutOfRange->AddChild(*new Condition_IsDragonToTargetDistanceGreaterThanConstant(json.GetJson(JsonManager::FileType::DRAGON)["LONG_ATTACK_RANGE"]));
			Sequencer_OutOfLongRangeActionIfToTargetOutOfRange->AddChild(*new Dragon_Walk());
			Selector_LongRangeAttack->AddChild(*Sequencer_OutOfLongRangeActionIfToTargetOutOfRange);
			//�{���Ԏ��������U���T�u�c���[
			BehaviorTreeNode* Sequencer_AngryLongRangeAttackIfBossStateAngry = new SequencerNode();
			Sequencer_AngryLongRangeAttackIfBossStateAngry->AddChild(*new Condition_IsDragonStateIsSameAsTheSpecifiedState(static_cast<int>(Enemy::BossState::ANGRY)));
			Sequencer_AngryLongRangeAttackIfBossStateAngry->AddChild(*new Dragon_Breath());
			Selector_LongRangeAttack->AddChild(*Sequencer_AngryLongRangeAttackIfBossStateAngry);
			//�ʏ펞�������U���A�N�V����
			Selector_LongRangeAttack->AddChild(*new Dragon_Walk());
			//�������U���T�u�c���[�ɒǉ�
			Sequencer_LongRangeAttackIfToTargetOutOfRange->AddChild(*new Condition_IsDragonToTargetDistanceGreaterThanConstant(json.GetJson(JsonManager::FileType::DRAGON)["NEAR_ATTACK_RANGE"]));
			Sequencer_LongRangeAttackIfToTargetOutOfRange->AddChild(*Selector_LongRangeAttack);
		}
		//�͈͍U��
		BehaviorTreeNode* Sequencer_RangeAttackIfTargetOutOfRay = new SequencerNode();
		{
			Sequencer_RangeAttackIfTargetOutOfRay->AddChild(*new Condition_IsDragonStateIsSameAsTheSpecifiedState(static_cast<int>(Enemy::BossState::ANGRY)));
			Sequencer_RangeAttackIfTargetOutOfRay->AddChild(*new Condition_IsDragonActionIntervalIsOver(static_cast<int>(ActionType::ROTATE)));
			Sequencer_RangeAttackIfTargetOutOfRay->AddChild(*new Condition_IsTargetOutOfRangeOfDragonRay(json.GetJson(JsonManager::FileType::DRAGON)["TOLERANCE_RANGE"][0], json.GetJson(JsonManager::FileType::DRAGON)["TOLERANCE_RANGE"][1]));
			Sequencer_RangeAttackIfTargetOutOfRay->AddChild(*new Dragon_Rotate());
		}
		//����ȊO��������ߐڍU���������_���őI��
		BehaviorTreeNode* Selector_RandomAttackAction = new RandomSelector();
		{
			Selector_RandomAttackAction->AddChild(*new Dragon_Smash());	//�@����
			Selector_RandomAttackAction->AddChild(*new Dragon_Sweep());	//�Ȃ�����
			Selector_RandomAttackAction->AddChild(*new Dragon_Rotate());	//��]�U��
		}
		BehaviorTreeNode* Selector_BattleAction = new SelectorNode();
		Selector_BattleAction->AddChild(*Sequencer_LongRangeAttackIfToTargetOutOfRange);
		Selector_BattleAction->AddChild(*Sequencer_RangeAttackIfTargetOutOfRay);
		Selector_BattleAction->AddChild(*Selector_RandomAttackAction);

		//�o�g���T�u�c���[�ɏ����ƃA�N�V�����I���T�u�c���[��ǉ�
		Sequencer_BattleActionIfAttackCountLeft->AddChild(*new Condition_IsDragonAttackCountLeft());
		Sequencer_BattleActionIfAttackCountLeft->AddChild(*Selector_BattleAction);
	}

	/*�A�N�V�������I������Ă��邩*/
	BehaviorTreeNode* Sequencer_PlayCurrentIfSelectedBattleAction = new SequencerNode();
	{
		Sequencer_PlayCurrentIfSelectedBattleAction->AddChild(*new Condition_IsSelectedDragonBattleAction());
		Sequencer_PlayCurrentIfSelectedBattleAction->AddChild(*new Dragon_PlayCurrentBattleAction());
	}

	/*���K�A�N�V����*/
	SequencerNode* RoarIfIntervalIsOver = new SequencerNode();
	{
		RoarIfIntervalIsOver->AddChild(*new Condition_IsDragonActionIntervalIsOver(static_cast<int>(ActionType::ROAR)));
		RoarIfIntervalIsOver->AddChild(*new Dragon_Roar());
	}

	/*���[�g�m�[�h�Ɋe�A�N�V�����c���[��}��*/
	this->Selector_DyingOrBattleOrIdle->AddChild(*Sequencer_DyingIfHpIsLessThanZero);
	this->Selector_DyingOrBattleOrIdle->AddChild(*Sequencer_PlayCurrentIfSelectedBattleAction);
	this->Selector_DyingOrBattleOrIdle->AddChild(*RoarIfIntervalIsOver);
	this->Selector_DyingOrBattleOrIdle->AddChild(*Sequencer_BattleActionIfAttackCountLeft);
	this->Selector_DyingOrBattleOrIdle->AddChild(*new Dragon_Idle());
}

/// <summary>
/// �����o�ϐ��̍X�V
/// </summary>
void DragonBehaviorTree::UpdateMemberVariables()
{
	/*�ڕW�܂ł̋��������߂�*/
	auto& player = Singleton<PlayerManager>::GetInstance();
	auto& enemy = Singleton<EnemyManager>::GetInstance();
	VECTOR toTarget = VSub(player.GetRigidbody().GetPosition(), enemy.GetRigidbody().GetPosition());
	this->toTargetDistance = VSize(toTarget);

	/*�{�X�̌����Ă�������ƃv���C���[�̈ʒu�Ƃ̓��ς����*/
	VECTOR enemyDirection = VTransform(VGet(0.0f, 0.0f, -1.0f), MGetRotY(enemy.GetRigidbody().GetRotation().y));
	this->innerProductOfDirectionToTarget = VDot(enemyDirection, VNorm(VSub(enemy.GetRigidbody().GetPosition(), player.GetRigidbody().GetPosition())));

}

/// <summary>
/// �X�V
/// </summary>
void DragonBehaviorTree::Update()
{
	/*�����o�ϐ��̍X�V*/
	UpdateMemberVariables();

	//printfDx("DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD:%f\n", this->innerProductOfDirectionToTarget);
	//printfDx("DRAGON_TO_TARGET:%f\n", this->toTargetDistance);
	//printfDx("DRAGON_ACTION_STATE:%d\n", this->currentAction);
	//printfDx("DRAGON_ACTION_SELECT:%d\n", this->isSelectedBattleAction);
	//printfDx("DRAGON_CANCEL_ACTION:%d\n", this->isCancelAction);

	/*�c���[�̎��s*/
	this->prevNodeState = this->Selector_DyingOrBattleOrIdle->Update();
	//this->prevNodeState = this->debugActionNode->Update();
}

/// <summary>
/// �`��
/// </summary>
const void DragonBehaviorTree::Draw()const
{
#ifdef _DEBUG
	//this->debugActionNode->Draw();
#endif // _DEBUG

}

/// <summary>
/// �o�g���A�N�V�����̓o�^
/// </summary>
void DragonBehaviorTree::EntryCurrentBattleAction(BehaviorTreeNode& _action)
{
	this->currentBattleAction = &_action;
	this->isSelectedBattleAction = true;
}

/// <summary>
/// �o�g���A�N�V�����̉���
/// </summary>
void DragonBehaviorTree::ExitCurrentBattleAction()
{
	this->currentBattleAction = nullptr;
	this->isSelectedBattleAction = false;
}