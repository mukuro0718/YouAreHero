#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "BehaviorTreeNode.h"
#include "ActionNode.h"
#include "Dragon_Idle.h"
#include "Character.h"
#include "Enemy.h"
#include "Dragon.h"
#include "EnemyManager.h"
#include "DragonBehaviorTree.h"

/// <summary>
/// コンストラクタ
/// </summary>
Dragon_Idle::Dragon_Idle()
	: STANDBY_TIME		(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::DRAGON)["STANDBY_TIME"])
	, currentStandbyTime(0)
{
	auto& json = Singleton<JsonManager>::GetInstance();
	this->animationType		= static_cast<int>(Dragon::AnimationType::IDLE);
	this->animationPlayTime	= json.GetJson(JsonManager::FileType::DRAGON)["ANIMATION_PLAY_TIME"][this->animationType];
	this->actionType		= static_cast<short>(DragonBehaviorTree::ActionType::IDLE);
	this->maxSpeed			= 0.0f;
	this->accel				= json.GetJson(JsonManager::FileType::DRAGON)["ACCEL"];
	this->decel				= json.GetJson(JsonManager::FileType::DRAGON)["DECEL"];
}

/// <summary>
/// デストラクタ
/// </summary>
Dragon_Idle::~Dragon_Idle()
{

}

/// <summary>
/// 更新処理
/// </summary>
Dragon_Idle::NodeState Dragon_Idle::Update()
{
	/*速度が０以上または最初にこのアクションになった時移動処理を行う*/
	auto& enemyManager = Singleton<EnemyManager>::GetInstance();
	auto& enemy = dynamic_cast<Dragon&>(enemyManager.GetCharacter());
	auto& rootNode = Singleton<DragonBehaviorTree>::GetInstance();
	int prevAction = rootNode.GetCurrentAction();
	if (enemy.GetSpeed() != 0.0f || prevAction != this->actionType)
	{
		enemy.UpdateSpeed(this->maxSpeed, this->accel, this->decel);
		enemy.UpdateVelocity(false);
		rootNode.SetCurrentAction(this->actionType);
		this->standbyTime = this->STANDBY_TIME;
	}

	/*アニメーションの再生*/
	enemy.PlayAnimation(this->animationType, this->animationPlayTime);

	/*待機時間を計測*/
	this->currentStandbyTime++;

	/*状態を返す*/
	//指定の待機時間を超えていたらSUCCESSを返す
	if (this->currentStandbyTime >= this->standbyTime)
	{
		this->currentStandbyTime = 0;
		enemy.SetAttackCount();
		return ActionNode::NodeState::SUCCESS;
	}
	return ActionNode::NodeState::RUNNING;
}
