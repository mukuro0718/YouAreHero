#include "UseSTL.h"
#include "BeastBehaviorTreeHeader.h"
#include "EnemyManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
CheckDyingCondition::CheckDyingCondition()
{

}

/// <summary>
/// デストラクタ
/// </summary>
CheckDyingCondition::~CheckDyingCondition()
{

}

/// <summary>
/// 更新
/// </summary>
CheckDyingCondition::NodeState CheckDyingCondition::Update()
{
	auto& enemy = Singleton<EnemyManager>::GetInstance();
	auto& rootNode = Singleton<BeastBehaviorTree>::GetInstance();

	/*ダウン値が定数以上になったら成功を返す*/
	if (enemy.GetHP() <= 0)
	{
		return NodeState::SUCCESS;
	}
	return NodeState::FAILURE;
}
