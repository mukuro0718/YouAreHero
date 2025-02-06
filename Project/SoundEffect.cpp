#include <DxLib.h>
#include "UseSTL.h"
#include "Sound.h"
#include "SoundEffect.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
SoundEffect::SoundEffect(const int _soundHandle, const int _volume)
	: Sound(_soundHandle, _volume)
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
SoundEffect::~SoundEffect()
{
	StopSoundMem(this->soundHandle);
}

/// <summary>
/// �X�V
/// </summary>
void SoundEffect::Update()
{
}