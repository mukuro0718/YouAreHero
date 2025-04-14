#include <cassert>
#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "DeleteInstance.h"
#include "Rigidbody.h"
#include "ReactionType.h"
#include "ColliderData.h"
#include "HitStop.h"
#include "AttackData.h"
#include "AttackCapsuleColliderData.h"
#include "BossAttack.h"
#include "BossJumpAttack.h"
#include "EnemyManager.h"
#include "Debug.h"

/// <summary>
/// コンストラクタ
/// </summary>
BossJumpAttack::BossJumpAttack(const int _attackIndex)
	: BossAttack()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*この攻撃がボスの何番目の攻撃かを保存しておく*/
	this->attackIndex = _attackIndex;

	/*コライダーデータの作成*/
	AttackData* data = new AttackData();
	this->collider = new AttackCapsuleColliderData(ColliderData::Priority::STATIC, GameObjectTag::MUTANT, data);

	/*コライダーの初期化*/
	auto& collider = dynamic_cast<AttackCapsuleColliderData&>(*this->collider);
	collider.radius							= json.GetJson(JsonManager::FileType::ENEMY)["ATTACK_RADIUS"][this->attackIndex];
	collider.data->damage					= json.GetJson(JsonManager::FileType::ENEMY)["ATTACK_DAMAGE"][this->attackIndex];
	collider.data->reactionType				= static_cast<int>(Gori::PlayerReactionType::BLOW_BIG);
	collider.data->hitStopTime				= json.GetJson(JsonManager::FileType::ENEMY)["DEFENSE_HIT_STOP_TIME"][this->attackIndex];
	collider.data->hitStopType				= static_cast<int>(HitStop::Type::STOP);
	collider.data->hitStopDelay				= json.GetJson(JsonManager::FileType::ENEMY)["DEFENSE_HIT_STOP_DELAY"][this->attackIndex];
	collider.data->slowFactor				= json.GetJson(JsonManager::FileType::ENEMY)["DEFENSE_SLOW_FACTOR"][this->attackIndex];
	collider.data->isHitAttack				= false;
	collider.data->blockStaminaConsumption	= json.GetJson(JsonManager::FileType::ENEMY)["BLOCK_STAMINA_CONSUMPTION"][this->attackIndex];

	this->startHitCheckFrame = json.GetJson(JsonManager::FileType::ENEMY)["START_HIT_CHECK_PLAY_TIME"][this->attackIndex];
	this->endHitCheckFrame	 = json.GetJson(JsonManager::FileType::ENEMY)["END_HIT_CHECK_PLAY_TIME"][this->attackIndex];
	this->positionOffset	 = json.GetJson(JsonManager::FileType::ENEMY)["ATTACK_OFFSET"][this->attackIndex];
	this->yOffset			 = json.GetJson(JsonManager::FileType::ENEMY)["ATTACK_OFFSET_Y"][this->attackIndex];
	this->totalAnimPlayTime  = json.GetJson(JsonManager::FileType::ENEMY)["TOTAL_ANIMATION_PLAY_TIME"][this->attackIndex];

}

/// <summary>
/// デストラクタ
/// </summary>
BossJumpAttack::~BossJumpAttack()
{

}

/// <summary>
/// 初期化
/// </summary>
void BossJumpAttack::Initialize()
{
	/*コライダーの初期化*/
	auto& collider = dynamic_cast<AttackCapsuleColliderData&>(*this->collider);
	collider.data->isHitAttack = false;

	/*変数の初期化*/
	this->frameCount	  = 0;
	this->isStartHitCheck = false;
	this->isNotOnHit	  = false;

	/*物理挙動の初期化*/
	this->collider->rigidbody.Initialize(false);
}
/// <summary>
/// 更新
/// </summary>
void BossJumpAttack::Update(const float _playTime)
{
	/*当たり判定の確認が開始している*/
	if (this->isStartHitCheck)
	{
		auto& enemy = Singleton<EnemyManager>::GetInstance();
		auto& collider = dynamic_cast<AttackCapsuleColliderData&>(*this->collider);
		//再生時間が定数を超えていなかったら早期リターン
		if (_playTime < this->startHitCheckFrame)return;

		//今回の攻撃中に当たり判定フラグが一度もたっていなかったら
		if (!this->isNotOnHit)
		{
			collider.data->isDoHitCheck = true;
			this->isNotOnHit = true;
		}

		//頭の座標
		VECTOR headPosition = MV1GetFramePosition(enemy.GetModelHandle(), 6);
		//尻の座標
		VECTOR hipPosition = MV1GetFramePosition(enemy.GetModelHandle(), 1);
		//頭から尻へ伸びるベクトル
		VECTOR headToHipVector = VNorm(VSub(hipPosition, headPosition));
		//頭から尻へ伸びるベクトルを定数でスケーリングしたものを頭の座標に足したものをカプセル下座標とする
		VECTOR underPosition = VScale(headToHipVector, this->backBornSize);
		underPosition = VAdd(underPosition, headPosition);
		//ひじの座標をカプセル下座標とする
		collider.rigidbody.SetPosition(underPosition);
		//爪先の座標をカプセル上座標とする
		collider.topPositon = headPosition;

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
const void BossJumpAttack::Draw()const
{
	if (this->isStartHitCheck)
	{
		auto& collider = dynamic_cast<AttackCapsuleColliderData&>(*this->collider);

		DrawCapsule3D(collider.rigidbody.GetPosition(), collider.topPositon, collider.radius, 16, GetColor(100, 100, 150), GetColor(100, 100, 150), FALSE);
	}
	VECTOR position = this->collider->rigidbody.GetPosition();
	printfDx("BossJumpAttack X:%f,Y:%f,Z:%f\n", position.x, position.y, position.z);
}