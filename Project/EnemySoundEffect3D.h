//=======================================================
// @brief�@3DSoundEffect�N���X
//=======================================================
#pragma once

class SoundEffect;
class EnemySoundEffect3D : public SoundEffect
{
public:
	 EnemySoundEffect3D(const int _soundHandle, const int _volume);	//�R���X�g���N�^
	~EnemySoundEffect3D();											//�f�X�g���N�^

	void OnIsPlay() override;//�Đ��t���O�𗧂Ă�
	void Update() override;//�X�V
};

