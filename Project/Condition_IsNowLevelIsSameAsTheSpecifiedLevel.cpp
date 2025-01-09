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
Condition_IsNowLevelIsSameAsTheSpecifiedLevel::Condition_IsNowLevelIsSameAsTheSpecifiedLevel(const int _specifiedLevel)
	: SPECIFIED_LEVEL(_specifiedLevel)
{

}

/// <summary>
/// デストラクタ
/// </summary>
Condition_IsNowLevelIsSameAsTheSpecifiedLevel::~Condition_IsNowLevelIsSameAsTheSpecifiedLevel()
{

}

/// <summary>
/// 更新
/// </summary>
Condition_IsNowLevelIsSameAsTheSpecifiedLevel::NodeState Condition_IsNowLevelIsSameAsTheSpecifiedLevel::Update()
{
	auto& rootNode = Singleton<BeastBehaviorTree>::GetInstance();

	/*インターバルが終了していなければFAILUREを返す*/
	if (rootNode.GetLevel() != this->SPECIFIED_LEVEL)
	{
		printfDx("CONDITION_LEVEL_FAILURE\n");
		return NodeState::FAILURE;
	}
	printfDx("CONDITION_LEVEL_SUCCESS\n");
	return NodeState::SUCCESS;
}