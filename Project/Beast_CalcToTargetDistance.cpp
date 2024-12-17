#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "BehaviorTreeNode.h"
#include "ActionNode.h"
#include "Beast_CalcToTargetDistance.h"
#include "Rigidbody.h"
#include "Character.h"
#include "Enemy.h"
#include "Beast.h"
#include "EnemyManager.h"
#include "PlayerManager.h"
#include "BeastBehaviorTree.h"

/// <summary>
/// コンストラクタ
/// </summary>
Beast_CalcToTargetDistance::Beast_CalcToTargetDistance()
{

}

/// <summary>
/// デストラクタ
/// </summary>
Beast_CalcToTargetDistance::~Beast_CalcToTargetDistance()
{

}

/// <summary>
/// 更新処理
/// </summary>
Beast_CalcToTargetDistance::NodeState Beast_CalcToTargetDistance::Update()
{
	auto& tree = Singleton<BeastBehaviorTree>::GetInstance();
	auto& enemy = Singleton<EnemyManager>::GetInstance();
	auto& player = Singleton<PlayerManager>::GetInstance();
	
	/*距離を求める*/
	VECTOR toTarget = VSub(player.GetRigidbody().GetPosition(),enemy.GetRigidbody().GetPosition());
	float distance = VSize(toTarget);
	tree.SetToTargetDistance(distance);
	
	/*状態を返す*/
	{
		return ActionNode::NodeState::SUCCESS;
	}
}
