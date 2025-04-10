#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "DeleteInstance.h"
#include "Rigidbody.h"
#include "CharacterData.h"
#include "Character.h"
#include "BeastBehaviorTreeHeader.h"
#include "Enemy.h"
#include "Beast.h"
#include "Player.h"
#include "EnemyManager.h"
#include "PlayerManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
BeastBehaviorTree::BeastBehaviorTree()
{
	/*�C���^�[�o����������*/
	this->intervalSet.clear();
	for (int i = 0; i < static_cast<int>(ActionType::COMBO_ATTACK) + 1; i++)
	{
		this->intervalSet.emplace_back(0);
	}

	/*�f�o�b�N�p�m�[�h*/
	this->debugNode = new Beast_Down();
	
	/*������*/
	Initialize();
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
BeastBehaviorTree::~BeastBehaviorTree()
{
}

/// <summary>
/// �r�w�C�r�A�c���[���쐬
/// </summary>
void BeastBehaviorTree::CreateBehaviorTree()
{
	DeleteMemberInstance(this->mainNode);

	/*�匳�̃c���[�̍쐬*/
	this->mainNode = new SelectorNode();

	auto& json = Singleton<JsonManager>::GetInstance();
	/*HP���Ȃ��Ƃ��̃A�N�V����*/
	{
		//�f�X����m�[�h
		BehaviorTreeNode* Sequencer_DeathIfHpIsLessThanZero = new SequencerNode();
		Sequencer_DeathIfHpIsLessThanZero->AddChild(*new Condition_IsHpBelowConstant(0));
		Sequencer_DeathIfHpIsLessThanZero->AddChild(*new Beast_Dying());
		//root�m�[�h�ɒǉ�
		this->mainNode->AddChild(*Sequencer_DeathIfHpIsLessThanZero);
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
			Sequencer_DownIfDownValueMoreThanConstant->AddChild(*new Condition_IsNowStateIsSameAsTheSpecifiedState(static_cast<int>(Beast::BossState::TIRED)));
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
		this->mainNode->AddChild(*Selector_DownOrLongOrShortReaction);
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
		Sequencer_AngryLongRangeActionIfStateNormal->AddChild(*new Condition_IsNowStateIsSameAsTheSpecifiedState(static_cast<int>(Beast::BossState::ANGRY)));
		Sequencer_AngryLongRangeActionIfStateNormal->AddChild(*new Beast_Rush());
		//�ʏ��Ԃ��������u���X
		BehaviorTreeNode* Sequencer_NormalLongRangeActionIfStateNormal = new SequencerNode();
		Sequencer_NormalLongRangeActionIfStateNormal->AddChild(*new Condition_IsActionIntervalIsOver(static_cast<int>(ActionType::WEAK_BREATH)));
		Sequencer_NormalLongRangeActionIfStateNormal->AddChild(*new Condition_IsNowStateIsSameAsTheSpecifiedState(static_cast<int>(Beast::BossState::NORMAL)));
		Sequencer_NormalLongRangeActionIfStateNormal->AddChild(*new Beast_WeakBreath());
		//�������U���A�N�V�����Z���N�^�[�Ɋe�V�[�P���T�m�[�h������
		Selector_LongRangeAction->AddChild(*Sequencer_AngryLongRangeActionIfStateNormal);
		Selector_LongRangeAction->AddChild(*Sequencer_NormalLongRangeActionIfStateNormal);
		Selector_LongRangeAction->AddChild(*new Beast_Walk());
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
		Sequencer_ExplositonIfTargetOutOfRangeRay->AddChild(*new Condition_IsActionIntervalIsOver(static_cast<int>(ActionType::EXPLOSION)));
		Sequencer_ExplositonIfTargetOutOfRangeRay->AddChild(*new Beast_Explosion());
		//�ʏ펞�̍U���T�u�c���[
		BehaviorTreeNode* Sequencer_NormalNearActionIfStateNormal = new SequencerNode();
		Sequencer_NormalNearActionIfStateNormal->AddChild(*new Condition_IsNowStateIsSameAsTheSpecifiedState(static_cast<int>(Beast::BossState::NORMAL)));
		//�ʏ��Ԃ������烉���_���ŉE��U�����R���{�U�����o��
		BehaviorTreeNode* Selector_NormalStateAction = new RandomSelector();
		Selector_NormalStateAction->AddChild(*new Beast_RightFootAttack());
		Selector_NormalStateAction->AddChild(*new Beast_FootComboAttack());
		Selector_NormalStateAction->AddChild(*new Beast_Idle());
		//�ʏ펞�̍U���T�u�c���[�ɒǉ�
		Sequencer_NormalNearActionIfStateNormal->AddChild(*Selector_NormalStateAction);
		//�{�莞�̍U���T�u�c���[
		BehaviorTreeNode* Sequencer_AngryNearActionIfStateNormal = new SequencerNode();
		Sequencer_AngryNearActionIfStateNormal->AddChild(*new Condition_IsNowStateIsSameAsTheSpecifiedState(static_cast<int>(Beast::BossState::ANGRY)));
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
		Sequencer_RoarIfAngryValueGreaterThanConstant->AddChild(*new Condition_IsActionIntervalIsOver(static_cast<int>(ActionType::ROAR)));
		Sequencer_RoarIfAngryValueGreaterThanConstant->AddChild(*new Beast_Roar());

		//�e�U���A�N�V�����́A�U���񐔂��c���Ă���ꍇ�̂ݍs��
		BehaviorTreeNode* Selector_AttackAction = new SelectorNode();
		Selector_AttackAction->AddChild(*Sequencer_RoarIfAngryValueGreaterThanConstant);
		Selector_AttackAction->AddChild(*Sequencer_SpecialActionIfIntervalIsOver);
		Selector_AttackAction->AddChild(*Sequencer_ApproachIfTargetOutOfRangeOfLongRangeAttack);
		Selector_AttackAction->AddChild(*Sequencer_LongRangeAttackIfTargetOutOfRangeOfNearRangeAttack);
		Selector_AttackAction->AddChild(*Selector_NearRangeAction);
		BehaviorTreeNode* Sequencer_AttackIfAttackCountIsOver = new SequencerNode();
		Sequencer_AttackIfAttackCountIsOver->AddChild(*Selector_AttackAction);

		//�U�������߂�T�u�c���[�Ɋe�U���T�u�c���[��ǉ�
		Selector_ApproachOrSpecialOrLongRangeOrNearRangeAttack->AddChild(*Sequencer_PlayCurrentIfSelectedBattleAction);
		Selector_ApproachOrSpecialOrLongRangeOrNearRangeAttack->AddChild(*Sequencer_AttackIfAttackCountIsOver);

		//root�m�[�h�ɒǉ�
		this->mainNode->AddChild(*Selector_ApproachOrSpecialOrLongRangeOrNearRangeAttack);
	}
}

/// <summary>
/// ������
/// </summary>
void BeastBehaviorTree::Initialize()
{
	CreateBehaviorTree();
	auto& json								= Singleton<JsonManager>::GetInstance();
	this->prevHp							= json.GetJson(JsonManager::FileType::BEAST)["HP"];
	this->damage							= 0;
	this->level								= 0;
	this->selectAction						= -1;
	this->nodeState							= BehaviorTreeNode::NodeState::NONE_ACTIVE;
	this->toTargetDistance					= 0.0f;
	this->innerProductOfDirectionToTarget	= 0.0f;
	this->attackCount						= 0;
	//this->isDestroyedPart					= false;
	this->isSelectedBattleAction			= false;
	this->isSelectedReaction				= false;

	SetInterval(static_cast<int>(ActionType::ROAR), 0);

	this->currentBattleNode	  = nullptr;
	this->currentReactionNode = nullptr;
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
void BeastBehaviorTree::Update(Character& _chara)
{
	/*�����o�ϐ��̍X�V*/
	UpdateMemberVariables();

	/*�c���[�̎��s*/
	BehaviorTreeNode::NodeState state = this->mainNode->Update(*this, _chara);

#ifdef _DEBUG
	//BehaviorTreeNode::NodeState state = this->debugActionNode->Update();
	//int debugState = static_cast<int>(state);
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

