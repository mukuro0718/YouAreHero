#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "Character.h"
#include "BrawlerEnemyBehaviorTreeHeader.h"
#include "Enemy.h"
#include "EnemyManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Condition_IsCanAttack::Condition_IsCanAttack()
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Condition_IsCanAttack::~Condition_IsCanAttack()
{

}

/// <summary>
/// �X�V
/// </summary>
Condition_IsCanAttack::NodeState Condition_IsCanAttack::Update(BehaviorTree& _tree, Character& _chara)
{
	auto& enemy = dynamic_cast<Enemy&>(_chara);

	/*�U���������邩*/
	if (enemy.GetIsCanAttack())
	{
		//printfDx("CONDITION_DOT_SUCCESS\n");
		return NodeState::SUCCESS;
	}
	//printfDx("CONDITION_DOT_FAILURE\n");
	return NodeState::FAILURE;
}
