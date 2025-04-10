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
Condition_IsPrevActionIsSameAsSpecifiedAction::Condition_IsPrevActionIsSameAsSpecifiedAction(const int _specifiedAction)
	: SPECIFIED_ACTION(_specifiedAction)
{

}

/// <summary>
/// デストラクタ
/// </summary>
Condition_IsPrevActionIsSameAsSpecifiedAction::~Condition_IsPrevActionIsSameAsSpecifiedAction()
{

}

/// <summary>
/// 更新
/// </summary>
Condition_IsPrevActionIsSameAsSpecifiedAction::NodeState Condition_IsPrevActionIsSameAsSpecifiedAction::Update(BehaviorTree& _tree, Character& _chara)
{
	/*インターバルが終了していなければFAILUREを返す*/
	if (_tree.GetNowSelectAction() == this->SPECIFIED_ACTION)
	{
		//printfDx("CONDITION_NODESTATE_SUCCESS\n");
		return NodeState::SUCCESS;
	}
	//printfDx("CONDITION_NODESTATE_FAILURE\n");
	return NodeState::FAILURE;
}