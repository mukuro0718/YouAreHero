#include <DxLib.h>
#include "UseSTL.h"
#include "BeastBehaviorTreeHeader.h"

/// <summary>
/// コンストラクタ
/// </summary>
Condition_IsFrighteningValueGreaterThanConstant::Condition_IsFrighteningValueGreaterThanConstant(const int _specifiedFrighteningValue)
	: ConditionNode()
	, SPECIFIED_FRIGHTENING_VALUE(_specifiedFrighteningValue)
{

}

/// <summary>
/// デストラクタ
/// </summary>
Condition_IsFrighteningValueGreaterThanConstant::~Condition_IsFrighteningValueGreaterThanConstant()
{

}

/// <summary>
/// 更新
/// </summary>
Condition_IsFrighteningValueGreaterThanConstant::NodeState Condition_IsFrighteningValueGreaterThanConstant::Update()
{
	/*ダウン値が０以上なら成功を返す*/
	auto& rootNode = Singleton<BeastBehaviorTree>::GetInstance();
	if (rootNode.GetDamage() >= this->SPECIFIED_FRIGHTENING_VALUE)
	{
		rootNode.SetDamage(0);
		//printfDx("CONDITION_FRIGHT_SUCCESS\n");
		return NodeState::SUCCESS;
	}
	//printfDx("CONDITION_FRIGHT_FAILURE\n");
	return NodeState::FAILURE;
}
