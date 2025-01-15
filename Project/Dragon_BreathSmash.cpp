#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "BehaviorTreeNode.h"
#include "ActionNode.h"
#include "Rigidbody.h"
#include "ColliderData.h"
#include "AttackCapsuleColliderData.h"
#include "AttackData.h"
#include "Character.h"
#include "Enemy.h"
#include "Dragon.h"
#include "Dragon_BreathSmash.h"
#include "EnemyManager.h"
#include "DragonBehaviorTree.h"
#include "ReactionType.h"
#include "HitStop.h"

/// <summary>
/// コンストラクタ
/// </summary>
Dragon_BreathSmash::Dragon_BreathSmash()
	: MAX_USE_ANIMATION	(static_cast<int>(UseAnimationType::SMASH))
	, NEAR_ATTACK_RANGE(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::DRAGON)["NEAR_ATTACK_RANGE"])
	, useAnimationType(0)
	, SWEEP_CANCEL_PLAY_TIME(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::DRAGON)["NEAR_ATTACK_RANGE"])
{
	/*使用するアニメーション*/
	this->animationType.emplace_back(static_cast<int>(Dragon::AnimationType::BREATH));
	this->animationType.emplace_back(static_cast<int>(Dragon::AnimationType::WALK));
	this->animationType.emplace_back(static_cast<int>(Dragon::AnimationType::SMASH));
	
	/*メンバ変数の初期化*/
	auto& json = Singleton<JsonManager>::GetInstance();
	vector<float> playTime = json.GetJson(JsonManager::FileType::DRAGON)["BREATH_SMASH_PLAY_TIME"];
	this->animationPlayTime = playTime;
	this->actionType		= static_cast<short>(DragonBehaviorTree::ActionType::BREATH_SMASH);
	this->maxSpeed			= json.GetJson(JsonManager::FileType::DRAGON)["WALK_SPEED"];
	this->accel				= json.GetJson(JsonManager::FileType::DRAGON)["ACCEL"];
	this->decel				= json.GetJson(JsonManager::FileType::DRAGON)["DECEL"];
}

/// <summary>
/// デストラクタ
/// </summary>
Dragon_BreathSmash::~Dragon_BreathSmash()
{

}

/// <summary>
/// 更新処理
/// </summary>
Dragon_BreathSmash::NodeState Dragon_BreathSmash::Update()
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
	enemy.PlayAnimation(this->animationType[this->useAnimationType], this->animationPlayTime[this->useAnimationType]);

	/*状態を返す*/
	//移動状態かつ近接攻撃範囲なら次のアニメーションに移行する
	if (this->useAnimationType == static_cast<short>(UseAnimationType::WALK))
	{
		if (rootNode.GetToTargetDistance() <= this->NEAR_ATTACK_RANGE)
		{
			this->useAnimationType++;
		}
	}
	//アニメーションが終了していたら
	if (enemy.GetIsChangeAnimation())
	{
		//叩きつけアニメーションが終了していたら成功を返す
		if (this->useAnimationType == this->MAX_USE_ANIMATION)
		{
			//アクションの解除
			rootNode.ExitCurrentBattleAction();
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
