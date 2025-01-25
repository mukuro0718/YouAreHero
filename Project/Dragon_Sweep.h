//=====================================================
// @brief �Ȃ������A�N�V����
//=====================================================
#pragma once

class ActionNode;
class Dragon_Sweep : public ActionNode
{
public:
	 Dragon_Sweep();//�R���X�g���N�^
	~Dragon_Sweep();//�f�X�g���N�^

	NodeState Update()override;//�X�V
private:
	/*�萔*/
	const short USE_COLLIDER_INDEX;		//�g�p����U���̔ԍ�
	const short FIX_ROTATE_FRAME;		//��]�␳���s���t���[��
	const short SWEEP_CANCEL_PLAY_TIME;	//�Ȃ������U���̃L�����Z���Đ�����

	/*�����o�ϐ�*/
	short frameCount;		 //�t���[���J�E���g
	float nowTotalPlayTime;	 //���݂̑��Đ�����
	short currentDragonStage;//���݂̃{�X�̒i�K
	vector<float> animationPlayTime;//�A�j���[�V�����Đ�����

};
