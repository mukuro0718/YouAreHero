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
#include "Brawler_Attack.h"
#include "Player.h"
#include "Enemy.h"
#include "BrawlerEnemy.h"
#include "EnemyManager.h"
#include "PlayerManager.h"
#include "BrawlerEnemyBehaviorTree.h"
#include "ReactionType.h"
#include "HitStop.h"

/// <summary>
/// コンストラクタ
/// </summary>
Brawler_Attack::Brawler_Attack()
	: attackStartCount(0)
	, attackEndCount(0)
	, frameCount(0)
	, frameIndexUsedCapsuleDirection1(0)
	, frameIndexUsedCapsuleDirection2(0)
	, collider(nullptr)
{
	auto& json = Singleton<JsonManager>::GetInstance();
	this->animationType					  = static_cast<int>(BrawlerEnemy::AnimationType::ATTACK);
	this->animationPlayTime				  = json.GetJson(JsonManager::FileType::BRAWLER_ENEMY)["ANIMATION_PLAY_TIME"][this->animationType];
	this->actionType					  = static_cast<int>(BrawlerEnemyBehaviorTree::ActionType::ATTACK);
	this->interval						  = json.GetJson(JsonManager::FileType::BRAWLER_ENEMY)["ACTION_INTERVAL"][this->actionType];
	this->maxSpeed						  = 0.0f;
	this->accel							  = json.GetJson(JsonManager::FileType::BRAWLER_ENEMY)["ACCEL"];
	this->decel							  = json.GetJson(JsonManager::FileType::BRAWLER_ENEMY)["DECEL"];
	this->attackStartCount				  = json.GetJson(JsonManager::FileType::BRAWLER_ENEMY)["ATTACK_START_COUNT"];
	this->attackEndCount				  = json.GetJson(JsonManager::FileType::BRAWLER_ENEMY)["ATTACK_END_COUNT"];
	this->frameIndexUsedCapsuleDirection1 = json.GetJson(JsonManager::FileType::BRAWLER_ENEMY)["FRAME_INDEX_USED_CAPSULE_DIRECTION"][0];
	this->frameIndexUsedCapsuleDirection2 = json.GetJson(JsonManager::FileType::BRAWLER_ENEMY)["FRAME_INDEX_USED_CAPSULE_DIRECTION"][1];

	/*コライダーの作成*/
	this->collider								  = new AttackCapsuleColliderData(ColliderData::Priority::STATIC, GameObjectTag::BRAWLER, new AttackData());
	this->collider->radius						  = json.GetJson(JsonManager::FileType::BRAWLER_ENEMY)["RADIUS"];
	this->collider->data->damage				  = json.GetJson(JsonManager::FileType::BRAWLER_ENEMY)["DAMAGE"];
	this->collider->data->reactionType			  = static_cast<int>(Gori::PlayerReactionType::NORMAL);
	this->collider->data->hitStopTime			  = json.GetJson(JsonManager::FileType::BRAWLER_ENEMY)["HIT_STOP_TIME"];
	this->collider->data->hitStopType			  = static_cast<int>(HitStop::Type::STOP);
	this->collider->data->hitStopDelay			  = json.GetJson(JsonManager::FileType::BRAWLER_ENEMY)["HIT_STOP_DELAY"];
	this->collider->data->slowFactor			  = json.GetJson(JsonManager::FileType::BRAWLER_ENEMY)["SLOW_FACTOR"];
	this->collider->data->isHitAttack			  = false;
	this->collider->data->isDoHitCheck			  = false;
	this->collider->data->blockStaminaConsumption = json.GetJson(JsonManager::FileType::BRAWLER_ENEMY)["BLOCK_STAMINA_CONSUMPTION"];
}

/// <summary>
/// デストラクタ
/// </summary>
Brawler_Attack::~Brawler_Attack()
{

}

/// <summary>
/// 初期化
/// </summary>
void Brawler_Attack::Initialize()
{
	this->frameCount = 0;
	this->isFixRotate = false;
}

/// <summary>
/// 更新処理
/// </summary>
Brawler_Attack::NodeState Brawler_Attack::Update(BehaviorTree& _tree, Character& _chara)
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
		//自分をRootに登録
		_tree.EntryCurrentBattleAction(*this);
	}

	/*コライダーの更新*/
	//指定フレームを超えていなければフレームの増加
	if (this->frameCount < this->attackEndCount)
	{
		this->frameCount++;
		//指定フレームを超えていたら
		if (this->frameCount == this->attackStartCount)
		{
			//auto& sound = Singleton<SoundManager>::GetInstance();
			//sound.OnIsPlayEffect(SoundManager::EffectType::MONSTER_SWING_2);
			this->collider->data->isDoHitCheck = true;
		}
		if (this->frameCount >= this->attackEndCount)
		{
			this->collider->data->isDoHitCheck = false;
			this->collider->data->isHitAttack = false;
		}
	}
	//あたり判定が取れる状態だったら
	if (this->collider->data->isDoHitCheck)
	{
		VECTOR position1 = MV1GetFramePosition(enemy.GetModelHandle(), this->frameIndexUsedCapsuleDirection1);
		VECTOR position2 = MV1GetFramePosition(enemy.GetModelHandle(), this->frameIndexUsedCapsuleDirection2);
		this->collider->rigidbody.SetPosition(position1);
		this->collider->topPositon = position2;
	}
	//当たっていたらヒットストップを設定する
	if (this->collider->data->isHitAttack)
	{
		//auto& sound = Singleton<SoundManager>::GetInstance();
		//sound.OnIsPlayEffect(SoundManager::EffectType::MONSTER_HEAVY_ATTACK);
		//攻撃ヒットフラグを下す
		this->collider->data->isHitAttack = false;
	}

	/*アニメーションの再生*/
	enemy.PlayAnimation();

	/*移動*/
	float maxSpeed = 0.0f;
	if (this->frameCount < this->attackStartCount)
	{
		maxSpeed = this->maxSpeed;
		//目標までのベクトルを出す
		auto& player = Singleton<PlayerManager>::GetInstance();
		VECTOR toTarget = VSub(enemy.GetRigidbody().GetPosition(), player.GetRigidbody().GetPosition());
		//正規化
		//toTarget = VNorm(toTarget);
		enemy.UpdateRotation(toTarget);
	}
	//移動速度の更新
	enemy.UpdateSpeed(maxSpeed, this->accel, this->decel);
	//移動ベクトルを出す
	enemy.UpdateVelocity(false);

	/*状態を返す*/
	{
		//アニメーションが終了していたら
		if (enemy.GetIsChangeAnimation())
		{
			//インターバルの設定
			_tree.SetInterval(this->actionType, this->interval);
			//登録を解除
			_tree.ExitCurrentBattleAction();
			this->frameCount = 0;
			this->collider->data->isDoHitCheck = false;
			this->collider->data->isHitAttack = false;
			return ActionNode::NodeState::SUCCESS;
		}
		//それ以外は実行中を返す
		else
		{
			return ActionNode::NodeState::RUNNING;
		}
	}
}
