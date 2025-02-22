//=====================================================
// @brief ��]�U���A�N�V����
//=====================================================
#pragma once

class ActionNode;
class Dragon_Rotate : public ActionNode
{
public:
	 Dragon_Rotate();//�R���X�g���N�^
	~Dragon_Rotate();//�f�X�g���N�^

	NodeState Update()override;//�X�V
private:
	/*�萔*/
	const short USE_COLLIDER_INDEX;		//�g�p����U���̔ԍ�

	/*�����o�ϐ�*/
	float			nowTotalPlayTime;	//���݂̑��Đ�����
	short			currentDragonState;	//���݂̃{�X�̒i�K
	vector<float>	animationPlayTime;	//�A�j���[�V�����Đ�����
	short			frameCount;			//�t���[���J�E���g
};

