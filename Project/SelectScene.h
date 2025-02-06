//==================================================
// @brief �`���[�g���A���V�[��
//==================================================
#pragma once

class SceneBase;
class SelectScene : public SceneBase
{
public:
	 SelectScene(); //�R���X�g���N�^
	~SelectScene();//�f�X�g���N�^

	void		Initialize	() override;		//������
	void		Finalize	() override;		//�ŏI����
	void		Update		() override;		//�X�V
	const void	Draw		() const override;	//�`��

private:
	/*���������֐�*/
	bool IsEnd();

	/*�����o�ϐ�*/
	int frameCount;
};

