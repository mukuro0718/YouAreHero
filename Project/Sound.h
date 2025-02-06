//=======================================================
// @brief�@sound�̊��N���X
// �esound�N���X�͂��̃N���X���p������
//=======================================================
#pragma once
class Sound abstract
{
public:
			 Sound(const int _soundHandle, const int _volume);	//�R���X�g���N�^
	virtual ~Sound();						//�f�X�g���N�^

	virtual void Update	 ()abstract;//�X�V
	virtual void OnIsPlay();		//�Đ��t���O�𗧂Ă�
	virtual void OffIsPlay();		//�Đ��t���O������
	const bool GetCheckSoundState();
protected:
	/*�萔*/
	const float DISTANCE_SOUND_CAN_HEARD;

	/*�����o�ϐ�*/
	int  soundHandle;//�T�E���h�n���h��
	bool isPlay;	 //�Đ����邩
	int  volume;	 //����
};

