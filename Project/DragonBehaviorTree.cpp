#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "DeleteInstance.h"
#include "Rigidbody.h"
#include "EnemyManager.h"
#include "Character.h"
#include "Enemy.h"
#include "DragonBehaviorTreeHeader.h"
#include "Dragon.h"
#include "Player.h"
#include "PlayerManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
DragonBehaviorTree::DragonBehaviorTree()
{
	/*インターバルを初期化*/
	this->intervalSet.clear();
	for (int i = 0; i < static_cast<int>(ActionType::BREATH) + 1; i++)
	{
		this->intervalSet.emplace_back(0);
	}

	/*デバック用ノード*/
	this->debugNode = new Dragon_Sweep();

	/*初期化*/
	Initialize();
}

/// <summary>
/// デストラクタ
/// </summary>
DragonBehaviorTree::~DragonBehaviorTree()
{
}

/// <summary>
/// 初期化
/// </summary>
void DragonBehaviorTree::Initialize()
{
	CreateBehaviorTree();
	auto& json								= Singleton<JsonManager>::GetInstance();
	this->prevHp							= json.GetJson(JsonManager::FileType::BEAST)["HP"];
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
	SetInterval(static_cast<int>(DragonBehaviorTree::ActionType::ROAR), 1);
}

/// <summary>
/// ビヘイビアツリーを作成
/// </summary>
void DragonBehaviorTree::CreateBehaviorTree()
{
	DeleteMemberInstance(this->mainNode);
	/*大元のツリーの作成*/
	this->mainNode = new SelectorNode();

	auto& json = Singleton<JsonManager>::GetInstance();
	/*デスノード*/
	BehaviorTreeNode* Sequencer_DyingIfHpIsLessThanZero = new SequencerNode();
	{
		Sequencer_DyingIfHpIsLessThanZero->AddChild(*new Condition_IsHpBelowConstant(0));
		Sequencer_DyingIfHpIsLessThanZero->AddChild(*new Dragon_Dying());
	}

	/*バトルアクションサブツリー*/
	BehaviorTreeNode* Sequencer_BattleActionIfAttackCountLeft = new SequencerNode();
	//アクション選択サブツリー
	{
		//遠距離攻撃サブツリー
		BehaviorTreeNode* Sequencer_LongRangeAttackIfToTargetOutOfRange = new SequencerNode();
		{
			BehaviorTreeNode* Selector_LongRangeAttack = new SelectorNode();
			//遠距離攻撃範囲外攻撃サブツリー
			BehaviorTreeNode* Sequencer_OutOfLongRangeActionIfToTargetOutOfRange = new SequencerNode();
			Sequencer_OutOfLongRangeActionIfToTargetOutOfRange->AddChild(*new Condition_IsToTargetDistanceGreaterThanConstant(json.GetJson(JsonManager::FileType::DRAGON)["LONG_ATTACK_RANGE"]));
			Sequencer_OutOfLongRangeActionIfToTargetOutOfRange->AddChild(*new Dragon_Walk());
			Selector_LongRangeAttack->AddChild(*Sequencer_OutOfLongRangeActionIfToTargetOutOfRange);
			//怒り状態時遠距離攻撃サブツリー
			BehaviorTreeNode* Sequencer_AngryLongRangeAttackIfBossStateAngry = new SequencerNode();
			Sequencer_AngryLongRangeAttackIfBossStateAngry->AddChild(*new Condition_IsNowStateIsSameAsTheSpecifiedState(static_cast<int>(Enemy::BossState::ANGRY)));
			Sequencer_AngryLongRangeAttackIfBossStateAngry->AddChild(*new Dragon_Breath());
			Selector_LongRangeAttack->AddChild(*Sequencer_AngryLongRangeAttackIfBossStateAngry);
			//通常時遠距離攻撃アクション
			Selector_LongRangeAttack->AddChild(*new Dragon_Walk());
			//遠距離攻撃サブツリーに追加
			Sequencer_LongRangeAttackIfToTargetOutOfRange->AddChild(*new Condition_IsToTargetDistanceGreaterThanConstant(json.GetJson(JsonManager::FileType::DRAGON)["NEAR_ATTACK_RANGE"]));
			Sequencer_LongRangeAttackIfToTargetOutOfRange->AddChild(*Selector_LongRangeAttack);
		}
		//範囲攻撃
		BehaviorTreeNode* Sequencer_RangeAttackIfTargetOutOfRay = new SequencerNode();
		{
			Sequencer_RangeAttackIfTargetOutOfRay->AddChild(*new Condition_IsNowStateIsSameAsTheSpecifiedState(static_cast<int>(Enemy::BossState::ANGRY)));
			Sequencer_RangeAttackIfTargetOutOfRay->AddChild(*new Condition_IsActionIntervalIsOver(static_cast<int>(ActionType::ROTATE)));
			Sequencer_RangeAttackIfTargetOutOfRay->AddChild(*new Condition_IsTargetOutOfRangeOfRay(json.GetJson(JsonManager::FileType::DRAGON)["TOLERANCE_RANGE"][0], json.GetJson(JsonManager::FileType::DRAGON)["TOLERANCE_RANGE"][1]));
			Sequencer_RangeAttackIfTargetOutOfRay->AddChild(*new Dragon_Rotate());
		}
		//それ以外だったら近接攻撃をランダムで選ぶ
		BehaviorTreeNode* Selector_RandomAttackAction = new RandomSelector();
		{
			Selector_RandomAttackAction->AddChild(*new Dragon_Smash());	//叩きつけ
			Selector_RandomAttackAction->AddChild(*new Dragon_Sweep());	//なぎ払い
			Selector_RandomAttackAction->AddChild(*new Dragon_Rotate());	//回転攻撃
		}
		BehaviorTreeNode* Selector_BattleAction = new SelectorNode();
		Selector_BattleAction->AddChild(*Sequencer_LongRangeAttackIfToTargetOutOfRange);
		Selector_BattleAction->AddChild(*Sequencer_RangeAttackIfTargetOutOfRay);
		Selector_BattleAction->AddChild(*Selector_RandomAttackAction);

		//バトルサブツリーに条件とアクション選択サブツリーを追加
		Sequencer_BattleActionIfAttackCountLeft->AddChild(*new Conditon_IsAttackCountIsOver());
		Sequencer_BattleActionIfAttackCountLeft->AddChild(*Selector_BattleAction);
	}

	/*アクションが選択されているか*/
	BehaviorTreeNode* Sequencer_PlayCurrentIfSelectedBattleAction = new SequencerNode();
	{
		Sequencer_PlayCurrentIfSelectedBattleAction->AddChild(*new Condition_IsSelectedBattleAction());
		Sequencer_PlayCurrentIfSelectedBattleAction->AddChild(*new Dragon_PlayCurrentBattleAction());
	}

	/*咆哮アクション*/
	SequencerNode* RoarIfIntervalIsOver = new SequencerNode();
	{
		RoarIfIntervalIsOver->AddChild(*new Condition_IsActionIntervalIsOver(static_cast<int>(ActionType::ROAR)));
		RoarIfIntervalIsOver->AddChild(*new Dragon_Roar());
	}

	/*ルートノードに各アクションツリーを挿入*/
	this->mainNode->AddChild(*Sequencer_DyingIfHpIsLessThanZero);
	this->mainNode->AddChild(*Sequencer_PlayCurrentIfSelectedBattleAction);
	this->mainNode->AddChild(*RoarIfIntervalIsOver);
	this->mainNode->AddChild(*Sequencer_BattleActionIfAttackCountLeft);
	this->mainNode->AddChild(*new Dragon_Idle());
}

/// <summary>
/// メンバ変数の更新
/// </summary>
void DragonBehaviorTree::UpdateMemberVariables()
{
	/*目標までの距離を求める*/
	auto& player = Singleton<PlayerManager>::GetInstance();
	auto& enemy = Singleton<EnemyManager>::GetInstance();
	VECTOR toTarget = VSub(player.GetRigidbody().GetPosition(), enemy.GetRigidbody().GetPosition());
	this->toTargetDistance = VSize(toTarget);

	/*ボスの向いている方向とプレイヤーの位置との内積を取る*/
	VECTOR enemyDirection = VTransform(VGet(0.0f, 0.0f, -1.0f), MGetRotY(enemy.GetRigidbody().GetRotation().y));
	this->innerProductOfDirectionToTarget = VDot(enemyDirection, VNorm(VSub(enemy.GetRigidbody().GetPosition(), player.GetRigidbody().GetPosition())));

	
}

/// <summary>
/// 更新
/// </summary>
void DragonBehaviorTree::Update(Character& _chara)
{
	/*メンバ変数の更新*/
	UpdateMemberVariables();

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
const void DragonBehaviorTree::Draw()const
{
#ifdef _DEBUG
	//this->debugActionNode->Draw();
#endif // _DEBUG

}