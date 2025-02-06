//=============================================
// @brief �{�X�X���b�V���U���A�N�V����Ver.2
//=============================================
#pragma once

class Boss;
class BossAction;
class BossSlash2Action : public BossAction
{
public:
	 BossSlash2Action();//�R���X�g���N�^
	~BossSlash2Action();//�f�X�g���N�^

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
	const short	 ROTATE_FIX_COUNT;	//��]�␳�p
	const short	 ROTATE_PLAY_TIME;	//��]�␳�p
	const VECTOR ROTATE_LERP_VALUE;	//��]�␳��
	const short	 MOVE_PLAY_TIME;	//�ړ��p
	const short	 STOP_PLAY_TIME;	//��~�p
	const float	 STOP_DISTANCE;		//��~���鋗��
	const short  MAX_INTERVAL;		//�ő�C���^�[�o��
	const float	 SLOW_PLAY_TIME;	//�X���[���̍Đ�����
	const float	 SLOW_LIMIT_TIME;	//�A�j���[�V������x������ő厞��
	const float	 ACTION_DISTANCE;	//�A�N�V�������\�ɂȂ鋗��
	const short	 CHECK_STATE;		//�ǂ̏�ԂŊm�F���邩

	/*�����o�ϐ�*/
	bool	isClose;			//�߂Â�����
	float	speed;				//���x
};

