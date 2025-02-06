//============================================
// @brief �{�X���K�p�����[�^
//============================================
#pragma once

class Boss;
class BossAction;
class BossRoarAction : public BossAction
{
public:
	 BossRoarAction();//�R���X�g���N�^
	~BossRoarAction();//�f�X�g���N�^
	
	void Initialize	  ()				  override;//������
	void Update		  (Boss& _boss)		  override;//�p�����[�^�̌v�Z
	void CalcParameter(const Boss& _boss) override;//�p�����[�^�[�̌v�Z
private:
	/*�萔*/
	const int PLAY_ROAR_SOUND_PLAY_TIME;//���K�̌��ʉ���炷�Đ�����

	/*�����o�ϐ��̏�����*/
	int	  prevState;			 //�O�̓{����
	bool  isFinishedFirstRoar;	 //�ŏ��̙��K���I���������t���O
	short checkedState;			 //�~���l�̌v�Z���s���Ƃ��Ɏg�p����m�F���������
};

