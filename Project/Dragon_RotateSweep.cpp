#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "BehaviorTreeNode.h"
#include "ActionNode.h"
#include "Dragon_RotateSweep.h"
#include "Character.h"
#include "Enemy.h"
#include "Dragon.h"
#include "EnemyManager.h"
#include "DragonBehaviorTree.h"

/// <summary>
/// コンストラクタ
/// </summary>
Dragon_RotateSweep::Dragon_RotateSweep()
	: useAnimationType	(0)
	, maxUseAnimation	(0)
	, nearAttackRange	(0)
	, FIX_ROTATE_FRAME	(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::DRAGON)["FIX_ROTATE_FRAME"])
	, frameCount		(0)
	, isClose			(false)
{
	/*使用するアニメーション*/
	this->animationType.emplace_back(static_cast<int>(Dragon::AnimationType::WALK));
	this->animationType.emplace_back(static_cast<int>(Dragon::AnimationType::ROTATE_ATTACK));
	this->animationType.emplace_back(static_cast<int>(Dragon::AnimationType::SWEEP));

	/*攻撃の当たり判定のタイミングマップ*/
	this->useColliderIndex.emplace(static_cast<short>(UseAnimationType::ROTATE), static_cast<short>(Dragon::AttackCollider::ROTATE));
	this->useColliderIndex.emplace(static_cast<short>(UseAnimationType::SWEEP), static_cast<short>(Dragon::AttackCollider::SWEEP));

	/*メンバ変数の初期化*/
	auto& json = Singleton<JsonManager>::GetInstance();
	vector<float> playTime  = json.GetJson(JsonManager::FileType::DRAGON)["ROTATE_SWEEP_PLAY_TIME"];
	this->animationPlayTime = playTime;
	this->maxUseAnimation	= static_cast<int>(UseAnimationType::SWEEP);
	this->actionType		= static_cast<short>(DragonBehaviorTree::ActionType::SMASH_ROTATE);
	this->maxSpeed			= json.GetJson(JsonManager::FileType::DRAGON)["WALK_SPEED"];
	this->accel				= json.GetJson(JsonManager::FileType::DRAGON)["ACCEL"];
	this->decel				= json.GetJson(JsonManager::FileType::DRAGON)["DECEL"];
	this->nearAttackRange	= json.GetJson(JsonManager::FileType::DRAGON)["NEAR_ATTACK_RANGE"];
}

/// <summary>
/// デストラクタ
/// </summary>
Dragon_RotateSweep::~Dragon_RotateSweep()
{

}

/// <summary>
/// 更新処理
/// </summary>
Dragon_RotateSweep::NodeState Dragon_RotateSweep::Update()
{
	/*移動処理*/
	auto& enemyManager = Singleton<EnemyManager>::GetInstance();
	auto& enemy = dynamic_cast<Dragon&>(enemyManager.GetCharacter());
	short walkIndex = static_cast<short>(UseAnimationType::WALK);
	if (this->useAnimationType == walkIndex || !this->isFixRotate)
	{
		float speed = 0.0f;
		if (!this->isClose)
		{
			speed = this->maxSpeed;
		}
		enemy.Move(speed, this->accel, this->decel, false);
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
	if (this->useAnimationType != static_cast<short>(UseAnimationType::WALK))
	{
		enemy.UpdateAttackCollider(this->useColliderIndex[this->useAnimationType], this->nowTotalPlayTime);
	}

	/*アニメーションの再生*/
	float playTime = this->animationPlayTime[this->useAnimationType];
	this->nowTotalPlayTime += playTime;
	enemy.PlayAnimation(this->animationType[this->useAnimationType], playTime);

	/*状態を返す*/
	//移動状態かつ近接攻撃範囲なら次のアニメーションに移行する
	if (this->useAnimationType == walkIndex)
	{
		if (rootNode.GetToTargetDistance() < this->nearAttackRange)
		{
			this->isClose = true;
			this->useAnimationType++;
			this->nowTotalPlayTime = 0.0f;
		}
		return ActionNode::NodeState::RUNNING;
	}
	//アニメーションが終了していたら
	else if (enemy.GetIsChangeAnimation())
	{
		enemy.OffAttackCollider(this->useColliderIndex[this->useAnimationType]);
		this->nowTotalPlayTime = 0.0f;
		//叩きつけアニメーションが終了していたら成功を返す
		if (this->useAnimationType == this->maxUseAnimation)
		{
			this->useAnimationType	= 0;
			this->isClose			= false;
			this->isFixRotate		= false;
			this->frameCount		= 0;
			//アクションの解除
			rootNode.ExitCurrentBattleAction();
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
