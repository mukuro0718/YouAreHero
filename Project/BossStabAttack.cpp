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
#include "HitStop.h"
#include "BossAttack.h"
#include "BossStabAttack.h"
#include "EnemyManager.h"
#include "Debug.h"

/// <summary>
/// コンストラクタ
/// </summary>
BossStabAttack::BossStabAttack(const int _attackIndex)
	: BossAttack()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*この攻撃がボスの何番目の攻撃かを保存しておく*/
	this->attackIndex = _attackIndex;

	/*コライダーデータの作成*/
	this->collider = new AttackCapsuleColliderData(ColliderData::Priority::STATIC, GameObjectTag::BOSS_ATTACK, new AttackData());
}

/// <summary>
/// デストラクタ
/// </summary>
BossStabAttack::~BossStabAttack()
{

}

/// <summary>
/// 初期化
/// </summary>
void BossStabAttack::Initialize()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*コライダーの初期化*/
	auto& collider	= dynamic_cast<AttackCapsuleColliderData&>(*this->collider);
	collider.radius				= json.GetJson(JsonManager::FileType::ENEMY)["ATTACK_RADIUS"][this->attackIndex];
	collider.data->damage		= json.GetJson(JsonManager::FileType::ENEMY)["ATTACK_DAMAGE"][this->attackIndex];
	collider.data->reactionType = static_cast<int>(Gori::PlayerReactionType::BLOW_BIG);
	//ここでのヒットストップ系の変数は、キャラクター側に与えるものになる
	collider.data->hitStopTime	= json.GetJson(JsonManager::FileType::ENEMY)["DEFENSE_HIT_STOP_TIME"][this->attackIndex];
	collider.data->hitStopType	= static_cast<int>(HitStop::Type::STOP);
	collider.data->hitStopDelay	= json.GetJson(JsonManager::FileType::ENEMY)["DEFENSE_HIT_STOP_DELAY"][this->attackIndex];
	collider.data->slowFactor	= json.GetJson(JsonManager::FileType::ENEMY)["DEFENSE_SLOW_FACTOR"][this->attackIndex];
	collider.data->isHitAttack	= false;

	/*変数の初期化*/
	this->frameCount	  = 0;
	this->isStartHitCheck = false;
	this->isStartHitCheck = false;
	this->isNotOnHit	  = false;

	/*物理挙動の初期化*/
	this->collider->rigidbody.Initialize(false);
}
/// <summary>
/// 更新
/// </summary>
void BossStabAttack::Update()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& enemy = Singleton<EnemyManager>::GetInstance();
	auto& collider = dynamic_cast<AttackCapsuleColliderData&>(*this->collider);

	/*当たり判定の確認が開始している*/
	if (this->isStartHitCheck)
	{
		//変数の準備
		const int	START_HIT_CHECK_FRAME = json.GetJson(JsonManager::FileType::ENEMY)["START_HIT_CHECK_FRAME"]	[this->attackIndex];
		const int	END_HIT_CHECK_FRAME	  = json.GetJson(JsonManager::FileType::ENEMY)["END_HIT_CHECK_FRAME"]	[this->attackIndex];
		const float POSITION_OFFSET		  = json.GetJson(JsonManager::FileType::ENEMY)["ATTACK_OFFSET"]			[this->attackIndex];
		const float Y_OFFSET			  = json.GetJson(JsonManager::FileType::ENEMY)["ATTACK_OFFSET_Y"]		[this->attackIndex];

		//フレームを増やす
		this->frameCount++;
		//フレームが定数を超えていなかったら早期リターン
		if (this->frameCount < START_HIT_CHECK_FRAME)return;

		//今回の攻撃中に当たり判定フラグが一度もたっていなかったら
		if (!this->isNotOnHit)
		{
			collider.data->isDoHitCheck = true;
			this->isNotOnHit = true;
		}

		//当たり判定位置の更新
		collider.rigidbody.SetPosition(MV1GetFramePosition(enemy.GetModelHandle(), 9));
		collider.topPositon = MV1GetFramePosition(enemy.GetModelHandle(), 11);
		//フレームが定数を超えていたら当たり判定開始フラグを下す
		if (this->frameCount > END_HIT_CHECK_FRAME || (this->isNotOnHit && !collider.data->isDoHitCheck))
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
const void BossStabAttack::Draw()const
{
	if (this->isStartHitCheck)
	{
		auto& collider = dynamic_cast<AttackCapsuleColliderData&>(*this->collider);
		DrawCapsule3D(collider.rigidbody.GetPosition(), collider.topPositon, collider.radius, 16, GetColor(100, 100, 150), GetColor(100, 100, 150), FALSE);
	}
	VECTOR position = this->collider->rigidbody.GetPosition();
	printfDx("JUMP_ATTACK X:%f,Y:%f,Z:%f\n", position.x, position.y, position.z);
}