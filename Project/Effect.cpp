#include <DxLib.h>
#include "EffekseerForDXLib.h"
#include "UseSTL.h"
#include "DeleteInstance.h"
#include "Transform.h"
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
	, firstFrame			(0)
{
	this->transform = new Transform();
}

/// <summary>
/// コンストラクタ
/// </summary>
Effect::~Effect()
{
	DeleteMemberInstance(this->transform);
	this->playingEffectHandle.clear();
}

/// <summary>
/// 初期化
/// </summary>
void Effect::Initialize()
{
	this->playingEffectHandle.emplace_back(-1);
	this->frameCount		  = 0;
	this->isPlayEffect		  = false;
	this->startFrame		  = 0;
	this->endFrame			  = 0;
}

/// <summary>
/// 更新
/// </summary>
void Effect::Update()
{
	/*再生フラグが立っていなければ早期リターン*/
	if (!this->isPlayEffect)return;

	if (this->frameCount == 0)
	{
		/*エフェクトの再生に必要な情報の設定*/
		SetInfoToPlayTheEffect();
	}

	/*フレーム計測*/
	//フレームの増加
	this->frameCount++;
	//開始フレームを超えていなければ早期リターン
	if (this->frameCount < this->startFrame)return;

	/*エフェクトの更新*/
	//再生エフェクトのハンドル
	if (this->frameCount == this->startFrame)
	{
		this->playingEffectHandle[0] = PlayEffekseer3DEffect(this->effectResourceHandle);
		SetRotationPlayingEffekseer3DEffect(this->playingEffectHandle[0], this->transform->GetRotation().x, this->transform->GetRotation().y, this->transform->GetRotation().z);
		SetScalePlayingEffekseer3DEffect(this->playingEffectHandle[0], this->transform->GetScale().x, this->transform->GetScale().y, this->transform->GetScale().z);
	}
	// 再生中のエフェクトを移動する
	SetPosPlayingEffekseer3DEffect(this->playingEffectHandle[0], this->transform->GetPosition().x, this->transform->GetPosition().y, this->transform->GetPosition().z);
	//Effekseerにより再生中のエフェクトを更新する。
	UpdateEffekseer3D();

	//終了フレームを超えていたら再生フラグを下す
	if (this->frameCount >= this->endFrame)
	{
		this->isPlayEffect = false;
		this->frameCount = this->firstFrame;
		StopEffekseer3DEffect(this->playingEffectHandle[0]);
		this->playingEffectHandle[0] = -1;
	}
}

/// <summary>
/// 描画
/// </summary>
const void Effect::Draw()const
{
	/*再生フラグが立っていなければ早期リターン*/
	if (!this->isPlayEffect)return;
	// Effekseerにより再生中のエフェクトを描画する。
	DrawEffekseer3D();
}

/// <summary>
/// 座標の設定
/// </summary>
void Effect::SetPosition(const VECTOR _position)
{
	this->transform->SetPosition(_position);
}