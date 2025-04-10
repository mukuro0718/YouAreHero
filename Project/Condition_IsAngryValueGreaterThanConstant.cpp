#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "Character.h"
#include "BeastBehaviorTreeHeader.h"
#include "Enemy.h"
#include "EnemyManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Condition_IsAngryValueGreaterThanConstant::Condition_IsAngryValueGreaterThanConstant(const int _specifiedAngryValue)
	: SPECIFIED_ANGRY_VALUE(_specifiedAngryValue)
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Condition_IsAngryValueGreaterThanConstant::~Condition_IsAngryValueGreaterThanConstant()
{

}

/// <summary>
/// �X�V
/// </summary>
Condition_IsAngryValueGreaterThanConstant::NodeState Condition_IsAngryValueGreaterThanConstant::Update(BehaviorTree& _tree, Character& _chara)
{
	/*�{��l���O�ȏ�Ȃ琬����Ԃ�*/
	//if (enemy.GetAngry() >= this->SPECIFIED_ANGRY_VALUE)
	//{
	//	//printfDx("CONDITION_ANGRY_SUCCESS\n");
	//	return NodeState::SUCCESS;
	//}
	//printfDx("CONDITION_ANGRY_FAILURE\n");
	return NodeState::FAILURE;
}
