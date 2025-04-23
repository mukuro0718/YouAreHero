//====================================================
// @brief �^���N�^�̓G�p�r�w�C�r�A�c���[
//====================================================
#pragma once

class BehaviorTree;
class BehaviorTreeNode;
class TankEnemyBehaviorTree : public BehaviorTree
{
public:
	TankEnemyBehaviorTree();//�R���X�g���N�^
	~TankEnemyBehaviorTree();//�f�X�g���N�^

	/*�񋓑�*/
	//�A�N�V�����̎��
	enum class ActionType
	{
		DYING	 = 0, //�f�X
		IDLE	 = 1, //�ҋ@
		WALK	 = 2, //����
		ATTACK	 = 3, //��U��
		REACTION = 4, //���A�N�V����
	};
		  void Initialize()					 override;		//������
		  void Update	 (Character& _chara) override;		//�X�V
	const void Draw		 ()					 const override;//�`��

private:
	/*���������֐�*/
	void UpdateMemberVariables	(Character& _chara);//�����o�ϐ��̍X�V
	void CreateBehaviorTree		();					//�r�w�C�r�A�c���[���쐬
};