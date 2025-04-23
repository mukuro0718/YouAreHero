#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "Rigidbody.h"
#include "Character.h"
#include "BehaviorTreeNode.h"
#include "BehaviorTree.h"
#include "ActionNode.h"
#include "Tank_Idle.h"
#include "Boid.h"
#include "Enemy.h"
#include "TankEnemy.h"
#include "EnemyManager.h"
#include "TankEnemyBehaviorTree.h"

/// <summary>
/// コンストラクタ
/// </summary>
Tank_Idle::Tank_Idle()
	: isInitialize		(false)
{
	auto& json = Singleton<JsonManager>::GetInstance();
	this->animationType		= static_cast<int>(TankEnemy::AnimationType::WALK_FRONT);
	this->animationPlayTime = json.GetJson(JsonManager::FileType::TANK_ENEMY)["ANIMATION_PLAY_TIME"][this->animationType];
	this->actionType		= static_cast<short>(TankEnemyBehaviorTree::ActionType::IDLE);
	this->maxSpeed			= json.GetJson(JsonManager::FileType::TANK_ENEMY)["WALK_SPEED"];
	this->accel				= json.GetJson(JsonManager::FileType::TANK_ENEMY)["ACCEL"];
	this->decel				= json.GetJson(JsonManager::FileType::TANK_ENEMY)["DECEL"];
}

/// <summary>
/// デストラクタ
/// </summary>
Tank_Idle::~Tank_Idle()
{

}

/// <summary>
/// 初期化
/// </summary>
void Tank_Idle::Initialize()
{
	this->isInitialize = false;
}

/// <summary>
/// 更新処理
/// </summary>
Tank_Idle::NodeState Tank_Idle::Update(BehaviorTree& _tree, Character& _chara)
{
	auto& enemy = dynamic_cast<TankEnemy&>(_chara);

	/*生存フラグが下りていたら以下の処理は行わない*/
	if (!enemy.GetIsAlive())return ActionNode::NodeState::SUCCESS;

	/*登録されているアクションと実際のアクションが異なっていたら*/
	if (!this->isInitialize)
	{
		//アニメーションの種類を設定
		enemy.SetNowAnimation(this->animationType);
		//アニメーション再生時間の設定
		enemy.SetAnimationPlayTime(this->animationPlayTime);
		this->isInitialize = true;
	}

	/*移動*/
	auto& enemyManager = Singleton<EnemyManager>::GetInstance();
	VECTOR					 nowVelocity = enemy.GetRigidbody().GetVelocity();	//現在の移動ベクトル
	const vector<Character*> weakEnemyList = enemyManager.GetWeakEnemyList();		//雑魚敵のリスト
	VECTOR					 toTarget = enemy.GetRigidbody().GetVelocity();	//移動目標へのベクトル
	//回転率の更新
	enemy.UpdateRotation(VScale(toTarget, -1.0f));
	//移動速度の更新
	enemy.UpdateSpeed(this->maxSpeed, this->accel, this->decel);
	//移動ベクトルを出す
	VECTOR v1 = enemy.GetBoid().RuleCohesion(enemy);
	VECTOR v2 = VScale(enemy.GetBoid().RuleSeparation(enemy, weakEnemyList), 1.5f);
	VECTOR v3 = VGet(0, 0, 0);
	VECTOR v4 = enemy.GetBoid().RuleBounding(enemy);
	VECTOR newVelocity = VAdd(nowVelocity, VAdd(v1, VAdd(v2, VAdd(v3, v4))));
	float newVelocitySize = VSize(newVelocity);
	if (newVelocitySize > this->maxSpeed)
	{
		newVelocity = VScale(VNorm(newVelocity), this->maxSpeed);
	}
	//新しい移動ベクトルを出す
	newVelocity = VGet(newVelocity.x, nowVelocity.y, newVelocity.z);
	enemy.SetVelocity(newVelocity);

	/*アニメーション*/
	//移動していなければIDLEアニメーション、移動していたらWALKアニメーション
	if (newVelocitySize >= 0.1f)
	{
		//アニメーションの種類を設定
		enemy.SetNowAnimation(static_cast<int>(TankEnemy::AnimationType::WALK_FRONT));
	}
	else
	{
		enemy.SetNowAnimation(static_cast<int>(TankEnemy::AnimationType::IDLE));
	}
	enemy.PlayAnimation();


	/*状態を返す*/
	return ActionNode::NodeState::SUCCESS;
}
