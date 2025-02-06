#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "BehaviorTreeNode.h"
#include "ActionNode.h"
#include "Demon_Dying.h"
#include "Character.h"
#include "Enemy.h"
#include "Demon.h"
#include "EnemyManager.h"
#include "DemonBehaviorTree.h"

/// <summary>
/// コンストラクタ
/// </summary>
Demon_Dying::Demon_Dying()
	: isInitialize		(false)
{
	auto& json = Singleton<JsonManager>::GetInstance();
	this->animationType		= static_cast<int>(Demon::AnimationType::DYING);
	this->animationPlayTime = json.GetJson(JsonManager::FileType::DEMON_PANDA)["ANIMATION_PLAY_TIME"][this->animationType];
	this->actionType		= static_cast<short>(DemonBehaviorTree::ActionType::DYING);
	this->maxSpeed			= 0.0f;
	this->accel				= json.GetJson(JsonManager::FileType::DEMON_PANDA)["ACCEL"];
	this->decel				= json.GetJson(JsonManager::FileType::DEMON_PANDA)["DECEL"];
}

/// <summary>
/// デストラクタ
/// </summary>
Demon_Dying::~Demon_Dying()
{

}

/// <summary>
/// 初期化
/// </summary>
void Demon_Dying::Initialize()
{
	this->isInitialize = false;
}

/// <summary>
/// 更新処理
/// </summary>
Demon_Dying::NodeState Demon_Dying::Update()
{
	auto& enemyManager = Singleton<EnemyManager>::GetInstance();
	auto& enemy = dynamic_cast<Demon&>(enemyManager.GetCharacter());

	/*生存フラグが下りていたら以下の処理は行わない*/
	if (!enemy.GetIsAlive())return ActionNode::NodeState::SUCCESS;

	auto& rootNode = Singleton<DemonBehaviorTree>::GetInstance();
	/*登録されているアクションと実際のアクションが異なっていたら*/
	if (!this->isInitialize)
	{
		//アニメーションの種類を設定
		enemy.SetNowAnimation(this->animationType);
		//アニメーション再生時間の設定
		enemy.SetAnimationPlayTime(this->animationPlayTime);
	}

	/*アニメーションの再生*/
	enemy.PlayAnimation();

	/*移動*/
	if (enemy.GetSpeed() != 0.0f)
	{
		enemy.UpdateSpeed(this->maxSpeed, this->accel, this->decel);
		enemy.UpdateVelocity(false);

	}

	/*状態を返す*/
	//アニメーションが終了していたら
	if (enemy.GetIsChangeAnimation())
	{
		enemy.OffIsAlive();
		return ActionNode::NodeState::SUCCESS;
	}
	//それ以外は実行中を返す
	else
	{
		return ActionNode::NodeState::RUNNING;
	}
}
