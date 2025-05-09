#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "Rigidbody.h"
#include "ColliderData.h"
#include "AttackCapsuleColliderData.h"
#include "AttackData.h"
#include "Character.h"
#include "BehaviorTreeNode.h"
#include "BehaviorTree.h"
#include "ActionNode.h"
#include "Enemy.h"
#include "Dragon.h"
#include "Dragon_Rotate.h"
#include "EnemyManager.h"
#include "DragonBehaviorTree.h"
#include "ReactionType.h"
#include "HitStop.h"
#include "SoundManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
Dragon_Rotate::Dragon_Rotate()
	: USE_COLLIDER_INDEX(static_cast<short>(Dragon::AttackCollider::ROTATE))
	, frameCount		(0)
	, currentDragonState(0)
{
	auto& json = Singleton<JsonManager>::GetInstance();
	this->animationType				= static_cast<int>(Dragon::AnimationType::ROTATE_ATTACK);
	vector<float>animationPlayTime	= json.GetJson(JsonManager::FileType::DRAGON)["ROTATE_PLAY_TIME"];
	this->animationPlayTime			= animationPlayTime;
	this->actionType				= static_cast<short>(DragonBehaviorTree::ActionType::ROTATE);
	this->maxSpeed					= 0.0f;
	this->accel						= json.GetJson(JsonManager::FileType::DRAGON)["ACCEL"];
	this->decel						= json.GetJson(JsonManager::FileType::DRAGON)["DECEL"];
	this->isFixRotate				= true;
	this->interval					= json.GetJson(JsonManager::FileType::DRAGON)["ROTATE_INTERVAL"];
}

/// <summary>
/// デストラクタ
/// </summary>
Dragon_Rotate::~Dragon_Rotate()
{

}

/// <summary>
/// 更新処理
/// </summary>
Dragon_Rotate::NodeState Dragon_Rotate::Update(BehaviorTree& _tree, Character& _chara)
{
	/*アクションの状態をセット*/
	auto& enemy = dynamic_cast<Dragon&>(_chara);
	if (this->frameCount == 0)
	{
		//アクションの設定
		_tree.SetNowSelectAction(this->actionType);
		//アクションの登録
		_tree.EntryCurrentBattleAction(*this);
		//段階を取得
		this->currentDragonState = enemy.GetBossState();
		this->frameCount++;
	}

	/*移動*/
	enemy.UpdateSpeed(0.0f, this->accel, this->decel);
	enemy.UpdateVelocity(false);

	/*当たり判定コライダーの更新*/
	enemy.UpdateAttackCollider(this->USE_COLLIDER_INDEX, this->nowTotalPlayTime);

	/*アニメーションの再生*/
	float playTime = this->animationPlayTime[this->currentDragonState];
	this->nowTotalPlayTime += playTime;
	enemy.PlayAnimation(this->animationType, playTime);

	/*状態を返す*/
	//アニメーションが終了していたら
	if (enemy.GetIsChangeAnimation())
	{
		this->nowTotalPlayTime = 0.0f;
		this->frameCount = 0;
		enemy.OffAttackCollider(this->USE_COLLIDER_INDEX);
		_tree.ExitCurrentBattleAction();
		_tree.SetInterval(this->actionType, this->interval);
		enemy.DecAttackComboCount();
		return ActionNode::NodeState::SUCCESS;
	}
	//それ以外は実行中を返す
	return ActionNode::NodeState::RUNNING;
}
