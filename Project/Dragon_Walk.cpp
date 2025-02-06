#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "BehaviorTreeNode.h"
#include "ActionNode.h"
#include "Dragon_Walk.h"
#include "Character.h"
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
Dragon_Walk::NodeState Dragon_Walk::Update()
{
	auto& sound = Singleton<SoundManager>::GetInstance();
	this->frameCount++;
	if (this->frameCount == this->walkSoundCount)
	{
		sound.OnIsPlayEffect(SoundManager::EffectType::MONSTER_FOOTSTEPS);
		this->frameCount = 0;
	}


	/*アクションの状態をセット*/
	auto& rootNode = Singleton<DragonBehaviorTree>::GetInstance();
	if (rootNode.GetCurrentAction() != this->actionType)
	{
		rootNode.SetCurrentAction(this->actionType);
	}

	/*アニメーションの再生*/
	auto& enemyManager = Singleton<EnemyManager>::GetInstance();
	auto& enemy = dynamic_cast<Dragon&>(enemyManager.GetCharacter());
	enemy.PlayAnimation(this->animationType, this->animationPlayTime);

	/*移動*/
	enemy.Move(this->maxSpeed, this->accel, this->decel, false);

	/*状態を返す*/
	return ActionNode::NodeState::SUCCESS;
}
