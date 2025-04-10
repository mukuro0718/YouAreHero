//====================================================
// @brief ���N�X���I�p�r�w�C�r�A�c���[
//====================================================
#pragma once
#include "Singleton.h"

class BehaviorTree;
class BehaviorTreeNode;
class DemonBehaviorTree : public BehaviorTree
{
public:
	 DemonBehaviorTree	  ();//�R���X�g���N�^
	~DemonBehaviorTree	  ();//�f�X�g���N�^

	/*�񋓑�*/
	enum class ActionType
	{
		DYING				= 0, //�f�X
		IDLE				= 1, //�ҋ@
	};
		  void Initialize	()					override;		//������
		  void Update		(Character& _chara) override;		//�X�V
	const void Draw			()					const override;	//�`��

	/*getter/setter*/
private:
	/*���������֐�*/

	/*�����o�ϐ�*/
	BehaviorTreeNode* action_Dying;	//demon�A�N�V����
	BehaviorTreeNode* action_Idle;	//demon�A�N�V����
};