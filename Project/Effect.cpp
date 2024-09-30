#include <DxLib.h>
#include "EffekseerForDXLib.h"
#include "Transform.h"
#include "Effect.h"

Effect::Effect(const int _effectResourceHandle)
	: effectResourceHandle(_effectResourceHandle)
	, playingEffectHandle(-1)
	, frameCount(0)
	, isPlayEffect(false)
	, startFrame(0)
	, endFrame(0)
	, transform(nullptr)
{
	this->transform = new Transform();
}

/// <summary>
/// 更新
/// </summary>
void Effect::Update()
{
	/*再生フラグが立っていなければ早期リターン*/
	if (!this->isPlayEffect)return;

	/*エフェクトの再生に必要な情報の設定*/
	SetInfoToPlayTheEffect();

	/*フレーム計測*/
	//フレームの増加
	this->frameCount++;
	//開始フレームを超えていなければ早期リターン
	if (this->frameCount < this->startFrame)return;

	/*エフェクトの更新*/
	//再生エフェクトのハンドル
	if (this->frameCount == this->startFrame)
	{
		this->playingEffectHandle = PlayEffekseer3DEffect(this->effectResourceHandle);
		SetRotationPlayingEffekseer3DEffect(this->playingEffectHandle, this->transform->GetRotation().x, this->transform->GetRotation().y, this->transform->GetRotation().z);
		SetScalePlayingEffekseer3DEffect(this->playingEffectHandle, this->transform->GetScale().x, this->transform->GetScale().y, this->transform->GetScale().z);
	}
	// 再生中のエフェクトを移動する
	SetPosPlayingEffekseer3DEffect(this->playingEffectHandle, this->transform->GetPosition().x, this->transform->GetPosition().y, this->transform->GetPosition().z);
	//Effekseerにより再生中のエフェクトを更新する。
	UpdateEffekseer3D();

	//終了フレームを超えていたら再生フラグを下す
	if (this->frameCount >= this->endFrame)
	{
		this->isPlayEffect = false;
		this->frameCount = 0;
		StopEffekseer3DEffect(this->playingEffectHandle);
		this->playingEffectHandle = -1;
	}
}

/// <summary>
/// 描画
/// </summary>
const void Effect::Draw()const
{
	/*再生フラグが立っていなければ早期リターン*/
	if (this->playingEffectHandle == -1)return;
	// Effekseerにより再生中のエフェクトを描画する。
	DrawEffekseer3D();
}

/// <summary>
/// vector<float>をVECTORに変換
/// </summary>
const VECTOR Effect::Convert(std::vector<float> _in)const
{
	VECTOR out = VGet(0.0f, 0.0f, 0.0f);
	out.x = _in[0];
	out.y = _in[1];
	out.z = _in[2];
	return out;
}
