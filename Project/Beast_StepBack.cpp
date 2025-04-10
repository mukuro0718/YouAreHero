#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "Character.h"
#include "BehaviorTreeNode.h"
#include "BehaviorTree.h"
#include "ActionNode.h"
#include "Beast_StepBack.h"
#include "Enemy.h"
#include "Beast.h"
#include "EnemyManager.h"
#include "BeastBehaviorTree.h"

/// <summary>
/// コンストラクタ
/// </summary>
Beast_StepBack::Beast_StepBack()
{
	auto& json = Singleton<JsonManager>::GetInstance();
	this->animationType		= static_cast<int>(Beast::AnimationType::STEP_BACK);
	this->animationPlayTime = json.GetJson(JsonManager::FileType::BEAST)["ANIMATION_PLAY_TIME"][this->animationType];
	this->actionType		= static_cast<short>(BeastBehaviorTree::ActionType::STEP_BACK);
	this->maxSpeed			= json.GetJson(JsonManager::FileType::BEAST)["STEP_BACK_SPEED"];
	this->accel				= json.GetJson(JsonManager::FileType::BEAST)["ACCEL"];
	this->decel				= json.GetJson(JsonManager::FileType::BEAST)["DECEL"];
}

/// <summary>
/// デストラクタ
/// </summary>
Beast_StepBack::~Beast_StepBack()
{

}

/// <summary>
/// 更新処理
/// </summary>
Beast_StepBack::NodeState Beast_StepBack::Update(BehaviorTree& _tree, Character& _chara)
{
	auto& enemy = dynamic_cast<Beast&>(_chara);

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
	{
		//アニメーションが終了していたら
		if (enemy.GetIsChangeAnimation())
		{
			//インターバルの設定
			_tree.SetInterval(this->actionType, this->interval);
			return ActionNode::NodeState::SUCCESS;
		}
		//それ以外は実行中を返す
		else
		{
			return ActionNode::NodeState::RUNNING;
		}
	}
}
