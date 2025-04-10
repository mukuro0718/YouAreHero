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
		DYING			= 0, //�f�X
		IDLE			= 1, //�ҋ@
		RUN				= 2, //����
		WALK			= 3, //����
		WEAK_ATTACK		= 4, //��U��
		STRONG_ATTACK	= 5, //���U��
		REACTION		= 6, //���A�N�V����
		GUARD			= 7, //�K�[�h
		GUARD_REACTION	= 8, //�K�[�h�����A�N�V����
	};
		  void Initialize()					 override;		//������
		  void Update	 (Character& _chara) override;		//�X�V
	const void Draw		 ()					 const override;//�`��

private:
	/*���������֐�*/
	void UpdateMemberVariables	(Character& _chara);//�����o�ϐ��̍X�V
	void CreateBehaviorTree		();					//�r�w�C�r�A�c���[���쐬
};