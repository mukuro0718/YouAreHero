//=======================================================
// @brief�@3DSoundEffect�N���X
//=======================================================
#pragma once

class SoundEffect;
class PlayerSoundEffect3D : public SoundEffect
{
public:
	 PlayerSoundEffect3D(const int _soundHandle, const int _volume);//�R���X�g���N�^
	~PlayerSoundEffect3D();											//�f�X�g���N�^

	void OnIsPlay() override;//�Đ��t���O�𗧂Ă�
	void Update() override;//�X�V
};

