//=============================================================
// @brief �v���C���[���݃A�N�V�����N���X
//=============================================================
#pragma once

class PlayerAction;
class PlayerKnockdown : public PlayerAction
{
public:
	 PlayerKnockdown();	//�R���X�g���N�^
	~PlayerKnockdown();	//�f�X�g���N�^
	void Initialize	() override;				//������
	void Finalize	() override;				//�㏈��
	void Update		(Player& _player) override;	//�X�V
private:
	/*�萔*/
	const float MAX_SPEED;			//�ő呬�x
	const float DECEL;				//�����x
	const int   VIBRATION_POWER;	//�U�����鋭��
	const int   VIBRATION_TIME;		//�U�����鎞��

	/*�����o�ϐ�*/
	float nowSpeed;//���݂̑��x
};

