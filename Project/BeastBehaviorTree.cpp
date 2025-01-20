#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "BeastBehaviorTreeHeader.h"
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
	, prevNodeState								(BehaviorTreeNode::NodeState::NONE_ACTIVE)
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
		//サブツリー
		BehaviorTreeNode* Selector_ApproachOrSpecialOrLongRangeOrNearRangeAttack = new SelectorNode();

		//移動サブツリー
		BehaviorTreeNode* Sequencer_ApproachIfTargetOutOfRangeOfLongRangeAttack = new SequencerNode();
		{
			//突撃
			BehaviorTreeNode* Sequencer_RushIfIntervalOver = new SequencerNode();
			Sequencer_RushIfIntervalOver->AddChild(*new Condition_IsActionIntervalIsOver(static_cast<int>(ActionType::RUSH)));
			Sequencer_RushIfIntervalOver->AddChild(*new Beast_Rush());
			//歩き
			BehaviorTreeNode* Sequencer_WalkIfStateNormal = new SequencerNode();
			Sequencer_RushIfIntervalOver->AddChild(*new Condition_IsNowStateIsSameAsTheSpecifiedState(BeastState::NORMAL));
			Sequencer_RushIfIntervalOver->AddChild(*new Beast_Walk());
			//アクション選択ノード
			BehaviorTreeNode* Selector_TurnOrRushOrWalkOrRun = new SelectorNode();
			Sequencer_RushIfIntervalOver->AddChild(*Sequencer_RushIfIntervalOver);
			Sequencer_RushIfIntervalOver->AddChild(*Sequencer_WalkIfStateNormal);
			Sequencer_RushIfIntervalOver->AddChild(*new Beast_Run());
			//移動サブツリーに追加
			Sequencer_ApproachIfTargetOutOfRangeOfLongRangeAttack->AddChild(*new Condition_IsToTargetDistanceGreaterThanConstant(json.GetJson(JsonManager::FileType::BEAST)["FAR_ATTACK_RANGE"]));
			Sequencer_ApproachIfTargetOutOfRangeOfLongRangeAttack->AddChild(*Selector_TurnOrRushOrWalkOrRun);
		}
		//回転
		BehaviorTreeNode* Sequencer_TurnIfTargetOutOfRangeRay = new SequencerNode();
		Sequencer_TurnIfTargetOutOfRangeRay->AddChild(*new Condition_IsTargetOutOfRangeOfRay(json.GetJson(JsonManager::FileType::BEAST)["TOLERANCE_RANGE"][0], json.GetJson(JsonManager::FileType::BEAST)["TOLERANCE_RANGE"][1]));
		Sequencer_TurnIfTargetOutOfRangeRay->AddChild(*new Beast_TurnLeft());
		//特殊攻撃サブツリー
		BehaviorTreeNode* Sequencer_SpecialActionIfIntervalIsOver = new SequencerNode();
		{
			//スーパーノヴァ
			BehaviorTreeNode* Sequencer_SuperNovaIfLevelIsMax = new SequencerNode();
			Sequencer_SuperNovaIfLevelIsMax->AddChild(*new Condition_IsNowLevelIsSameAsTheSpecifiedLevel(static_cast<int>(LevelStage::Lv4)));
			Sequencer_SuperNovaIfLevelIsMax->AddChild(*new Beast_SuperNova());
			//アクション選択ノード
			BehaviorTreeNode* Selector_SuperNovaOrRaiseLevel = new SelectorNode();
			Selector_SuperNovaOrRaiseLevel->AddChild(*Sequencer_SuperNovaIfLevelIsMax);
			Selector_SuperNovaOrRaiseLevel->AddChild(*new Beast_RaiseLevel());
			//特殊攻撃サブツリーに追加
			Sequencer_SpecialActionIfIntervalIsOver->AddChild(*new Condition_IsActionIntervalIsOver(static_cast<int>(ActionType::RAISE_LEVEL)));
			Sequencer_SpecialActionIfIntervalIsOver->AddChild(*Selector_SuperNovaOrRaiseLevel);
		}
		//遠距離攻撃サブツリー
		BehaviorTreeNode* Sequencer_LongRangeAttackIfTargetOutOfRangeOfNearRangeAttack = new SequencerNode();
		{
			//ブレス
			BehaviorTreeNode* Sequencer_BreathIfLevelFourAndIntervalIsOver = new SequencerNode();
			Sequencer_BreathIfLevelFourAndIntervalIsOver->AddChild(*new Condition_IsNowLevelIsSameAsTheSpecifiedLevel(static_cast<int>(LevelStage::Lv3)));
			Sequencer_BreathIfLevelFourAndIntervalIsOver->AddChild(*new Condition_IsActionIntervalIsOver(static_cast<int>(ActionType::BREATH)));
			Sequencer_BreathIfLevelFourAndIntervalIsOver->AddChild(*new Beast_Breath());
			//後退しながらブレス
			BehaviorTreeNode* Sequencer_BackingIfLevelFourAndIntervalIsOver = new SequencerNode();
			Sequencer_BackingIfLevelFourAndIntervalIsOver->AddChild(*new Condition_IsNowLevelIsSameAsTheSpecifiedLevel(static_cast<int>(LevelStage::Lv2)));
			Sequencer_BackingIfLevelFourAndIntervalIsOver->AddChild(*new Condition_IsActionIntervalIsOver(static_cast<int>(ActionType::BACKING_BREATH)));
			Sequencer_BackingIfLevelFourAndIntervalIsOver->AddChild(*new Beast_BackingBreath());
			//弱ブレス
			BehaviorTreeNode* Sequencer_WeakIfLevelFourAndIntervalIsOver = new SequencerNode();
			Sequencer_WeakIfLevelFourAndIntervalIsOver->AddChild(*new Condition_IsNowLevelIsSameAsTheSpecifiedLevel(static_cast<int>(LevelStage::Lv1)));
			Sequencer_WeakIfLevelFourAndIntervalIsOver->AddChild(*new Condition_IsActionIntervalIsOver(static_cast<int>(ActionType::WEAK_BREATH)));
			Sequencer_WeakIfLevelFourAndIntervalIsOver->AddChild(*new Beast_WeakBreath());
			//アクション選択ノード
			BehaviorTreeNode* Selector_BreathOrBackingOrExplosionOrWeak = new SelectorNode();
			Selector_BreathOrBackingOrExplosionOrWeak->AddChild(*Sequencer_BreathIfLevelFourAndIntervalIsOver);
			Selector_BreathOrBackingOrExplosionOrWeak->AddChild(*Sequencer_BackingIfLevelFourAndIntervalIsOver);
			Selector_BreathOrBackingOrExplosionOrWeak->AddChild(*Sequencer_WeakIfLevelFourAndIntervalIsOver);
			//遠距離攻撃サブツリーに追加
			Sequencer_LongRangeAttackIfTargetOutOfRangeOfNearRangeAttack->AddChild(*new Condition_IsToTargetDistanceGreaterThanConstant(json.GetJson(JsonManager::FileType::BEAST)["NEAR_ATTACK_RANGE"]));
			Sequencer_LongRangeAttackIfTargetOutOfRangeOfNearRangeAttack->AddChild(*Selector_BreathOrBackingOrExplosionOrWeak);
		}
		//近距離攻撃サブツリー
		BehaviorTreeNode* Selector_WalkOrNormalActionOrAngryAction = new SelectorNode();
		{
			//ステップバック
			BehaviorTreeNode* Sequencer_StepBackIfIntervalIsOver = new SequencerNode();
			Sequencer_StepBackIfIntervalIsOver->AddChild(*new Condition_IsActionIntervalIsOver(static_cast<int>(ActionType::STEP_BACK)));
			Sequencer_StepBackIfIntervalIsOver->AddChild(*new Beast_StepBack());
			//右足攻撃
			BehaviorTreeNode* Sequencer_RightAttackIfIntervalIsOver = new SequencerNode();
			Sequencer_RightAttackIfIntervalIsOver->AddChild(*new Condition_IsActionIntervalIsOver(static_cast<int>(ActionType::RIGHT_FOOT_ATTACK)));
			Sequencer_RightAttackIfIntervalIsOver->AddChild(*new Beast_RightFootAttack());
			//コンボアタック
			BehaviorTreeNode* Sequencer_ComboAttackIfIntervalIsOver = new SequencerNode();
			Sequencer_ComboAttackIfIntervalIsOver->AddChild(*new Condition_IsActionIntervalIsOver(static_cast<int>(ActionType::COMBO_ATTACK)));
			Sequencer_ComboAttackIfIntervalIsOver->AddChild(*new Beast_FootComboAttack());
			//右溜め攻撃
			BehaviorTreeNode* Sequencer_ChargeRightAttackIfIntervalIsOver = new SequencerNode();
			Sequencer_ChargeRightAttackIfIntervalIsOver->AddChild(*new Condition_IsActionIntervalIsOver(static_cast<int>(ActionType::CHARGE_RIGHT_ATTACK)));
			Sequencer_ChargeRightAttackIfIntervalIsOver->AddChild(*new Beast_ChargeRightFootAttack());
			//両足攻撃
			BehaviorTreeNode* Sequencer_BothAttackIfIntervalIsOver = new SequencerNode();
			Sequencer_BothAttackIfIntervalIsOver->AddChild(*new Condition_IsActionIntervalIsOver(static_cast<int>(ActionType::CHARGE_BOTH_ATTACK)));
			Sequencer_BothAttackIfIntervalIsOver->AddChild(*new Beast_ChargeBothFootAttack());
			//右足後回転攻撃
			BehaviorTreeNode* Sequencer_RightAndRotAttackIfIntervalIsOver = new SequencerNode();
			Sequencer_RightAndRotAttackIfIntervalIsOver->AddChild(*new Condition_IsActionIntervalIsOver(static_cast<int>(ActionType::EXPLOSION)));
			Sequencer_RightAndRotAttackIfIntervalIsOver->AddChild(*new Beast_Explosion());

			//通常時の攻撃ノード
			BehaviorTreeNode* Selector_RightOrComboAttack = new SelectorNode();
			Selector_RightOrComboAttack->AddChild(*Sequencer_RightAttackIfIntervalIsOver);
			Selector_RightOrComboAttack->AddChild(*Sequencer_ComboAttackIfIntervalIsOver);
			//怒り時の攻撃ノード
			BehaviorTreeNode* Selector_ChargeOrBothOrRightAndRotAttack = new SelectorNode();
			//Selector_ChargeOrBothOrRightAndRotAttack->AddChild(*Sequencer_StepBackIfIntervalIsOver);
			Selector_ChargeOrBothOrRightAndRotAttack->AddChild(*Sequencer_BothAttackIfIntervalIsOver);
			Selector_ChargeOrBothOrRightAndRotAttack->AddChild(*Sequencer_RightAndRotAttackIfIntervalIsOver);

			//通常時のアクション選択ノード
			BehaviorTreeNode* Sequencer_NormalActionIfStateNormal = new SequencerNode();
			Sequencer_NormalActionIfStateNormal->AddChild(*new Condition_IsNowStateIsSameAsTheSpecifiedState(BeastState::NORMAL));
			Sequencer_NormalActionIfStateNormal->AddChild(*Selector_RightOrComboAttack);
			//怒り時のアクション選択ノード
			BehaviorTreeNode* Sequencer_AngryActionIfStateNormal = new SequencerNode();
			Sequencer_AngryActionIfStateNormal->AddChild(*new Condition_IsNowStateIsSameAsTheSpecifiedState(BeastState::ANGRY));
			Sequencer_AngryActionIfStateNormal->AddChild(*Selector_ChargeOrBothOrRightAndRotAttack);
			//歩き
			BehaviorTreeNode* Sequencer_WalkIfTargetOutOfRangeOfNearRangeAttack = new SequencerNode();
			Sequencer_WalkIfTargetOutOfRangeOfNearRangeAttack->AddChild(*new Condition_IsToTargetDistanceGreaterThanConstant(json.GetJson(JsonManager::FileType::BEAST)["NEAR_ATTACK_RANGE"]));
			Sequencer_WalkIfTargetOutOfRangeOfNearRangeAttack->AddChild(*new Beast_Walk());
			//アクション選択ノード
			Selector_WalkOrNormalActionOrAngryAction->AddChild(*Sequencer_WalkIfTargetOutOfRangeOfNearRangeAttack);
			Selector_WalkOrNormalActionOrAngryAction->AddChild(*Sequencer_NormalActionIfStateNormal);
			Selector_WalkOrNormalActionOrAngryAction->AddChild(*Sequencer_AngryActionIfStateNormal);
		}
		//アクションが選択されているか
		BehaviorTreeNode* Sequencer_PlayCurrentIfSelectedBattleAction = new SequencerNode();
		Sequencer_PlayCurrentIfSelectedBattleAction->AddChild(*new Condition_IsSelectedBattleAction());
		Sequencer_PlayCurrentIfSelectedBattleAction->AddChild(*new Beast_PlayCurrentBattleAction());
		//咆哮アクション
		BehaviorTreeNode* Sequencer_RoarIfAngryValueGreaterThanConstant = new SequencerNode();
		Sequencer_RoarIfAngryValueGreaterThanConstant->AddChild(*new Condition_IsAngryValueGreaterThanConstant(json.GetJson(JsonManager::FileType::BEAST)["MAX_ANGRY_VALUE"]));
		Sequencer_RoarIfAngryValueGreaterThanConstant->AddChild(*new Beast_Roar());
		//サブツリーに追加
		Selector_ApproachOrSpecialOrLongRangeOrNearRangeAttack->AddChild(*Sequencer_PlayCurrentIfSelectedBattleAction);
		Selector_ApproachOrSpecialOrLongRangeOrNearRangeAttack->AddChild(*Sequencer_RoarIfAngryValueGreaterThanConstant);
		Selector_ApproachOrSpecialOrLongRangeOrNearRangeAttack->AddChild(*Sequencer_TurnIfTargetOutOfRangeRay);
		Selector_ApproachOrSpecialOrLongRangeOrNearRangeAttack->AddChild(*Sequencer_ApproachIfTargetOutOfRangeOfLongRangeAttack);
		Selector_ApproachOrSpecialOrLongRangeOrNearRangeAttack->AddChild(*Sequencer_SpecialActionIfIntervalIsOver);
		Selector_ApproachOrSpecialOrLongRangeOrNearRangeAttack->AddChild(*Sequencer_LongRangeAttackIfTargetOutOfRangeOfNearRangeAttack);
		Selector_ApproachOrSpecialOrLongRangeOrNearRangeAttack->AddChild(*Selector_WalkOrNormalActionOrAngryAction);

		//rootノードに追加
		this->Selector_DeathOrReactionOrBattleOrBreak->AddChild(*Sequencer_RoarIfAngryValueGreaterThanConstant);
		this->Selector_DeathOrReactionOrBattleOrBreak->AddChild(*Selector_ApproachOrSpecialOrLongRangeOrNearRangeAttack);
		this->Selector_DeathOrReactionOrBattleOrBreak->AddChild(*new Beast_Idle());
	}

	this->debugActionNode = new Beast_BackingBreath();
	
	/*初期化*/
	Initialize();
}

/// <summary>
/// デストラクタ
/// </summary>
BeastBehaviorTree::~BeastBehaviorTree()
{

}

/// <summary>
/// 初期化
/// </summary>
void BeastBehaviorTree::Initialize()
{
	auto& json				= Singleton<JsonManager>::GetInstance();
	this->prevHp			= json.GetJson(JsonManager::FileType::BEAST)["HP"];
	this->state				= BeastState::NORMAL;
	this->downValue			= 0;
	this->damage			= 0;
	this->level				= 0;
	this->toTargetDistance	= 0;
	this->selectAction		= -1;
	this->isDestroyedPart	= false;
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

#ifdef _DEBUG
	printfDx("DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD:%f\n", this->innerProductOfDirectionToTarget);
	printfDx("BEAST_TO_TARGET:%f\n", this->toTargetDistance);
	printfDx("BEAST_ACTION_STATE:%d\n", this->selectAction);
#endif // _DEBUG

	/*ツリーの実行*/
	this->prevNodeState = this->Selector_DeathOrReactionOrBattleOrBreak->Update();
	//this->prevNodeState = this->debugActionNode->Update();
}

/// <summary>
/// 描画
/// </summary>
const void BeastBehaviorTree::Draw()const
{
#ifdef _DEBUG
	if (this->currentBattleAction)
	{
		this->currentBattleAction->Draw();
	}
	this->debugActionNode->Draw();
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
	this->currentReaction->Initialize();
	this->currentReaction = nullptr; 
	this->isSelectedReaction = false; 
}
/// <summary>
/// バトルアクションの解除
/// </summary>
void BeastBehaviorTree::ExitCurrentBattleAction() 
{ 
	this->currentBattleAction->Initialize();
	this->currentBattleAction = nullptr;
	this->isSelectedBattleAction = false; 
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
			//MV1SetTextureGraphHandle(this->modelHandle, 0, this->angryTexture, FALSE);
		}
		break;
		//疲れ
	case BeastState::DOWN:
		//疲れ時間を増加
		this->downValue--;
		////ダウン値が０になったら状態を通常にする
		//if (this->downValue <= 0 )
		//{
		//	this->state = BeastState::NORMAL;
		//	//MV1SetTextureGraphHandle(this->modelHandle, 0, this->normalTexture, FALSE);
		//}
		break;
	}
}