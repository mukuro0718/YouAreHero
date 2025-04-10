#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "Character.h"
#include "BehaviorTreeNode.h"
#include "BehaviorTree.h"
#include "ActionNode.h"
#include "Beast_RaiseLevel.h"
#include "Enemy.h"
#include "Beast.h"
#include "EnemyManager.h"
#include "BeastBehaviorTree.h"
#include "EffectManager.h"
#include "SoundManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
Beast_RaiseLevel::Beast_RaiseLevel()
	: effectStartCount	(false)
	, frameCount		(0)
	, roarFrame			(0)
{
	auto& json = Singleton<JsonManager>::GetInstance();
	this->animationType		= static_cast<int>(Beast::AnimationType::RAISE_LEVEL);
	this->actionType		= static_cast<int>(BeastBehaviorTree::ActionType::RAISE_LEVEL);
	this->animationPlayTime = json.GetJson(JsonManager::FileType::BEAST)["ANIMATION_PLAY_TIME"][this->animationType];
	this->interval			= json.GetJson(JsonManager::FileType::BEAST)["ACTION_INTERVAL"][this->actionType];
	this->maxSpeed			= 0.0f;
	this->accel				= json.GetJson(JsonManager::FileType::BEAST)["ACCEL"];
	this->decel				= json.GetJson(JsonManager::FileType::BEAST)["DECEL"];
	this->effectStartCount	= json.GetJson(JsonManager::FileType::BEAST)["RAISE_LEVEL_EFFECT_START_COUNT"];
	this->roarFrame			= json.GetJson(JsonManager::FileType::BEAST)["RAISE_LEVEL_ROAR_COUNT"];
}

/// <summary>
/// デストラクタ
/// </summary>
Beast_RaiseLevel::~Beast_RaiseLevel()
{

}

/// <summary>
/// 更新処理
/// </summary>
Beast_RaiseLevel::NodeState Beast_RaiseLevel::Update(BehaviorTree& _tree, Character& _chara)
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
		//自分をRootに登録
		_tree.EntryCurrentBattleAction(*this);
		enemy.UpdateSpeed(this->maxSpeed, this->accel, this->decel);
		enemy.UpdateVelocity(false);
		enemy.DecAttackComboCount();
	}

	/*エフェクトの制御*/
	this->frameCount++;
	if (this->frameCount < this->effectStartCount)
	{
		if (this->frameCount == this->effectStartCount)
		{
			auto& effect = Singleton<EffectManager>::GetInstance();
			effect.OnIsEffect(EffectManager::EffectType::BEAST_RAISE_LEVEL);
		}
	}
	if (this->frameCount == this->roarFrame)
	{
		auto& sound = Singleton<SoundManager>::GetInstance();
		sound.OnIsPlayEffect(SoundManager::EffectType::LUXURIO_ROAR);
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
	{
		//アニメーションが終了していたら
		if (enemy.GetIsChangeAnimation())
		{
			//インターバルの設定
			_tree.SetInterval(this->actionType, this->interval);
			//登録を解除
			_tree.ExitCurrentBattleAction();
			//レベルの上昇
			auto& beastTree = dynamic_cast<BeastBehaviorTree&>(_tree);
			beastTree.RaiseLevel();
			this->frameCount = 0;
			return ActionNode::NodeState::SUCCESS;
		}
		//それ以外は実行中を返す
		else
		{
			return ActionNode::NodeState::RUNNING;
		}
	}
}
