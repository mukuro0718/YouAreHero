#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "Sound.h"

/// <summary>
/// コンストラクタ
/// </summary>
Sound::Sound(const int _soundHandle, const int _volume)
	: soundHandle				(_soundHandle)
	, isPlay					(false)
	, volume					(_volume)
	, DISTANCE_SOUND_CAN_HEARD	(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::SOUND)["DISTANCE_SOUND_CAN_HEARD"])
{
	ChangeVolumeSoundMem(this->volume, this->soundHandle);
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

/// <summary>
/// 音を再生しているか
/// </summary>
const bool Sound::GetCheckSoundState()
{
	int state = CheckSoundMem(this->soundHandle);
	if (state == 1)return true;
	return false;
}