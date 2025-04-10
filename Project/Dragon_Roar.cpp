#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "Character.h"
#include "BehaviorTreeNode.h"
#include "BehaviorTree.h"
#include "ActionNode.h"
#include "Dragon_Roar.h"
#include "Enemy.h"
#include "Dragon.h"
#include "EnemyManager.h"
#include "DragonBehaviorTree.h"
#include "SoundManager.h"
#include "EffectManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
Dragon_Roar::Dragon_Roar()
	: frameCount(0)
	, roarFrame (0)
{
	auto& json = Singleton<JsonManager>::GetInstance();
	this->animationType		= static_cast<int>(Dragon::AnimationType::ROAR);
	this->animationPlayTime	= json.GetJson(JsonManager::FileType::DRAGON)["ANIMATION_PLAY_TIME"][this->animationType];
	this->actionType		= static_cast<short>(DragonBehaviorTree::ActionType::ROAR);
	this->maxSpeed			= 0.0f;
	this->accel				= json.GetJson(JsonManager::FileType::DRAGON)["ACCEL"];
	this->decel				= json.GetJson(JsonManager::FileType::DRAGON)["DECEL"];
	this->roarFrame			= json.GetJson(JsonManager::FileType::DRAGON)["ROAR_SOUND_COUNT"];
}

/// <summary>
/// デストラクタ
/// </summary>
Dragon_Roar::~Dragon_Roar()
{

}

/// <summary>
/// 更新処理
/// </summary>
Dragon_Roar::NodeState Dragon_Roar::Update(BehaviorTree& _tree, Character& _chara)
{
	/*咆哮を鳴らす*/
	this->frameCount++;
	if (this->frameCount == this->roarFrame)
	{
		//音を鳴らす
		auto& sound = Singleton<SoundManager>::GetInstance();
		sound.OnIsPlayEffect(SoundManager::EffectType::MONSTER_ROAR);
		//エフェクトの再生
		//auto& effect = Singleton<EffectManager>  ::GetInstance();
		//effect.OnIsEffect(EffectManager::EffectType::BOSS_ROAR);
	}

	/*速度が０以上または最初にこのアクションになった時移動処理を行う*/
	auto& enemy = dynamic_cast<Dragon&>(_chara);
	int prevAction = _tree.GetNowSelectAction();
	if (enemy.GetSpeed() != 0.0f || prevAction != this->actionType)
	{
		enemy.UpdateSpeed(this->maxSpeed, this->accel, this->decel);
		enemy.UpdateVelocity(false);
	}

	/*アクションの状態をセット*/
	if (prevAction != this->actionType)
	{
		//アクションの設定
		_tree.SetNowSelectAction(this->actionType);
		//アクションの登録
		_tree.EntryCurrentBattleAction(*this);
	}

	/*アニメーションの再生*/
	enemy.PlayAnimation(this->animationType, this->animationPlayTime);

	/*状態を返す*/
	//アニメーションが終了していたら
	if (enemy.GetIsChangeAnimation())
	{
		//インターバルの設定
		_tree.ExitCurrentBattleAction();
		_tree.SetInterval(this->actionType);
		this->frameCount = 0;
		return ActionNode::NodeState::SUCCESS;
	}
	//それ以外は実行中を返す
	else
	{
		return ActionNode::NodeState::RUNNING;
	}
}
