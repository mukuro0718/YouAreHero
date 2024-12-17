#include "UseSTL.h"
#include "UseJson.h"
#include "BeastBehaviorTreeHeader.h"

/// <summary>
/// コンストラクタ
/// </summary>
CheckDownCondition::CheckDownCondition()
{

}

/// <summary>
/// デストラクタ
/// </summary>
CheckDownCondition::~CheckDownCondition()
{

}

/// <summary>
/// 更新
/// </summary>
CheckDownCondition::NodeState CheckDownCondition::Update()
{
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& rootNode = Singleton<BeastBehaviorTree>::GetInstance();

	/*ダウン値が定数以上になったら成功を返す*/
	if (rootNode.GetDownValue() >= json.GetJson(JsonManager::FileType::BEAST)["MAX_DOWN_VALUE"])
	{
		return NodeState::SUCCESS;
	}
	return NodeState::FAILURE;
}
