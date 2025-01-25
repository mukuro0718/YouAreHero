#include <DxLib.h>
#include "UseSTL.h"
#include "Sound.h"

/// <summary>
/// コンストラクタ
/// </summary>
Sound::Sound(const int _soundHandle, const int _volume)
	: soundHandle(_soundHandle)
	, isPlay	 (false)
	, volume	 (_volume)
{
	int state = ChangeVolumeSoundMem(this->volume, this->soundHandle);
	if (state == -1)printfDx("miss");
}

/// <summary>
/// デストラクタ
/// </summary>
Sound::~Sound()
{
	StopSoundMem(this->soundHandle);
}

/// <summary>
/// 更新
/// </summary>
void Sound::Update()
{

}

/// <summary>
/// 再生フラグを立てる
/// </summary>
void Sound::OnIsPlay()
{
	this->isPlay = true;
	StopSoundMem(this->soundHandle);
	SetCurrentPositionSoundMem(0, this->soundHandle);
	PlaySoundMem(this->soundHandle, DX_PLAYTYPE_BACK);
}

/// <summary>
/// 再生フラグを下す
/// </summary>
void Sound::OffIsPlay()
{
	this->isPlay = false;
	StopSoundMem(this->soundHandle);
	SetCurrentPositionSoundMem(0, this->soundHandle);
}