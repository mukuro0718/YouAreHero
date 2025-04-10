//===========================================
// @brief �r�w�C�r�A�c���[�̃m�[�h�̃x�[�X�N���X
//===========================================
#pragma once

class BehaviorTree;
class Character;
class BehaviorTreeNode abstract
{
public:
	/*�񋓑�*/
	//�m�[�h�̏��
	enum class NodeState
	{
		NONE_ACTIVE,//���s����Ă��Ȃ�
		SUCCESS,	//����
		FAILURE,	//���s
		RUNNING,	//���s��
		COMPLETED,	//����
	};

			 BehaviorTreeNode(); //�R���X�g���N�^
	virtual ~BehaviorTreeNode(){}//�f�X�g���N�^

	virtual void	   Initialize(){}												//������
	virtual NodeState  Update	 (BehaviorTree& _tree, Character& _chara) abstract;	//�X�V�A�m�[�h�̏�Ԃ�Ԃ�
	virtual const void Draw		 ()const {}											//�`��
	virtual void	   AddChild	 (BehaviorTreeNode& _child){}						//�q���̒ǉ��i�ʏ�͏����Ȃ��j
protected:
	/*�����o�ϐ�*/
	NodeState state;//�m�[�h�̏��
};

