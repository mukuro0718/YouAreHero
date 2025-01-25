#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "BehaviorTreeNode.h"
#include "ActionNode.h"
#include "Dragon_SweepBreathSmash.h"
#include "Character.h"
#include "Enemy.h"
#include "Dragon.h"
#include "EnemyManager.h"
#include "DragonBehaviorTree.h"

/// <summary>
/// コンストラクタ
/// </summary>
Dragon_SweepBreathSmash::Dragon_SweepBreathSmash()
	: useAnimationType	(0)
	, maxUseAnimation	(0)
	, nearAttackRange	(0.0f)
	, FIX_ROTATE_FRAME	(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::DRAGON)["FIX_ROTATE_FRAME"])
	, frameCount		(0)
	, isClose			(false)
{
	/*使用するアニメーション*/
	this->animationType.emplace_back(static_cast<int>(Dragon::AnimationType::WALK));
	this->animationType.emplace_back(static_cast<int>(Dragon::AnimationType::SWEEP));
	this->animationType.emplace_back(static_cast<int>(Dragon::AnimationType::BREATH));
	this->animationType.emplace_back(static_cast<int>(Dragon::AnimationType::WALK));
	this->animationType.emplace_back(static_cast<int>(Dragon::AnimationType::SMASH));

	/*攻撃の当たり判定のタイミングマップ*/
	this->useColliderIndex.emplace(static_cast<short>(UseAnimationType::SWEEP), static_cast<short>(Dragon::AttackCollider::SWEEP));
	this->useColliderIndex.emplace(static_cast<short>(UseAnimationType::BREATH), static_cast<short>(Dragon::AttackCollider::BREATH));
	this->useColliderIndex.emplace(static_cast<short>(UseAnimationType::SMASH), static_cast<short>(Dragon::AttackCollider::SMASH));

	/*使用するアニメーションの再生時間*/
	auto& json = Singleton<JsonManager>::GetInstance();
	vector<float> playTime	= json.GetJson(JsonManager::FileType::DRAGON)["SWEEP_BREATH_SMASH_PLAY_TIME"];
	this->animationPlayTime = playTime;
	this->maxUseAnimation	= static_cast<int>(UseAnimationType::SMASH);
	this->actionType		= static_cast<short>(DragonBehaviorTree::ActionType::SWEEP_BREATH_SMASH);
	this->maxSpeed			= json.GetJson(JsonManager::FileType::DRAGON)["WALK_SPEED"];
	this->accel				= json.GetJson(JsonManager::FileType::DRAGON)["ACCEL"];
	this->decel				= json.GetJson(JsonManager::FileType::DRAGON)["DECEL"];
	this->nearAttackRange	= json.GetJson(JsonManager::FileType::DRAGON)["NEAR_ATTACK_RANGE"];
}

/// <summary>
/// デストラクタ
/// </summary>
Dragon_SweepBreathSmash::~Dragon_SweepBreathSmash()
{

}

/// <summary>
/// 更新処理
/// </summary>
Dragon_SweepBreathSmash::NodeState Dragon_SweepBreathSmash::Update()
{
	/*移動処理*/
	auto& enemyManager = Singleton<EnemyManager>::GetInstance();
	auto& enemy = dynamic_cast<Dragon&>(enemyManager.GetCharacter());
	short walkIndex1 = static_cast<short>(UseAnimationType::WALK_1);
	short walkIndex2 = static_cast<short>(UseAnimationType::WALK_2);
	if (this->useAnimationType == walkIndex1 || this->useAnimationType == walkIndex2 || !this->isFixRotate)
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

	/*フレームカウントの増加*/
	if (!this->isFixRotate)
	{
		this->frameCount++;
		if (this->frameCount >= this->FIX_ROTATE_FRAME)
		{
			this->isFixRotate = true;
		}
	}

	/*当たり判定コライダーの更新*/
	//指定のアニメーションじゃなければ
	if (this->useAnimationType != walkIndex1 || this->useAnimationType != walkIndex2)
	{
		enemy.UpdateAttackCollider(this->useColliderIndex[this->useAnimationType], this->nowTotalPlayTime);
	}

	/*アニメーションの再生*/
	float playTime = this->animationPlayTime[this->useAnimationType];
	this->nowTotalPlayTime += playTime;
	enemy.PlayAnimation(this->animationType[this->useAnimationType], playTime);

	/*状態を返す*/
	//移動状態かつ近接攻撃範囲なら次のアニメーションに移行する
	if (this->useAnimationType == walkIndex1 || this->useAnimationType == walkIndex2)
	{
		if (rootNode.GetToTargetDistance() < this->nearAttackRange)
		{
			this->isClose = true;
			this->useAnimationType++;
			this->nowTotalPlayTime = 0.0f;
		}
	}
	//アニメーションが終了していたら
	else if (enemy.GetIsChangeAnimation())
	{
		if (this->useAnimationType == static_cast<short>(UseAnimationType::SWEEP))
		{
			if (rootNode.GetToTargetDistance() < this->nearAttackRange)
			{
				this->useAnimationType = static_cast<short>(UseAnimationType::WALK_2);
				return ActionNode::NodeState::RUNNING;
			}
		}
		enemy.OffAttackCollider(this->useColliderIndex[this->useAnimationType]);
		this->nowTotalPlayTime = 0.0f;
		//回転攻撃アニメーションが終了していたら成功を返す
		if (this->useAnimationType == this->maxUseAnimation)
		{
			this->isClose = false;
			this->isFixRotate = false;
			this->frameCount = 0;
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
