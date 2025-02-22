#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "BehaviorTreeNode.h"
#include "ActionNode.h"
#include "Beast_FootComboAttack.h"
#include "Rigidbody.h"
#include "ColliderData.h"
#include "AttackCapsuleColliderData.h"
#include "AttackData.h"
#include "Character.h"
#include "Player.h"
#include "Enemy.h"
#include "Beast.h"
#include "EnemyManager.h"
#include "BeastBehaviorTree.h"
#include "ReactionType.h"
#include "HitStop.h"
#include "PlayerManager.h"
#include "SoundManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
Beast_FootComboAttack::Beast_FootComboAttack()
	: frameCount	(0)
	, maxAttackCount(0)
	, attackCount	(0)
	, collider		(nullptr)
{
	auto& json = Singleton<JsonManager>::GetInstance();
	this->animationPlayTime						  = json.GetJson(JsonManager::FileType::BEAST)["ANIMATION_PLAY_TIME"][this->animationType];
	this->animationType							  = static_cast<int>(Beast::AnimationType::COMBO_ATTACK);
	this->actionType							  = static_cast<int>(BeastBehaviorTree::ActionType::COMBO_ATTACK);
	this->interval								  = json.GetJson(JsonManager::FileType::BEAST)["ACTION_INTERVAL"][this->actionType];
	this->maxSpeed								  = json.GetJson(JsonManager::FileType::BEAST)["ATTACKING_SPEED"];
	this->accel									  = json.GetJson(JsonManager::FileType::BEAST)["ACCEL"];
	this->decel									  = json.GetJson(JsonManager::FileType::BEAST)["DECEL"];
	vector<short> attackStartCount				  = json.GetJson(JsonManager::FileType::BEAST)["COMBO_1_ATTACK_START_COUNT"];
	vector<short> attackEndCount				  = json.GetJson(JsonManager::FileType::BEAST)["COMBO_1_ATTACK_END_COUNT"];
	this->attackStartCount						  = attackStartCount;
	this->attackEndCount						  = attackEndCount;
	vector<short> moveStartCount				  = json.GetJson(JsonManager::FileType::BEAST)["COMBO_1_MOVE_START_COUNT"];
	vector<short> moveEndCount					  = json.GetJson(JsonManager::FileType::BEAST)["COMBO_1_MOVE_END_COUNT"];
	this->moveStartCount						  = moveStartCount;
	this->moveEndCount							  = moveEndCount;
	this->maxAttackCount						  = json.GetJson(JsonManager::FileType::BEAST)["COMBO_1_MAX_ATTACK_COUNT"];
	vector<short> frameIndexUsedCapsuleDirection1 = json.GetJson(JsonManager::FileType::BEAST)["COMBO_1_FRAME_INDEX_USED_CAPSULE_DIRECTION_1"];
	vector<short> frameIndexUsedCapsuleDirection2 = json.GetJson(JsonManager::FileType::BEAST)["COMBO_1_FRAME_INDEX_USED_CAPSULE_DIRECTION_2"];
	this->frameIndexUsedCapsuleDirection1		  = frameIndexUsedCapsuleDirection1;
	this->frameIndexUsedCapsuleDirection2		  = frameIndexUsedCapsuleDirection2;

	/*コライダーの作成*/
	this->collider = new AttackCapsuleColliderData(ColliderData::Priority::STATIC, GameObjectTag::BOSS_ATTACK, new AttackData());
	this->collider->radius							= json.GetJson(JsonManager::FileType::BEAST)["COMBO_1_RADIUS"];
	this->collider->data->damage					= json.GetJson(JsonManager::FileType::BEAST)["COMBO_1_DAMAGE"];
	this->collider->data->reactionType				= static_cast<int>(Gori::PlayerReactionType::BLOW_BIG);
	this->collider->data->hitStopTime				= json.GetJson(JsonManager::FileType::BEAST)["COMBO_1_HIT_STOP_TIME"];
	this->collider->data->hitStopType				= static_cast<int>(HitStop::Type::STOP);
	this->collider->data->hitStopDelay				= json.GetJson(JsonManager::FileType::BEAST)["COMBO_1_HIT_STOP_DELAY"];
	this->collider->data->slowFactor				= json.GetJson(JsonManager::FileType::BEAST)["COMBO_1_SLOW_FACTOR"];
	this->collider->data->isHitAttack				= false;
	this->collider->data->isDoHitCheck				= false;
	this->collider->data->blockStaminaConsumption	= json.GetJson(JsonManager::FileType::BEAST)["COMBO_1_BLOCK_STAMINA_CONSUMPTION"];

}

/// <summary>
/// デストラクタ
/// </summary>
Beast_FootComboAttack::~Beast_FootComboAttack()
{

}
/// <summary>
/// 初期化
/// </summary>
void Beast_FootComboAttack::Initialize()
{
	this->frameCount  = 0;
	this->attackCount = 0;
	this->isFixRotate = false;
}

/// <summary>
/// 更新処理
/// </summary>
Beast_FootComboAttack::NodeState Beast_FootComboAttack::Update()
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
	bool isMove = false;
	if (this->attackCount < this->maxAttackCount)
	{
		//フレームの増加
		this->frameCount++;
		//指定フレームを超えていたら
		if (this->frameCount >= this->moveStartCount[this->attackCount])
		{
			isMove = true;
		}
		if (this->frameCount >= this->moveEndCount[this->attackCount])
		{
			isMove = false;
		}
		if (this->frameCount == this->attackStartCount[this->attackCount])
		{
			auto& sound = Singleton<SoundManager>::GetInstance();
			sound.OnIsPlayEffect(SoundManager::EffectType::MONSTER_SWING_1);
			this->collider->data->isDoHitCheck = true;
		}
		if (this->frameCount >= this->attackEndCount[this->attackCount])
		{
			this->collider->data->isDoHitCheck = false;
			this->collider->data->isHitAttack = false;
			this->attackCount++;
		}
		//あたり判定が取れる状態だったら
		if (this->collider->data->isDoHitCheck)
		{
			VECTOR position1 = MV1GetFramePosition(enemy.GetModelHandle(), this->frameIndexUsedCapsuleDirection1[this->attackCount]);
			VECTOR position2 = MV1GetFramePosition(enemy.GetModelHandle(), this->frameIndexUsedCapsuleDirection2[this->attackCount]);
			this->collider->rigidbody.SetPosition(position1);
			this->collider->topPositon = position2;
		}
		//当たっていたらヒットストップを設定する
		if (this->collider->data->isHitAttack)
		{
			auto& sound = Singleton<SoundManager>::GetInstance();
			sound.OnIsPlayEffect(SoundManager::EffectType::MONSTER_LIGHT_ATTACK);
			//攻撃ヒットフラグを下す
			this->collider->data->isHitAttack = false;
		}
	}

	/*アニメーションの再生*/
	enemy.PlayAnimation();

	/*移動*/
	float maxSpeed = 0.0f;
	if (isMove)
	{
		maxSpeed = this->maxSpeed;
		//目標までのベクトルを出す
		auto& player = Singleton<PlayerManager>::GetInstance();
		VECTOR toTarget = VSub(enemy.GetRigidbody().GetPosition(), player.GetRigidbody().GetPosition());
		//正規化
		enemy.UpdateRotation(toTarget);
	}
	//移動速度の更新
	enemy.UpdateSpeed(maxSpeed, this->accel, this->decel);
	//移動ベクトルを出す
	enemy.UpdateVelocity(false);

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
		this->attackCount = 0;
		return ActionNode::NodeState::SUCCESS;
	}
	return ActionNode::NodeState::RUNNING;
}

/// <summary>
/// 描画
/// </summary>
const void Beast_FootComboAttack::Draw()const
{
#ifdef _DEBUG
	DrawCapsule3D(this->collider->rigidbody.GetPosition(), this->collider->topPositon, this->collider->radius, 16, GetColor(255, 0, 0), GetColor(255, 0, 0), FALSE);
#endif // _DEBUG
}
