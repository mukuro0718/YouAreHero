//=============================================
// @brief �{�X��]�R��U���A�N�V����
//=============================================
#pragma once

class Boss;
class BossAction;
class BossSlashCombo2Action : public BossAction
{
public:
	 BossSlashCombo2Action();//�R���X�g���N�^
	~BossSlashCombo2Action();//�f�X�g���N�^

	void Initialize	  ()				  override;//������
	void Update		  (Boss& _boss)		  override;//�X�V
	void CalcParameter(const Boss& _boss) override;//�p�����[�^�[�̌v�Z

private:
	/*�萔*/
	const int	 ATTACK_TYPE;		//�U���̎��
	const int	 HIT_STOP_DELAY;	//�q�b�g�X�g�b�v�f�B���C
	const float	 HIT_STOP_TIME;		//�q�b�g�X�g�b�v����
	const int	 HIT_STOP_TYPE;		//�q�b�g�X�g�b�v�̎��
	const float	 SLOW_FACTOR;		//�X���[�W��
	const VECTOR ROTATE_LERP_VALUE;	//��]�␳��
	const short	 ROTATE_FIX_COUNT;	//��]�␳�p
	const short	 ROTATE_PLAY_TIME_1;//��]�␳�p
	const short	 ROTATE_PLAY_TIME_2;//��]�␳�p
	const short	 MOVE_PLAY_TIME;	//�ړ��p
	const short	 STOP_PLAY_TIME;	//��~�p
	const float	 STOP_DISTANCE;		//��~���鋗��
	const short  MAX_INTERVAL;		//�ő�C���^�[�o��
	const float	 ACTION_DISTANCE;	//�A�N�V�������\�ɂȂ鋗��
	const short	 CHECK_STATE;		//�ǂ̏�ԂŊm�F���邩
	const float	 SLOW_PLAY_TIME;	//�X���[���̍Đ�����
	const float	 SLOW_LIMIT_TIME;	//�A�j���[�V������x������ő厞��
	const float  TARGET_OFFSET;		//�ڕW�I�t�Z�b�g

	/*�����o�ϐ�*/
	bool	isClose;//��x�ł��߂Â�����
	float	speed;	//���x
};

