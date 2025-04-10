//=====================================================
// @brief �����A�N�V����
//=====================================================
#pragma once

class BehaviorTree;
class ActionNode;
class Character;
class Dragon_Walk : public ActionNode
{
public:
	 Dragon_Walk();//�R���X�g���N�^
	~Dragon_Walk();//�f�X�g���N�^

	NodeState Update(BehaviorTree& _tree, Character& _chara)override;//�X�V
private:
	short frameCount;		//�t���[���J�E���g
	short walkSoundCount;	//�����̉����o���J�E���g

};

