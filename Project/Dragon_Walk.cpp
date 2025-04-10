#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "Character.h"
#include "BehaviorTreeNode.h"
#include "BehaviorTree.h"
#include "ActionNode.h"
#include "Dragon_Walk.h"
#include "Enemy.h"
#include "Dragon.h"
#include "EnemyManager.h"
#include "DragonBehaviorTree.h"
#include "SoundManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
Dragon_Walk::Dragon_Walk()
	: frameCount	(0)
	, walkSoundCount(0)
{
	auto& json = Singleton<JsonManager>::GetInstance();
	this->animationType		= static_cast<int>(Dragon::AnimationType::WALK);
	this->animationPlayTime	= json.GetJson(JsonManager::FileType::DRAGON)["ANIMATION_PLAY_TIME"][this->animationType];
	this->actionType		= static_cast<short>(DragonBehaviorTree::ActionType::WALK);
	this->maxSpeed			= json.GetJson(JsonManager::FileType::DRAGON)["WALK_SPEED"];
	this->accel				= json.GetJson(JsonManager::FileType::DRAGON)["ACCEL"];
	this->decel				= json.GetJson(JsonManager::FileType::DRAGON)["DECEL"];
	this->walkSoundCount	= json.GetJson(JsonManager::FileType::DRAGON)["WALK_SOUND_COUNT"];
}

/// <summary>
/// デストラクタ
/// </summary>
Dragon_Walk::~Dragon_Walk()
{

}

/// <summary>
/// 更新処理
/// </summary>
Dragon_Walk::NodeState Dragon_Walk::Update(BehaviorTree& _tree, Character& _chara)
{
	auto& sound = Singleton<SoundManager>::GetInstance();
	this->frameCount++;
	if (this->frameCount == this->walkSoundCount)
	{
		sound.OnIsPlayEffect(SoundManager::EffectType::MONSTER_FOOTSTEPS);
		this->frameCount = 0;
	}


	/*アクションの状態をセット*/
	if (_tree.GetNowSelectAction() != this->actionType)
	{
		_tree.SetNowSelectAction(this->actionType);
	}

	/*アニメーションの再生*/
	auto& enemy = dynamic_cast<Dragon&>(_chara);
	enemy.PlayAnimation(this->animationType, this->animationPlayTime);

	/*移動*/
	enemy.Move(this->maxSpeed, this->accel, this->decel, false);

	/*状態を返す*/
	return ActionNode::NodeState::SUCCESS;
}
