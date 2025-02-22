#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "BehaviorTreeNode.h"
#include "ActionNode.h"
#include "Beast_Roar.h"
#include "Character.h"
#include "Enemy.h"
#include "Beast.h"
#include "EnemyManager.h"
#include "BeastBehaviorTree.h"
#include "SoundManager.h"
#include "EffectManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
Beast_Roar::Beast_Roar()
	: frameCount		 (0)
	, roarFrame			 (0)
	, isFinishedFirstRoar(false)
{
	auto& json = Singleton<JsonManager>::GetInstance();
	this->animationType		= static_cast<int>(Beast::AnimationType::ROAR);
	this->animationPlayTime = json.GetJson(JsonManager::FileType::BEAST)["ANIMATION_PLAY_TIME"][this->animationType];
	this->actionType		= static_cast<short>(BeastBehaviorTree::ActionType::ROAR);
	this->maxSpeed			= 0.0f;
	this->accel				= json.GetJson(JsonManager::FileType::BEAST)["ACCEL"];
	this->decel				= json.GetJson(JsonManager::FileType::BEAST)["DECEL"];
	this->roarFrame			= json.GetJson(JsonManager::FileType::BEAST)["NORMAL_ROAR_COUNT"];
}

/// <summary>
/// デストラクタ
/// </summary>
Beast_Roar::~Beast_Roar()
{

}

/// <summary>
/// 更新処理
/// </summary>
Beast_Roar::NodeState Beast_Roar::Update()
{
	/*アニメーション*/
	auto& enemyManager = Singleton<EnemyManager>::GetInstance();
	auto& enemy = dynamic_cast<Beast&>(enemyManager.GetCharacter());
	//初期化されていなければ
	if (this->frameCount == 0)
	{
		//アニメーションの種類を設定
		enemy.SetNowAnimation(this->animationType);
		//アニメーション再生時間の設定
		enemy.SetAnimationPlayTime(this->animationPlayTime);
		//アクションの状態をセット
		auto& rootNode = Singleton<BeastBehaviorTree>::GetInstance();
		rootNode.SetSelectAction(this->actionType);
		//アクションの登録
		rootNode.EntryCurrentBattleAction(*this);
		enemy.UpdateSpeed(this->maxSpeed, this->accel, this->decel);
		enemy.UpdateVelocity(false);
	}

	/*咆哮を鳴らす*/
	this->frameCount++;
	if (this->frameCount == this->roarFrame)
	{
		auto& sound = Singleton<SoundManager>::GetInstance();
		sound.OnIsPlayEffect(SoundManager::EffectType::LUXURIO_ROAR);
		this->isFinishedFirstRoar = true;
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
	{
		//アニメーションが終了していたら
		if (enemy.GetIsChangeAnimation())
		{
			auto& rootNode = Singleton<BeastBehaviorTree>::GetInstance();
			rootNode.SetInterval(this->actionType);
			//アクションの解除
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
}
