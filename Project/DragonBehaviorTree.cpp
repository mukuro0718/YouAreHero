#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "DragonBehaviorTreeHeader.h"
#include "Rigidbody.h"
#include "EnemyManager.h"
#include "Character.h"
#include "Enemy.h"
#include "Dragon.h"
#include "Player.h"
#include "PlayerManager.h"

/// <summary>
/// コンストラクタ
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

	/*大元のツリーの作成*/
	this->Selector_DyingOrBattleOrIdle = new SelectorNode();
	
	/*デスノード*/
	BehaviorTreeNode* Sequencer_DyingIfHpIsLessThanZero = new SequencerNode();
	{
		Sequencer_DyingIfHpIsLessThanZero->AddChild(*new Condition_IsDragonHpIsLessThanConstant(0));
		Sequencer_DyingIfHpIsLessThanZero->AddChild(*new Dragon_Dying());
	}

	/*バトルアクションサブツリー*/
	BehaviorTreeNode* Sequencer_BattleAction= new SequencerNode();
	//アクション選択サブツリー
	{
		//段階ごとの攻撃サブツリー
		{
			BehaviorTreeNode* Selector_StepByStepAttackAction = new SelectorNode();
			//3段階目の攻撃アクションサブツリー
			{
				BehaviorTreeNode* Selector_ThreeStepAttackAction = new SelectorNode();
				//遠距離攻撃範囲外だったら歩く
				BehaviorTreeNode* Sequencer_ApproachIfTargetOutOfRangeOfLongRangeAttack = new SequencerNode();
				Sequencer_ApproachIfTargetOutOfRangeOfLongRangeAttack->AddChild(*new Condition_IsDragonToTargetDistanceGreaterThanConstant(json.GetJson(JsonManager::FileType::DRAGON)["LONG_ATTACK_RANGE"]));
				Sequencer_ApproachIfTargetOutOfRangeOfLongRangeAttack->AddChild(*new Dragon_Walk());
				//近距離攻撃範囲外だったらブレス
				BehaviorTreeNode* Sequencer_BreathIfTargetOutOfRangeOfNearRangeAttack = new SequencerNode();
				Sequencer_BreathIfTargetOutOfRangeOfNearRangeAttack->AddChild(*new Condition_IsDragonToTargetDistanceGreaterThanConstant(json.GetJson(JsonManager::FileType::DRAGON)["NEAR_ATTACK_RANGE"]));
				Sequencer_BreathIfTargetOutOfRangeOfNearRangeAttack->AddChild(*new Dragon_Breath());
				//Rayの外だったら回転攻撃
				BehaviorTreeNode* Sequencer_RotateAttackIfTargetOutOfRangeRay = new SequencerNode();
				Sequencer_RotateAttackIfTargetOutOfRangeRay->AddChild(*new Condition_IsTargetOutOfRangeOfDragonRay(json.GetJson(JsonManager::FileType::DRAGON)["TOLERANCE_RANGE"][0], json.GetJson(JsonManager::FileType::DRAGON)["TOLERANCE_RANGE"][1]));
				Sequencer_RotateAttackIfTargetOutOfRangeRay->AddChild(*new Dragon_Rotate());
				//それ以外だったらランダムで選ぶ
				BehaviorTreeNode* Selector_RandomAttackAction = new RandomSelector();
				Selector_RandomAttackAction->AddChild(*new Dragon_Smash());	//叩きつけ
				Selector_RandomAttackAction->AddChild(*new Dragon_Sweep());	//なぎ払い
				Selector_RandomAttackAction->AddChild(*new Dragon_Rotate());	//回転攻撃
				Selector_RandomAttackAction->AddChild(*new Dragon_Breath());	//ブレス
				//3段階目の攻撃アクションサブツリーに追加
				Selector_ThreeStepAttackAction->AddChild(*Sequencer_ApproachIfTargetOutOfRangeOfLongRangeAttack);
				Selector_ThreeStepAttackAction->AddChild(*Sequencer_BreathIfTargetOutOfRangeOfNearRangeAttack);
				Selector_ThreeStepAttackAction->AddChild(*Sequencer_RotateAttackIfTargetOutOfRangeRay);
				Selector_ThreeStepAttackAction->AddChild(*Selector_RandomAttackAction);
				//３段階目の攻撃アクションサブツリーを選択する条件を追加
				BehaviorTreeNode* Sequencer_ThreeStepActionIfHpIsLessThanConstant = new SequencerNode();
				Sequencer_ThreeStepActionIfHpIsLessThanConstant->AddChild(*new Condition_IsDragonHpIsLessThanConstant(json.GetJson(JsonManager::FileType::DRAGON)["UNLEASHED_FURY"]));
				Sequencer_ThreeStepActionIfHpIsLessThanConstant->AddChild(*Selector_ThreeStepAttackAction);
				//段階ごとの攻撃サブツリーに追加
				Selector_StepByStepAttackAction->AddChild(*Sequencer_ThreeStepActionIfHpIsLessThanConstant);
			}
			//2段階目の攻撃アクションサブツリー
			{
				BehaviorTreeNode* Selector_TwoStepAttackAction = new SelectorNode();
				//遠距離攻撃範囲外だったら歩く
				BehaviorTreeNode* Sequencer_ApproachIfTargetOutOfRangeOfLongRangeAttack = new SequencerNode();
				Sequencer_ApproachIfTargetOutOfRangeOfLongRangeAttack->AddChild(*new Condition_IsDragonToTargetDistanceGreaterThanConstant(json.GetJson(JsonManager::FileType::DRAGON)["LONG_ATTACK_RANGE"]));
				Sequencer_ApproachIfTargetOutOfRangeOfLongRangeAttack->AddChild(*new Dragon_Walk());
				//近距離攻撃範囲外だったらブレス
				BehaviorTreeNode* Sequencer_BreathIfTargetOutOfRangeOfNearRangeAttack = new SequencerNode();
				Sequencer_BreathIfTargetOutOfRangeOfNearRangeAttack->AddChild(*new Condition_IsDragonToTargetDistanceGreaterThanConstant(json.GetJson(JsonManager::FileType::DRAGON)["NEAR_ATTACK_RANGE"]));
				Sequencer_BreathIfTargetOutOfRangeOfNearRangeAttack->AddChild(*new Dragon_Breath());
				//Rayの外だったら回転攻撃
				BehaviorTreeNode* Sequencer_RotateAttackIfTargetOutOfRangeRay = new SequencerNode();
				Sequencer_RotateAttackIfTargetOutOfRangeRay->AddChild(*new Condition_IsTargetOutOfRangeOfDragonRay(json.GetJson(JsonManager::FileType::BEAST)["TOLERANCE_RANGE"][0], json.GetJson(JsonManager::FileType::BEAST)["TOLERANCE_RANGE"][1]));
				Sequencer_RotateAttackIfTargetOutOfRangeRay->AddChild(*new Dragon_Rotate());
				//それ以外だったらランダムで選ぶ
				BehaviorTreeNode* Selector_RandomAttackAction = new RandomSelector();
				Selector_RandomAttackAction->AddChild(*new Dragon_Smash());	//叩きつけ
				Selector_RandomAttackAction->AddChild(*new Dragon_Sweep());	//なぎ払い
				//2段階目の攻撃アクションサブツリーに追加
				Selector_TwoStepAttackAction->AddChild(*Sequencer_ApproachIfTargetOutOfRangeOfLongRangeAttack);
				Selector_TwoStepAttackAction->AddChild(*Sequencer_BreathIfTargetOutOfRangeOfNearRangeAttack);
				Selector_TwoStepAttackAction->AddChild(*Sequencer_RotateAttackIfTargetOutOfRangeRay);
				Selector_TwoStepAttackAction->AddChild(*Selector_RandomAttackAction);
				//３段階目の攻撃アクションサブツリーを選択する条件を追加
				BehaviorTreeNode* Sequencer_TwoStepActionIfHpIsLessThanConstant = new SequencerNode();
				Sequencer_TwoStepActionIfHpIsLessThanConstant->AddChild(*new Condition_IsDragonHpIsLessThanConstant(json.GetJson(JsonManager::FileType::DRAGON)["THRESHOLD_OF_FURY"]));
				Sequencer_TwoStepActionIfHpIsLessThanConstant->AddChild(*Selector_TwoStepAttackAction);
				//段階ごとの攻撃サブツリーに追加
				Selector_StepByStepAttackAction->AddChild(*Sequencer_TwoStepActionIfHpIsLessThanConstant);
			}
			//1段階目の攻撃アクションサブツリー
			{
				BehaviorTreeNode* Selector_OneStepAttackAction = new SelectorNode();
				//近距離攻撃範囲外だったら歩く
				BehaviorTreeNode* Sequencer_ApproachIfTargetOutOfRangeOfNearRangeAttack = new SequencerNode();
				Sequencer_ApproachIfTargetOutOfRangeOfNearRangeAttack->AddChild(*new Condition_IsDragonToTargetDistanceGreaterThanConstant(json.GetJson(JsonManager::FileType::DRAGON)["NEAR_ATTACK_RANGE"]));
				Sequencer_ApproachIfTargetOutOfRangeOfNearRangeAttack->AddChild(*new Dragon_Walk());
				//それ以外だったらランダムで選ぶ
				BehaviorTreeNode* Selector_RandomAttackAction = new RandomSelector();
				Selector_RandomAttackAction->AddChild(*new Dragon_Smash());	//叩きつけ
				Selector_RandomAttackAction->AddChild(*new Dragon_Sweep());	//なぎ払い
				//1段階目の攻撃アクションサブツリーに追加
				Selector_OneStepAttackAction->AddChild(*Sequencer_ApproachIfTargetOutOfRangeOfNearRangeAttack);
				Selector_OneStepAttackAction->AddChild(*Selector_RandomAttackAction);
				//段階ごとの攻撃サブツリーに追加
				Selector_StepByStepAttackAction->AddChild(*Selector_OneStepAttackAction);
			}
			//バトルサブツリーに条件とアクション選択サブツリーを追加
			Sequencer_BattleAction->AddChild(*new Condition_IsDragonAttackCountLeft());
			Sequencer_BattleAction->AddChild(*Selector_StepByStepAttackAction);
		}
	}

	/*アクションが選択されているか*/
	BehaviorTreeNode* Sequencer_PlayCurrentIfSelectedBattleAction = new SequencerNode();
	{
		Sequencer_PlayCurrentIfSelectedBattleAction->AddChild(*new Condition_IsSelectedDragonBattleAction());
		Sequencer_PlayCurrentIfSelectedBattleAction->AddChild(*new Dragon_PlayCurrentBattleAction());
	}

	/*咆哮アクション*/
	SequencerNode* RoarIfDragonStageIsDifferent = new SequencerNode();
	{
		RoarIfDragonStageIsDifferent->AddChild(*new Condition_IsStageIsDifferent());
		RoarIfDragonStageIsDifferent->AddChild(*new Dragon_Roar());
	}

	/*ルートノードに各アクションツリーを挿入*/
	this->Selector_DyingOrBattleOrIdle->AddChild(*Sequencer_DyingIfHpIsLessThanZero);
	this->Selector_DyingOrBattleOrIdle->AddChild(*Sequencer_PlayCurrentIfSelectedBattleAction);
	this->Selector_DyingOrBattleOrIdle->AddChild(*RoarIfDragonStageIsDifferent);
	this->Selector_DyingOrBattleOrIdle->AddChild(*Sequencer_BattleAction);
	this->Selector_DyingOrBattleOrIdle->AddChild(*new Dragon_Idle());

	this->debugActionNode = new Dragon_Breath();

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
	this->state			= DragonState::NORMAL;
	this->currentStage	= DragonStage::AWAKENING;
	SetAttackCount();
}

/// <summary>
/// ボスの段階を判定
/// </summary>
bool DragonBehaviorTree::JudgeBossStage()
{
	auto& enemy = Singleton<EnemyManager>::GetInstance();
	int hp = enemy.GetHP();

	/*HPがリミット以下だったら段階を変える*/
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

	/*もし段階が今保存されている状態と異なっていたら状態を変える*/
	if (this->currentStage != newStage)
	{
		this->currentStage = newStage;
		return true;
	}
	return false;
}


/// <summary>
/// メンバ変数の更新
/// </summary>
void DragonBehaviorTree::UpdateMemberVariables()
{
	/*インターバルの計算*/
	for (auto interval : this->intervalSet)
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

	/*ボスの向いている方向とプレイヤーの位置との内積を取る*/
	VECTOR enemyDirection = VTransform(VGet(0.0f, 0.0f, -1.0f), MGetRotY(enemy.GetRigidbody().GetRotation().y));
	this->innerProductOfDirectionToTarget = VDot(enemyDirection, VNorm(VSub(enemy.GetRigidbody().GetPosition(), player.GetRigidbody().GetPosition())));

}

/// <summary>
/// 更新
/// </summary>
void DragonBehaviorTree::Update()
{
	/*メンバ変数の更新*/
	UpdateMemberVariables();

	printfDx("DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD:%f\n", this->innerProductOfDirectionToTarget);
	printfDx("DRAGON_TO_TARGET:%f\n", this->toTargetDistance);
	printfDx("DRAGON_ACTION_STATE:%d\n", this->currentAction);
	printfDx("DRAGON_ACTION_SELECT:%d\n", this->isSelectedBattleAction);
	printfDx("DRAGON_CANCEL_ACTION:%d\n", this->isCancelAction);

	/*ツリーの実行*/
	this->prevNodeState = this->Selector_DyingOrBattleOrIdle->Update();
	//this->prevNodeState = this->debugActionNode->Update();
}

/// <summary>
/// 描画
/// </summary>
const void DragonBehaviorTree::Draw()const
{
#ifdef _DEBUG
	this->debugActionNode->Draw();
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
/// バトルアクションの登録
/// </summary>
void DragonBehaviorTree::EntryCurrentBattleAction(BehaviorTreeNode& _action)
{
	this->currentBattleAction = &_action;
	this->isSelectedBattleAction = true;
}

/// <summary>
/// バトルアクションの解除
/// </summary>
void DragonBehaviorTree::ExitCurrentBattleAction()
{
	this->currentBattleAction = nullptr;
	this->isSelectedBattleAction = false;
}