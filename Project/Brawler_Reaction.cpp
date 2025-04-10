#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "Character.h"
#include "BehaviorTreeNode.h"
#include "BehaviorTree.h"
#include "ActionNode.h"
#include "Brawler_Reaction.h"
#include "Enemy.h"
#include "BrawlerEnemy.h"
#include "EnemyManager.h"
#include "BrawlerEnemyBehaviorTree.h"

/// <summary>
/// コンストラクタ
/// </summary>
Brawler_Reaction::Brawler_Reaction()
	: isInitialize		(false)
{
	auto& json = Singleton<JsonManager>::GetInstance();
	this->animationType		= static_cast<int>(BrawlerEnemy::AnimationType::REACTION);
	this->animationPlayTime = json.GetJson(JsonManager::FileType::BRAWLER_ENEMY)["ANIMATION_PLAY_TIME"][this->animationType];
	this->actionType		= static_cast<short>(BrawlerEnemyBehaviorTree::ActionType::REACTION);
	this->maxSpeed			= 0.0f;
	this->accel				= json.GetJson(JsonManager::FileType::BRAWLER_ENEMY)["ACCEL"];
	this->decel				= json.GetJson(JsonManager::FileType::BRAWLER_ENEMY)["DECEL"];
}

/// <summary>
/// デストラクタ
/// </summary>
Brawler_Reaction::~Brawler_Reaction()
{

}

/// <summary>
/// 初期化
/// </summary>
void Brawler_Reaction::Initialize()
{
	this->isInitialize = false;
}

/// <summary>
/// 更新処理
/// </summary>
Brawler_Reaction::NodeState Brawler_Reaction::Update(BehaviorTree& _tree, Character& _chara)
{
	auto& enemy = dynamic_cast<BrawlerEnemy&>(_chara);

	/*生存フラグが下りていたら以下の処理は行わない*/
	if (!enemy.GetIsAlive())return ActionNode::NodeState::SUCCESS;

	/*登録されているアクションと実際のアクションが異なっていたら*/
	if (_tree.GetNowSelectAction() != this->actionType)
	{
		//アクションの設定
		_tree.SetNowSelectAction(this->actionType);
		//アクションの登録
		_tree.EntryCurrentReaction(*this);
		enemy.ChangeTiredColor();
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
		//アクションの解除
		_tree.ExitCurrentReaction();
		//状態を通常に戻す
		_tree.ExitCurrentBattleAction();
		return ActionNode::NodeState::SUCCESS;
	}
	//それ以外は実行中を返す
	else
	{
		return ActionNode::NodeState::RUNNING;
	}
}
