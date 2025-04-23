#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "Character.h"
#include "BrawlerEnemyBehaviorTreeHeader.h"
#include "Enemy.h"
#include "EnemyManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
Condition_IsCanAttack::Condition_IsCanAttack()
{

}

/// <summary>
/// デストラクタ
/// </summary>
Condition_IsCanAttack::~Condition_IsCanAttack()
{

}

/// <summary>
/// 更新
/// </summary>
Condition_IsCanAttack::NodeState Condition_IsCanAttack::Update(BehaviorTree& _tree, Character& _chara)
{
	auto& enemy = dynamic_cast<Enemy&>(_chara);

	/*攻撃権があるか*/
	if (enemy.GetIsCanAttack())
	{
		//printfDx("CONDITION_DOT_SUCCESS\n");
		return NodeState::SUCCESS;
	}
	//printfDx("CONDITION_DOT_FAILURE\n");
	return NodeState::FAILURE;
}
