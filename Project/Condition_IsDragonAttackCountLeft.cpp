#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "DragonBehaviorTreeHeader.h"
#include "Rigidbody.h"
#include "Character.h"
#include "Enemy.h"
#include "EnemyManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
Condition_IsDragonAttackCountLeft::Condition_IsDragonAttackCountLeft()
{

}

/// <summary>
/// デストラクタ
/// </summary>
Condition_IsDragonAttackCountLeft::~Condition_IsDragonAttackCountLeft()
{

}

/// <summary>
/// 更新
/// </summary>
Condition_IsDragonAttackCountLeft::NodeState Condition_IsDragonAttackCountLeft::Update()
{
	auto& dragon = Singleton<EnemyManager>::GetInstance();

	/*攻撃回数が残っているならSUCCESSを返す*/
	int count = dragon.GetAttackCount();
	if (count > 0)
	{
		//printfDx("CONDITION_HP_SUCCESS\n");
		return NodeState::SUCCESS;
	}
	//printfDx("CONDITION_HP_FAILURE\n");
	return NodeState::FAILURE;
}