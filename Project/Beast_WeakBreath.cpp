#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "BehaviorTreeNode.h"
#include "ActionNode.h"
#include "Beast_WeakBreath.h"
#include "Character.h"
#include "Enemy.h"
#include "Beast.h"
#include "EnemyManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
Beast_WeakBreath::Beast_WeakBreath()
{

}

/// <summary>
/// デストラクタ
/// </summary>
Beast_WeakBreath::~Beast_WeakBreath()
{

}

/// <summary>
/// 更新処理
/// </summary>
Beast_WeakBreath::NodeState Beast_WeakBreath::Update()
{
	/*アニメーション*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& enemyManager = Singleton<EnemyManager>::GetInstance();
	auto& enemy = dynamic_cast<Beast&>(enemyManager.GetCharacter());
	{
		//アニメーションの種類を設定
		int animationType = static_cast<int>(Beast::AnimationType::WEAK_BREATH);
		enemy.SetNowAnimation(animationType);
		//アニメーション再生時間の設定
		enemy.SetAnimationPlayTime(json.GetJson(JsonManager::FileType::BEAST)["ANIMATION_PLAY_TIME"][animationType]);
		//アニメーションの再生
		enemy.PlayAnimation();
	}

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
