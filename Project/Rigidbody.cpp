#include <DxLib.h>
#include "Rigidbody.h"

/// <summary>
/// コンストラクタ
/// </summary>
Rigidbody::Rigidbody()
	: position	(VGet(0.0f, 0.0f, 0.0f))
	, rotation	(VGet(0.0f, 0.0f, 0.0f))
	, scale		(VGet(0.0f, 0.0f, 0.0f))
	, direction	(VGet(0.0f, 0.0f, 0.0f))
	, velocity	(VGet(0.0f, 0.0f, 0.0f))
	, useGravity(false)
{
	//処理なし
}

/// <summary>
/// 初期化
/// </summary>
/// <param name="_useGravity"></param>
void Rigidbody::Initialize(bool _useGravity)
{
	this->position	 = VGet(0.0f, 0.0f, 0.0f);
	this->direction  = VGet(0.0f, 0.0f, 0.0f);
	this->velocity	 = VGet(0.0f, 0.0f, 0.0f);
	this->useGravity = _useGravity;
}

/// <summary>
/// velocityのsetとdirectionの自動計算
/// </summary>
/// <param name="_set"></param>
void Rigidbody::SetVelocity(const VECTOR& _set)
{
	this->velocity = _set;
	if (this->velocity.x != 0.0f || this->velocity.z != 0.0f)
	{
		this->direction = VNorm(this->velocity);
	}
}