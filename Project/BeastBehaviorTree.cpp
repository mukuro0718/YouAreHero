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
/// コンストラクタ
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
	/*インターバルを初期化*/
	this->intervalSet = { 0 };

	/*大元のツリーの作成*/
	this->Selector_DeathOrReactionOrBattleOrBreak = new SelectorNode();

	/*HPがないときのアクション*/
	{
		//デス判定ノード
		BehaviorTreeNode* Sequencer_DeathIfHpIsLessThanZero = new SequencerNode();
		Sequencer_DeathIfHpIsLessThanZero->AddChild(*new Condition_IsHpBelowConstant(0));
		Sequencer_DeathIfHpIsLessThanZero->AddChild(*new Beast_Dying());
		//rootノードに追加
		this->Selector_DeathOrReactionOrBattleOrBreak->AddChild(*Sequencer_DeathIfHpIsLessThanZero);
	}

	/*リアクション中のアクション*/
	{
		//サブツリー
		BehaviorTreeNode* Selector_DownOrLongOrShortReaction = new SelectorNode();
		//サブツリーにぶら下げる子ノード
		{
			//リアクションが選択されていたら
			BehaviorTreeNode* Sequencer_PlayCurrentIfSelectedReaction = new SequencerNode();
			Sequencer_PlayCurrentIfSelectedReaction->AddChild(*new Condition_IsSelectedReaction());
			Sequencer_PlayCurrentIfSelectedReaction->AddChild(*new Beast_PlayCurrentReaction());
			//ダウン
			BehaviorTreeNode* Sequencer_DownIfDownValueMoreThanConstant = new SequencerNode();
			Sequencer_DownIfDownValueMoreThanConstant->AddChild(*new Condition_IsNowStateIsSameAsTheSpecifiedState(BeastState::DOWN));
			Sequencer_DownIfDownValueMoreThanConstant->AddChild(*new Beast_Down());
			//長い怯み
			BehaviorTreeNode* Sequencer_LongFrighteningIfFrighteningValueMoreThanConstant = new SequencerNode();
			Sequencer_LongFrighteningIfFrighteningValueMoreThanConstant->AddChild(*new Condition_IsFrighteningValueGreaterThanConstant(json.GetJson(JsonManager::FileType::BEAST)["SPECIFIED_FRIGHTENING_VALUE"]));
			Sequencer_LongFrighteningIfFrighteningValueMoreThanConstant->AddChild(*new Beast_LongFrightening());
			//短い怯み
			BehaviorTreeNode* Sequencer_ShortFrighteningIfPartDestroyed = new SequencerNode();
			Sequencer_ShortFrighteningIfPartDestroyed->AddChild(*new Condition_IsDestoroyedPart());
			Sequencer_ShortFrighteningIfPartDestroyed->AddChild(*new Beast_ShortFrightening());
			//サブツリー追加
			Selector_DownOrLongOrShortReaction->AddChild(*Sequencer_PlayCurrentIfSelectedReaction);
			Selector_DownOrLongOrShortReaction->AddChild(*Sequencer_DownIfDownValueMoreThanConstant);
			Selector_DownOrLongOrShortReaction->AddChild(*Sequencer_LongFrighteningIfFrighteningValueMoreThanConstant);
			Selector_DownOrLongOrShortReaction->AddChild(*Sequencer_ShortFrighteningIfPartDestroyed);
		}
		this->Selector_DeathOrReactionOrBattleOrBreak->AddChild(*Selector_DownOrLongOrShortReaction);
	}

	/*戦闘中のアクション*/
	{
		//攻撃を決めるサブツリー
		BehaviorTreeNode* Selector_ApproachOrSpecialOrLongRangeOrNearRangeAttack = new SelectorNode();

		//特殊攻撃サブツリー
		BehaviorTreeNode* Sequencer_SpecialActionIfIntervalIsOver = new SequencerNode();
		//特殊攻撃アクションセレクター
		BehaviorTreeNode* Selector_SpecialAction = new SelectorNode();
		//LEVELが最大だったらスーパーノヴァ
		BehaviorTreeNode* Sequencer_SuperNovaIfLevelIsMax = new SequencerNode();
		Sequencer_SuperNovaIfLevelIsMax->AddChild(*new Condition_IsNowLevelIsSameAsTheSpecifiedLevel(static_cast<int>(LevelStage::Lv4)));
		Sequencer_SuperNovaIfLevelIsMax->AddChild(*new Beast_SuperNova());
		//それ以外でインターバルが終了していたらレベル上昇アクション
		BehaviorTreeNode* Sequencer_RaiseLevelIfIntervalIsOver = new SequencerNode();
		Sequencer_RaiseLevelIfIntervalIsOver->AddChild(*new Condition_IsActionIntervalIsOver(static_cast<int>(ActionType::RAISE_LEVEL)));
		Sequencer_RaiseLevelIfIntervalIsOver->AddChild(*new Beast_RaiseLevel());
		//特殊攻撃アクションセレクターに追加
		Selector_SpecialAction->AddChild(*Sequencer_SuperNovaIfLevelIsMax);
		Selector_SpecialAction->AddChild(*Sequencer_RaiseLevelIfIntervalIsOver);
		//特殊攻撃サブツリーに代入
		Sequencer_SpecialActionIfIntervalIsOver->AddChild(*Selector_SpecialAction);
		
		//遠距離攻撃範囲外だったら歩く
		BehaviorTreeNode* Sequencer_ApproachIfTargetOutOfRangeOfLongRangeAttack = new SequencerNode();
		Sequencer_ApproachIfTargetOutOfRangeOfLongRangeAttack->AddChild(*new Condition_IsToTargetDistanceGreaterThanConstant(json.GetJson(JsonManager::FileType::BEAST)["FAR_ATTACK_RANGE"]));
		Sequencer_ApproachIfTargetOutOfRangeOfLongRangeAttack->AddChild(*new Beast_Walk());

		//遠距離攻撃サブツリー
		BehaviorTreeNode* Sequencer_LongRangeAttackIfTargetOutOfRangeOfNearRangeAttack = new SequencerNode();
		Sequencer_LongRangeAttackIfTargetOutOfRangeOfNearRangeAttack->AddChild(*new Condition_IsToTargetDistanceGreaterThanConstant(json.GetJson(JsonManager::FileType::BEAST)["NEAR_ATTACK_RANGE"]));
		//遠距離攻撃アクションセレクター
		BehaviorTreeNode* Selector_LongRangeAction = new SelectorNode();
		//怒り状態だったら突進
		BehaviorTreeNode* Sequencer_AngryLongRangeActionIfStateNormal = new SequencerNode();
		Sequencer_AngryLongRangeActionIfStateNormal->AddChild(*new Condition_IsNowStateIsSameAsTheSpecifiedState(BeastState::ANGRY));
		Sequencer_AngryLongRangeActionIfStateNormal->AddChild(*new Beast_Rush());
		//通常状態だったら弱ブレス
		BehaviorTreeNode* Sequencer_NormalLongRangeActionIfStateNormal = new SequencerNode();
		Sequencer_NormalLongRangeActionIfStateNormal->AddChild(*new Condition_IsNowStateIsSameAsTheSpecifiedState(BeastState::NORMAL));
		Sequencer_NormalLongRangeActionIfStateNormal->AddChild(*new Beast_WeakBreath());
		//遠距離攻撃アクションセレクターに各シーケンサノードを入れる
		Selector_LongRangeAction->AddChild(*Sequencer_AngryLongRangeActionIfStateNormal);
		Selector_LongRangeAction->AddChild(*Sequencer_NormalLongRangeActionIfStateNormal);
		//遠距離攻撃アクションセレクターを遠距離攻撃サブツリーに入れる
		Sequencer_LongRangeAttackIfTargetOutOfRangeOfNearRangeAttack->AddChild(*Selector_LongRangeAction);

		//近距離攻撃アクションセレクター(ここまで通ったということはプレイヤーは近接攻撃範囲内なのでConditionで評価はしない)
		BehaviorTreeNode* Selector_NearRangeAction = new SelectorNode();
		//前のアクションがIDLEだったら後退ブレス
		BehaviorTreeNode* Sequencer_BackingBreathIfSelectActionIsIdle = new SequencerNode();
		Sequencer_BackingBreathIfSelectActionIsIdle->AddChild(*new Condition_IsPrevActionIsSameAsSpecifiedAction(static_cast<int>(ActionType::IDLE)));
		Sequencer_BackingBreathIfSelectActionIsIdle->AddChild(*new Beast_BackingBreath());
		//プレイヤーがRayのそとだったら爆発攻撃
		BehaviorTreeNode* Sequencer_ExplositonIfTargetOutOfRangeRay = new SequencerNode();
		Sequencer_ExplositonIfTargetOutOfRangeRay->AddChild(*new Condition_IsTargetOutOfRangeOfRay(json.GetJson(JsonManager::FileType::BEAST)["TOLERANCE_RANGE"][0], json.GetJson(JsonManager::FileType::BEAST)["TOLERANCE_RANGE"][1]));
		Sequencer_ExplositonIfTargetOutOfRangeRay->AddChild(*new Beast_Explosion());
		//通常時の攻撃サブツリー
		BehaviorTreeNode* Sequencer_NormalNearActionIfStateNormal = new SequencerNode();
		Sequencer_NormalNearActionIfStateNormal->AddChild(*new Condition_IsNowStateIsSameAsTheSpecifiedState(BeastState::NORMAL));
		//通常状態だったらランダムで右弱攻撃かコンボ攻撃を出す
		BehaviorTreeNode* Selector_NormalStateAction = new RandomSelector();
		Selector_NormalStateAction->AddChild(*new Beast_RightFootAttack());
		Selector_NormalStateAction->AddChild(*new Beast_FootComboAttack());
		Selector_NormalStateAction->AddChild(*new Beast_Idle());
		//通常時の攻撃サブツリーに追加
		Sequencer_NormalNearActionIfStateNormal->AddChild(*Selector_NormalStateAction);
		//怒り時の攻撃サブツリー
		BehaviorTreeNode* Sequencer_AngryNearActionIfStateNormal = new SequencerNode();
		Sequencer_AngryNearActionIfStateNormal->AddChild(*new Condition_IsNowStateIsSameAsTheSpecifiedState(BeastState::ANGRY));
		//怒り状態だったら溜め右攻撃か両足溜め攻撃か右＋回転攻撃
		BehaviorTreeNode* Selector_AngryStateAction = new RandomSelector();
		Selector_AngryStateAction->AddChild(*new Beast_ChargeBothFootAttack());
		Selector_AngryStateAction->AddChild(*new Beast_ChargeRightFootAttack());
		//怒り時の攻撃サブツリーに追加
		Sequencer_AngryNearActionIfStateNormal->AddChild(*Selector_AngryStateAction);
		//近接攻撃アクションセレクターに各アクションを追加
		Selector_NearRangeAction->AddChild(*Sequencer_BackingBreathIfSelectActionIsIdle);
		Selector_NearRangeAction->AddChild(*Sequencer_ExplositonIfTargetOutOfRangeRay);
		Selector_NearRangeAction->AddChild(*Sequencer_NormalNearActionIfStateNormal);
		Selector_NearRangeAction->AddChild(*Sequencer_AngryNearActionIfStateNormal);
		//アクションが選択されているか
		BehaviorTreeNode* Sequencer_PlayCurrentIfSelectedBattleAction = new SequencerNode();
		Sequencer_PlayCurrentIfSelectedBattleAction->AddChild(*new Condition_IsSelectedBattleAction());
		Sequencer_PlayCurrentIfSelectedBattleAction->AddChild(*new Beast_PlayCurrentBattleAction());
		//咆哮アクション
		BehaviorTreeNode* Sequencer_RoarIfAngryValueGreaterThanConstant = new SequencerNode();
		Sequencer_RoarIfAngryValueGreaterThanConstant->AddChild(*new Condition_IsAngryValueGreaterThanConstant(json.GetJson(JsonManager::FileType::BEAST)["MAX_ANGRY_VALUE"]));
		Sequencer_RoarIfAngryValueGreaterThanConstant->AddChild(*new Beast_Roar());

		//攻撃を決めるサブツリーに各攻撃サブツリーを追加
		Selector_ApproachOrSpecialOrLongRangeOrNearRangeAttack->AddChild(*Sequencer_PlayCurrentIfSelectedBattleAction);
		Selector_ApproachOrSpecialOrLongRangeOrNearRangeAttack->AddChild(*Sequencer_RoarIfAngryValueGreaterThanConstant);
		Selector_ApproachOrSpecialOrLongRangeOrNearRangeAttack->AddChild(*Sequencer_SpecialActionIfIntervalIsOver);
		Selector_ApproachOrSpecialOrLongRangeOrNearRangeAttack->AddChild(*Sequencer_ApproachIfTargetOutOfRangeOfLongRangeAttack);
		Selector_ApproachOrSpecialOrLongRangeOrNearRangeAttack->AddChild(*Sequencer_LongRangeAttackIfTargetOutOfRangeOfNearRangeAttack);
		Selector_ApproachOrSpecialOrLongRangeOrNearRangeAttack->AddChild(*Selector_NearRangeAction);

		//rootノードに追加
		this->Selector_DeathOrReactionOrBattleOrBreak->AddChild(*Selector_ApproachOrSpecialOrLongRangeOrNearRangeAttack);
	}

	this->debugActionNode = new Beast_Down();
	
	/*初期化*/
	Initialize();
}

/// <summary>
/// デストラクタ
/// </summary>
BeastBehaviorTree::~BeastBehaviorTree()
{
	DeleteMemberInstance(this->Selector_DeathOrReactionOrBattleOrBreak);
	DeleteMemberInstance(this->currentBattleAction);
	DeleteMemberInstance(this->currentReaction);
	DeleteMemberInstance(this->debugActionNode);
}

/// <summary>
/// 初期化
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
/// メンバ変数の更新
/// </summary>
void BeastBehaviorTree::UpdateMemberVariables()
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
	auto& enemy = Singleton<EnemyManager>::GetInstance();
	VECTOR toTarget = VSub(player.GetRigidbody().GetPosition(), enemy.GetRigidbody().GetPosition());
	this->toTargetDistance = VSize(toTarget);

	/*HPの更新*/
	this->prevHp = enemy.GetHP();

	/*ボスの向いている方向とプレイヤーの位置との内積を取る*/
	VECTOR enemyDirection = VTransform(VGet(0.0f, 0.0f, -1.0f), MGetRotY(enemy.GetRigidbody().GetRotation().y));
	this->innerProductOfDirectionToTarget = VDot(enemyDirection, VNorm(VSub(enemy.GetRigidbody().GetPosition(), player.GetRigidbody().GetPosition())));
}

/// <summary>
/// 更新
/// </summary>
void BeastBehaviorTree::Update()
{
	/*メンバ変数の更新*/
	UpdateMemberVariables();
	ChangeState();


	/*ツリーの実行*/
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
/// 描画
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
/// バトルアクションの登録
/// </summary>
void BeastBehaviorTree::EntryCurrentBattleAction(BehaviorTreeNode& _action)
{
	this->currentBattleAction = &_action;
	this->isSelectedBattleAction = true; 
}
/// <summary>
/// リアクションアクションの登録
/// </summary>
void BeastBehaviorTree::EntryCurrentReaction(BehaviorTreeNode& _action)
{
	this->currentReaction = &_action;
	this->isSelectedReaction = true;
}
/// <summary>
/// リアクションアクションの解除
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
/// バトルアクションの解除
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
/// 状態の変更
/// </summary>
void BeastBehaviorTree::ChangeState()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& enemy = Singleton<EnemyManager>::GetInstance();

	/*怒り状態*/
	switch (this->state)
	{
		//怒り
	case BeastState::ANGRY:
		this->angryValue--;
		if (this->angryValue < 0)
		{
			this->state = BeastState::DOWN;
			this->angryValue = 0;
			this->downValue = json.GetJson(JsonManager::FileType::BEAST)["MAX_DOWN_VALUE"];
		}
		break;
		//通常
	case BeastState::NORMAL:
		//怒り値を増加
		this->angryValue++;
		//攻撃が当たっていたら怒り値をさらに増加
		
		if (enemy.GetCharacterData().isHit)
		{
			this->angryValue++;
		}
		//怒り値が最大以上だったら状態をANGRYにする
		if (this->angryValue >= json.GetJson(JsonManager::FileType::BEAST)["MAX_ANGRY_VALUE"])
		{
			this->angryValue = json.GetJson(JsonManager::FileType::BEAST)["MAX_ANGRY_VALUE"];
		}
		break;
		//疲れ
	case BeastState::DOWN:
		this->downValue -= 2;
		break;
	}
}