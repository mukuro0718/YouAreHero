#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "Character.h"
#include "BehaviorTreeNode.h"
#include "BehaviorTree.h"
#include "ActionNode.h"
#include "Brawler_Walk.h"
#include "Enemy.h"
#include "BrawlerEnemy.h"
#include "EnemyManager.h"
#include "BrawlerEnemyBehaviorTree.h"

/// <summary>
/// コンストラクタ
/// </summary>
Brawler_Walk::Brawler_Walk()
	: frameCount(0)
	, walkSoundCount(0)
{
	auto& json = Singleton<JsonManager>::GetInstance();
	this->animationType		= static_cast<int>(BrawlerEnemy::AnimationType::WALK_FRONT);
	this->animationPlayTime = json.GetJson(JsonManager::FileType::BRAWLER_ENEMY)["ANIMATION_PLAY_TIME"][this->animationType];
	this->maxSpeed			= json.GetJson(JsonManager::FileType::BRAWLER_ENEMY)["WALK_SPEED"];;
	this->accel				= json.GetJson(JsonManager::FileType::BRAWLER_ENEMY)["ACCEL"];
	this->walkSoundCount	= json.GetJson(JsonManager::FileType::BRAWLER_ENEMY)["WALK_SOUND_COUNT"];
	this->decel				= json.GetJson(JsonManager::FileType::BRAWLER_ENEMY)["DECEL"];
	this->actionType		= static_cast<short>(BrawlerEnemyBehaviorTree::ActionType::WALK);
}

/// <summary>
/// デストラクタ
/// </summary>
Brawler_Walk::~Brawler_Walk()
{

}

/// <summary>
/// 更新処理
/// </summary>
Brawler_Walk::NodeState Brawler_Walk::Update(BehaviorTree& _tree, Character& _chara)
{
	auto& enemy = dynamic_cast<BrawlerEnemy&>(_chara);

	/*生存フラグが下りていたら以下の処理は行わない*/
	if (!enemy.GetIsAlive())return ActionNode::NodeState::SUCCESS;

	//auto& sound = Singleton<SoundManager>::GetInstance();
	//this->frameCount++;
	//if (this->frameCount == this->walkSoundCount)
	//{
	//	sound.OnIsPlayEffect(SoundManager::EffectType::MONSTER_FOOTSTEPS);
	//	this->frameCount = 0;
	//}

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
