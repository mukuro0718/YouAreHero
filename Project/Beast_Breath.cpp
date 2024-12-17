#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "BehaviorTreeNode.h"
#include "ActionNode.h"
#include "Beast_Breath.h"
#include "Character.h"
#include "Enemy.h"
#include "Beast.h"
#include "EnemyManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
Beast_Breath::Beast_Breath()
	: stage(AnimationStage::START)
	, animationSet()
	, nextStageSet()
{
	this->animationSet.emplace(AnimationStage::START, static_cast<int>(Beast::AnimationType::BREATH_START));
	this->animationSet.emplace(AnimationStage::LOOP, static_cast<int>(Beast::AnimationType::BREATH_LOOP));
	this->animationSet.emplace(AnimationStage::END, static_cast<int>(Beast::AnimationType::BREATH_END));
	this->nextStageSet.emplace(AnimationStage::START, AnimationStage::LOOP);
	this->nextStageSet.emplace(AnimationStage::LOOP, AnimationStage::END);
	this->nextStageSet.emplace(AnimationStage::END, AnimationStage::START);

}

/// <summary>
/// デストラクタ
/// </summary>
Beast_Breath::~Beast_Breath()
{

}

/// <summary>
/// 更新処理
/// </summary>
Beast_Breath::NodeState Beast_Breath::Update()
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
		this->stage = this->nextStageSet[this->stage];
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
