#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "Character.h"
#include "BeastBehaviorTreeHeader.h"

/// <summary>
/// コンストラクタ
/// </summary>
Condition_IsDownValueGreaterThanConstant::Condition_IsDownValueGreaterThanConstant(const int _specifiedDownValue)
	: SPECIFIED_DOWN_VALUE(_specifiedDownValue)
{

}

/// <summary>
/// デストラクタ
/// </summary>
Condition_IsDownValueGreaterThanConstant::~Condition_IsDownValueGreaterThanConstant()
{

}

/// <summary>
/// 更新
/// </summary>
Condition_IsDownValueGreaterThanConstant::NodeState Condition_IsDownValueGreaterThanConstant::Update(BehaviorTree& _tree, Character& _chara)
{
	/*ダウン値が０以上なら成功を返す*/
	//if (rootNode.GetDownValue() >= this->SPECIFIED_DOWN_VALUE)
	//{
	//	rootNode.SetDownValue(0);
	//	//printfDx("CONDITION_DOWN_SUCCESS\n");
	//	return NodeState::SUCCESS;
	//}
	//printfDx("CONDITION_DOWN_FAILURE\n");
	return NodeState::FAILURE;
}
