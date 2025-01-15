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
Condition_IsDestoroyedPart::NodeState Condition_IsDestoroyedPart::Update()
{
	/*部位破壊されていたら成功*/
	auto& rootNode = Singleton<BeastBehaviorTree>::GetInstance();
	if (rootNode.GetIsDestroyedPart())
	{
		rootNode.SetIsDestroyedPart(false);
		//printfDx("CONDITION_DESTROY_SUCCESS\n");
		return NodeState::SUCCESS;
	}
	//printfDx("CONDITION_DESTROY_FAILURE\n");
	return NodeState::FAILURE;
}