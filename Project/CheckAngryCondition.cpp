#include "UseSTL.h"
#include "UseJson.h"
#include "BeastBehaviorTreeHeader.h"
#include "EnemyManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
CheckAngryCondition::CheckAngryCondition()
{

}

/// <summary>
/// デストラクタ
/// </summary>
CheckAngryCondition::~CheckAngryCondition()
{

}

/// <summary>
/// 更新
/// </summary>
CheckAngryCondition::NodeState CheckAngryCondition::Update()
{
	/*状態がNORMAL出なければFAILUREを返す*/
	auto& rootNode = Singleton<BeastBehaviorTree>::GetInstance();
	if (rootNode.GetBeastState() != BeastBehaviorTree::BeastState::NORMAL) return NodeState::FAILURE;

	/*ダメージの蓄積*/
	auto& enemy = Singleton<EnemyManager>::GetInstance();
	int nowDamage = rootNode.GetDamage();
	int prevHp = rootNode.GetPrevHp();
	int nowHP = enemy.GetHP();
	int newDamage = nowDamage + prevHp - nowHP;
	rootNode.SetDamage(newDamage);
	rootNode.SetPrevHp(nowHP);

	/*蓄積したダメージが定数以上になったら成功を返す*/
	auto& json = Singleton<JsonManager>::GetInstance();
	if (newDamage >= json.GetJson(JsonManager::FileType::BEAST)["CHANGE_ANGRY_LIMIT_DAMAGE"])
	{
		return NodeState::SUCCESS;
	}
	return NodeState::FAILURE;
}
