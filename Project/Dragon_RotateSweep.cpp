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
	: useAnimationType(0)
	, maxUseAnimation(0)
	, nearAttackRange(0)
{
	/*使用するアニメーション*/
	this->animationType.emplace_back(static_cast<int>(Dragon::AnimationType::WALK));
	this->animationType.emplace_back(static_cast<int>(Dragon::AnimationType::ROTATE_ATTACK));
	this->animationType.emplace_back(static_cast<int>(Dragon::AnimationType::SWEEP));

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
	if (this->useAnimationType == static_cast<short>(UseAnimationType::WALK))
	{
		if (rootNode.GetToTargetDistance() <= this->nearAttackRange)
		{
			this->useAnimationType++;
		}
		return ActionNode::NodeState::RUNNING;
	}
	//アニメーションが終了していたら
	else if (enemy.GetIsChangeAnimation())
	{
		//叩きつけアニメーションが終了していたら成功を返す
		if (this->useAnimationType == this->maxUseAnimation)
		{
			this->useAnimationType = 0;
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
