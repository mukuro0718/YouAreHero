#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "BehaviorTreeNode.h"
#include "ActionNode.h"
#include "Beast_StepBack.h"
#include "Character.h"
#include "Enemy.h"
#include "Beast.h"
#include "EnemyManager.h"
#include "BeastBehaviorTree.h"

/// <summary>
/// コンストラクタ
/// </summary>
Beast_StepBack::Beast_StepBack()
{

}

/// <summary>
/// デストラクタ
/// </summary>
Beast_StepBack::~Beast_StepBack()
{

}

/// <summary>
/// 更新処理
/// </summary>
Beast_StepBack::NodeState Beast_StepBack::Update()
{
	/*アニメーション*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& enemyManager = Singleton<EnemyManager>::GetInstance();
	auto& enemy = dynamic_cast<Beast&>(enemyManager.GetCharacter());
	{
		//アニメーションの種類を設定
		int animationType = static_cast<int>(Beast::AnimationType::STEP_BACK);
		enemy.SetNowAnimation(animationType);
		//アニメーション再生時間の設定
		enemy.SetAnimationPlayTime(json.GetJson(JsonManager::FileType::BEAST)["ANIMATION_PLAY_TIME"][animationType]);
		//アニメーションの再生
		enemy.PlayAnimation();
	}

	/*移動*/
	float maxSpeed = json.GetJson(JsonManager::FileType::BEAST)["STEP_BACK_SPEED"];
	float accel = json.GetJson(JsonManager::FileType::BEAST)["ACCEL"];
	float decel = json.GetJson(JsonManager::FileType::BEAST)["DECEL"];
	//移動速度の更新
	enemy.UpdateSpeed(maxSpeed, decel, accel);
	//移動ベクトルを出す
	enemy.UpdateVelocity(false);

	/*状態を返す*/
	{
		//アニメーションが終了していたら
		if (enemy.GetIsChangeAnimation())
		{
			return ActionNode::NodeState::SUCCESS;
		}
		//それ以外は実行中を返す
		else
		{
			return ActionNode::NodeState::RUNNING;
		}
	}
}
