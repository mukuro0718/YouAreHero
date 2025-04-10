#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "Character.h"
#include "BehaviorTreeNode.h"
#include "BehaviorTree.h"
#include "ActionNode.h"
#include "Demon_Idle.h"
#include "Enemy.h"
#include "Demon.h"
#include "EnemyManager.h"
#include "DemonBehaviorTree.h"

/// <summary>
/// コンストラクタ
/// </summary>
Demon_Idle::Demon_Idle()
	: isInitialize		(false)
{
	auto& json = Singleton<JsonManager>::GetInstance();
	this->animationType		= static_cast<int>(Demon::AnimationType::IDLE);
	this->animationPlayTime = json.GetJson(JsonManager::FileType::DEMON_PANDA)["ANIMATION_PLAY_TIME"][this->animationType];
	this->actionType		= static_cast<short>(DemonBehaviorTree::ActionType::IDLE);
	this->maxSpeed			= 0.0f;
	this->accel				= json.GetJson(JsonManager::FileType::DEMON_PANDA)["ACCEL"];
	this->decel				= json.GetJson(JsonManager::FileType::DEMON_PANDA)["DECEL"];
}

/// <summary>
/// デストラクタ
/// </summary>
Demon_Idle::~Demon_Idle()
{

}

/// <summary>
/// 初期化
/// </summary>
void Demon_Idle::Initialize()
{
	this->isInitialize = false;
}

/// <summary>
/// 更新処理
/// </summary>
Demon_Idle::NodeState Demon_Idle::Update(BehaviorTree& _tree, Character& _chara)
{
	auto& enemy = dynamic_cast<Demon&>(_chara);

	if (!this->isInitialize)
	{
		/*登録されているアクションと実際のアクションが異なっていたら*/
		//アニメーションの種類を設定
		enemy.SetNowAnimation(this->animationType);
		//アニメーション再生時間の設定
		enemy.SetAnimationPlayTime(this->animationPlayTime);
		enemy.UpdateSpeed(this->maxSpeed, this->accel, this->decel);
		enemy.UpdateVelocity(false);
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
		return ActionNode::NodeState::SUCCESS;
	}
	//それ以外は実行中を返す
	else
	{
		return ActionNode::NodeState::RUNNING;
	}
}
