#include <DxLib.h>
#include "UseSTL.h"
#include "Sound.h"
#include "Bgm.h"

/// <summary>
/// コンストラクタ
/// </summary>
Bgm::Bgm(const int _soundHandle, const int _volume)
	: Sound(_soundHandle, _volume)
{
}

/// <summary>
/// デストラクタ
/// </summary>
Bgm::~Bgm()
{
	StopSoundMem(this->soundHandle);
}

/// <summary>
/// 更新
/// </summary>
void Bgm::Update()
{
}

/// <summary>
/// 再生フラグを立てる
/// </summary>
void Bgm::OnIsPlay()
{
	this->isPlay = true;
	StopSoundMem(this->soundHandle);
	SetCurrentPositionSoundMem(0, this->soundHandle);
	PlaySoundMem(this->soundHandle, DX_PLAYTYPE_LOOP);
}
