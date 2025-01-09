#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "BeastBehaviorTreeHeader.h"
#include "Rigidbody.h"
#include "Character.h"
#include "Enemy.h"
#include "EnemyManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
Condition_IsHpBelowConstant::Condition_IsHpBelowConstant(const int _specifiedHp)
	: SPECIFIED_HP(_specifiedHp)
{

}

/// <summary>
/// デストラクタ
/// </summary>
Condition_IsHpBelowConstant::~Condition_IsHpBelowConstant()
{

}

/// <summary>
/// 更新
/// </summary>
Condition_IsHpBelowConstant::NodeState Condition_IsHpBelowConstant::Update()
{
	auto& rootNode = Singleton<BeastBehaviorTree>::GetInstance();

	/*HPが定数以下なら成功*/
	if (rootNode.GetPrevHp() <= this->SPECIFIED_HP)
	{
		printfDx("CONDITION_HP_SUCCESS\n");
		return NodeState::SUCCESS;
	}
	printfDx("CONDITION_HP_FAILURE\n");
	return NodeState::FAILURE;
}