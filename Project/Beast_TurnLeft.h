//=====================================================
// @brief ���։��A�N�V����
//=====================================================
#pragma once

class BehaviorTree;
class ActionNode;
class Character;
class Beast_TurnLeft : public ActionNode
{
public:
	 Beast_TurnLeft();//�R���X�g���N�^
	~Beast_TurnLeft();//�f�X�g���N�^

	NodeState	Update(BehaviorTree& _tree, Character& _chara)override;	//�X�V
private:
	short maxFrameCount;
	short frameCount;
};

