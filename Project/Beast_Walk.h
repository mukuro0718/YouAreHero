//=====================================================
// @brief �ҋ@�A�N�V����
//=====================================================
#pragma once

class ActionNode;
class Beast_Walk : public ActionNode
{
public:
	 Beast_Walk();//�R���X�g���N�^
	~Beast_Walk();//�f�X�g���N�^

	NodeState Update()override;//�X�V
private:
	short frameCount;		//�t���[���J�E���g
	short walkSoundCount;	//�����̉����o���J�E���g
};

