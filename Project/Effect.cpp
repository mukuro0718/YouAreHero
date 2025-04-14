#include <DxLib.h>
#include "EffekseerForDXLib.h"
#include "UseSTL.h"
#include "DeleteInstance.h"
#include "Transform.h"
#include "Rigidbody.h"
#include "Character.h"
#include "Effect.h"

/// <summary>
/// コンストラクタ
/// </summary>
Effect::Effect(const int _effectResourceHandle)
	: effectResourceHandle	(_effectResourceHandle)
	, isPlayEffect			(false)
	, transform				(nullptr)
	, frameCount			(0)
	, startFrame			(0)
	, endFrame				(0)
{
	this->transform = new Transform();
}

/// <summary>
/// コンストラクタ
/// </summary>
Effect::~Effect()
{
	DeleteMemberInstance(this->transform);
}

/// <summary>
/// 初期化
/// </summary>
void Effect::Initialize()
{
	this->frameCount		  = 0;
	this->isPlayEffect		  = false;
}

/// <summary>
/// 描画
/// </summary>
const void Effect::Draw()const
{
	/*再生フラグが立っていないまたは開始フレームを超えていなければ早期リターン*/
	if (!this->isPlayEffect || this->frameCount < this->startFrame)return;
	// Effekseerにより再生中のエフェクトを描画する。
	DrawEffekseer3D();
}

/// <summary>
/// 座標のセット
/// </summary>
void Effect::SetPosition(const VECTOR _position)
{
	this->transform->SetPosition(_position);
}

/// <summary>
/// 回転率のセット
/// </summary>
void Effect::SetRotation(const VECTOR _rotation)
{
	this->transform->SetRotation(_rotation);
}