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
Condition_IsDragonHpIsLessThanConstant::Condition_IsDragonHpIsLessThanConstant(const int _compareHp)
	: COMPARE_HP(_compareHp)
{

}

/// <summary>
/// デストラクタ
/// </summary>
Condition_IsDragonHpIsLessThanConstant::~Condition_IsDragonHpIsLessThanConstant()
{

}

/// <summary>
/// 更新
/// </summary>
Condition_IsDragonHpIsLessThanConstant::NodeState Condition_IsDragonHpIsLessThanConstant::Update()
{
	auto& enemy = Singleton<EnemyManager>::GetInstance();

	/*HPが定数以下ならSUCCESSを返す*/
	if (enemy.GetHP() <= this->COMPARE_HP)
	{
		//printfDx("CONDITION_HP_SUCCESS\n");
		return NodeState::SUCCESS;
	}
	//printfDx("CONDITION_HP_FAILURE\n");
	return NodeState::FAILURE;
}