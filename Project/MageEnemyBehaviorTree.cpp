#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "DeleteInstance.h"
#include "Rigidbody.h"
#include "CharacterData.h"
#include "Character.h"
#include "Enemy.h"
#include "MageEnemyBehaviorTreeHeader.h"
#include "MageEnemy.h"
#include "Player.h"
#include "EnemyManager.h"
#include "PlayerManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
MageEnemyBehaviorTree::MageEnemyBehaviorTree()
{
	/*インターバルを初期化*/
	this->intervalSet.clear();
	for (int i = 0; i < static_cast<int>(ActionType::REACTION) + 1; i++)
	{
		this->intervalSet.emplace_back(0);
	}

	/*デバック用ノード*/
	//this->debugNode = new Dragon_Sweep();

	/*初期化*/
	Initialize();
}

/// <summary>
/// デストラクタ
/// </summary>
MageEnemyBehaviorTree::~MageEnemyBehaviorTree()
{
}

/// <summary>
/// 初期化
/// </summary>
void MageEnemyBehaviorTree::Initialize()
{
	CreateBehaviorTree();
	auto& json					= Singleton<JsonManager>::GetInstance();
	this->prevHp							= json.GetJson(JsonManager::FileType::MAGE_ENEMY)["HP"];
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
	//SetInterval(static_cast<int>(MageEnemyBehaviorTree::ActionType::ROAR), 1);
}

/// <summary>
/// ビヘイビアツリーを作成
/// </summary>
void MageEnemyBehaviorTree::CreateBehaviorTree()
{
	DeleteMemberInstance(this->mainNode);
	/*大元のツリーの作成*/
	this->mainNode = new SelectorNode();

	auto& json = Singleton<JsonManager>::GetInstance();
	/*デスサブツリー*/
	BehaviorTreeNode* Sequencer_DyingIfHpIsLessThanZero = new SequencerNode();
	{
		//HPが０ならデスアクションを行う
		Sequencer_DyingIfHpIsLessThanZero->AddChild(*new Condition_IsHpBelowConstant(0));
		Sequencer_DyingIfHpIsLessThanZero->AddChild(*new Mage_Dying());
	}

	/*リアクションサブツリー*/
	BehaviorTreeNode* Selector_NewOrAlreadyReaction = new SelectorNode();
	{
		//すでにリアクションが選択されているなら、そのリアクションを優先する
		BehaviorTreeNode* Sequencer_IfAlreadySelectedReaction = new SequencerNode();
		{
			Sequencer_IfAlreadySelectedReaction->AddChild(*new Condition_IsSelectedReaction());
			Sequencer_IfAlreadySelectedReaction->AddChild(*new Mage_PlayCurrentReaction());
		}
		//攻撃を受けていたらリアクションを行う
		BehaviorTreeNode* Sequencer_ReactionIfHitAttack = new SequencerNode();
		{
			Sequencer_ReactionIfHitAttack->AddChild(*new Condition_IsHitAttack());
			Sequencer_ReactionIfHitAttack->AddChild(*new Mage_Reaction());
		}
		Selector_NewOrAlreadyReaction->AddChild(*Sequencer_IfAlreadySelectedReaction);
		Selector_NewOrAlreadyReaction->AddChild(*Sequencer_ReactionIfHitAttack);
	}

	/*バトルアクションサブツリー*/
	BehaviorTreeNode* Selector_AttackIfTargetGreaterThanConstant = new SelectorNode();
	{
		//攻撃中だったらその攻撃を続行する
		BehaviorTreeNode* Sequencer_IfAlreadySelectedAttack = new SequencerNode();
		{
			Sequencer_IfAlreadySelectedAttack->AddChild(*new Condition_IsSelectedBattleAction());
			Sequencer_IfAlreadySelectedAttack->AddChild(*new Mage_PlayCurrentBattleAction());
		}
		//攻撃権があれば以下の行動を行う
		BehaviorTreeNode* Sequencer_IfCanAttack = new SequencerNode();
		{
			//攻撃対象が攻撃範囲外にいたら走る
			BehaviorTreeNode* Sequencer_WalkIfToTargetOutOfRange = new SequencerNode();
			{
				Sequencer_WalkIfToTargetOutOfRange->AddChild(*new Condition_IsToTargetDistanceGreaterThanConstant(json.GetJson(JsonManager::FileType::MAGE_ENEMY)["NEAR_ATTACK_RANGE"]));
				Sequencer_WalkIfToTargetOutOfRange->AddChild(*new Mage_Run());
			}
			//インターバルが残っていたら攻撃は行わない
			BehaviorTreeNode* Sequencer_AttackIfIntervalIsOver = new SequencerNode();
			{
				Sequencer_AttackIfIntervalIsOver->AddChild(*new Condition_IsActionIntervalIsOver(static_cast<int>(ActionType::ATTACK)));
				Sequencer_AttackIfIntervalIsOver->AddChild(*new Mage_Attack());
			}
			//攻撃権があるときの行動を選ぶ
			BehaviorTreeNode* Selector_AttackAction = new SelectorNode();
			Selector_AttackAction->AddChild(*Sequencer_WalkIfToTargetOutOfRange);
			Selector_AttackAction->AddChild(*Sequencer_AttackIfIntervalIsOver);

			Sequencer_IfCanAttack->AddChild(*new Condition_IsCanAttack());
			Sequencer_IfCanAttack->AddChild(*Selector_AttackAction);
		}
		Selector_AttackIfTargetGreaterThanConstant->AddChild(*Sequencer_IfAlreadySelectedAttack);
		Selector_AttackIfTargetGreaterThanConstant->AddChild(*Sequencer_IfCanAttack);
		Selector_AttackIfTargetGreaterThanConstant->AddChild(*new Mage_Walk());
	}

	/*待機アクション*/
	BehaviorTreeNode* Selector_IdleIfTargetGreaterThanConstant = new SequencerNode();
	{
		Selector_IdleIfTargetGreaterThanConstant->AddChild(*new Condition_IsToTargetDistanceGreaterThanConstant(json.GetJson(JsonManager::FileType::MAGE_ENEMY)["ATTACK_MAX_RANGE"]));
		Selector_IdleIfTargetGreaterThanConstant->AddChild(*new Mage_Idle());
	}

	/*サブツリーを大元のツリーに入れる*/
	//各ノードをすべて通った結果何もアクションが選択されていなければ,IDLEを選択する
	this->mainNode->AddChild(*Sequencer_DyingIfHpIsLessThanZero);
	this->mainNode->AddChild(*Selector_NewOrAlreadyReaction);
	this->mainNode->AddChild(*Selector_IdleIfTargetGreaterThanConstant);
	this->mainNode->AddChild(*Selector_AttackIfTargetGreaterThanConstant);
	this->mainNode->AddChild(*new Mage_Idle());
}

/// <summary>
/// メンバ変数の更新
/// </summary>
void MageEnemyBehaviorTree::UpdateMemberVariables(Character& _chara)
{
	/*インターバルの計算*/
	for (int& interval : this->intervalSet)
	{
		if (interval != 0)
		{
			interval--;
		}
	}

	/*目標までの距離を求める*/
	auto& player = Singleton<PlayerManager>::GetInstance();
	VECTOR toTarget = VSub(player.GetRigidbody().GetPosition(), _chara.GetRigidbody().GetPosition());
	this->toTargetDistance = VSize(toTarget);

	/*ボスの向いている方向とプレイヤーの位置との内積を取る*/
	VECTOR enemyDirection = VTransform(VGet(0.0f, 0.0f, -1.0f), MGetRotY(_chara.GetRigidbody().GetRotation().y));
	this->innerProductOfDirectionToTarget = VDot(enemyDirection, VNorm(VSub(_chara.GetRigidbody().GetPosition(), player.GetRigidbody().GetPosition())));

	this->prevHp = _chara.GetHP();
}

/// <summary>
/// 更新
/// </summary>
void MageEnemyBehaviorTree::Update(Character& _chara)
{
	/*メンバ変数の更新*/
	UpdateMemberVariables(_chara);

	//printfDx("DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD:%f\n", this->innerProductOfDirectionToTarget);
	//printfDx("DRAGON_TO_TARGET:%f\n", this->toTargetDistance);
	//printfDx("DRAGON_ACTION_STATE:%d\n", this->currentAction);
	//printfDx("DRAGON_ACTION_SELECT:%d\n", this->isSelectedBattleAction);
	//printfDx("DRAGON_CANCEL_ACTION:%d\n", this->isCancelAction);

	/*ツリーの実行*/
	this->nodeState = this->mainNode->Update(*this, _chara);

	//this->prevNodeState = this->debugActionNode->Update();
}

/// <summary>
/// 描画
/// </summary>
const void MageEnemyBehaviorTree::Draw()const
{
#ifdef _DEBUG
	//this->debugActionNode->Draw();
#endif // _DEBUG

}