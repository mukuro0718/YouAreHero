#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "Character.h"
#include "BehaviorTreeNode.h"
#include "BehaviorTree.h"
#include "ActionNode.h"
#include "Tank_Run.h"
#include "Enemy.h"
#include "TankEnemy.h"
#include "EnemyManager.h"
#include "TankEnemyBehaviorTree.h"

/// <summary>
/// コンストラクタ
/// </summary>
Tank_Run::Tank_Run()
	: isInitialize		(false)
{
	auto& json = Singleton<JsonManager>::GetInstance();
	this->animationType		= static_cast<int>(TankEnemy::AnimationType::DYING);
	this->animationPlayTime = json.GetJson(JsonManager::FileType::TANK_ENEMY)["ANIMATION_PLAY_TIME"][this->animationType];
	this->actionType		= static_cast<short>(TankEnemyBehaviorTree::ActionType::DYING);
	this->maxSpeed			= json.GetJson(JsonManager::FileType::TANK_ENEMY)["RUN_SPEED"];
	this->accel				= json.GetJson(JsonManager::FileType::TANK_ENEMY)["ACCEL"];
	this->decel				= json.GetJson(JsonManager::FileType::TANK_ENEMY)["DECEL"];
}

/// <summary>
/// デストラクタ
/// </summary>
Tank_Run::~Tank_Run()
{

}

/// <summary>
/// 初期化
/// </summary>
void Tank_Run::Initialize()
{
	this->isInitialize = false;
}

/// <summary>
/// 更新処理
/// </summary>
Tank_Run::NodeState Tank_Run::Update(BehaviorTree& _tree, Character& _chara)
{
	auto& enemy = dynamic_cast<TankEnemy&>(_chara);

	/*生存フラグが下りていたら以下の処理は行わない*/
	if (!enemy.GetIsAlive())return ActionNode::NodeState::SUCCESS;

	/*登録されているアクションと実際のアクションが異なっていたら*/
	if (_tree.GetNowSelectAction() != this->actionType)
	{
		//アニメーションの種類を設定
		enemy.SetNowAnimation(this->animationType);
		//アニメーション再生時間の設定
		enemy.SetAnimationPlayTime(this->animationPlayTime);
		//アクションの設定
		_tree.SetNowSelectAction(this->actionType);
	}

	/*アニメーションの再生*/
	enemy.PlayAnimation();

	/*移動*/
	enemy.Move(this->maxSpeed, this->accel, this->decel, false);

	/*状態を返す*/
	return ActionNode::NodeState::SUCCESS;
}
