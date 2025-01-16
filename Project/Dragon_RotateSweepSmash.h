//=====================================================
// @brief ��]�U���{�Ȃ������{�@�����A�N�V����
//=====================================================
#pragma once

class ActionNode;
class Dragon_RotateSweepSmash : public ActionNode
{
public:
	 Dragon_RotateSweepSmash();//�R���X�g���N�^
	~Dragon_RotateSweepSmash();//�f�X�g���N�^

	NodeState Update()override;//�X�V
private:
	/*�񋓑�*/
	//�A�j���[�V�����̎��
	enum class UseAnimationType
	{
		WALK_1	= 0,//����
		ROTATE	= 1,//��]�U��
		WALK_2	= 2,//����
		SWEEP	= 3,//�Ȃ�����
		SMASH	= 4,//�@����
	};

	/*�����o�ϐ�*/
	vector<short>	animationType;		//�A�j���[�V�����̎��
	vector<float>	animationPlayTime;	//�A�j���[�V�����Đ�����
	short			useAnimationType;	//�g�p����A�j���[�V�����̃^�C�v
	short			maxUseAnimation;	//�g�p����A�j���[�V�����̍ő�
	float			nearAttackRange;	//�ߐڍU���͈�
	float			sweepCancelPlayTime;//�Ȃ������U���L�����Z���t���[��
	float			totalPlayTime;		//���Đ�����
};
