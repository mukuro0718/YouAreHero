#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "BeastBehaviorTreeHeader.h"
#include "DeleteInstance.h"
#include "Rigidbody.h"
#include "CharacterData.h"
#include "EnemyManager.h"
#include "Character.h"
#include "Player.h"
#include "PlayerManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
BeastBehaviorTree::BeastBehaviorTree()
	: Selector_DeathOrReactionOrBattleOrBreak	(nullptr)
	, state										(BeastState::NORMAL)
	, downValue									(0)
	, prevHp									(0)
	, damage									(0)
	, level										(0)
	, toTargetDistance							(0.0f)
	, isDestroyedPart							(false)
	, selectAction								(-1)
	, isSelectedBattleAction					(false)
	, isSelectedReaction						(false)
	, currentBattleAction						(nullptr)
	, currentReaction							(nullptr)
{
	auto& json = Singleton<JsonManager>::GetInstance();
	/*�C���^�[�o����������*/
	this->intervalSet = { 0 };

	/*�匳�̃c���[�̍쐬*/
	this->Selector_DeathOrReactionOrBattleOrBreak = new SelectorNode();

	/*HP���Ȃ��Ƃ��̃A�N�V����*/
	{
		//�f�X����m�[�h
		BehaviorTreeNode* Sequencer_DeathIfHpIsLessThanZero = new SequencerNode();
		Sequencer_DeathIfHpIsLessThanZero->AddChild(*new Condition_IsHpBelowConstant(0));
		Sequencer_DeathIfHpIsLessThanZero->AddChild(*new Beast_Dying());
		//root�m�[�h�ɒǉ�
		this->Selector_DeathOrReactionOrBattleOrBreak->AddChild(*Sequencer_DeathIfHpIsLessThanZero);
	}

	/*���A�N�V�������̃A�N�V����*/
	{
		//�T�u�c���[
		BehaviorTreeNode* Selector_DownOrLongOrShortReaction = new SelectorNode();
		//�T�u�c���[�ɂԂ牺����q�m�[�h
		{
			//���A�N�V�������I������Ă�����
			BehaviorTreeNode* Sequencer_PlayCurrentIfSelectedReaction = new SequencerNode();
			Sequencer_PlayCurrentIfSelectedReaction->AddChild(*new Condition_IsSelectedReaction());
			Sequencer_PlayCurrentIfSelectedReaction->AddChild(*new Beast_PlayCurrentReaction());
			//�_�E��
			BehaviorTreeNode* Sequencer_DownIfDownValueMoreThanConstant = new SequencerNode();
			Sequencer_DownIfDownValueMoreThanConstant->AddChild(*new Condition_IsNowStateIsSameAsTheSpecifiedState(BeastState::DOWN));
			Sequencer_DownIfDownValueMoreThanConstant->AddChild(*new Beast_Down());
			//��������
			BehaviorTreeNode* Sequencer_LongFrighteningIfFrighteningValueMoreThanConstant = new SequencerNode();
			Sequencer_LongFrighteningIfFrighteningValueMoreThanConstant->AddChild(*new Condition_IsFrighteningValueGreaterThanConstant(json.GetJson(JsonManager::FileType::BEAST)["SPECIFIED_FRIGHTENING_VALUE"]));
			Sequencer_LongFrighteningIfFrighteningValueMoreThanConstant->AddChild(*new Beast_LongFrightening());
			//�Z������
			BehaviorTreeNode* Sequencer_ShortFrighteningIfPartDestroyed = new SequencerNode();
			Sequencer_ShortFrighteningIfPartDestroyed->AddChild(*new Condition_IsDestoroyedPart());
			Sequencer_ShortFrighteningIfPartDestroyed->AddChild(*new Beast_ShortFrightening());
			//�T�u�c���[�ǉ�
			Selector_DownOrLongOrShortReaction->AddChild(*Sequencer_PlayCurrentIfSelectedReaction);
			Selector_DownOrLongOrShortReaction->AddChild(*Sequencer_DownIfDownValueMoreThanConstant);
			Selector_DownOrLongOrShortReaction->AddChild(*Sequencer_LongFrighteningIfFrighteningValueMoreThanConstant);
			Selector_DownOrLongOrShortReaction->AddChild(*Sequencer_ShortFrighteningIfPartDestroyed);
		}
		this->Selector_DeathOrReactionOrBattleOrBreak->AddChild(*Selector_DownOrLongOrShortReaction);
	}

	/*�퓬���̃A�N�V����*/
	{
		//�U�������߂�T�u�c���[
		BehaviorTreeNode* Selector_ApproachOrSpecialOrLongRangeOrNearRangeAttack = new SelectorNode();

		//����U���T�u�c���[
		BehaviorTreeNode* Sequencer_SpecialActionIfIntervalIsOver = new SequencerNode();
		//����U���A�N�V�����Z���N�^�[
		BehaviorTreeNode* Selector_SpecialAction = new SelectorNode();
		//LEVEL���ő傾������X�[�p�[�m���@
		BehaviorTreeNode* Sequencer_SuperNovaIfLevelIsMax = new SequencerNode();
		Sequencer_SuperNovaIfLevelIsMax->AddChild(*new Condition_IsNowLevelIsSameAsTheSpecifiedLevel(static_cast<int>(LevelStage::Lv4)));
		Sequencer_SuperNovaIfLevelIsMax->AddChild(*new Beast_SuperNova());
		//����ȊO�ŃC���^�[�o�����I�����Ă����烌�x���㏸�A�N�V����
		BehaviorTreeNode* Sequencer_RaiseLevelIfIntervalIsOver = new SequencerNode();
		Sequencer_RaiseLevelIfIntervalIsOver->AddChild(*new Condition_IsActionIntervalIsOver(static_cast<int>(ActionType::RAISE_LEVEL)));
		Sequencer_RaiseLevelIfIntervalIsOver->AddChild(*new Beast_RaiseLevel());
		//����U���A�N�V�����Z���N�^�[�ɒǉ�
		Selector_SpecialAction->AddChild(*Sequencer_SuperNovaIfLevelIsMax);
		Selector_SpecialAction->AddChild(*Sequencer_RaiseLevelIfIntervalIsOver);
		//����U���T�u�c���[�ɑ��
		Sequencer_SpecialActionIfIntervalIsOver->AddChild(*Selector_SpecialAction);
		
		//�������U���͈͊O�����������
		BehaviorTreeNode* Sequencer_ApproachIfTargetOutOfRangeOfLongRangeAttack = new SequencerNode();
		Sequencer_ApproachIfTargetOutOfRangeOfLongRangeAttack->AddChild(*new Condition_IsToTargetDistanceGreaterThanConstant(json.GetJson(JsonManager::FileType::BEAST)["FAR_ATTACK_RANGE"]));
		Sequencer_ApproachIfTargetOutOfRangeOfLongRangeAttack->AddChild(*new Beast_Walk());

		//�������U���T�u�c���[
		BehaviorTreeNode* Sequencer_LongRangeAttackIfTargetOutOfRangeOfNearRangeAttack = new SequencerNode();
		Sequencer_LongRangeAttackIfTargetOutOfRangeOfNearRangeAttack->AddChild(*new Condition_IsToTargetDistanceGreaterThanConstant(json.GetJson(JsonManager::FileType::BEAST)["NEAR_ATTACK_RANGE"]));
		//�������U���A�N�V�����Z���N�^�[
		BehaviorTreeNode* Selector_LongRangeAction = new SelectorNode();
		//�{���Ԃ�������ːi
		BehaviorTreeNode* Sequencer_AngryLongRangeActionIfStateNormal = new SequencerNode();
		Sequencer_AngryLongRangeActionIfStateNormal->AddChild(*new Condition_IsNowStateIsSameAsTheSpecifiedState(BeastState::ANGRY));
		Sequencer_AngryLongRangeActionIfStateNormal->AddChild(*new Beast_Rush());
		//�ʏ��Ԃ��������u���X
		BehaviorTreeNode* Sequencer_NormalLongRangeActionIfStateNormal = new SequencerNode();
		Sequencer_NormalLongRangeActionIfStateNormal->AddChild(*new Condition_IsNowStateIsSameAsTheSpecifiedState(BeastState::NORMAL));
		Sequencer_NormalLongRangeActionIfStateNormal->AddChild(*new Beast_WeakBreath());
		//�������U���A�N�V�����Z���N�^�[�Ɋe�V�[�P���T�m�[�h������
		Selector_LongRangeAction->AddChild(*Sequencer_AngryLongRangeActionIfStateNormal);
		Selector_LongRangeAction->AddChild(*Sequencer_NormalLongRangeActionIfStateNormal);
		//�������U���A�N�V�����Z���N�^�[���������U���T�u�c���[�ɓ����
		Sequencer_LongRangeAttackIfTargetOutOfRangeOfNearRangeAttack->AddChild(*Selector_LongRangeAction);

		//�ߋ����U���A�N�V�����Z���N�^�[(�����܂Œʂ����Ƃ������Ƃ̓v���C���[�͋ߐڍU���͈͓��Ȃ̂�Condition�ŕ]���͂��Ȃ�)
		BehaviorTreeNode* Selector_NearRangeAction = new SelectorNode();
		//�O�̃A�N�V������IDLE���������ރu���X
		BehaviorTreeNode* Sequencer_BackingBreathIfSelectActionIsIdle = new SequencerNode();
		Sequencer_BackingBreathIfSelectActionIsIdle->AddChild(*new Condition_IsPrevActionIsSameAsSpecifiedAction(static_cast<int>(ActionType::IDLE)));
		Sequencer_BackingBreathIfSelectActionIsIdle->AddChild(*new Beast_BackingBreath());
		//�v���C���[��Ray�̂��Ƃ������甚���U��
		BehaviorTreeNode* Sequencer_ExplositonIfTargetOutOfRangeRay = new SequencerNode();
		Sequencer_ExplositonIfTargetOutOfRangeRay->AddChild(*new Condition_IsTargetOutOfRangeOfRay(json.GetJson(JsonManager::FileType::BEAST)["TOLERANCE_RANGE"][0], json.GetJson(JsonManager::FileType::BEAST)["TOLERANCE_RANGE"][1]));
		Sequencer_ExplositonIfTargetOutOfRangeRay->AddChild(*new Beast_Explosion());
		//�ʏ펞�̍U���T�u�c���[
		BehaviorTreeNode* Sequencer_NormalNearActionIfStateNormal = new SequencerNode();
		Sequencer_NormalNearActionIfStateNormal->AddChild(*new Condition_IsNowStateIsSameAsTheSpecifiedState(BeastState::NORMAL));
		//�ʏ��Ԃ������烉���_���ŉE��U�����R���{�U�����o��
		BehaviorTreeNode* Selector_NormalStateAction = new RandomSelector();
		Selector_NormalStateAction->AddChild(*new Beast_RightFootAttack());
		Selector_NormalStateAction->AddChild(*new Beast_FootComboAttack());
		Selector_NormalStateAction->AddChild(*new Beast_Idle());
		//�ʏ펞�̍U���T�u�c���[�ɒǉ�
		Sequencer_NormalNearActionIfStateNormal->AddChild(*Selector_NormalStateAction);
		//�{�莞�̍U���T�u�c���[
		BehaviorTreeNode* Sequencer_AngryNearActionIfStateNormal = new SequencerNode();
		Sequencer_AngryNearActionIfStateNormal->AddChild(*new Condition_IsNowStateIsSameAsTheSpecifiedState(BeastState::ANGRY));
		//�{���Ԃ������痭�߉E�U�����������ߍU�����E�{��]�U��
		BehaviorTreeNode* Selector_AngryStateAction = new RandomSelector();
		Selector_AngryStateAction->AddChild(*new Beast_ChargeBothFootAttack());
		Selector_AngryStateAction->AddChild(*new Beast_ChargeRightFootAttack());
		//�{�莞�̍U���T�u�c���[�ɒǉ�
		Sequencer_AngryNearActionIfStateNormal->AddChild(*Selector_AngryStateAction);
		//�ߐڍU���A�N�V�����Z���N�^�[�Ɋe�A�N�V������ǉ�
		Selector_NearRangeAction->AddChild(*Sequencer_BackingBreathIfSelectActionIsIdle);
		Selector_NearRangeAction->AddChild(*Sequencer_ExplositonIfTargetOutOfRangeRay);
		Selector_NearRangeAction->AddChild(*Sequencer_NormalNearActionIfStateNormal);
		Selector_NearRangeAction->AddChild(*Sequencer_AngryNearActionIfStateNormal);
		//�A�N�V�������I������Ă��邩
		BehaviorTreeNode* Sequencer_PlayCurrentIfSelectedBattleAction = new SequencerNode();
		Sequencer_PlayCurrentIfSelectedBattleAction->AddChild(*new Condition_IsSelectedBattleAction());
		Sequencer_PlayCurrentIfSelectedBattleAction->AddChild(*new Beast_PlayCurrentBattleAction());
		//���K�A�N�V����
		BehaviorTreeNode* Sequencer_RoarIfAngryValueGreaterThanConstant = new SequencerNode();
		Sequencer_RoarIfAngryValueGreaterThanConstant->AddChild(*new Condition_IsAngryValueGreaterThanConstant(json.GetJson(JsonManager::FileType::BEAST)["MAX_ANGRY_VALUE"]));
		Sequencer_RoarIfAngryValueGreaterThanConstant->AddChild(*new Beast_Roar());

		//�U�������߂�T�u�c���[�Ɋe�U���T�u�c���[��ǉ�
		Selector_ApproachOrSpecialOrLongRangeOrNearRangeAttack->AddChild(*Sequencer_PlayCurrentIfSelectedBattleAction);
		Selector_ApproachOrSpecialOrLongRangeOrNearRangeAttack->AddChild(*Sequencer_RoarIfAngryValueGreaterThanConstant);
		Selector_ApproachOrSpecialOrLongRangeOrNearRangeAttack->AddChild(*Sequencer_SpecialActionIfIntervalIsOver);
		Selector_ApproachOrSpecialOrLongRangeOrNearRangeAttack->AddChild(*Sequencer_ApproachIfTargetOutOfRangeOfLongRangeAttack);
		Selector_ApproachOrSpecialOrLongRangeOrNearRangeAttack->AddChild(*Sequencer_LongRangeAttackIfTargetOutOfRangeOfNearRangeAttack);
		Selector_ApproachOrSpecialOrLongRangeOrNearRangeAttack->AddChild(*Selector_NearRangeAction);

		//root�m�[�h�ɒǉ�
		this->Selector_DeathOrReactionOrBattleOrBreak->AddChild(*Selector_ApproachOrSpecialOrLongRangeOrNearRangeAttack);
	}

	this->debugActionNode = new Beast_Down();
	
	/*������*/
	Initialize();
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
BeastBehaviorTree::~BeastBehaviorTree()
{
	DeleteMemberInstance(this->Selector_DeathOrReactionOrBattleOrBreak);
	DeleteMemberInstance(this->currentBattleAction);
	DeleteMemberInstance(this->currentReaction);
	DeleteMemberInstance(this->debugActionNode);
}

/// <summary>
/// ������
/// </summary>
void BeastBehaviorTree::Initialize()
{
	auto& json				= Singleton<JsonManager>::GetInstance();
	this->state				= BeastState::NORMAL;
	this->downValue = 0;
	this->prevHp			= json.GetJson(JsonManager::FileType::BEAST)["HP"];
	this->damage			= 0;
	this->level				= 0;
	this->toTargetDistance	= 0;
	this->isDestroyedPart	= false;
	this->selectAction		= -1;
	this->isSelectedBattleAction = false;
	this->isSelectedReaction = false;
	if (this->currentReaction != nullptr)
	{
		this->currentReaction->Initialize();
	}
	if (this->currentBattleAction != nullptr)
	{
		this->currentBattleAction->Initialize();
	}
	this->angryValue = 0;

	for (int i = 0; i < this->intervalSet.size(); i++)
	{
		this->intervalSet[i] = 0;
	}
}

/// <summary>
/// �����o�ϐ��̍X�V
/// </summary>
void BeastBehaviorTree::UpdateMemberVariables()
{
	/*�C���^�[�o���̌v�Z*/
	for (int& interval : this->intervalSet)
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

	/*HP�̍X�V*/
	this->prevHp = enemy.GetHP();

	/*�{�X�̌����Ă�������ƃv���C���[�̈ʒu�Ƃ̓��ς����*/
	VECTOR enemyDirection = VTransform(VGet(0.0f, 0.0f, -1.0f), MGetRotY(enemy.GetRigidbody().GetRotation().y));
	this->innerProductOfDirectionToTarget = VDot(enemyDirection, VNorm(VSub(enemy.GetRigidbody().GetPosition(), player.GetRigidbody().GetPosition())));
}

/// <summary>
/// �X�V
/// </summary>
void BeastBehaviorTree::Update()
{
	/*�����o�ϐ��̍X�V*/
	UpdateMemberVariables();
	ChangeState();


	/*�c���[�̎��s*/
	BehaviorTreeNode::NodeState state = this->Selector_DeathOrReactionOrBattleOrBreak->Update();
	//BehaviorTreeNode::NodeState state = this->debugActionNode->Update();
	int intState = static_cast<int>(state);

#ifdef _DEBUG
	//printfDx("DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD:%f\n", this->innerProductOfDirectionToTarget);
	//printfDx("BEAST_TO_TARGET:%f\n", this->toTargetDistance);
	//printfDx("BEAST_ACTION_STATE:%d\n", this->selectAction);
	//printfDx("BEAST_NODE_STATE:%d\n", intState);
#endif // _DEBUG
}

/// <summary>
/// �`��
/// </summary>
const void BeastBehaviorTree::Draw()const
{
#ifdef _DEBUG
	//if (this->currentBattleAction)
	//{
	//	this->currentBattleAction->Draw();
	//}
	//this->debugActionNode->Draw();
#endif // _DEBUG

}

/// <summary>
/// �o�g���A�N�V�����̓o�^
/// </summary>
void BeastBehaviorTree::EntryCurrentBattleAction(BehaviorTreeNode& _action)
{
	this->currentBattleAction = &_action;
	this->isSelectedBattleAction = true; 
}
/// <summary>
/// ���A�N�V�����A�N�V�����̓o�^
/// </summary>
void BeastBehaviorTree::EntryCurrentReaction(BehaviorTreeNode& _action)
{
	this->currentReaction = &_action;
	this->isSelectedReaction = true;
}
/// <summary>
/// ���A�N�V�����A�N�V�����̉���
/// </summary>
void BeastBehaviorTree::ExitCurrentReaction()
{ 
	this->isSelectedReaction = false; 
	if (this->currentReaction != nullptr)
	{
		this->currentReaction->Initialize();
	}
	this->currentReaction = nullptr; 
}
/// <summary>
/// �o�g���A�N�V�����̉���
/// </summary>
void BeastBehaviorTree::ExitCurrentBattleAction() 
{ 
	this->isSelectedBattleAction = false; 
	if (this->currentBattleAction != nullptr)
	{
		this->currentBattleAction->Initialize();
	}
	this->currentBattleAction = nullptr;
}

/// <summary>
/// ��Ԃ̕ύX
/// </summary>
void BeastBehaviorTree::ChangeState()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& enemy = Singleton<EnemyManager>::GetInstance();

	/*�{����*/
	switch (this->state)
	{
		//�{��
	case BeastState::ANGRY:
		this->angryValue--;
		if (this->angryValue < 0)
		{
			this->state = BeastState::DOWN;
			this->angryValue = 0;
			this->downValue = json.GetJson(JsonManager::FileType::BEAST)["MAX_DOWN_VALUE"];
		}
		break;
		//�ʏ�
	case BeastState::NORMAL:
		//�{��l�𑝉�
		this->angryValue++;
		//�U�����������Ă�����{��l������ɑ���
		
		if (enemy.GetCharacterData().isHit)
		{
			this->angryValue++;
		}
		//�{��l���ő�ȏゾ�������Ԃ�ANGRY�ɂ���
		if (this->angryValue >= json.GetJson(JsonManager::FileType::BEAST)["MAX_ANGRY_VALUE"])
		{
			this->angryValue = json.GetJson(JsonManager::FileType::BEAST)["MAX_ANGRY_VALUE"];
		}
		break;
		//���
	case BeastState::DOWN:
		this->downValue -= 2;
		break;
	}
}