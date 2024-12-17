#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "BehaviorTreeNode.h"
#include "ActionNode.h"
#include "Beast_Walk.h"
#include "Character.h"
#include "Enemy.h"
#include "Beast.h"
#include "EnemyManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
Beast_Walk::Beast_Walk()
{

}

/// <summary>
/// デストラクタ
/// </summary>
Beast_Walk::~Beast_Walk()
{

}

/// <summary>
/// 更新処理
/// </summary>
Beast_Walk::NodeState Beast_Walk::Update()
{
	/*アニメーション*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& enemyManager = Singleton<EnemyManager>::GetInstance();
	auto& enemy = dynamic_cast<Beast&>(enemyManager.GetCharacter());
	{
		//アニメーションの種類を設定
		int animationType = static_cast<int>(Beast::AnimationType::WALK);
		enemy.SetNowAnimation(animationType);
		//アニメーション再生時間の設定
		enemy.SetAnimationPlayTime(json.GetJson(JsonManager::FileType::BEAST)["ANIMATION_PLAY_TIME"][animationType]);
		//アニメーションの再生
		enemy.PlayAnimation();
	}

	/*移動*/
	float maxSpeed = json.GetJson(JsonManager::FileType::BEAST)["WALK_SPEED"];
	float accel = json.GetJson(JsonManager::FileType::BEAST)["ACCEL"];
	float decel = json.GetJson(JsonManager::FileType::BEAST)["DECEL"];
	enemy.Move(maxSpeed, accel, decel, false);

	/*状態を返す*/
	{
		return ActionNode::NodeState::SUCCESS;
	}
}
