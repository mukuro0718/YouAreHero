#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "Rigidbody.h"
#include "Character.h"
#include "BeastBehaviorTreeHeader.h"
#include "Enemy.h"
#include "EnemyManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Condition_IsHpBelowConstant::Condition_IsHpBelowConstant(const int _specifiedHp)
	: SPECIFIED_HP(_specifiedHp)
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Condition_IsHpBelowConstant::~Condition_IsHpBelowConstant()
{

}

/// <summary>
/// �X�V
/// </summary>
Condition_IsHpBelowConstant::NodeState Condition_IsHpBelowConstant::Update(BehaviorTree& _tree, Character& _chara)
{
	/*HP���萔�ȉ��Ȃ琬��*/
	if (_tree.GetPrevHp() <= this->SPECIFIED_HP)
	{
		//printfDx("CONDITION_HP_SUCCESS\n");
		return NodeState::SUCCESS;
	}
	//printfDx("CONDITION_HP_FAILURE\n");
	return NodeState::FAILURE;
}