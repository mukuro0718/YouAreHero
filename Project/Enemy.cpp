#include <DxLib.h>
#include <Effekseer.h>
#include <EffekseerRendererDX11.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "VECTORtoUseful.h"
#include "DeleteInstance.h"
#include "ReactionType.h"
#include "Rigidbody.h"
#include "CharacterData.h"
#include "ColliderData.h"
#include "CharacterColliderData.h"
#include "Animation.h"
#include "Character.h"
#include "Player.h"
#include "Enemy.h"
#include "PlayerManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
Enemy::Enemy()
	: moveTarget			(Gori::ORIGIN)
	, animationPlayTime		(0.0f)
	, nowAnimation			(0)

{
	/*コライダーデータの作成*/
	this->collider = new CharacterColliderData(ColliderData::Priority::HIGH, GameObjectTag::BOSS, new CharacterData());
}

/// <summary>
/// デストラクタ
/// </summary>
Enemy::~Enemy()
{
}

/// <summary>
/// アニメーション再生時間の取得
/// </summary>
const float Enemy::GetAnimationPlayTime()const
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();
	return json.GetJson(JsonManager::FileType::ENEMY)["ANIMATION_PLAY_TIME"][this->nowAnimation];
}

/// <summary>
/// 回転率の設定
/// </summary>
void Enemy::SetRotation(const VECTOR _rotation)
{
	this->collider->rigidbody.SetRotation(_rotation);
}

/// <summary>
/// 移動ベクトルの設定
/// </summary>
void Enemy::SetVelocity(const VECTOR _velocity)
{
	this->collider->rigidbody.SetVelocity(_velocity);
}

/// <summary>
/// 回転率の更新
/// </summary>
void Enemy::UpdateRotation(VECTOR _toTarget)
{
	/*カメラの向きとスティックの入力をもとに回転率を出す*/
	this->nextRotation.y = static_cast<float>(atan2(static_cast<double>(_toTarget.x), static_cast<double>(_toTarget.z)));

	/*現在の回転率をラープで補完して出す*/
	VECTOR nowRotation = this->collider->rigidbody.GetRotation();
	this->collider->rigidbody.SetRotation(Gori::LerpAngle(nowRotation, this->nextRotation, this->ROTATE_LERP_VALUE));
}

/// <summary>
/// 移動速度の更新
/// </summary>
void Enemy::UpdateSpeed(const float _maxSpeed, const float _accel, const float _decel)
{
	/*新しい速度を出す*/
	auto& json = Singleton<JsonManager>::GetInstance();
	if (_maxSpeed != 0)
	{
		this->speed += _accel;
		//最大速度を超えないように調整する
		if (this->speed >= _maxSpeed || this->speed <= _maxSpeed)
		{
			this->speed = _maxSpeed;
		}
	}
	else
	{
		this->speed += _decel;
		//０以下にならないように調整する
		if (this->speed <= 0)
		{
			this->speed = 0;
		}
	}
}

/// <summary>
/// 移動ベクトルの更新
/// </summary>
void Enemy::UpdateVelocity(const bool _isLerp)
{
	/*移動する向きを出す*/
	VECTOR rotation = this->collider->rigidbody.GetRotation();
	VECTOR direction = VGet(-sinf(rotation.y), 0.0f, -cosf(rotation.y));
	direction = VNorm(direction);

	/*新しい移動ベクトルを出す*/
	VECTOR ainVelocity = VScale(direction, this->speed);
	VECTOR prevVelcity = this->collider->rigidbody.GetVelocity();
	VECTOR newVelocity = VGet(ainVelocity.x, prevVelcity.y, ainVelocity.z);

	/*補正フラグが立っていたら補正する*/
	if (_isLerp)
	{
		auto& json = Singleton<JsonManager>::GetInstance();
		newVelocity = Gori::LerpVECTOR(prevVelcity, ainVelocity, this->VELOCITY_LERP_VALUE);
	}
	
	this->collider->rigidbody.SetVelocity(newVelocity);
}

/// <summary>
/// 移動処理
/// </summary>
void Enemy::Move(const float _maxSpeed, const float _accel, const float _decel, const bool _isLerp)
{
	/*回転率の更新*/
	//移動目標の設定
	auto& player = Singleton<PlayerManager>::GetInstance();
	this->moveTarget = player.GetRigidbody().GetPosition();
	//目標までのベクトルを出す
	VECTOR toTarget = VSub(this->collider->rigidbody.GetPosition(), this->moveTarget);
	//回転率の更新
	UpdateRotation(toTarget);

	/*移動速度の更新*/
	UpdateSpeed(_maxSpeed, _accel, _decel);

	/*移動ベクトルを出す*/
	UpdateVelocity(_isLerp);
}

