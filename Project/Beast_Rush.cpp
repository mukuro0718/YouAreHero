#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "BehaviorTreeNode.h"
#include "ActionNode.h"
#include "Beast_Rush.h"
#include "Character.h"
#include "Enemy.h"
#include "Beast.h"
#include "EnemyManager.h"
#include "BeastBehaviorTree.h"

/// <summary>
/// コンストラクタ
/// </summary>
Beast_Rush::Beast_Rush()
{
	this->animationSet.emplace(AnimationStage::START, static_cast<int>(Beast::AnimationType::RUSH_START));
	this->animationSet.emplace(AnimationStage::LOOP, static_cast<int>(Beast::AnimationType::RUSH_LOOP));
	this->animationSet.emplace(AnimationStage::NORMAL_END, static_cast<int>(Beast::AnimationType::RUSH_NORMAL_END));
	this->animationSet.emplace(AnimationStage::ANGRY_END, static_cast<int>(Beast::AnimationType::RUSH_ROTATE_END));
	this->nextStageSet.emplace(AnimationStage::START, AnimationStage::LOOP);
	this->nextStageSet.emplace(AnimationStage::LOOP, AnimationStage::NORMAL_END);
	this->nextStageSet.emplace(AnimationStage::NORMAL_END, AnimationStage::START);
	this->nextStageSet.emplace(AnimationStage::ANGRY_END, AnimationStage::START);

}

/// <summary>
/// デストラクタ
/// </summary>
Beast_Rush::~Beast_Rush()
{

}

/// <summary>
/// 更新処理
/// </summary>
Beast_Rush::NodeState Beast_Rush::Update()
{
	/*アニメーション*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& enemyManager = Singleton<EnemyManager>::GetInstance();
	auto& enemy = dynamic_cast<Beast&>(enemyManager.GetCharacter());
	{
		//アニメーションの種類を設定
		int animationType = this->animationSet[this->stage];
		enemy.SetNowAnimation(animationType);
		//アニメーション再生時間の設定
		enemy.SetAnimationPlayTime(json.GetJson(JsonManager::FileType::BEAST)["ANIMATION_PLAY_TIME"][animationType]);
		//アニメーションの再生
		enemy.PlayAnimation();
	}

	/*状態を返す*/
	//アニメーションが終了していたら
	if (enemy.GetIsChangeAnimation())
	{
		auto& rootNode = Singleton<BeastBehaviorTree>::GetInstance();
		this->stage = this->nextStageSet[this->stage];
		if (this->stage == AnimationStage::NORMAL_END && 
			rootNode.GetBeastState() == BeastBehaviorTree::BeastState::ANGRY)
		{
			this->stage = AnimationStage::ANGRY_END;
		}
		if (this->stage == AnimationStage::START)
		{
			return ActionNode::NodeState::SUCCESS;
		}
		return ActionNode::NodeState::RUNNING;
	}
	else
	{
		return ActionNode::NodeState::RUNNING;
	}
}
