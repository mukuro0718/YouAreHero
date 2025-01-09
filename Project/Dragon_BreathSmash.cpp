#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "BehaviorTreeNode.h"
#include "ActionNode.h"
#include "Dragon_BreathSmash.h"
#include "Character.h"
#include "Enemy.h"
#include "Dragon.h"
#include "EnemyManager.h"
#include "DragonBehaviorTree.h"

/// <summary>
/// コンストラクタ
/// </summary>
Dragon_BreathSmash::Dragon_BreathSmash()
	: useAnimationType(0)
{
	auto& json = Singleton<JsonManager>::GetInstance();
	this->animationType.emplace_back(static_cast<int>(Dragon::AnimationType::WALK));
	this->animationType.emplace_back(static_cast<int>(Dragon::AnimationType::BREATH));
	this->animationType.emplace_back(static_cast<int>(Dragon::AnimationType::SMASH));
	
	for (int i = 0; i < this->animationType.size(); i++)
	{
		this->animationPlayTime.emplace_back(json.GetJson(JsonManager::FileType::DRAGON)["ANIMATION_PLAY_TIME"][this->animationType[i]]);
	}
	this->maxUseAnimation	= static_cast<int>(UseAnimationType::SMASH);
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
	/*速度が０以上または最初にこのアクションになった時移動処理を行う*/
	auto& enemyManager = Singleton<EnemyManager>::GetInstance();
	auto& enemy = dynamic_cast<Dragon&>(enemyManager.GetCharacter());

	if (this->useAnimationType == 1)
	{
		enemy.UpdateSpeed(0.0f, this->accel, this->decel);
		enemy.UpdateVelocity(false);
	}
	else
	{
		enemy.Move(this->maxSpeed, this->accel, this->decel, false);
	}

	/*アクションの状態をセット*/
	auto& rootNode = Singleton<DragonBehaviorTree>::GetInstance();
	if (rootNode.GetCurrentAction() != this->actionType)
	{
		rootNode.SetCurrentAction(this->actionType);
	}

	/*アニメーションの再生*/
	enemy.PlayAnimation(this->animationType[this->useAnimationType], this->animationPlayTime[this->useAnimationType]);

	/*状態を返す*/
	//アニメーションが終了していたら
	if (enemy.GetIsChangeAnimation())
	{
		//叩きつけアニメーションが終了していたら成功を返す
		if (this->useAnimationType == static_cast<short>(UseAnimationType::SMASH))
		{
			return ActionNode::NodeState::SUCCESS;
		}
		//それ以外なら
		else
		{
			//目標までの距離が近接攻撃範囲なら
			if (rootNode.GetToTargetDistance() >= this->nearAttackRange)
			{
				this->useAnimationType = static_cast<short>(UseAnimationType::WALK);
			}
			else
			{
				this->useAnimationType = static_cast<short>(UseAnimationType::SMASH);
			}
			return ActionNode::NodeState::RUNNING;
		}
	}
	//それ以外は実行中を返す
	else
	{
		return ActionNode::NodeState::RUNNING;
	}
}
