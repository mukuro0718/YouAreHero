//===============================================
// @brief �{�X�N���X
//===============================================
#pragma once

class Character;
class Enemy;
class DemonBehaviorTree;
class CharacterData;
class CharacterColliderData;
class Demon : public Enemy
{
public:
	 Demon();//�R���X�g���N�^
	~Demon();//�f�X�g���N�^

	void		Initialize			() override;		//������
	void		Finalize			() override;		//�㏈��
	void		Update				() override;		//�X�V
	const void	DrawCharacterInfo	() const override;	//�`��
	void		PlayAnimation		();					//�A�j���[�V�����̍Đ�

	/*�񋓑�*/
	//�A�j���[�V����
	enum class AnimationType
	{
		DYING	,//�_�C
		IDLE	,//�ҋ@
	};

	/*getter/setter*/
	const bool GetIsAttack	 ()const override;	//�R�E�Q�L������
		  void SetAttackCount() override {}		//�U���񐔂̐ݒ�
private:
	/*�ÓI�萔*/
	static constexpr float SHADOW_HEIGHT = 10.0f;//�e�𓊉e���鍂��
	static constexpr float SHADOW_SIZE	 = 8.0f; //�e�̃T�C�Y
	static constexpr float LOCKON_OFFSET = 10.0f;//���b�N�I���I�t�Z�b�g
};

