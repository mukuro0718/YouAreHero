#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "Rigidbody.h"
#include "Character.h"
#include "BehaviorTreeNode.h"
#include "BehaviorTree.h"
#include "ActionNode.h"
#include "Tank_Walk.h"
#include "Boid.h"
#include "Enemy.h"
#include "Player.h"
#include "TankEnemy.h"
#include "EnemyManager.h"
#include "PlayerManager.h"
#include "TankEnemyBehaviorTree.h"

/// <summary>
/// コンストラクタ
/// </summary>
Tank_Walk::Tank_Walk()
	: frameCount(0)
	, walkSoundCount(0)
{
	auto& json = Singleton<JsonManager>::GetInstance();
	this->animationType		= static_cast<int>(TankEnemy::AnimationType::WALK_FRONT);
	this->animationPlayTime = json.GetJson(JsonManager::FileType::TANK_ENEMY)["ANIMATION_PLAY_TIME"][this->animationType];
	this->actionType		= static_cast<short>(TankEnemyBehaviorTree::ActionType::WALK);
	this->maxSpeed			= json.GetJson(JsonManager::FileType::TANK_ENEMY)["WALK_SPEED"];
	this->accel				= json.GetJson(JsonManager::FileType::TANK_ENEMY)["ACCEL"];
	this->decel				= json.GetJson(JsonManager::FileType::TANK_ENEMY)["DECEL"];
	this->walkSoundCount	= json.GetJson(JsonManager::FileType::TANK_ENEMY)["WALK_SOUND_COUNT"];
}

/// <summary>
/// デストラクタ
/// </summary>
Tank_Walk::~Tank_Walk()
{

}

/// <summary>
/// 更新処理
/// </summary>
Tank_Walk::NodeState Tank_Walk::Update(BehaviorTree& _tree, Character& _chara)
{
	auto& enemyManager = Singleton<EnemyManager>::GetInstance();
	auto& player = Singleton<PlayerManager>::GetInstance();
	auto& enemy = dynamic_cast<TankEnemy&>(_chara);

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
		this->frameCount = 0;
	}

	/*移動*/
	VECTOR					 nowVelocity	= enemy.GetRigidbody().GetVelocity();													//現在の移動ベクトル
	const vector<Character*> weakEnemyList	= enemyManager.GetWeakEnemyList();														//雑魚敵のリスト
	VECTOR					 toTarget		= VSub(enemy.GetRigidbody().GetPosition(), player.GetRigidbody().GetPosition());	//移動目標へのベクトル
	//回転率の更新
	enemy.UpdateRotation(toTarget);
	//移動速度の更新
	enemy.UpdateSpeed(this->maxSpeed, this->accel, this->decel);
	//移動ベクトルを出す
	//VECTOR v1 = enemy.GetBoid().RuleSurroundPlayer(enemy, player.GetRigidbody().GetPosition());
	VECTOR v1 = VGet(0, 0, 0);
	VECTOR v2 = enemy.GetBoid().RuleSeparation(enemy, weakEnemyList);
	VECTOR v3 = enemy.GetBoid().RuleAlignment(enemy, weakEnemyList);
	VECTOR v4 = enemy.GetBoid().RuleBounding(enemy);
	VECTOR newVelocity = VAdd(nowVelocity, VAdd(v1, VAdd(v2, VAdd(v3, v4))));
	if (VSize(newVelocity) > this->maxSpeed)
	{
		newVelocity = VScale(VNorm(newVelocity), this->maxSpeed);
	}
	//新しい移動ベクトルを出す
	newVelocity = VGet(newVelocity.x, nowVelocity.y, newVelocity.z);
	enemy.SetVelocity(newVelocity);

	/*アニメーションの種類を決める*/
	VECTOR forward = VNorm(toTarget);
	VECTOR velocity = newVelocity;
	float dot = VDot(forward, velocity);
	VECTOR cross = VCross(forward, velocity);
	//判定
	TankEnemy::AnimationType animationType = TankEnemy::AnimationType::IDLE;
	if (VSize(newVelocity) > 0.01f)
	{
		if (dot > this->THRESHOLD)
		{
			animationType = TankEnemy::AnimationType::WALK_FRONT;
		}
		else if (dot < -this->THRESHOLD)
		{
			animationType = TankEnemy::AnimationType::WALK_BACK;
		}
		else if (cross.y > this->THRESHOLD)
		{
			animationType = TankEnemy::AnimationType::WALK_LEFT;
		}
		else if (cross.y < -this->THRESHOLD)
		{
			animationType = TankEnemy::AnimationType::WALK_RIGHT;
		}
	}
	enemy.SetNowAnimation(static_cast<int>(animationType));
	/*アニメーションの再生*/
	enemy.PlayAnimation();

	/*状態を返す*/
	return ActionNode::NodeState::SUCCESS;
}
