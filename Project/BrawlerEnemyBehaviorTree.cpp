#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "DeleteInstance.h"
#include "Rigidbody.h"
#include "CharacterData.h"
#include "Character.h"
#include "EnemyManager.h"
#include "Character.h"
#include "Enemy.h"
#include "BrawlerEnemyBehaviorTreeHeader.h"
#include "BrawlerEnemy.h"
#include "Player.h"
#include "PlayerManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
BrawlerEnemyBehaviorTree::BrawlerEnemyBehaviorTree()
{
	/*�C���^�[�o����������*/
	this->intervalSet.clear();
	for (int i = 0; i < static_cast<int>(ActionType::SWAY) + 1; i++)
	{
		this->intervalSet.emplace_back(0);
	}

	/*�f�o�b�N�p�m�[�h*/
	//this->debugNode = new Brawler_Idle();

	/*������*/
	Initialize();
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
BrawlerEnemyBehaviorTree::~BrawlerEnemyBehaviorTree()
{
}

/// <summary>
/// ������
/// </summary>
void BrawlerEnemyBehaviorTree::Initialize()
{
	CreateBehaviorTree();
	auto& json								= Singleton<JsonManager>::GetInstance();
	this->prevHp							= json.GetJson(JsonManager::FileType::BRAWLER_ENEMY)["HP"];
	this->damage							= 0;
	this->selectAction						= -1;
	this->nodeState							= BehaviorTreeNode::NodeState::NONE_ACTIVE;
	this->toTargetDistance					= 0.0f;
	this->innerProductOfDirectionToTarget	= 0.0f;
	this->attackCount						= 0;
	this->isSelectedBattleAction			= false;
	this->isSelectedReaction				= false;
	this->isCancelAction					= false;
	this->currentBattleNode					= nullptr;
	//SetInterval(static_cast<int>(BrawlerEnemyBehaviorTree::ActionType::ROAR), 1);
}

/// <summary>
/// �r�w�C�r�A�c���[���쐬
/// </summary>
void BrawlerEnemyBehaviorTree::CreateBehaviorTree()
{
	DeleteMemberInstance(this->mainNode);
	/*�匳�̃c���[�̍쐬*/
	this->mainNode = new SelectorNode();

	auto& json = Singleton<JsonManager>::GetInstance();
	/*�f�X�T�u�c���[*/
	BehaviorTreeNode* Sequencer_DyingIfHpIsLessThanZero = new SequencerNode();
	{
		//HP���O�Ȃ�f�X�A�N�V�������s��
		Sequencer_DyingIfHpIsLessThanZero->AddChild(*new Condition_IsHpBelowConstant(0));
		Sequencer_DyingIfHpIsLessThanZero->AddChild(*new Brawler_Dying());
	}

	/*���A�N�V�����T�u�c���[*/
	BehaviorTreeNode* Selector_NewOrAlreadyReaction = new SelectorNode();
	{
		//���łɃ��A�N�V�������I������Ă���Ȃ�A���̃��A�N�V������D�悷��
		BehaviorTreeNode* Sequencer_IfAlreadySelectedReaction = new SequencerNode();
		{
			Sequencer_IfAlreadySelectedReaction->AddChild(*new Condition_IsSelectedReaction());
			Sequencer_IfAlreadySelectedReaction->AddChild(*new Brawler_PlayCurrentReaction());
		}
		//�U�����󂯂Ă����烊�A�N�V�������s��
		BehaviorTreeNode* Sequencer_ReactionIfHitAttack = new SequencerNode();
		{
			Sequencer_ReactionIfHitAttack->AddChild(*new Condition_IsHitAttack());
			Sequencer_ReactionIfHitAttack->AddChild(*new Brawler_Reaction());
		}
		Selector_NewOrAlreadyReaction->AddChild(*Sequencer_IfAlreadySelectedReaction);
		Selector_NewOrAlreadyReaction->AddChild(*Sequencer_ReactionIfHitAttack);
	}

	/*�o�g���A�N�V�����T�u�c���[*/
	BehaviorTreeNode* Selector_AttackIfTargetGreaterThanConstant = new SelectorNode();
	{
		//�U�����������炻�̍U���𑱍s����
		BehaviorTreeNode* Sequencer_IfAlreadySelectedAttack = new SequencerNode();
		{
			Sequencer_IfAlreadySelectedAttack->AddChild(*new Condition_IsSelectedBattleAction());
			Sequencer_IfAlreadySelectedAttack->AddChild(*new Brawler_PlayCurrentBattleAction());
		}
		//�U���Ώۂ��U���͈̔͊O�ɂ����瑖��
		BehaviorTreeNode* Sequencer_WalkIfToTargetOutOfRange = new SequencerNode();
		{
			Sequencer_WalkIfToTargetOutOfRange->AddChild(*new Condition_IsToTargetDistanceGreaterThanConstant(json.GetJson(JsonManager::FileType::BRAWLER_ENEMY)["NEAR_ATTACK_RANGE"]));
			Sequencer_WalkIfToTargetOutOfRange->AddChild(*new Brawler_Run());
		}
		//�C���^�[�o�����c���Ă�����U���͍s��Ȃ�
		BehaviorTreeNode* Sequencer_AttackIfIntervalIsOver = new SequencerNode();
		{
			Sequencer_AttackIfIntervalIsOver->AddChild(*new Condition_IsActionIntervalIsOver(static_cast<int>(ActionType::ATTACK)));
			Sequencer_AttackIfIntervalIsOver->AddChild(*new Brawler_Attack());
		}
		Selector_AttackIfTargetGreaterThanConstant->AddChild(*Sequencer_IfAlreadySelectedAttack);
		Selector_AttackIfTargetGreaterThanConstant->AddChild(*Sequencer_WalkIfToTargetOutOfRange);
		Selector_AttackIfTargetGreaterThanConstant->AddChild(*Sequencer_AttackIfIntervalIsOver);
	}

	/*�ҋ@�A�N�V����*/
	BehaviorTreeNode* Selector_IdleIfTargetGreaterThanConstant = new SequencerNode();
	{
		Selector_IdleIfTargetGreaterThanConstant->AddChild(*new Condition_IsToTargetDistanceGreaterThanConstant(json.GetJson(JsonManager::FileType::BRAWLER_ENEMY)["ATTACK_MAX_RANGE"]));
		Selector_IdleIfTargetGreaterThanConstant->AddChild(*new Brawler_Idle());
	}

	/*�T�u�c���[��匳�̃c���[�ɓ����*/
	//�e�m�[�h�����ׂĒʂ������ʉ����A�N�V�������I������Ă��Ȃ����,IDLE��I������
	this->mainNode->AddChild(*Sequencer_DyingIfHpIsLessThanZero);
	this->mainNode->AddChild(*Selector_NewOrAlreadyReaction);
	this->mainNode->AddChild(*Selector_IdleIfTargetGreaterThanConstant);
	this->mainNode->AddChild(*Selector_AttackIfTargetGreaterThanConstant);
	this->mainNode->AddChild(*new Brawler_Idle());
}

/// <summary>
/// �����o�ϐ��̍X�V
/// </summary>
void BrawlerEnemyBehaviorTree::UpdateMemberVariables(Character& _chara)
{
	/*�C���^�[�o���̌v�Z*/
	for (int& interval : this->intervalSet)
	{
		if (interval != 0)
		{
			interval--;
		}
	}

	if (_chara.GetCharacterData().isHit)
	{
		int i = 0;
	}

	/*�ڕW�܂ł̋��������߂�*/
	auto& player = Singleton<PlayerManager>::GetInstance();
	VECTOR toTarget = VSub(player.GetRigidbody().GetPosition(), _chara.GetRigidbody().GetPosition());
	this->toTargetDistance = VSize(toTarget);

	/*�{�X�̌����Ă�������ƃv���C���[�̈ʒu�Ƃ̓��ς����*/
	VECTOR enemyDirection = VTransform(VGet(0.0f, 0.0f, -1.0f), MGetRotY(_chara.GetRigidbody().GetRotation().y));
	this->innerProductOfDirectionToTarget = VDot(enemyDirection, VNorm(VSub(_chara.GetRigidbody().GetPosition(), player.GetRigidbody().GetPosition())));

	this->prevHp = _chara.GetHP();
}

/// <summary>
/// �X�V
/// </summary>
void BrawlerEnemyBehaviorTree::Update(Character& _chara)
{
	/*�����o�ϐ��̍X�V*/
	UpdateMemberVariables(_chara);

	//printfDx("DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD:%f\n", this->innerProductOfDirectionToTarget);
	//printfDx("DRAGON_TO_TARGET:%f\n", this->toTargetDistance);
	//printfDx("DRAGON_ACTION_STATE:%d\n", this->currentAction);
	//printfDx("DRAGON_ACTION_SELECT:%d\n", this->isSelectedBattleAction);
	//printfDx("DRAGON_CANCEL_ACTION:%d\n", this->isCancelAction);

	/*�c���[�̎��s*/
	this->nodeState = this->mainNode->Update(*this, _chara);
	//this->nodeState = this->debugNode->Update(*this, _chara);
}

/// <summary>
/// �`��
/// </summary>
const void BrawlerEnemyBehaviorTree::Draw()const
{
#ifdef _DEBUG
	//this->debugActionNode->Draw();
#endif // _DEBUG

}