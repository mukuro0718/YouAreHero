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
Condition_IsDragonHpIsLessThanZero::Condition_IsDragonHpIsLessThanZero()
{

}

/// <summary>
/// デストラクタ
/// </summary>
Condition_IsDragonHpIsLessThanZero::~Condition_IsDragonHpIsLessThanZero()
{

}

/// <summary>
/// 更新
/// </summary>
Condition_IsDragonHpIsLessThanZero::NodeState Condition_IsDragonHpIsLessThanZero::Update()
{
	auto& enemy = Singleton<EnemyManager>::GetInstance();

	/*HPが０以下ならSUCCESSを返す*/
	if (enemy.GetHP() <= 0)
	{
		printfDx("CONDITION_HP_SUCCESS\n");
		return NodeState::SUCCESS;
	}
	printfDx("CONDITION_HP_FAILURE\n");
	return NodeState::FAILURE;
}