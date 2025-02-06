//=============================================================
// @brief �v���C���[���U���A�N�V�����N���X
//=============================================================
#pragma once

class PlayerAction;
class AttackSphereColliderData;
class PlayerStrongAttack : public PlayerAction
{
public:
	 PlayerStrongAttack();	//�R���X�g���N�^
	~PlayerStrongAttack();	//�f�X�g���N�^
	void Initialize	() override;				//������
	void Finalize	() override;				//�㏈��
	void Update		(Player& _player) override;	//�X�V
private:
	/*�萔*/
	const float FIRST_PLAY_TIME;			//�ŏ��̃A�j���[�V�����Đ�����
	const float CANCELABLE_PLAY_TIME;		//�L�����Z���\�Đ�����
	const float START_HIT_CHECK_PLAY_TIME;	//�����蔻��J�n�Đ�����
	const float END_HIT_CHECK_PLAY_TIME;	//�����蔻��Đ�����
	const float	POSITION_OFFSET;			//���W�I�t�Z�b�g
	const float	Y_OFFSET;					//Y�I�t�Z�b�g
	const int	HIT_STOP_TIME;				//�q�b�g�X�g�b�v����
	const int	HIT_STOP_DELAY;				//�q�b�g�X�g�b�v�f�B���C
	const int	HIT_STOP_TYPE;				//�q�b�g�X�g�b�v�̎��
	const float	SLOW_FACTOR;				//�X���[�W��
	const float	STAMINA_CONSUMPTION;		//�X�^�~�i�����

	/*�����o�ϐ�*/
	AttackSphereColliderData*	collider;		//�R���C�_�[
	VECTOR						firstDirection;	//�ŏ��̌���
	bool						isStartHitCheck;//�����蔻�肪��x�ł��J�n������
};

