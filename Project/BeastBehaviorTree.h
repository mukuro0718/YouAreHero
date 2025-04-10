//====================================================
// @brief ���N�X���I�p�r�w�C�r�A�c���[
//====================================================
#pragma once

class BehaviorTree;
class BehaviorTreeNode;
class Character;
class BeastBehaviorTree : public BehaviorTree
{
public:
	 BeastBehaviorTree();//�R���X�g���N�^
	~BeastBehaviorTree();//�f�X�g���N�^

	/*�񋓑�*/
	enum class ActionType
	{
		DYING				= 0, //�f�X
		IDLE				= 1, //�ҋ@
		ROAR				= 2, //���K(�ʏ큨�{���Ԃ̕ω�)
		WALK				= 3, //����
		RUN					= 4, //�_�b�V��
		TURN_LEFT			= 5, //��������
		TURN_RIGHT			= 6, //�E������
		DOWN				= 7, //�_�E��
		LONG_FRIGHT			= 8, //��������
		SHORT_FRIGHT		= 9, //�Z������
		REST				= 10,//�x�e
		STEP_BACK			= 11,//���ɃW�����v���ĉ�����
		RAISE_LEVEL			= 12,//���̃`���[�W�����㏸������i���K�j
		BACKING_BREATH		= 13,//���ɉ�����Ȃ���u���X
		RUSH				= 14,//�ːi
		CHARGE_RIGHT_ATTACK	= 15,//�E�O�����ߍU��
		CHARGE_BOTH_ATTACK	= 16,//���O�����ߍU��
		BREATH				= 17,//�u���X
		SUPER_NOVA			= 18,//�X�[�p�[�m���@
		WEAK_BREATH			= 19,//��u���X
		SMALL_EXPLOSION		= 20,//������
		RIGHT_FOOT_ATTACK	= 21,//�E���U��
		EXPLOSION			= 22,//�����U���i�X�[�p�[�m���@����������)
		COMBO_ATTACK		= 23,//3�A���U��
	};
	//���x���̒i�K
	enum class LevelStage
	{
		Lv1 = 0,
		Lv2 = 1,
		Lv3 = 2,
		Lv4 = 3,
	};
		  void Initialize	() override;	 //������
		  void Update		(Character& _chara) override;	 //�X�V
	const void Draw			()const override;//�`��
private:
	/*���������֐�*/
	void UpdateMemberVariables();//�����o�ϐ��̍X�V
	void CreateBehaviorTree();//�r�w�C�r�A�c���[���쐬
};