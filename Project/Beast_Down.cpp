#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "Character.h"
#include "BehaviorTreeNode.h"
#include "BehaviorTree.h"
#include "ActionNode.h"
#include "Beast_Down.h"
#include "Enemy.h"
#include "Beast.h"
#include "EnemyManager.h"
#include "BeastBehaviorTree.h"

/// <summary>
/// コンストラクタ
/// </summary>
Beast_Down::Beast_Down()
	: isInitialize(false)
{
	this->animationSet.emplace(AnimationStage::START, static_cast<int>(Beast::AnimationType::SHORT_FRIGHT_START));
	this->animationSet.emplace(AnimationStage::LOOP, static_cast<int>(Beast::AnimationType::SHORT_FRIGHT_LOOP));
	this->animationSet.emplace(AnimationStage::END, static_cast<int>(Beast::AnimationType::SHORT_FRIGHT_END));
	this->nextStageSet.emplace(AnimationStage::START, AnimationStage::LOOP);
	this->nextStageSet.emplace(AnimationStage::LOOP, AnimationStage::END);
	this->nextStageSet.emplace(AnimationStage::END, AnimationStage::START);

	auto& json = Singleton<JsonManager>::GetInstance();
	this->animationPlayTime = json.GetJson(JsonManager::FileType::BEAST)["ANIMATION_PLAY_TIME"][this->animationType];
	this->actionType		= static_cast<short>(BeastBehaviorTree::ActionType::DOWN);
	this->maxSpeed			= 0.0f;
	this->accel				= json.GetJson(JsonManager::FileType::BEAST)["ACCEL"];
	this->decel				= json.GetJson(JsonManager::FileType::BEAST)["DECEL"];

}

/// <summary>
/// デストラクタ
/// </summary>
Beast_Down::~Beast_Down()
{

}

/// <summary>
/// 更新処理
/// </summary>
Beast_Down::NodeState Beast_Down::Update(BehaviorTree& _tree, Character& _chara)
{
	auto& enemy = dynamic_cast<Beast&>(_chara);

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

	/*アニメーション*/
	//アニメーションの種類を設定
	int nowAnimationType = this->animationSet[this->stage];
	if (this->animationType != nowAnimationType)
	{
		//再生するアニメーションを設定
		this->animationType = nowAnimationType;
		enemy.SetNowAnimation(this->animationType);
		//アニメーション再生時間の設定
		auto& json = Singleton<JsonManager>::GetInstance();
		enemy.SetAnimationPlayTime(json.GetJson(JsonManager::FileType::BEAST)["ANIMATION_PLAY_TIME"][this->animationType]);
	}
	//アニメーションの再生
	enemy.PlayAnimation();

	/*移動*/
	if (enemy.GetSpeed() != 0.0f)
	{
		enemy.UpdateSpeed(this->maxSpeed, this->accel, this->decel);
		enemy.UpdateVelocity(false);
	}

	/*状態を返す*/
	if (enemy.GetIsChangeAnimation())
	{
		/*状態がループの時にダウン値が残っていたらstageを変えない*/
		if (this->stage == AnimationStage::LOOP && enemy.GetBossState() == static_cast<int>(Beast::BossState::TIRED))
		{
			return ActionNode::NodeState::RUNNING;
		}
		this->stage = this->nextStageSet[this->stage];
		if (this->stage == AnimationStage::START)
		{
			//アクションの解除
			_tree.ExitCurrentReaction();
			//状態を通常に戻す
			_tree.ExitCurrentBattleAction();
			return ActionNode::NodeState::SUCCESS;
		}
		return ActionNode::NodeState::RUNNING;
	}
	return ActionNode::NodeState::RUNNING;
}
