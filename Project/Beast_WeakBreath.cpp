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
#include "Beast_WeakBreath.h"
#include "Enemy.h"
#include "Beast.h"
#include "EnemyManager.h"
#include "BeastBehaviorTree.h"
#include "ReactionType.h"
#include "HitStop.h"
#include "EffectManager.h"
#include "SoundManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
Beast_WeakBreath::Beast_WeakBreath()
	: frameCount					 (0)
	, attackStartCount				 (0)
	, attackEndCount				 (0)
	, frameIndexUsedCapsuleDirection1(0)
	, frameIndexUsedCapsuleDirection2(0)
	, collider						 (nullptr)
	, breathLength					 (0.0f)
	, FIX_ROTATE_COUNT				 (Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::BEAST)["ROTATE_FIX_COUNT"])
{
	auto& json = Singleton<JsonManager>::GetInstance();
	this->actionType					  = static_cast<int>(BeastBehaviorTree::ActionType::WEAK_BREATH);
	this->interval						  = json.GetJson(JsonManager::FileType::BEAST)["ACTION_INTERVAL"][this->actionType];
	this->animationType					  = static_cast<int>(Beast::AnimationType::WEAK_BREATH);
	this->animationPlayTime				  = json.GetJson(JsonManager::FileType::BEAST)["ANIMATION_PLAY_TIME"][this->animationType];
	this->maxSpeed						  = 0.0f;
	this->accel							  = json.GetJson(JsonManager::FileType::BEAST)["ACCEL"];
	this->decel							  = json.GetJson(JsonManager::FileType::BEAST)["DECEL"];
	this->attackStartCount				  = json.GetJson(JsonManager::FileType::BEAST)["WEAK_BREATH_ATTACK_START_COUNT"];
	this->attackEndCount				  = json.GetJson(JsonManager::FileType::BEAST)["WEAK_BREATH_ATTACK_END_COUNT"];
	this->breathLength					  = json.GetJson(JsonManager::FileType::BEAST)["WEAK_BREATH_LENGTH"];
	this->frameIndexUsedCapsuleDirection1 = json.GetJson(JsonManager::FileType::BEAST)["WEAK_BREATH_FRAME_INDEX_USED_CAPSULE_DIRECTION"][0];
	this->frameIndexUsedCapsuleDirection2 = json.GetJson(JsonManager::FileType::BEAST)["WEAK_BREATH_FRAME_INDEX_USED_CAPSULE_DIRECTION"][1];
	this->isFixRotate					  = true;

	/*コライダーの作成*/
	this->collider = new AttackCapsuleColliderData(ColliderData::Priority::STATIC, GameObjectTag::BEAST, new AttackData());
	this->collider->radius						  = json.GetJson(JsonManager::FileType::BEAST)["WEAK_BREATH_RADIUS"];
	this->collider->data->damage				  = json.GetJson(JsonManager::FileType::BEAST)["WEAK_BREATH_DAMAGE"];
	this->collider->data->reactionType			  = static_cast<int>(Gori::PlayerReactionType::BLOW_BIG);
	this->collider->data->hitStopTime			  = json.GetJson(JsonManager::FileType::BEAST)["WEAK_BREATH_HIT_STOP_TIME"];
	this->collider->data->hitStopType			  = static_cast<int>(HitStop::Type::STOP);
	this->collider->data->hitStopDelay			  = json.GetJson(JsonManager::FileType::BEAST)["WEAK_BREATH_HIT_STOP_DELAY"];
	this->collider->data->slowFactor			  = json.GetJson(JsonManager::FileType::BEAST)["WEAK_BREATH_SLOW_FACTOR"];
	this->collider->data->isHitAttack			  = false;
	this->collider->data->isDoHitCheck			  = false;
	this->collider->data->blockStaminaConsumption = json.GetJson(JsonManager::FileType::BEAST)["SUPER_NOVA_BLOCK_STAMINA_CONSUMPTION"];

}

/// <summary>
/// デストラクタ
/// </summary>
Beast_WeakBreath::~Beast_WeakBreath()
{

}
/// <summary>
/// 初期化
/// </summary>
void Beast_WeakBreath::Initialize()
{
	this->frameCount  = 0;
	this->isFixRotate = true;
}

/// <summary>
/// 更新処理
/// </summary>
Beast_WeakBreath::NodeState Beast_WeakBreath::Update(BehaviorTree& _tree, Character& _chara)
{
	auto& enemy = dynamic_cast<Beast&>(_chara);

	/*登録されているアクションと実際のアクションが異なっていたら初期化*/
	if (_tree.GetNowSelectAction() != this->actionType)
	{
		//アニメーションの種類を設定
		enemy.SetNowAnimation(this->animationType);
		//アニメーション再生時間の設定
		enemy.SetAnimationPlayTime(this->animationPlayTime);
		//アクションの設定
		_tree.SetNowSelectAction(this->actionType);
		//アクションの登録
		_tree.EntryCurrentBattleAction(*this);
		this->frameCount = 0;
		enemy.DecAttackComboCount();
	}

	/*コライダーの更新*/
	this->frameCount++;
	//指定フレームを超えていなければフレームの増加
	if (this->frameCount < this->attackEndCount)
	{
		//指定フレームを超えていたら
		if (this->frameCount == this->attackStartCount)
		{
			this->collider->data->isDoHitCheck = true;
			auto& effect = Singleton<EffectManager>::GetInstance();
			effect.OnIsEffect(EffectManager::EffectType::BEAST_WEAK_BREATH);
			auto& sound = Singleton<SoundManager>::GetInstance();
			sound.OnIsPlayEffect(SoundManager::EffectType::MONSTER_BREATH);
		}
		else if (this->frameCount % this->BREATH_HIT_COUNT == 0)
		{
			this->collider->data->isDoHitCheck = true;
		}
	}
	else if (this->frameCount == this->attackEndCount)
	{
		this->collider->data->isDoHitCheck = false;
		this->collider->data->isHitAttack = false;
		auto& sound = Singleton<SoundManager>::GetInstance();
		sound.OffIsPlayEffect(SoundManager::EffectType::MONSTER_BREATH);
	}
	//あたり判定が取れる状態だったら
	if (this->collider->data->isDoHitCheck)
	{
		VECTOR position1 = MV1GetFramePosition(enemy.GetModelHandle(), this->frameIndexUsedCapsuleDirection1);
		VECTOR position2 = MV1GetFramePosition(enemy.GetModelHandle(), this->frameIndexUsedCapsuleDirection2);
		this->collider->rigidbody.SetPosition(position1);
		VECTOR direction = VSub(position2, position1);
		direction.y = 0.0f;
		VECTOR directionScaling = VScale(direction, this->breathLength);
		this->collider->topPositon = VAdd(position1, directionScaling);
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
	if (this->isFixRotate)
	{
		enemy.Move(this->maxSpeed, this->accel, this->decel, false);
		if (this->frameCount >= this->FIX_ROTATE_COUNT)
		{
			this->isFixRotate = false;
		}
	}

	//アニメーションが終了していたら
	if (enemy.GetIsChangeAnimation())
	{
		//インターバルのセット
		_tree.SetInterval(this->actionType, this->interval);
		//アクションの解除
		_tree.ExitCurrentBattleAction();
		this->frameCount = 0;
		this->collider->data->isDoHitCheck = false;
		this->collider->data->isHitAttack = false;
		this->isFixRotate = true;
		return ActionNode::NodeState::SUCCESS;
	}
	//それ以外は実行中を返す
	else
	{
		return ActionNode::NodeState::RUNNING;
	}
}

/// <summary>
/// 描画
/// </summary>
const void Beast_WeakBreath::Draw()const
{
#ifdef _DEBUG
	DrawCapsule3D(this->collider->rigidbody.GetPosition(), this->collider->topPositon, this->collider->radius, 16, GetColor(255, 0, 0), GetColor(255, 0, 0), FALSE);
#endif // _DEBUG

}
