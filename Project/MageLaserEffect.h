//=====================================
// @brief�@�b�{�X��u���X�G�t�F�N�g
//=====================================
#pragma once

class Effect;
class MageLaserEffect : public Effect
{
public:
	MageLaserEffect(const int _effectResourceHandle);//�R���X�g���N�^
	~MageLaserEffect() {}//�f�X�g���N�^

	void Update() override;//�X�V
private:
	int		playingEffectHandle;//�Đ��G�t�F�N�g�n���h��
	bool	isPlay;				//�Ή�����G�t�F�N�g���Đ����Ă��邩
};

