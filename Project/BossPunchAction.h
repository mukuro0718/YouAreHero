//=============================================
// @brief �{�X�p���`�U���A�N�V����
//=============================================
#pragma once

class Boss;
class BossAction;
class BossPunchAction : public BossAction
{
public:
	 BossPunchAction();//�R���X�g���N�^
	~BossPunchAction();//�f�X�g���N�^

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
	const float	 ACTION_DISTANCE;	//�A�N�V�������\�ɂȂ鋗��
	const short	 CHECK_STATE;		//�ǂ̏�ԂŊm�F���邩
	const short  MAX_INTERVAL;		//�ő�C���^�[�o��
	const VECTOR ROTATE_LERP_VALUE;	//��]�␳��
};

