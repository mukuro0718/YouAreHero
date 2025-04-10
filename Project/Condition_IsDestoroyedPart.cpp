#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "Rigidbody.h"
#include "Character.h"
#include "BeastBehaviorTreeHeader.h"
#include "Enemy.h"
#include "EnemyManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
Condition_IsDestoroyedPart::Condition_IsDestoroyedPart()
{

}

/// <summary>
/// デストラクタ
/// </summary>
Condition_IsDestoroyedPart::~Condition_IsDestoroyedPart()
{

}

/// <summary>
/// 更新
/// </summary>
Condition_IsDestoroyedPart::NodeState Condition_IsDestoroyedPart::Update(BehaviorTree& _tree, Character& _chara)
{
	/*部位破壊されていたら成功*/
	if (_tree.GetIsDestroyedPart())
	{
		//_tree.SetIsDestroyedPart(false);
		//printfDx("CONDITION_DESTROY_SUCCESS\n");
		return NodeState::SUCCESS;
	}
	//printfDx("CONDITION_DESTROY_FAILURE\n");
	return NodeState::FAILURE;
}