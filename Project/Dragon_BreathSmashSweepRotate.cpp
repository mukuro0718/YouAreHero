#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "BehaviorTreeNode.h"
#include "ActionNode.h"
#include "Dragon_BreathSmashSweepRotate.h"
#include "Character.h"
#include "Enemy.h"
#include "Dragon.h"
#include "EnemyManager.h"
#include "DragonBehaviorTree.h"

/// <summary>
/// コンストラクタ
/// </summary>
Dragon_BreathSmashSweepRotate::Dragon_BreathSmashSweepRotate()
	: useAnimationType	 (0)
	, maxUseAnimation	 (0)
	, nearAttackRange	 (0.0f)
	, smashCancelPlayTime(0.0f)
	, totalPlayTime		 (0.0f)
	, sweepCancelPlayTime(0.0f)
{
	/*使用するアニメーション*/
	this->animationType.emplace_back(static_cast<int>(Dragon::AnimationType::BREATH));
	this->animationType.emplace_back(static_cast<int>(Dragon::AnimationType::WALK));
	this->animationType.emplace_back(static_cast<int>(Dragon::AnimationType::SMASH));
	this->animationType.emplace_back(static_cast<int>(Dragon::AnimationType::SWEEP));
	this->animationType.emplace_back(static_cast<int>(Dragon::AnimationType::ROTATE_ATTACK));

	/*使用するアニメーションの再生時間*/
	auto& json = Singleton<JsonManager>::GetInstance();
	vector<float> playTime  = json.GetJson(JsonManager::FileType::DRAGON)["BREATH_SMASH_SWEEP_ROTATE_PLAY_TIME"];
	this->animationPlayTime = playTime;
	this->maxUseAnimation	= static_cast<int>(UseAnimationType::ROTATE);
	this->actionType		= static_cast<short>(DragonBehaviorTree::ActionType::BREATH_SMASH_SWEEP_ROTATE);
	this->maxSpeed			= json.GetJson(JsonManager::FileType::DRAGON)["WALK_SPEED"];
	this->accel				= json.GetJson(JsonManager::FileType::DRAGON)["ACCEL"];
	this->decel				= json.GetJson(JsonManager::FileType::DRAGON)["DECEL"];
	this->nearAttackRange	= json.GetJson(JsonManager::FileType::DRAGON)["NEAR_ATTACK_RANGE"];
	this->smashCancelPlayTime = json.GetJson(JsonManager::FileType::DRAGON)["SMASH_CANCEL_PLAY_TIME"];
	this->sweepCancelPlayTime = json.GetJson(JsonManager::FileType::DRAGON)["SWEEP_CANCEL_PLAY_TIME"];
}

/// <summary>
/// デストラクタ
/// </summary>
Dragon_BreathSmashSweepRotate::~Dragon_BreathSmashSweepRotate()
{

}

/// <summary>
/// 更新処理
/// </summary>
Dragon_BreathSmashSweepRotate::NodeState Dragon_BreathSmashSweepRotate::Update()
{
	/*移動処理*/
	auto& enemyManager = Singleton<EnemyManager>::GetInstance();
	auto& enemy = dynamic_cast<Dragon&>(enemyManager.GetCharacter());

	if (this->useAnimationType == static_cast<short>(UseAnimationType::WALK))
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
		rootNode.SetCurrentAction(this->actionType);
	}

	/*アニメーションの再生*/
	float playTime = this->animationPlayTime[this->useAnimationType];
	enemy.PlayAnimation(this->animationType[this->useAnimationType], playTime);

	/*状態を返す*/
	//移動状態かつ近接攻撃範囲なら次のアニメーションに移行する
	if (this->useAnimationType == static_cast<short>(UseAnimationType::WALK))
	{
		//目標までの距離が近接攻撃範囲なら
		if (rootNode.GetToTargetDistance() <= this->nearAttackRange)
		{
			this->useAnimationType++;
		}
	}
	//叩きつけ攻撃時に指定の再生フレームを終了していたら次のアニメーションに移行する
	else if (this->useAnimationType == static_cast<short>(UseAnimationType::SMASH))
	{
		this->totalPlayTime += playTime;
		if (this->smashCancelPlayTime <= this->totalPlayTime)
		{
			this->useAnimationType++;
			this->totalPlayTime = 0.0f;
		}
	}
	//叩きつけ攻撃時に指定の再生フレームを終了していたら次のアニメーションに移行する
	else if (this->useAnimationType == static_cast<short>(UseAnimationType::SWEEP))
	{
		this->totalPlayTime += playTime;
		if (this->sweepCancelPlayTime <= this->totalPlayTime)
		{
			this->useAnimationType++;
			this->totalPlayTime = 0.0f;
		}
	}
	//アニメーションが終了していたら
	if (enemy.GetIsChangeAnimation())
	{
		//回転攻撃アニメーションが終了していたら成功を返す
		if (this->useAnimationType == this->maxUseAnimation)
		{
			this->useAnimationType = 0;
			return ActionNode::NodeState::SUCCESS;
		}
		//それ以外なら
		this->useAnimationType++;
		return ActionNode::NodeState::RUNNING;
	}
	//それ以外は実行中を返す
	else
	{
		return ActionNode::NodeState::RUNNING;
	}
}
