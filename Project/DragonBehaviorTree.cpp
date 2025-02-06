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
	, state							 (DragonState::NORMAL)
	, prevNodeState					 (BehaviorTreeNode::NodeState::NONE_ACTIVE)
	, toTargetDistance				 (0.0f)
	, innerProductOfDirectionToTarget(0.0f)
	, currentAction					 (0)
	, attackCount					 (0)
	, isSelectedBattleAction		 (false)
	, THRESHOLD_OF_FURY				 (Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::DRAGON)["THRESHOLD_OF_FURY"])
	, UNLEASHED_FURY				 (Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::DRAGON)["UNLEASHED_FURY"])
{
	auto& json = Singleton<JsonManager>::GetInstance();

	/*�匳�̃c���[�̍쐬*/
	this->Selector_DyingOrBattleOrIdle = new SelectorNode();
	
	/*�f�X�m�[�h*/
	BehaviorTreeNode* Sequencer_DyingIfHpIsLessThanZero = new SequencerNode();
	{
		Sequencer_DyingIfHpIsLessThanZero->AddChild(*new Condition_IsDragonHpIsLessThanConstant(0));
		Sequencer_DyingIfHpIsLessThanZero->AddChild(*new Dragon_Dying());
	}

	/*�o�g���A�N�V�����T�u�c���[*/
	BehaviorTreeNode* Sequencer_BattleAction= new SequencerNode();
	//�A�N�V�����I���T�u�c���[
	{
		//�i�K���Ƃ̍U���T�u�c���[
		{
			BehaviorTreeNode* Selector_StepByStepAttackAction = new SelectorNode();
			//3�i�K�ڂ̍U���A�N�V�����T�u�c���[
			{
				BehaviorTreeNode* Selector_ThreeStepAttackAction = new SelectorNode();
				//�������U���͈͊O�����������
				BehaviorTreeNode* Sequencer_ApproachIfTargetOutOfRangeOfLongRangeAttack = new SequencerNode();
				Sequencer_ApproachIfTargetOutOfRangeOfLongRangeAttack->AddChild(*new Condition_IsDragonToTargetDistanceGreaterThanConstant(json.GetJson(JsonManager::FileType::DRAGON)["LONG_ATTACK_RANGE"]));
				Sequencer_ApproachIfTargetOutOfRangeOfLongRangeAttack->AddChild(*new Dragon_Walk());
				//�ߋ����U���͈͊O��������u���X
				BehaviorTreeNode* Sequencer_BreathIfTargetOutOfRangeOfNearRangeAttack = new SequencerNode();
				Sequencer_BreathIfTargetOutOfRangeOfNearRangeAttack->AddChild(*new Condition_IsDragonToTargetDistanceGreaterThanConstant(json.GetJson(JsonManager::FileType::DRAGON)["NEAR_ATTACK_RANGE"]));
				Sequencer_BreathIfTargetOutOfRangeOfNearRangeAttack->AddChild(*new Dragon_Breath());
				//Ray�̊O���������]�U��
				BehaviorTreeNode* Sequencer_RotateAttackIfTargetOutOfRangeRay = new SequencerNode();
				Sequencer_RotateAttackIfTargetOutOfRangeRay->AddChild(*new Condition_IsTargetOutOfRangeOfDragonRay(json.GetJson(JsonManager::FileType::DRAGON)["TOLERANCE_RANGE"][0], json.GetJson(JsonManager::FileType::DRAGON)["TOLERANCE_RANGE"][1]));
				Sequencer_RotateAttackIfTargetOutOfRangeRay->AddChild(*new Dragon_Rotate());
				//����ȊO�������烉���_���őI��
				BehaviorTreeNode* Selector_RandomAttackAction = new RandomSelector();
				Selector_RandomAttackAction->AddChild(*new Dragon_Smash());	//�@����
				Selector_RandomAttackAction->AddChild(*new Dragon_Sweep());	//�Ȃ�����
				Selector_RandomAttackAction->AddChild(*new Dragon_Rotate());	//��]�U��
				Selector_RandomAttackAction->AddChild(*new Dragon_Breath());	//�u���X
				//3�i�K�ڂ̍U���A�N�V�����T�u�c���[�ɒǉ�
				Selector_ThreeStepAttackAction->AddChild(*Sequencer_ApproachIfTargetOutOfRangeOfLongRangeAttack);
				Selector_ThreeStepAttackAction->AddChild(*Sequencer_BreathIfTargetOutOfRangeOfNearRangeAttack);
				Selector_ThreeStepAttackAction->AddChild(*Sequencer_RotateAttackIfTargetOutOfRangeRay);
				Selector_ThreeStepAttackAction->AddChild(*Selector_RandomAttackAction);
				//�R�i�K�ڂ̍U���A�N�V�����T�u�c���[��I�����������ǉ�
				BehaviorTreeNode* Sequencer_ThreeStepActionIfHpIsLessThanConstant = new SequencerNode();
				Sequencer_ThreeStepActionIfHpIsLessThanConstant->AddChild(*new Condition_IsDragonHpIsLessThanConstant(json.GetJson(JsonManager::FileType::DRAGON)["UNLEASHED_FURY"]));
				Sequencer_ThreeStepActionIfHpIsLessThanConstant->AddChild(*Selector_ThreeStepAttackAction);
				//�i�K���Ƃ̍U���T�u�c���[�ɒǉ�
				Selector_StepByStepAttackAction->AddChild(*Sequencer_ThreeStepActionIfHpIsLessThanConstant);
			}
			//2�i�K�ڂ̍U���A�N�V�����T�u�c���[
			{
				BehaviorTreeNode* Selector_TwoStepAttackAction = new SelectorNode();
				//�������U���͈͊O�����������
				BehaviorTreeNode* Sequencer_ApproachIfTargetOutOfRangeOfLongRangeAttack = new SequencerNode();
				Sequencer_ApproachIfTargetOutOfRangeOfLongRangeAttack->AddChild(*new Condition_IsDragonToTargetDistanceGreaterThanConstant(json.GetJson(JsonManager::FileType::DRAGON)["LONG_ATTACK_RANGE"]));
				Sequencer_ApproachIfTargetOutOfRangeOfLongRangeAttack->AddChild(*new Dragon_Walk());
				//�ߋ����U���͈͊O��������u���X
				BehaviorTreeNode* Sequencer_BreathIfTargetOutOfRangeOfNearRangeAttack = new SequencerNode();
				Sequencer_BreathIfTargetOutOfRangeOfNearRangeAttack->AddChild(*new Condition_IsDragonToTargetDistanceGreaterThanConstant(json.GetJson(JsonManager::FileType::DRAGON)["NEAR_ATTACK_RANGE"]));
				Sequencer_BreathIfTargetOutOfRangeOfNearRangeAttack->AddChild(*new Dragon_Breath());
				//Ray�̊O���������]�U��
				BehaviorTreeNode* Sequencer_RotateAttackIfTargetOutOfRangeRay = new SequencerNode();
				Sequencer_RotateAttackIfTargetOutOfRangeRay->AddChild(*new Condition_IsTargetOutOfRangeOfDragonRay(json.GetJson(JsonManager::FileType::BEAST)["TOLERANCE_RANGE"][0], json.GetJson(JsonManager::FileType::BEAST)["TOLERANCE_RANGE"][1]));
				Sequencer_RotateAttackIfTargetOutOfRangeRay->AddChild(*new Dragon_Rotate());
				//����ȊO�������烉���_���őI��
				BehaviorTreeNode* Selector_RandomAttackAction = new RandomSelector();
				Selector_RandomAttackAction->AddChild(*new Dragon_Smash());	//�@����
				Selector_RandomAttackAction->AddChild(*new Dragon_Sweep());	//�Ȃ�����
				//2�i�K�ڂ̍U���A�N�V�����T�u�c���[�ɒǉ�
				Selector_TwoStepAttackAction->AddChild(*Sequencer_ApproachIfTargetOutOfRangeOfLongRangeAttack);
				Selector_TwoStepAttackAction->AddChild(*Sequencer_BreathIfTargetOutOfRangeOfNearRangeAttack);
				Selector_TwoStepAttackAction->AddChild(*Sequencer_RotateAttackIfTargetOutOfRangeRay);
				Selector_TwoStepAttackAction->AddChild(*Selector_RandomAttackAction);
				//�R�i�K�ڂ̍U���A�N�V�����T�u�c���[��I�����������ǉ�
				BehaviorTreeNode* Sequencer_TwoStepActionIfHpIsLessThanConstant = new SequencerNode();
				Sequencer_TwoStepActionIfHpIsLessThanConstant->AddChild(*new Condition_IsDragonHpIsLessThanConstant(json.GetJson(JsonManager::FileType::DRAGON)["THRESHOLD_OF_FURY"]));
				Sequencer_TwoStepActionIfHpIsLessThanConstant->AddChild(*Selector_TwoStepAttackAction);
				//�i�K���Ƃ̍U���T�u�c���[�ɒǉ�
				Selector_StepByStepAttackAction->AddChild(*Sequencer_TwoStepActionIfHpIsLessThanConstant);
			}
			//1�i�K�ڂ̍U���A�N�V�����T�u�c���[
			{
				BehaviorTreeNode* Selector_OneStepAttackAction = new SelectorNode();
				//�ߋ����U���͈͊O�����������
				BehaviorTreeNode* Sequencer_ApproachIfTargetOutOfRangeOfNearRangeAttack = new SequencerNode();
				Sequencer_ApproachIfTargetOutOfRangeOfNearRangeAttack->AddChild(*new Condition_IsDragonToTargetDistanceGreaterThanConstant(json.GetJson(JsonManager::FileType::DRAGON)["NEAR_ATTACK_RANGE"]));
				Sequencer_ApproachIfTargetOutOfRangeOfNearRangeAttack->AddChild(*new Dragon_Walk());
				//����ȊO�������烉���_���őI��
				BehaviorTreeNode* Selector_RandomAttackAction = new RandomSelector();
				Selector_RandomAttackAction->AddChild(*new Dragon_Smash());	//�@����
				Selector_RandomAttackAction->AddChild(*new Dragon_Sweep());	//�Ȃ�����
				//1�i�K�ڂ̍U���A�N�V�����T�u�c���[�ɒǉ�
				Selector_OneStepAttackAction->AddChild(*Sequencer_ApproachIfTargetOutOfRangeOfNearRangeAttack);
				Selector_OneStepAttackAction->AddChild(*Selector_RandomAttackAction);
				//�i�K���Ƃ̍U���T�u�c���[�ɒǉ�
				Selector_StepByStepAttackAction->AddChild(*Selector_OneStepAttackAction);
			}
			//�o�g���T�u�c���[�ɏ����ƃA�N�V�����I���T�u�c���[��ǉ�
			Sequencer_BattleAction->AddChild(*new Condition_IsDragonAttackCountLeft());
			Sequencer_BattleAction->AddChild(*Selector_StepByStepAttackAction);
		}
	}

	/*�A�N�V�������I������Ă��邩*/
	BehaviorTreeNode* Sequencer_PlayCurrentIfSelectedBattleAction = new SequencerNode();
	{
		Sequencer_PlayCurrentIfSelectedBattleAction->AddChild(*new Condition_IsSelectedDragonBattleAction());
		Sequencer_PlayCurrentIfSelectedBattleAction->AddChild(*new Dragon_PlayCurrentBattleAction());
	}

	/*���K�A�N�V����*/
	SequencerNode* RoarIfDragonStageIsDifferent = new SequencerNode();
	{
		RoarIfDragonStageIsDifferent->AddChild(*new Condition_IsStageIsDifferent());
		RoarIfDragonStageIsDifferent->AddChild(*new Dragon_Roar());
	}

	/*���[�g�m�[�h�Ɋe�A�N�V�����c���[��}��*/
	this->Selector_DyingOrBattleOrIdle->AddChild(*Sequencer_DyingIfHpIsLessThanZero);
	this->Selector_DyingOrBattleOrIdle->AddChild(*Sequencer_PlayCurrentIfSelectedBattleAction);
	this->Selector_DyingOrBattleOrIdle->AddChild(*RoarIfDragonStageIsDifferent);
	this->Selector_DyingOrBattleOrIdle->AddChild(*Sequencer_BattleAction);
	this->Selector_DyingOrBattleOrIdle->AddChild(*new Dragon_Idle());

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
	this->state			= DragonState::NORMAL;
	this->currentStage	= DragonStage::AWAKENING;
	SetAttackCount();
}

/// <summary>
/// �{�X�̒i�K�𔻒�
/// </summary>
bool DragonBehaviorTree::JudgeBossStage()
{
	auto& enemy = Singleton<EnemyManager>::GetInstance();
	auto& json = Singleton<JsonManager>::GetInstance();
	int hp = enemy.GetHP();

	/*HP�����~�b�g�ȉ���������i�K��ς���*/
	DragonStage newStage;
	if (hp <= this->UNLEASHED_FURY)
	{
		newStage = DragonStage::RAMPAGE;
	}
	else if (hp <= this->THRESHOLD_OF_FURY)
	{
		newStage = DragonStage::FURY;
	}
	else
	{
		newStage = DragonStage::AWAKENING;
	}

	/*�����i�K�����ۑ�����Ă����ԂƈقȂ��Ă������Ԃ�ς���*/
	if (this->currentStage != newStage)
	{
		this->currentStage = newStage;
		return true;
	}
	return false;
}


/// <summary>
/// �����o�ϐ��̍X�V
/// </summary>
void DragonBehaviorTree::UpdateMemberVariables()
{
	/*�C���^�[�o���̌v�Z*/
	for (auto interval : this->intervalSet)
	{
		if (interval != 0)
		{
			interval--;
		}
	}

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

void DragonBehaviorTree::SetAttackCount()
{
	auto& json = Singleton<JsonManager>::GetInstance();
	switch (this->currentStage)
	{
	case DragonStage::AWAKENING:
		this->attackCount = json.GetJson(JsonManager::FileType::DRAGON)["AWAKENING_ATTACK_COUNT"];
		break;
	case DragonStage::FURY:
		this->attackCount = json.GetJson(JsonManager::FileType::DRAGON)["FURY_ATTACK_COUNT"];
		break;
	case DragonStage::RAMPAGE:
		this->attackCount = json.GetJson(JsonManager::FileType::DRAGON)["RAMPAGE_ATTACK_COUNT"];
		break;
	default:
		break;
	}
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