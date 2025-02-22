#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "BehaviorTreeNode.h"
#include "ActionNode.h"
#include "Beast_RightFootAttack.h"
#include "Rigidbody.h"
#include "ColliderData.h"
#include "AttackCapsuleColliderData.h"
#include "AttackData.h"
#include "Character.h"
#include "Enemy.h"
#include "Beast.h"
#include "EnemyManager.h"
#include "BeastBehaviorTree.h"
#include "ReactionType.h"
#include "HitStop.h"
#include "SoundManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
Beast_RightFootAttack::Beast_RightFootAttack()
	: attackStartCount				 (0)
	, attackEndCount				 (0)
	, frameCount					 (0)
	, frameIndexUsedCapsuleDirection1(0)
	, frameIndexUsedCapsuleDirection2(0)
	, collider						 (nullptr)
{
	auto& json = Singleton<JsonManager>::GetInstance();
	this->animationPlayTime				  = json.GetJson(JsonManager::FileType::BEAST)["ANIMATION_PLAY_TIME"][this->animationType];
	this->animationType					  = static_cast<int>(Beast::AnimationType::RIGHT_FOOT_ATTACK);
	this->actionType					  = static_cast<int>(BeastBehaviorTree::ActionType::RIGHT_FOOT_ATTACK);
	this->interval						  = json.GetJson(JsonManager::FileType::BEAST)["ACTION_INTERVAL"][this->actionType];
	this->maxSpeed						  = 0.0f;
	this->accel							  = json.GetJson(JsonManager::FileType::BEAST)["ACCEL"];
	this->decel							  = json.GetJson(JsonManager::FileType::BEAST)["DECEL"];
	this->attackStartCount				  = json.GetJson(JsonManager::FileType::BEAST)["RIGHT_ATTACK_START_COUNT"];
	this->attackEndCount				  = json.GetJson(JsonManager::FileType::BEAST)["RIGHT_ATTACK_END_COUNT"];
	this->frameIndexUsedCapsuleDirection1 = json.GetJson(JsonManager::FileType::BEAST)["RIGHT_FRAME_INDEX_USED_CAPSULE_DIRECTION"][0];
	this->frameIndexUsedCapsuleDirection2 = json.GetJson(JsonManager::FileType::BEAST)["RIGHT_FRAME_INDEX_USED_CAPSULE_DIRECTION"][1];

	/*コライダーの作成*/
	this->collider = new AttackCapsuleColliderData(ColliderData::Priority::STATIC, GameObjectTag::BOSS_ATTACK, new AttackData());
	this->collider->radius							= json.GetJson(JsonManager::FileType::BEAST)["RIGHT_RADIUS"];
	this->collider->data->damage					= json.GetJson(JsonManager::FileType::BEAST)["RIGHT_DAMAGE"];
	this->collider->data->reactionType				= static_cast<int>(Gori::PlayerReactionType::BLOW_BIG);
	this->collider->data->hitStopTime				= json.GetJson(JsonManager::FileType::BEAST)["RIGHT_HIT_STOP_TIME"];
	this->collider->data->hitStopType				= static_cast<int>(HitStop::Type::STOP);
	this->collider->data->hitStopDelay				= json.GetJson(JsonManager::FileType::BEAST)["RIGHT_HIT_STOP_DELAY"];
	this->collider->data->slowFactor				= json.GetJson(JsonManager::FileType::BEAST)["RIGHT_SLOW_FACTOR"];
	this->collider->data->isHitAttack				= false;
	this->collider->data->isDoHitCheck				= false;
	this->collider->data->blockStaminaConsumption	= json.GetJson(JsonManager::FileType::BEAST)["RIGHT_BLOCK_STAMINA_CONSUMPTION"];

}

/// <summary>
/// デストラクタ
/// </summary>
Beast_RightFootAttack::~Beast_RightFootAttack()
{

}
/// <summary>
/// 初期化
/// </summary>
void Beast_RightFootAttack::Initialize()
{
	this->frameCount  = 0;
	this->isFixRotate = false;
}

/// <summary>
/// 更新処理
/// </summary>
Beast_RightFootAttack::NodeState Beast_RightFootAttack::Update()
{
	auto& rootNode = Singleton<BeastBehaviorTree>::GetInstance();
	auto& enemyManager = Singleton<EnemyManager>::GetInstance();
	auto& enemy = dynamic_cast<Beast&>(enemyManager.GetCharacter());

	/*登録されているアクションと実際のアクションが異なっていたら*/
	if (rootNode.GetNowSelectAction() != this->actionType)
	{
		//アニメーションの種類を設定
		enemy.SetNowAnimation(this->animationType);
		//アニメーション再生時間の設定
		enemy.SetAnimationPlayTime(this->animationPlayTime);
		//アクションの設定
		rootNode.SetSelectAction(this->actionType);
		//自分をRootに登録
		rootNode.EntryCurrentBattleAction(*this);
		enemy.DecAttackComboCount();
	}

	/*コライダーの更新*/
	//指定フレームを超えていなければフレームの増加
	if (this->frameCount < this->attackEndCount)
	{
		this->frameCount++;
		//指定フレームを超えていたら
		if (this->frameCount == this->attackStartCount)
		{
			auto& sound = Singleton<SoundManager>::GetInstance();
			sound.OnIsPlayEffect(SoundManager::EffectType::MONSTER_SWING_1);
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
	//当たっていたら
	if (this->collider->data->isHitAttack)
	{
		auto& sound = Singleton<SoundManager>::GetInstance();
		sound.OnIsPlayEffect(SoundManager::EffectType::MONSTER_LIGHT_ATTACK);
		//攻撃ヒットフラグを下す
		this->collider->data->isHitAttack = false;
	}

	/*アニメーションの再生*/
	enemy.PlayAnimation();

	/*移動*/
	if (enemy.GetSpeed() != 0.0f || this->frameCount == 1)
	{
		enemy.UpdateSpeed(this->maxSpeed, this->accel, this->decel);
		enemy.UpdateVelocity(false);
	}

	/*状態を返す*/
	//アニメーションが終了していたら
	if (enemy.GetIsChangeAnimation())
	{
		//インターバルの設定
		rootNode.SetInterval(this->actionType, this->interval);
		//登録を解除
		rootNode.ExitCurrentBattleAction();
		this->frameCount = 0;
		this->collider->data->isDoHitCheck = false;
		this->collider->data->isHitAttack = false;
		return ActionNode::NodeState::SUCCESS;
	}
	return ActionNode::NodeState::RUNNING;
}

/// <summary>
/// 描画
/// </summary>
const void Beast_RightFootAttack::Draw()const
{
#ifdef _DEBUG
	DrawCapsule3D(this->collider->rigidbody.GetPosition(), this->collider->topPositon, this->collider->radius, 16, GetColor(255, 0, 0), GetColor(255, 0, 0), FALSE);
#endif // _DEBUG
}
