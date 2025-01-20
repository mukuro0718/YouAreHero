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
#include "BossSlashCombo2Attack.h"
#include "EnemyManager.h"
#include "Debug.h"
#include "HitStop.h"

/// <summary>
/// コンストラクタ
/// </summary>
BossSlashCombo2Attack::BossSlashCombo2Attack(const int _attackIndex)
	: BossAttack()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*この攻撃がボスの何番目の攻撃かを保存しておく*/
	this->attackIndex = _attackIndex;

	/*コライダーデータの作成*/
	this->collider = new AttackCapsuleColliderData(ColliderData::Priority::STATIC, GameObjectTag::BOSS_ATTACK, new AttackData());

	/*コライダーの初期化*/
	auto& collider = dynamic_cast<AttackCapsuleColliderData&>(*this->collider);
	collider.radius				= json.GetJson(JsonManager::FileType::ENEMY)["ATTACK_RADIUS"][this->attackIndex];
	collider.data->damage		= json.GetJson(JsonManager::FileType::ENEMY)["ATTACK_DAMAGE"][this->attackIndex];
	collider.data->reactionType = static_cast<int>(Gori::PlayerReactionType::BLOW_BIG);
	//ここでのヒットストップ系の変数は、キャラクター側に与えるものになる
	collider.data->hitStopTime	= json.GetJson(JsonManager::FileType::ENEMY)["DEFENSE_HIT_STOP_TIME"][this->attackIndex];
	collider.data->hitStopType	= static_cast<int>(HitStop::Type::STOP);
	collider.data->hitStopDelay = json.GetJson(JsonManager::FileType::ENEMY)["DEFENSE_HIT_STOP_DELAY"][this->attackIndex];
	collider.data->slowFactor	= json.GetJson(JsonManager::FileType::ENEMY)["DEFENSE_SLOW_FACTOR"][this->attackIndex];

	this->startHitCheckFrame = json.GetJson(JsonManager::FileType::ENEMY)["START_HIT_CHECK_PLAY_TIME"][this->attackIndex];
	this->endHitCheckFrame	 = json.GetJson(JsonManager::FileType::ENEMY)["END_HIT_CHECK_PLAY_TIME"][this->attackIndex];
	this->positionOffset	 = json.GetJson(JsonManager::FileType::ENEMY)["ATTACK_OFFSET"][this->attackIndex];
	this->yOffset			 = json.GetJson(JsonManager::FileType::ENEMY)["ATTACK_OFFSET_Y"][this->attackIndex];
	this->totalAnimPlayTime	 = json.GetJson(JsonManager::FileType::ENEMY)["TOTAL_ANIMATION_PLAY_TIME"][this->attackIndex];

	//35-40 57-62 97-102
	vector<int> onDoHitFrame  = json.GetJson(JsonManager::FileType::ENEMY)["SLASH_COMBO_2_ON_DO_HIT_PLAY_TIME"];
	vector<int> offDoHitFrame = json.GetJson(JsonManager::FileType::ENEMY)["SLASH_COMBO_2_OFF_DO_HIT_PLAY_TIME"];
	this->onHitCheckFrame	  = onDoHitFrame;
	this->offHitChekcFrame	  = offDoHitFrame;
}

/// <summary>
/// デストラクタ
/// </summary>
BossSlashCombo2Attack::~BossSlashCombo2Attack()
{

}

/// <summary>
/// 初期化
/// </summary>
void BossSlashCombo2Attack::Initialize()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*コライダーの初期化*/
	auto& collider = dynamic_cast<AttackCapsuleColliderData&>(*this->collider);
	collider.data->isHitAttack	= false;

	/*変数の初期化*/
	this->frameCount			= 0;
	this->isStartHitCheck		= false;
	this->isStartHitCheck		= false;
	this->isNotOnHit			= false;

	/*物理挙動の初期化*/
	this->collider->rigidbody.Initialize(false);
}
/// <summary>
/// 更新
/// </summary>
void BossSlashCombo2Attack::Update(const float _playTime)
{

	/*当たり判定の確認が開始している*/
	if (this->isStartHitCheck)
	{
		//再生時間が定数を超えていなかったら早期リターン
		if (_playTime < this->startHitCheckFrame)return;

		auto& enemy = Singleton<EnemyManager>::GetInstance();
		auto& collider = dynamic_cast<AttackCapsuleColliderData&>(*this->collider);

		if (!this->isNotOnHit)
		{
			collider.data->isDoHitCheck = true;
			this->isNotOnHit = true;
		}

		for (int i = 0; i < this->onHitCheckFrame.size(); i++)
		{
			//指定フレームの時に、当たり判定フラグをもう一度立てる
			if (this->frameCount == this->onHitCheckFrame[i])
			{
				collider.data->isDoHitCheck = true;
			}
			//指定フレームの時に当たり判定フラグを下す
			else if (this->frameCount == this->offHitChekcFrame[i])
			{
				collider.data->isDoHitCheck = false;
			}
		}


		//ひじの座標
		VECTOR elbowPosition = MV1GetFramePosition(enemy.GetModelHandle(), 9);
		//手の一番外の座標
		VECTOR topPositionBase = MV1GetFramePosition(enemy.GetModelHandle(), 11);
		//ひじから手へ伸びるベクトル
		VECTOR underToTopBaseVector = VNorm(VSub(topPositionBase, elbowPosition));
		//ひじから手へ伸びるベクトルを定数でスケーリングしたものをひじの座標に足したものを爪の先端座標とする
		VECTOR crowTopPosition = VScale(underToTopBaseVector,this->crowSize);
		crowTopPosition = VAdd(crowTopPosition, elbowPosition);
		//ひじの座標をカプセル下座標とする
		collider.rigidbody.SetPosition(elbowPosition);
		//爪先の座標をカプセル上座標とする
		collider.topPositon = crowTopPosition;

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
const void BossSlashCombo2Attack::Draw()const
{
	if (this->isStartHitCheck)
	{
		auto& collider = dynamic_cast<AttackCapsuleColliderData&>(*this->collider);
		DrawCapsule3D(collider.rigidbody.GetPosition(), collider.topPositon, collider.radius, 16, GetColor(100, 100, 150), GetColor(100, 100, 150), FALSE);
	}
	VECTOR position = this->collider->rigidbody.GetPosition();
	printfDx("SLASH_COMBO_2 X:%f,Y:%f,Z:%f\n", position.x, position.y, position.z);
}