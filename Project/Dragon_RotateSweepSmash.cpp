#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "BehaviorTreeNode.h"
#include "ActionNode.h"
#include "Dragon_RotateSweepSmash.h"
#include "Character.h"
#include "Enemy.h"
#include "Dragon.h"
#include "EnemyManager.h"
#include "DragonBehaviorTree.h"

/// <summary>
/// コンストラクタ
/// </summary>
Dragon_RotateSweepSmash::Dragon_RotateSweepSmash()
	: useAnimationType(0)
	, maxUseAnimation(0)
	, nearAttackRange(0.0f)
	, sweepCancelPlayTime(0.0f)
	, totalPlayTime(0.0f)
{
	/*使用するアニメーション*/
	this->animationType.emplace_back(static_cast<int>(Dragon::AnimationType::WALK));
	this->animationType.emplace_back(static_cast<int>(Dragon::AnimationType::ROTATE_ATTACK));
	this->animationType.emplace_back(static_cast<int>(Dragon::AnimationType::WALK));
	this->animationType.emplace_back(static_cast<int>(Dragon::AnimationType::SWEEP));
	this->animationType.emplace_back(static_cast<int>(Dragon::AnimationType::SMASH));

	/*使用するアニメーションの再生時間*/
	auto& json = Singleton<JsonManager>::GetInstance();
	vector<float> playTime		= json.GetJson(JsonManager::FileType::DRAGON)["ROTATE_SWEEP_SMASH_PLAY_TIME"];
	this->animationPlayTime		= playTime;
	this->maxUseAnimation		= static_cast<int>(UseAnimationType::SMASH);
	this->actionType			= static_cast<short>(DragonBehaviorTree::ActionType::ROTATE_SWEEP_SMASH);
	this->maxSpeed				= json.GetJson(JsonManager::FileType::DRAGON)["WALK_SPEED"];
	this->accel					= json.GetJson(JsonManager::FileType::DRAGON)["ACCEL"];
	this->decel					= json.GetJson(JsonManager::FileType::DRAGON)["DECEL"];
	this->nearAttackRange		= json.GetJson(JsonManager::FileType::DRAGON)["NEAR_ATTACK_RANGE"];
	this->sweepCancelPlayTime	= json.GetJson(JsonManager::FileType::DRAGON)["SWEEP_CANCEL_PLAY_TIME"];
}

/// <summary>
/// デストラクタ
/// </summary>
Dragon_RotateSweepSmash::~Dragon_RotateSweepSmash()
{

}

/// <summary>
/// 更新処理
/// </summary>
Dragon_RotateSweepSmash::NodeState Dragon_RotateSweepSmash::Update()
{
	/*移動処理*/
	auto& enemyManager = Singleton<EnemyManager>::GetInstance();
	auto& enemy = dynamic_cast<Dragon&>(enemyManager.GetCharacter());
	short walkIndex1 = static_cast<short>(UseAnimationType::WALK_1);
	short walkIndex2 = static_cast<short>(UseAnimationType::WALK_2);
	if (this->useAnimationType == walkIndex1 || this->useAnimationType == walkIndex2)
	{
		enemy.Move(this->maxSpeed, this->accel, this->decel, false);
	}
	else
	{
		enemy.UpdateSpeed(0.0f, this->accel, this->decel);
		enemy.UpdateVelocity(false);
	}

	/*アクションの状態をセット*/
	auto& rootNode = Singleton<DragonBehaviorTree>::GetInstance();
	if (rootNode.GetCurrentAction() != this->actionType)
	{
		//アクションの設定
		rootNode.SetCurrentAction(this->actionType);
		//アクションの登録
		rootNode.EntryCurrentBattleAction(*this);
	}

	/*アニメーションの再生*/
	float playTime = this->animationPlayTime[this->useAnimationType];
	enemy.PlayAnimation(this->animationType[this->useAnimationType], playTime);

	/*状態を返す*/
	//移動状態かつ近接攻撃範囲なら次のアニメーションに移行する
	if (this->useAnimationType == walkIndex1 || this->useAnimationType == walkIndex2)
	{
		if (rootNode.GetToTargetDistance() <= this->nearAttackRange)
		{
			this->useAnimationType++;
		}
	}
	//叩きつけ攻撃時に指定の再生フレームを終了していたら次のアニメーションに移行する
	else if (this->useAnimationType == static_cast<short>(UseAnimationType::SWEEP))
	{
		this->totalPlayTime += playTime;
		if (this->sweepCancelPlayTime <= this->totalPlayTime)
		{
			this->useAnimationType++;
		}
	}
	//アニメーションが終了していたら
	if (enemy.GetIsChangeAnimation())
	{
		//回転攻撃アニメーションが終了していたら成功を返す
		if (this->useAnimationType == this->maxUseAnimation)
		{
			this->totalPlayTime = 0;
			this->useAnimationType = 0;
			//アクションの解除
			rootNode.ExitCurrentBattleAction();
			return ActionNode::NodeState::SUCCESS;
		}
		//それ以外なら次のアニメーションに移行する
		this->useAnimationType++;
		return ActionNode::NodeState::RUNNING;
	}
	//それ以外は実行中を返す
	else
	{
		return ActionNode::NodeState::RUNNING;
	}
}
