#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "Rigidbody.h"
#include "Character.h"
#include "BeastBehaviorTreeHeader.h"
#include "Enemy.h"
#include "EnemyManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
Conditon_IsAttackCountIsOver::Conditon_IsAttackCountIsOver()
{

}

/// <summary>
/// デストラクタ
/// </summary>
Conditon_IsAttackCountIsOver::~Conditon_IsAttackCountIsOver()
{

}

/// <summary>
/// 更新
/// </summary>
Conditon_IsAttackCountIsOver::NodeState Conditon_IsAttackCountIsOver::Update(BehaviorTree& _tree, Character& _chara)
{
	auto& boss = dynamic_cast<Enemy&>(_chara);

	/*攻撃回数が残っていなかったらFAILUREを返す*/
	if (boss.GetAttackCount() <= 0)
	{
		//printfDx("CONDITION_INTERVAL_FAILURE\n");
		return NodeState::FAILURE;
	}
	//printfDx("CONDITION_INTERVAL_SUCCESS\n");
	return NodeState::SUCCESS;
}