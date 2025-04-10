#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "Character.h"
#include "BehaviorTreeNode.h"
#include "BehaviorTree.h"
#include "ActionNode.h"
#include "Mage_Reaction.h"
#include "Enemy.h"
#include "MageEnemy.h"
#include "EnemyManager.h"
#include "MageEnemyBehaviorTree.h"

/// <summary>
/// コンストラクタ
/// </summary>
Mage_Reaction::Mage_Reaction()
	: isInitialize		(false)
{
	auto& json = Singleton<JsonManager>::GetInstance();
	this->animationType		= static_cast<int>(MageEnemy::AnimationType::DYING);
	this->animationPlayTime = json.GetJson(JsonManager::FileType::MAGE_ENEMY)["ANIMATION_PLAY_TIME"][this->animationType];
	this->actionType		= static_cast<short>(MageEnemyBehaviorTree::ActionType::DYING);
	this->maxSpeed			= 0.0f;
	this->accel				= json.GetJson(JsonManager::FileType::MAGE_ENEMY)["ACCEL"];
	this->decel				= json.GetJson(JsonManager::FileType::MAGE_ENEMY)["DECEL"];
}

/// <summary>
/// デストラクタ
/// </summary>
Mage_Reaction::~Mage_Reaction()
{

}

/// <summary>
/// 初期化
/// </summary>
void Mage_Reaction::Initialize()
{
	this->isInitialize = false;
}

/// <summary>
/// 更新処理
/// </summary>
Mage_Reaction::NodeState Mage_Reaction::Update(BehaviorTree& _tree, Character& _chara)
{
	auto& enemy = dynamic_cast<MageEnemy&>(_chara);

	/*生存フラグが下りていたら以下の処理は行わない*/
	if (!enemy.GetIsAlive())return ActionNode::NodeState::SUCCESS;

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
		return ActionNode::NodeState::SUCCESS;
	}
	//それ以外は実行中を返す
	else
	{
		return ActionNode::NodeState::RUNNING;
	}
}
