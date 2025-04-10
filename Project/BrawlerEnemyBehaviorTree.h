//====================================================
// @brief �ߐڍU���^�̓G�p�r�w�C�r�A�c���[
//====================================================
#pragma once

class BehaviorTree;
class BehaviorTreeNode;
class BrawlerEnemyBehaviorTree : public BehaviorTree
{
public:
	 BrawlerEnemyBehaviorTree();//�R���X�g���N�^
	~BrawlerEnemyBehaviorTree();//�f�X�g���N�^

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
		SWAY	 = 6, //�K�[�h
	};
		  void Initialize()					 override;		//������
		  void Update	 (Character& _chara) override;		//�X�V
	const void Draw		 ()					 const override;//�`��

private:
	/*���������֐�*/
	void UpdateMemberVariables	(Character& _chara);//�����o�ϐ��̍X�V
	void CreateBehaviorTree		();					//�r�w�C�r�A�c���[���쐬
};