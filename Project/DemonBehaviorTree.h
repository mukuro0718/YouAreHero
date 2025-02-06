//====================================================
// @brief ���N�X���I�p�r�w�C�r�A�c���[
//====================================================
#pragma once
#include "Singleton.h"

class BehaviorTreeNode;
class DemonBehaviorTree : public Singleton<DemonBehaviorTree>
{
public:
	friend class Singleton<DemonBehaviorTree>;

	/*�񋓑�*/
	enum class ActionType
	{
		DYING				= 0, //�f�X
		IDLE				= 1, //�ҋ@
	};
		  void Initialize	();		//������
		  void Update		();		//�X�V
	const void Draw			()const;//�`��

	/*getter/setter*/
private:
	/*���������֐�*/
		  DemonBehaviorTree	  ();//�R���X�g���N�^
		 ~DemonBehaviorTree	  ();//�f�X�g���N�^

	/*�����o�ϐ�*/
	BehaviorTreeNode* action_Dying;	//demon�A�N�V����
	BehaviorTreeNode* action_Idle;	//demon�A�N�V����
};