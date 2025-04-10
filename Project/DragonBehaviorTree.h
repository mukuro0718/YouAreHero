//====================================================
// @brief �����i�N�g�p�r�w�C�r�A�c���[
//====================================================
#pragma once

class BehaviorTree;
class BehaviorTreeNode;
class DragonBehaviorTree : public BehaviorTree
{
public:
	 DragonBehaviorTree	();//�R���X�g���N�^
	~DragonBehaviorTree	();//�f�X�g���N�^

	/*�񋓑�*/
	//�A�N�V�����̎��
	enum class ActionType
	{
		DYING	= 0, //�f�X
		IDLE	= 1, //�ҋ@
		ROAR	= 2, //���K(�ʏ큨�{���Ԃ̕ω�)
		WALK	= 3, //����
		SMASH	= 4, //�@����
		SWEEP	= 5, //�Ȃ�����
		ROTATE	= 6, //��]�U��
		BREATH	= 7, //�u���X
	};
		  void Initialize		()					override;		//������
		  void Update			(Character& _chara) override;		//�X�V
	const void Draw				()					const override;	//�`��

private:
	/*���������֐�*/
	void UpdateMemberVariables	();//�����o�ϐ��̍X�V
	void CreateBehaviorTree		();//�r�w�C�r�A�c���[���쐬
};