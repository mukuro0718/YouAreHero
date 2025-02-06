#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "Sound.h"

/// <summary>
/// �R���X�g���N�^
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
/// �f�X�g���N�^
/// </summary>
Sound::~Sound()
{
	StopSoundMem(this->soundHandle);
}

/// <summary>
/// �X�V
/// </summary>
void Sound::Update()
{

}

/// <summary>
/// �Đ��t���O�𗧂Ă�
/// </summary>
void Sound::OnIsPlay()
{
	this->isPlay = true;
	StopSoundMem(this->soundHandle);
	SetCurrentPositionSoundMem(0, this->soundHandle);
	PlaySoundMem(this->soundHandle, DX_PLAYTYPE_BACK);
}

/// <summary>
/// �Đ��t���O������
/// </summary>
void Sound::OffIsPlay()
{
	this->isPlay = false;
	StopSoundMem(this->soundHandle);
	SetCurrentPositionSoundMem(0, this->soundHandle);
}

/// <summary>
/// �����Đ����Ă��邩
/// </summary>
const bool Sound::GetCheckSoundState()
{
	int state = CheckSoundMem(this->soundHandle);
	if (state == 1)return true;
	return false;
}