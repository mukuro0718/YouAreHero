#include <DxLib.h>
#include "UseSTL.h"
#include "Sound.h"
#include "SoundEffect.h"

/// <summary>
/// コンストラクタ
/// </summary>
SoundEffect::SoundEffect(const int _soundHandle, const int _volume)
	: Sound(_soundHandle, _volume)
{

}

/// <summary>
/// デストラクタ
/// </summary>
SoundEffect::~SoundEffect()
{
	StopSoundMem(this->soundHandle);
}

/// <summary>
/// 更新
/// </summary>
void SoundEffect::Update()
{
}

