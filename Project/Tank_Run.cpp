#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "VECTORtoUseful.h"
#include "Rigidbody.h"
#include "Character.h"
#include "BehaviorTreeNode.h"
#include "BehaviorTree.h"
#include "ActionNode.h"
#include "Tank_Run.h"
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
Tank_Run::Tank_Run()
	: isInitialize		(false)
{
	auto& json = Singleton<JsonManager>::GetInstance();
	this->animationType		= static_cast<int>(TankEnemy::AnimationType::WALK_FRONT);
	this->animationPlayTime = json.GetJson(JsonManager::FileType::TANK_ENEMY)["ANIMATION_PLAY_TIME"][this->animationType];
	this->actionType		= static_cast<short>(TankEnemyBehaviorTree::ActionType::WALK);
	this->maxSpeed			= json.GetJson(JsonManager::FileType::TANK_ENEMY)["WALK_SPEED"];
	this->accel				= json.GetJson(JsonManager::FileType::TANK_ENEMY)["ACCEL"];
	this->decel				= json.GetJson(JsonManager::FileType::TANK_ENEMY)["DECEL"];
}

/// <summary>
/// デストラクタ
/// </summary>
Tank_Run::~Tank_Run()
{

}

/// <summary>
/// 初期化
/// </summary>
void Tank_Run::Initialize()
{
	this->isInitialize = false;
}

/// <summary>
/// 更新処理
/// </summary>
Tank_Run::NodeState Tank_Run::Update(BehaviorTree& _tree, Character& _chara)
{
	auto& enemy = dynamic_cast<TankEnemy&>(_chara);

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
	}

	/*アニメーションの再生*/
	enemy.PlayAnimation();

	/*移動*/
	auto& enemyManager = Singleton<EnemyManager>::GetInstance();
	auto& player = Singleton<PlayerManager>::GetInstance();
	VECTOR					 nowVelocity = enemy.GetRigidbody().GetVelocity();													//現在の移動ベクトル
	const vector<Character*> weakEnemyList = enemyManager.GetWeakEnemyList();														//雑魚敵のリスト
	VECTOR					 toTarget = VSub(enemy.GetRigidbody().GetPosition(), player.GetRigidbody().GetPosition());	//移動目標へのベクトル
	//回転率の更新
	enemy.UpdateRotation(toTarget);
	//移動速度の更新
	enemy.UpdateSpeed(this->maxSpeed, this->accel, this->decel);

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
