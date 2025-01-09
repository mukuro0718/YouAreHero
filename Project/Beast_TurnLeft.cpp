#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "BehaviorTreeNode.h"
#include "ActionNode.h"
#include "Beast_TurnLeft.h"
#include "Character.h"
#include "Enemy.h"
#include "Beast.h"
#include "EnemyManager.h"
#include "BeastBehaviorTree.h"

/// <summary>
/// コンストラクタ
/// </summary>
Beast_TurnLeft::Beast_TurnLeft()
	: frameCount	(0)
	, maxFrameCount	(0)
{
	auto& json = Singleton<JsonManager>::GetInstance();
	this->animationPlayTime = json.GetJson(JsonManager::FileType::BEAST)["ANIMATION_PLAY_TIME"][this->animationType];
	this->animationType		= static_cast<int>(Beast::AnimationType::TURN_LEFT);
	this->actionType		= static_cast<int>(BeastBehaviorTree::ActionType::TURN_LEFT);
	this->interval			= json.GetJson(JsonManager::FileType::BEAST)["ACTION_INTERVAL"][this->actionType];
	this->maxSpeed			= 0.0f;
	this->accel				= json.GetJson(JsonManager::FileType::BEAST)["ACCEL"];
	this->decel				= json.GetJson(JsonManager::FileType::BEAST)["DECEL"];
	this->maxFrameCount		= json.GetJson(JsonManager::FileType::BEAST)["TURN_WAIT_COUNT"];

}

/// <summary>
/// デストラクタ
/// </summary>
Beast_TurnLeft::~Beast_TurnLeft()
{

}

/// <summary>
/// 更新処理
/// </summary>
Beast_TurnLeft::NodeState Beast_TurnLeft::Update()
{
	auto& rootNode = Singleton<BeastBehaviorTree>::GetInstance();
	auto& enemyManager = Singleton<EnemyManager>::GetInstance();
	auto& enemy = dynamic_cast<Beast&>(enemyManager.GetCharacter());

	/*登録されているアクションと実際のアクションが異なっていたら*/
	if (rootNode.GetNowSelectAction() != this->actionType)
	{
		//アニメーションの種類を設定
		enemy.SetNowAnimation(this->animationType);
		//アニメーション再生時間の設定
		enemy.SetAnimationPlayTime(this->animationPlayTime);
		//アクションの設定
		rootNode.SetSelectAction(this->actionType);
		//自分をRootに登録
		rootNode.EntryCurrentBattleAction(*this);
	}

	/*アニメーションの再生*/
	enemy.PlayAnimation();

	/*フレームカウントが最大以上でなければ早期リターン*/
	this->frameCount++;
	if (this->frameCount < this->maxFrameCount)return ActionNode::NodeState::RUNNING;


	/*移動*/
	enemy.Move(this->maxSpeed, this->accel, this->decel, false);

	/*状態を返す*/
	//アニメーションが終了していたら
	if (enemy.GetIsChangeAnimation())
	{
		//登録を解除
		rootNode.ExitCurrentBattleAction();
		this->frameCount = 0;
		return ActionNode::NodeState::SUCCESS;
	}
	//それ以外は実行中を返す
	else
	{
		return ActionNode::NodeState::RUNNING;
	}
}
