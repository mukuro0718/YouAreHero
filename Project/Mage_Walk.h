//=====================================================
// @brief �����A�N�V����
//=====================================================
#pragma once

class BehaviorTree;
class ActionNode;
class Character;
class Mage_Walk : public ActionNode
{
public:
	 Mage_Walk();//�R���X�g���N�^
	~Mage_Walk();//�f�X�g���N�^

	NodeState Update	(BehaviorTree& _tree, Character& _chara)override;	//�X�V
private:
	/*�ÓI�萔*/
	static constexpr float THRESHOLD = 0.1f;

	/*�����o�ϐ�*/
	short frameCount;		//�t���[���J�E���g
	short walkSoundCount;	//�����̉����o���J�E���g
};

