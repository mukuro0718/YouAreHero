#include <cassert>
#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "DeleteInstance.h"
#include "ReactionType.h"
#include "Rigidbody.h"
#include "ColliderData.h"
#include "AttackData.h"
#include "AttackCapsuleColliderData.h"
#include "BossAttack.h"
#include "BossPunchAttack.h"
#include "EnemyManager.h"
#include "Debug.h"
#include "HitStop.h"
#include "SoundManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
BossPunchAttack::BossPunchAttack(const int _attackIndex)
	: BossAttack()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*この攻撃がボスの何番目の攻撃かを保存しておく*/
	this->attackIndex = _attackIndex;

	/*コライダーデータの作成*/
	this->collider = new AttackCapsuleColliderData(ColliderData::Priority::STATIC, GameObjectTag::MUTANT, new AttackData());

	/*コライダーの初期化*/
	auto& collider = dynamic_cast<AttackCapsuleColliderData&>(*this->collider);
	collider.radius							= json.GetJson(JsonManager::FileType::ENEMY)["ATTACK_RADIUS"][this->attackIndex];
	collider.data->damage					= json.GetJson(JsonManager::FileType::ENEMY)["ATTACK_DAMAGE"][this->attackIndex];
	collider.data->reactionType				= static_cast<int>(Gori::PlayerReactionType::NORMAL);
	collider.data->hitStopTime				= json.GetJson(JsonManager::FileType::ENEMY)["DEFENSE_HIT_STOP_TIME"][this->attackIndex];
	collider.data->hitStopType				= static_cast<int>(HitStop::Type::STOP);
	collider.data->hitStopDelay				= json.GetJson(JsonManager::FileType::ENEMY)["DEFENSE_HIT_STOP_DELAY"][this->attackIndex];
	collider.data->slowFactor				= json.GetJson(JsonManager::FileType::ENEMY)["DEFENSE_SLOW_FACTOR"][this->attackIndex];
	collider.data->isHitAttack				= false;
	collider.data->blockStaminaConsumption	= json.GetJson(JsonManager::FileType::ENEMY)["BLOCK_STAMINA_CONSUMPTION"][this->attackIndex];

	this->startHitCheckFrame	= json.GetJson(JsonManager::FileType::ENEMY)["START_HIT_CHECK_PLAY_TIME"][this->attackIndex];
	this->endHitCheckFrame		= json.GetJson(JsonManager::FileType::ENEMY)["END_HIT_CHECK_PLAY_TIME"][this->attackIndex];
	this->positionOffset		= json.GetJson(JsonManager::FileType::ENEMY)["ATTACK_OFFSET"][this->attackIndex];
	this->yOffset				= json.GetJson(JsonManager::FileType::ENEMY)["ATTACK_OFFSET_Y"][this->attackIndex];
	this->totalAnimPlayTime		= json.GetJson(JsonManager::FileType::ENEMY)["TOTAL_ANIMATION_PLAY_TIME"][this->attackIndex];

}

/// <summary>
/// デストラクタ
/// </summary>
BossPunchAttack::~BossPunchAttack()
{

}

/// <summary>
/// 初期化
/// </summary>
void BossPunchAttack::Initialize()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*コライダーの初期化*/
	auto& collider = dynamic_cast<AttackCapsuleColliderData&>(*this->collider);
	collider.data->isHitAttack	= false;

	/*変数の初期化*/
	this->frameCount = 0;
	this->isStartHitCheck = false;
	this->isStartHitCheck = false;
	this->isNotOnHit = false;

	/*物理挙動の初期化*/
	this->collider->rigidbody.Initialize(false);
}
/// <summary>
/// 更新
/// </summary>
void BossPunchAttack::Update(const float _playTime)
{
	/*当たり判定の確認が開始している*/
	if (this->isStartHitCheck)
	{
		//再生時間が定数を超えていなかったら早期リターン
		if (_playTime < this->startHitCheckFrame)return;

		auto& enemy = Singleton<EnemyManager>::GetInstance();
		auto& collider = dynamic_cast<AttackCapsuleColliderData&>(*this->collider);

		//今回の攻撃中に当たり判定フラグが一度もたっていなかったら
		if (!this->isNotOnHit)
		{
			//サウンドエフェクトの再生
			auto& sound = Singleton<SoundManager>::GetInstance();
			sound.OnIsPlayEffect(SoundManager::EffectType::MONSTER_SWING_1);
			collider.data->isDoHitCheck = true;
			this->isNotOnHit = true;
		}

		//当たり判定位置の更新
		collider.rigidbody.SetPosition(MV1GetFramePosition(enemy.GetModelHandle(), 14));
		collider.topPositon = MV1GetFramePosition(enemy.GetModelHandle(), 19);
		//再生時間が定数を超えている、当たり判定フラグが降りていたら当たり判定開始フラグを下す
		if (_playTime > this->endHitCheckFrame)
		{
			this->isStartHitCheck = false;
			collider.data->isDoHitCheck = false;
			this->frameCount = 0;
			collider.data->isHitAttack = false;
		}
	}
}

/// <summary>
/// 描画
/// </summary>
const void BossPunchAttack::Draw()const
{
	if (this->isStartHitCheck)
	{
		auto& collider = dynamic_cast<AttackCapsuleColliderData&>(*this->collider);
		DrawCapsule3D(collider.rigidbody.GetPosition(), collider.topPositon, collider.radius, 16, GetColor(100, 100, 150), GetColor(100, 100, 150), FALSE);
	}
	VECTOR position = this->collider->rigidbody.GetPosition();
	printfDx("SLAP X:%f,Y:%f,Z:%f\n", position.x, position.y, position.z);
}