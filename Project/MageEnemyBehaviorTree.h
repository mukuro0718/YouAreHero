//====================================================
// @brief ���@�^�̓G�p�r�w�C�r�A�c���[
//====================================================
#pragma once

class BehaviorTree;
class BehaviorTreeNode;
class MageEnemyBehaviorTree : public BehaviorTree
{
public:
	 MageEnemyBehaviorTree();//�R���X�g���N�^
	~MageEnemyBehaviorTree();//�f�X�g���N�^

	/*�񋓑�*/
	//�A�N�V�����̎��
	enum class ActionType
	{
		DYING	 = 0, //�f�X
		IDLE	 = 1, //�ҋ@
		RUN		 = 2, //����
		WALK	 = 3, //����
		ATTACK	 = 4, //�U��
		REACTION = 5, //���A�N�V����
	};
		  void Initialize()					 override;		//������
		  void Update	 (Character& _chara) override;		//�X�V
	const void Draw		 ()					 const override;//�`��

private:
	/*���������֐�*/
	void UpdateMemberVariables	(Character& _chara);//�����o�ϐ��̍X�V
	void CreateBehaviorTree		();					//�r�w�C�r�A�c���[���쐬
};