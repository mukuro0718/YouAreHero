#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "Rigidbody.h"
#include "CharacterData.h"
#include "Character.h"
#include "TankEnemyBehaviorTreeHeader.h"
#include "Enemy.h"
#include "EnemyManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Condition_IsHitAttack::Condition_IsHitAttack()
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Condition_IsHitAttack::~Condition_IsHitAttack()
{

}

/// <summary>
/// �X�V
/// </summary>
Condition_IsHitAttack::NodeState Condition_IsHitAttack::Update(BehaviorTree& _tree, Character& _chara)
{
	/*�U�����󂯂Ă����琬��*/
	if (_chara.GetCharacterData().isHit)
	{
		//printfDx("CONDITION_SELECTED_REACTION_SUCCESS\n");
		return NodeState::SUCCESS;
	}
	//printfDx("CONDITION_SELECTED_REACTION_FAILURE\n");
	return NodeState::FAILURE;
}