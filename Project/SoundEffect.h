//=======================================================
// @brief�@SoundEffect�N���X
//=======================================================
#pragma once

class Sound;
class SoundEffect : public Sound
{
public:
	 SoundEffect(const int _soundHandle, const int _volume);	//�R���X�g���N�^
	~SoundEffect();												//�f�X�g���N�^

	void Update() override;//�X�V
};

