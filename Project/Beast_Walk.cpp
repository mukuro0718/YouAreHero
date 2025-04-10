#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "Character.h"
#include "BehaviorTreeNode.h"
#include "BehaviorTree.h"
#include "ActionNode.h"
#include "Beast_Walk.h"
#include "Enemy.h"
#include "Beast.h"
#include "EnemyManager.h"
#include "BeastBehaviorTree.h"
#include "SoundManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
Beast_Walk::Beast_Walk()
	: frameCount	(0)
	, walkSoundCount(0)
{
	auto& json = Singleton<JsonManager>::GetInstance();
	this->animationType		= static_cast<int>(Beast::AnimationType::WALK);
	this->animationPlayTime = json.GetJson(JsonManager::FileType::BEAST)["ANIMATION_PLAY_TIME"][this->animationType];
	this->maxSpeed			= json.GetJson(JsonManager::FileType::BEAST)["WALK_SPEED"];
	this->accel				= json.GetJson(JsonManager::FileType::BEAST)["ACCEL"];
	this->decel				= json.GetJson(JsonManager::FileType::BEAST)["DECEL"];
	this->walkSoundCount	= json.GetJson(JsonManager::FileType::BEAST)["WALK_SOUND_COUNT"];
	this->actionType		= static_cast<short>(BeastBehaviorTree::ActionType::WALK);
}

/// <summary>
/// デストラクタ
/// </summary>
Beast_Walk::~Beast_Walk()
{

}

/// <summary>
/// 更新処理
/// </summary>
Beast_Walk::NodeState Beast_Walk::Update(BehaviorTree& _tree, Character& _chara)
{
	auto& enemy = dynamic_cast<Beast&>(_chara);

	auto& sound = Singleton<SoundManager>::GetInstance();
	this->frameCount++;
	if (this->frameCount == this->walkSoundCount)
	{
		sound.OnIsPlayEffect(SoundManager::EffectType::MONSTER_FOOTSTEPS);
		this->frameCount = 0;
	}


	/*登録されているアクションと実際のアクションが異なっていたら*/
	if (_tree.GetNowSelectAction() != this->actionType)
	{
		//アニメーションの種類を設定
		enemy.SetNowAnimation(this->animationType);
		//アニメーション再生時間の設定
		enemy.SetAnimationPlayTime(this->animationPlayTime);
		//アクションの設定
		_tree.SetNowSelectAction(this->actionType);
		this->frameCount = 0;
	}

	/*アニメーションの再生*/
	enemy.PlayAnimation();

	/*移動*/
	enemy.Move(this->maxSpeed, this->accel, this->decel, false);

	/*状態を返す*/
	{
		return ActionNode::NodeState::SUCCESS;
	}
}
