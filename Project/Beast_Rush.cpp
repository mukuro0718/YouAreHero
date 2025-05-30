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
#include "Player.h"
#include "Beast.h"
#include "Beast_Rush.h"
#include "EnemyManager.h"
#include "PlayerManager.h"
#include "BeastBehaviorTree.h"
#include "ReactionType.h"
#include "HitStop.h"
#include "SoundManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
Beast_Rush::Beast_Rush()
	: attackStartCount				 (0)
	, attackEndCount				 (0)
	, frameCount					 (0)
	, frameIndexUsedCapsuleDirection1(0)
	, frameIndexUsedCapsuleDirection2(0)
	, stage							 (AnimationStage::START)
	, collider						 (nullptr)
	, STOP_DISTANCE					 (Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::BEAST)["STOP_RUSH_DISTANCE"])
	, FOOTSTEPS_SOUND_FRAME			 (Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::BEAST)["RUSH_FOOTSTEPS_SOUND_FRAME"])
{
	this->animationSet.emplace(AnimationStage::START, static_cast<int>(Beast::AnimationType::RUSH_START));
	this->animationSet.emplace(AnimationStage::LOOP, static_cast<int>(Beast::AnimationType::RUSH_LOOP));
	this->animationSet.emplace(AnimationStage::NORMAL_END, static_cast<int>(Beast::AnimationType::RUSH_NORMAL_END));
	this->animationSet.emplace(AnimationStage::ANGRY_END, static_cast<int>(Beast::AnimationType::RUSH_ROTATE_END));
	this->nextStageSet.emplace(AnimationStage::START, AnimationStage::LOOP);
	this->nextStageSet.emplace(AnimationStage::LOOP, AnimationStage::NORMAL_END);
	this->nextStageSet.emplace(AnimationStage::NORMAL_END, AnimationStage::START);
	this->nextStageSet.emplace(AnimationStage::ANGRY_END, AnimationStage::START);
	auto& json = Singleton<JsonManager>::GetInstance();
	this->actionType					  = static_cast<int>(BeastBehaviorTree::ActionType::BREATH);
	this->interval						  = json.GetJson(JsonManager::FileType::BEAST)["ACTION_INTERVAL"][this->actionType];
	this->maxSpeed						  = json.GetJson(JsonManager::FileType::BEAST)["RUSH_SPEED"];
	this->accel							  = json.GetJson(JsonManager::FileType::BEAST)["ACCEL"];
	this->decel							  = json.GetJson(JsonManager::FileType::BEAST)["DECEL"];
	this->attackStartCount				  = json.GetJson(JsonManager::FileType::BEAST)["RUSH_ATTACK_START_COUNT"];
	this->attackEndCount				  = json.GetJson(JsonManager::FileType::BEAST)["RUSH_ATTACK_END_COUNT"];
	this->frameIndexUsedCapsuleDirection1 = json.GetJson(JsonManager::FileType::BEAST)["RUSH_FRAME_INDEX_USED_CAPSULE_DIRECTION"][0];
	this->frameIndexUsedCapsuleDirection2 = json.GetJson(JsonManager::FileType::BEAST)["RUSH_FRAME_INDEX_USED_CAPSULE_DIRECTION"][1];

	/*コライダーの作成*/
	this->collider = new AttackCapsuleColliderData(ColliderData::Priority::STATIC, GameObjectTag::BEAST, new AttackData());
	this->collider->radius						  = json.GetJson(JsonManager::FileType::BEAST)["RUSH_RADIUS"];
	this->collider->data->damage				  = json.GetJson(JsonManager::FileType::BEAST)["RUSH_DAMAGE"];
	this->collider->data->reactionType			  = static_cast<int>(Gori::PlayerReactionType::BLOW_BIG);
	this->collider->data->hitStopTime			  = json.GetJson(JsonManager::FileType::BEAST)["RUSH_HIT_STOP_TIME"];
	this->collider->data->hitStopType			  = static_cast<int>(HitStop::Type::STOP);
	this->collider->data->hitStopDelay			  = json.GetJson(JsonManager::FileType::BEAST)["RUSH_STOP_DELAY"];
	this->collider->data->slowFactor			  = json.GetJson(JsonManager::FileType::BEAST)["RUSH_SLOW_FACTOR"];
	this->collider->data->isHitAttack			  = false;
	this->collider->data->isDoHitCheck			  = false;
	this->collider->data->blockStaminaConsumption = json.GetJson(JsonManager::FileType::BEAST)["RUSH_BLOCK_STAMINA_CONSUMPTION"];

}

/// <summary>
/// デストラクタ
/// </summary>
Beast_Rush::~Beast_Rush()
{

}
/// <summary>
/// 初期化
/// </summary>
void Beast_Rush::Initialize()
{
	this->frameCount  = 0;
	this->isFixRotate = false;
	this->stage		  = AnimationStage::START;
}

/// <summary>
/// 更新処理
/// </summary>
Beast_Rush::NodeState Beast_Rush::Update(BehaviorTree& _tree, Character& _chara)
{
	/*選択されているアクションと実際のアクションが異なっていたら初期化*/
	auto& enemy = dynamic_cast<Beast&>(_chara);
	if (_tree.GetNowSelectAction() != this->actionType)
	{
		//アクションの設定
		_tree.SetNowSelectAction(this->actionType);
		//アクションの登録
		_tree.EntryCurrentBattleAction(*this);
		enemy.DecAttackComboCount();
	}

	/*コライダーの更新*/
	//当たっていたら
	if (this->collider->data->isHitAttack)
	{
		//攻撃ヒットフラグを下す
		this->collider->data->isHitAttack = false;
		auto& sound = Singleton<SoundManager>::GetInstance();
		sound.OnIsPlayEffect(SoundManager::EffectType::MONSTER_HEAVY_ATTACK);
	}
	//アニメーションがLOOPだったらかつフレームカウントが1だったら当たり判定フラグを立てる
	if (this->stage == AnimationStage::LOOP)
	{
		this->frameCount++;
		//指定フレームを超えていたら
		if (this->frameCount == 1)
		{
			this->collider->data->isDoHitCheck = true;
		}
		//間隔で足音を鳴らす
		if (this->frameCount % this->FOOTSTEPS_SOUND_FRAME == 0)
		{
			auto& sound = Singleton<SoundManager>::GetInstance();
			sound.OnIsPlayEffect(SoundManager::EffectType::MONSTER_FOOTSTEPS);
		}
	}
	else
	{
		this->collider->data->isDoHitCheck = false;
		this->collider->data->isHitAttack = false;
	}
	//あたり判定が取れる状態だったら
	if (this->collider->data->isDoHitCheck)
	{
		VECTOR position1 = MV1GetFramePosition(enemy.GetModelHandle(), this->frameIndexUsedCapsuleDirection1);
		VECTOR position2 = MV1GetFramePosition(enemy.GetModelHandle(), this->frameIndexUsedCapsuleDirection2);
		this->collider->rigidbody.SetPosition(position1);
		this->collider->topPositon = position2;
	}

	/*アニメーション*/
	//アニメーションの種類を設定
	int nowAnimationType = this->animationSet[this->stage];
	if (this->animationType != nowAnimationType)
	{
		//再生するアニメーションを設定
		this->animationType = nowAnimationType;
		enemy.SetNowAnimation(this->animationType);
		//アニメーション再生時間の設定
		auto& json = Singleton<JsonManager>::GetInstance();
		enemy.SetAnimationPlayTime(json.GetJson(JsonManager::FileType::BEAST)["ANIMATION_PLAY_TIME"][this->animationType]);
	}
	//アニメーションの再生
	enemy.PlayAnimation();

	/*移動*/
	float maxSpeed = 0.0f;
	//もしアニメーションがSTARTなら移動目標を更新して回転
	if (this->stage == AnimationStage::START)
	{
		auto& player = Singleton<PlayerManager>::GetInstance();
		enemy.SetNowMoveTarget(player.GetRigidbody().GetPosition());
		VECTOR toTarget = VSub(enemy.GetRigidbody().GetPosition(), enemy.GetNowMoveTarget());
		enemy.UpdateRotation(toTarget);
	}
	//もしアニメーションがLOOP中なら速度を入れる
	else if (this->stage == AnimationStage::LOOP)
	{
		maxSpeed = this->maxSpeed;
	}
	//移動速度の更新
	enemy.UpdateSpeed(maxSpeed, this->accel, this->decel);
	//移動ベクトルを出す
	enemy.UpdateVelocity(false);

	/*状態を返す*/
	//アニメーションが終了していたら
	if (this->stage == AnimationStage::LOOP)
	{
		VECTOR toTarget = VSub(enemy.GetRigidbody().GetPosition(), enemy.GetNowMoveTarget());
		if (VSquareSize(toTarget) < this->STOP_DISTANCE)
		{
			this->stage = AnimationStage::NORMAL_END;
		}
		return ActionNode::NodeState::RUNNING;
	}
	else if (enemy.GetIsChangeAnimation())
	{
		this->stage = this->nextStageSet[this->stage];
		//ここでステージがSTARTならアニメーションがすべて終了したということ
		if (this->stage == AnimationStage::START)
		{
			//インターバルのセット
			_tree.SetInterval(this->actionType, this->interval);
			//アクションの解除
			_tree.ExitCurrentBattleAction();
			this->frameCount = 0;
			this->collider->data->isDoHitCheck = false;
			this->collider->data->isHitAttack = false;
			return ActionNode::NodeState::SUCCESS;
		}
		return ActionNode::NodeState::RUNNING;
	}
	else
	{
		return ActionNode::NodeState::RUNNING;
	}
}

/// <summary>
/// 描画
/// </summary>
const void Beast_Rush::Draw()const
{
#ifdef _DEBUG
	DrawCapsule3D(this->collider->rigidbody.GetPosition(), this->collider->topPositon, this->collider->radius, 16, GetColor(255, 0, 0), GetColor(255, 0, 0), FALSE);
#endif // _DEBUG
}
