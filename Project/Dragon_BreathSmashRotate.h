//=====================================================
// @brief �u���X�{�@�����{��]�U���A�N�V����
//=====================================================
#pragma once

class ActionNode;
class Dragon_BreathSmashRotate : public ActionNode
{
public:
	 Dragon_BreathSmashRotate();//�R���X�g���N�^
	~Dragon_BreathSmashRotate();//�f�X�g���N�^

	NodeState Update()override;//�X�V
private:
	/*�񋓑�*/
	//�A�j���[�V�����̎��
	enum class UseAnimationType
	{
		BREATH	= 0,//�u���X
		WALK	= 1,//����
		SMASH	= 2,//�@����
		ROTATE	= 3,//��]�U��
	};

	/*�����o�ϐ�*/
	vector<short>	animationType;		//�A�j���[�V�����̎��
	vector<float>	animationPlayTime;	//�A�j���[�V�����Đ�����
	short			useAnimationType;	//�g�p����A�j���[�V�����̃^�C�v
	short			maxUseAnimation;	//�g�p����A�j���[�V�����̍ő�
	float			nearAttackRange;	//�ߐڍU���͈�
	float			smashCancelPlayTime;//�@�����U���L�����Z���t���[��
	float			totalPlayTime;		//���Đ�����
};
