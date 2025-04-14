#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "Rigidbody.h"
#include "ColliderData.h"
#include "AttackSphereColliderData.h"
#include "AttackData.h"
#include "Character.h"
#include "BehaviorTreeNode.h"
#include "BehaviorTree.h"
#include "ActionNode.h"
#include "Beast_SuperNova.h"
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
Beast_SuperNova::Beast_SuperNova()
	: stage							(AnimationStage::START)
	, animationSet					()
	, nextStageSet					()
	, attackStartCount				(0)
	, attackEndCount				(0)
	, frameCount					(0)
	, frameIndexUsedSpherePosition	(0)
	, collider						(nullptr)
{
	this->animationSet.emplace(AnimationStage::START, static_cast<int>(Beast::AnimationType::SUPER_NOVA_START));
	this->animationSet.emplace(AnimationStage::LOOP, static_cast<int>(Beast::AnimationType::SUPER_NOVA_LOOP));
	this->animationSet.emplace(AnimationStage::END, static_cast<int>(Beast::AnimationType::SUPER_NOVA_END));
	this->nextStageSet.emplace(AnimationStage::START, AnimationStage::LOOP);
	this->nextStageSet.emplace(AnimationStage::LOOP, AnimationStage::END);
	this->nextStageSet.emplace(AnimationStage::END, AnimationStage::START);
	auto& json = Singleton<JsonManager>::GetInstance();
	this->actionType				   = static_cast<int>(BeastBehaviorTree::ActionType::SUPER_NOVA);
	this->interval					   = json.GetJson(JsonManager::FileType::BEAST)["ACTION_INTERVAL"][this->actionType];
	this->maxSpeed					   = 0.0f;
	this->accel						   = json.GetJson(JsonManager::FileType::BEAST)["ACCEL"];
	this->decel						   = json.GetJson(JsonManager::FileType::BEAST)["DECEL"];
	this->effectStartCount			   = json.GetJson(JsonManager::FileType::BEAST)["SUPER_NOVA_EFFECT_START_COUNT"];
	this->attackStartCount			   = json.GetJson(JsonManager::FileType::BEAST)["SUPER_NOVA_ATTACK_START_COUNT"];
	this->attackEndCount			   = json.GetJson(JsonManager::FileType::BEAST)["SUPER_NOVA_ATTACK_END_COUNT"];
	this->frameIndexUsedSpherePosition = json.GetJson(JsonManager::FileType::BEAST)["SUPER_NOVA_FRAME_INDEX_USED_SPHERE_POSITION"];

	/*コライダーの作成*/
	this->collider = new AttackSphereColliderData(ColliderData::Priority::STATIC, GameObjectTag::BEAST, new AttackData());
	this->collider->radius						  = json.GetJson(JsonManager::FileType::BEAST)["SUPER_NOVA_RADIUS"];
	this->collider->data->damage				  = json.GetJson(JsonManager::FileType::BEAST)["SUPER_NOVA_DAMAGE"];
	this->collider->data->reactionType			  = static_cast<int>(Gori::PlayerReactionType::BLOW_BIG);
	this->collider->data->hitStopTime			  = json.GetJson(JsonManager::FileType::BEAST)["SUPER_NOVA_HIT_STOP_TIME"];
	this->collider->data->hitStopType			  = static_cast<int>(HitStop::Type::STOP);
	this->collider->data->hitStopDelay			  = json.GetJson(JsonManager::FileType::BEAST)["SUPER_NOVA_HIT_STOP_DELAY"];
	this->collider->data->slowFactor			  = json.GetJson(JsonManager::FileType::BEAST)["SUPER_NOVA_SLOW_FACTOR"];
	this->collider->data->isHitAttack			  = false;
	this->collider->data->isDoHitCheck			  = false;
	this->collider->data->blockStaminaConsumption = json.GetJson(JsonManager::FileType::BEAST)["SUPER_NOVA_BLOCK_STAMINA_CONSUMPTION"];


}

/// <summary>
/// デストラクタ
/// </summary>
Beast_SuperNova::~Beast_SuperNova()
{

}
/// <summary>
/// 初期化
/// </summary>
void Beast_SuperNova::Initialize()
{
	this->frameCount  = 0;
	this->isFixRotate = false;
	this->stage		  = AnimationStage::START;
}

/// <summary>
/// 更新処理
/// </summary>
Beast_SuperNova::NodeState Beast_SuperNova::Update(BehaviorTree& _tree, Character& _chara)
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


	//指定フレームを超えていなければフレームの増加
	if (this->frameCount < this->attackEndCount)
	{
		this->frameCount++;
		//指定フレームを超えていたら
		if (this->frameCount < this->effectStartCount)
		{
			auto& sound = Singleton<SoundManager>::GetInstance();
			if (!sound.GetCheckEffectSoundState(SoundManager::EffectType::LUXURIO_WING))
			{
				sound.OnIsPlayEffect(SoundManager::EffectType::LUXURIO_WING);
			}
		}
		if (this->frameCount == this->effectStartCount)
		{
			auto& effect = Singleton<EffectManager>::GetInstance();
			effect.OnIsEffect(EffectManager::EffectType::BEAST_SUPER_NOVA);
			auto& sound = Singleton<SoundManager>::GetInstance();
			sound.OnIsPlayEffect(SoundManager::EffectType::LUXURIO_CHARGE);
		}
		if (this->frameCount == this->attackStartCount)
		{
			auto& sound = Singleton<SoundManager>::GetInstance();
			sound.OnIsPlayEffect(SoundManager::EffectType::LUXURIO_SUPER_NOVA);
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
		VECTOR position1 = MV1GetFramePosition(enemy.GetModelHandle(), this->frameIndexUsedSpherePosition);
		this->collider->rigidbody.SetPosition(position1);
	}
	//当たっていたら
	if (this->collider->data->isHitAttack)
	{
		auto& sound = Singleton<SoundManager>::GetInstance();
		sound.OnIsPlayEffect(SoundManager::EffectType::MONSTER_HEAVY_ATTACK);
		//攻撃ヒットフラグを下す
		this->collider->data->isHitAttack = false;
	}

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
	if (enemy.GetSpeed() != 0.0f || this->frameCount == 1)
	{
		enemy.UpdateSpeed(this->maxSpeed, this->accel, this->decel);
		enemy.UpdateVelocity(false);
	}

	/*状態を返す*/
	//アニメーションが終了していたら
	if (enemy.GetIsChangeAnimation())
	{
		this->stage = this->nextStageSet[this->stage];
		if (this->stage == AnimationStage::START)
		{
			//インターバルのセット
			_tree.SetInterval(this->actionType, this->interval);
			//アクションの解除
			_tree.ExitCurrentBattleAction();
			//レベルの上昇
			auto& beastTree = dynamic_cast<BeastBehaviorTree&>(_tree);
			beastTree.ResetLevel();
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
const void Beast_SuperNova::Draw()const
{
#ifdef _DEBUG
	DrawSphere3D(this->collider->rigidbody.GetPosition(), this->collider->radius, 16, GetColor(255, 0, 0), GetColor(255, 0, 0), FALSE);
#endif // _DEBUG
}
