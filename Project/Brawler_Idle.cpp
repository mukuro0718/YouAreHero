#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "Rigidbody.h"
#include "Character.h"
#include "BehaviorTreeNode.h"
#include "BehaviorTree.h"
#include "ActionNode.h"
#include "Brawler_Idle.h"
#include "Boid.h"
#include "Enemy.h"
#include "BrawlerEnemy.h"
#include "EnemyManager.h"
#include "BrawlerEnemyBehaviorTree.h"

/// <summary>
/// コンストラクタ
/// </summary>
Brawler_Idle::Brawler_Idle()
	: isInitialize		(false)
{
	auto& json = Singleton<JsonManager>::GetInstance();
	this->animationType		= static_cast<int>(BrawlerEnemy::AnimationType::WALK_FRONT);
	this->animationPlayTime = json.GetJson(JsonManager::FileType::BRAWLER_ENEMY)["ANIMATION_PLAY_TIME"][this->animationType];
	this->actionType		= static_cast<short>(BrawlerEnemyBehaviorTree::ActionType::IDLE);
	this->maxSpeed			= json.GetJson(JsonManager::FileType::BRAWLER_ENEMY)["WALK_SPEED"];
	this->accel				= json.GetJson(JsonManager::FileType::BRAWLER_ENEMY)["ACCEL"];
	this->decel				= json.GetJson(JsonManager::FileType::BRAWLER_ENEMY)["DECEL"];
}

/// <summary>
/// デストラクタ
/// </summary>
Brawler_Idle::~Brawler_Idle()
{

}

/// <summary>
/// 初期化
/// </summary>
void Brawler_Idle::Initialize()
{
	this->isInitialize = false;
}

/// <summary>
/// 更新処理
/// </summary>
Brawler_Idle::NodeState Brawler_Idle::Update(BehaviorTree& _tree, Character& _chara)
{
	auto& enemy = dynamic_cast<BrawlerEnemy&>(_chara);

	/*生存フラグが下りていたら以下の処理は行わない*/
	if (!enemy.GetIsAlive())return ActionNode::NodeState::SUCCESS;

	/*登録されているアクションと実際のアクションが異なっていたら*/
	if (_tree.GetNowSelectAction() != this->actionType)
	{
		//アニメーションの種類を設定
		enemy.SetNowAnimation(this->animationType);
		//アニメーション再生時間の設定
		enemy.SetAnimationPlayTime(this->animationPlayTime);
		this->isInitialize = true;
	}

	/*アニメーションの再生*/
	enemy.PlayAnimation();

	/*移動*/
	auto& enemyManager = Singleton<EnemyManager>::GetInstance();
	VECTOR					 nowVelocity	= enemy.GetRigidbody().GetVelocity();	//現在の移動ベクトル
	const vector<Character*> weakEnemyList	= enemyManager.GetWeakEnemyList();		//雑魚敵のリスト
	VECTOR					 toTarget		= enemy.GetRigidbody().GetVelocity();	//移動目標へのベクトル
	//回転率の更新
	enemy.UpdateRotation(VScale(toTarget,-1.0f));
	//移動速度の更新
	enemy.UpdateSpeed(this->maxSpeed, this->accel, this->decel);
	//移動ベクトルを出す
	VECTOR v1 = enemy.GetBoid().RuleCohesion(enemy);
	VECTOR v2 = VScale(enemy.GetBoid().RuleSeparation(enemy, weakEnemyList), 1.5f);
	VECTOR v3 = VGet(0, 0, 0);
	VECTOR v4 = enemy.GetBoid().RuleBounding(enemy);
	VECTOR newVelocity = VAdd(nowVelocity, VAdd(v1, VAdd(v2, VAdd(v3, v4))));
	if (VSize(newVelocity) > this->maxSpeed)
	{
		newVelocity = VScale(VNorm(newVelocity), this->maxSpeed);
	}
	//新しい移動ベクトルを出す
	newVelocity = VGet(newVelocity.x, nowVelocity.y, newVelocity.z);
	enemy.SetVelocity(newVelocity);

	/*状態を返す*/
	return ActionNode::NodeState::SUCCESS;
}
