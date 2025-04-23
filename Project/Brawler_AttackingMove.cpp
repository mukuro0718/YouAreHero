#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "VECTORtoUseful.h"
#include "Rigidbody.h"
#include "Character.h"
#include "BehaviorTreeNode.h"
#include "BehaviorTree.h"
#include "ActionNode.h"
#include "Brawler_AttackingMove.h"
#include "Boid.h"
#include "Enemy.h"
#include "Player.h"
#include "BrawlerEnemy.h"
#include "EnemyManager.h"
#include "PlayerManager.h"
#include "BrawlerEnemyBehaviorTree.h"

/// <summary>
/// コンストラクタ
/// </summary>
Brawler_AttackingMove::Brawler_AttackingMove()
	: isInitialize		(false)
{
	auto& json = Singleton<JsonManager>::GetInstance();
	this->animationType		= static_cast<int>(BrawlerEnemy::AnimationType::RUN);
	this->animationPlayTime = json.GetJson(JsonManager::FileType::BRAWLER_ENEMY)["ANIMATION_PLAY_TIME"][this->animationType];
	this->actionType		= static_cast<short>(BrawlerEnemyBehaviorTree::ActionType::RUN);
	this->maxSpeed			= json.GetJson(JsonManager::FileType::BRAWLER_ENEMY)["RUN_SPEED"];
	this->accel				= json.GetJson(JsonManager::FileType::BRAWLER_ENEMY)["ACCEL"];
	this->decel				= json.GetJson(JsonManager::FileType::BRAWLER_ENEMY)["DECEL"];
}

/// <summary>
/// デストラクタ
/// </summary>
Brawler_AttackingMove::~Brawler_AttackingMove()
{

}

/// <summary>
/// 初期化
/// </summary>
void Brawler_AttackingMove::Initialize()
{
	this->isInitialize = false;
}

/// <summary>
/// 更新処理
/// </summary>
Brawler_AttackingMove::NodeState Brawler_AttackingMove::Update(BehaviorTree& _tree, Character& _chara)
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
		//アクションの設定
		_tree.SetNowSelectAction(this->actionType);
	}

	/*アニメーションの再生*/
	enemy.PlayAnimation();

	/*移動*/
	auto&enemyManager = Singleton<EnemyManager>::GetInstance();
	auto& player = Singleton<PlayerManager>::GetInstance();
	VECTOR					 nowVelocity	= enemy.GetRigidbody().GetVelocity();													//現在の移動ベクトル
	const vector<Character*> weakEnemyList	= enemyManager.GetWeakEnemyList();														//雑魚敵のリスト
	VECTOR					 toTarget		= VSub(enemy.GetRigidbody().GetPosition(), player.GetRigidbody().GetPosition());	//移動目標へのベクトル
	//回転率の更新
	enemy.UpdateRotation(toTarget);
	//移動速度の更新
	enemy.UpdateSpeed(this->maxSpeed,this->accel,this->decel);

	if (VSize(toTarget) >= 8.0f)
	{
		//移動ベクトルを出す
		VECTOR v1 = enemy.GetBoid().RuleSeparation(enemy, weakEnemyList);
		VECTOR v2 = enemy.GetBoid().RuleBounding(enemy);
		VECTOR v3 = enemy.GetBoid().RuleTargetAttraction(enemy, player.GetRigidbody().GetPosition());
		VECTOR newVelocity = VAdd(nowVelocity, VAdd(v1, VAdd(v2, v3)));
		if (VSize(newVelocity) > this->maxSpeed)
		{
			newVelocity = VScale(VNorm(newVelocity), this->maxSpeed);
		}
		//新しい移動ベクトルを出す
		newVelocity = VGet(newVelocity.x, nowVelocity.y, newVelocity.z);
		enemy.SetVelocity(newVelocity);
	}
	else
	{
		enemy.SetVelocity(VGet(0.0f, nowVelocity.y, 0.0f));
	}
	/*状態を返す*/
	return ActionNode::NodeState::SUCCESS;
}
