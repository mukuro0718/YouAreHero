//=======================================================
// @brief�@BGM�N���X
//=======================================================
#pragma once

class Sound;
class Bgm : public Sound
{
public:
	 Bgm(const int _soundHandle, const int _volume);//�R���X�g���N�^
	~Bgm();											//�f�X�g���N�^

	void OnIsPlay() override;//�Đ��t���O�𗧂Ă�
	void Update	 () override;//�X�V
};

