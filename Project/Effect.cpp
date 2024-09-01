#include <DxLib.h>
#include "EffekseerForDXLib.h"
#include "GoriLib.h"
#include "Effect.h"

Effect::Effect(const int _effectResourceHandle)
	: effectResourceHandle(_effectResourceHandle)
	, playingEffectHandle(-1)
	, frameCount(0)
	, isPlayEffect(false)
{
}

/// <summary>
/// 更新
/// </summary>
void Effect::Update(const int _startFrame, const int _endFrame, const VECTOR _position, const VECTOR _rotation, const VECTOR _scale)
{
	/*再生フラグが立っていなければ早期リターン*/
	if (!this->isPlayEffect)return;

	/*フレーム計測*/
	//フレームの増加
	this->frameCount++;
	//開始フレームを超えていなければ早期リターン
	if (this->frameCount < _startFrame)return;

	/*エフェクトの更新*/
	//再生エフェクトのハンドル
	if (this->frameCount == _startFrame)
	{
		this->playingEffectHandle = PlayEffekseer3DEffect(this->effectResourceHandle);
		SetRotationPlayingEffekseer3DEffect(this->playingEffectHandle, _rotation.x, _rotation.y, _rotation.z);
		SetScalePlayingEffekseer3DEffect(this->playingEffectHandle, _scale.x, _scale.y, _scale.z);
	}
	// 再生中のエフェクトを移動する
	SetPosPlayingEffekseer3DEffect(this->playingEffectHandle, _position.x, _position.y, _position.z);
	//Effekseerにより再生中のエフェクトを更新する。
	UpdateEffekseer3D();

	//終了フレームを超えていたら再生フラグを下す
	if (this->frameCount >= _endFrame)
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