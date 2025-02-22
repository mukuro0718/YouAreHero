#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "BeastBehaviorTreeHeader.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Condition_IsDownValueGreaterThanConstant::Condition_IsDownValueGreaterThanConstant(const int _specifiedDownValue)
	: SPECIFIED_DOWN_VALUE(_specifiedDownValue)
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Condition_IsDownValueGreaterThanConstant::~Condition_IsDownValueGreaterThanConstant()
{

}

/// <summary>
/// �X�V
/// </summary>
Condition_IsDownValueGreaterThanConstant::NodeState Condition_IsDownValueGreaterThanConstant::Update()
{
	/*�_�E���l���O�ȏ�Ȃ琬����Ԃ�*/
	auto& rootNode = Singleton<BeastBehaviorTree>::GetInstance();
	//if (rootNode.GetDownValue() >= this->SPECIFIED_DOWN_VALUE)
	//{
	//	rootNode.SetDownValue(0);
	//	//printfDx("CONDITION_DOWN_SUCCESS\n");
	//	return NodeState::SUCCESS;
	//}
	//printfDx("CONDITION_DOWN_FAILURE\n");
	return NodeState::FAILURE;
}
