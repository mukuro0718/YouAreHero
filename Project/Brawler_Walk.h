//=====================================================
// @brief �����A�N�V����
//=====================================================
#pragma once

class BehaviorTree;
class ActionNode;
class Character;
class Brawler_Walk : public ActionNode
{
public:
	 Brawler_Walk();//�R���X�g���N�^
	~Brawler_Walk();//�f�X�g���N�^

	NodeState Update	(BehaviorTree& _tree, Character& _chara)override;	//�X�V
private:
	short frameCount;		//�t���[���J�E���g
	short walkSoundCount;	//�����̉����o���J�E���g
};

