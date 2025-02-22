//===========================================
// @brief �N���W�b�g�\�L�N���X
//===========================================
#pragma once
class CreditUI
{
public:
	 CreditUI();//�R���X�g���N�^
	~CreditUI();//�f�X�g���N�^

	void Update();//�X�V
	const void Draw()const;//�`��

	const bool GetIsOpen()const { return isOpenCredit; }//�I������Ă��邩�擾
private:
	/*�萔*/
	const int			 ADD_Y_OFFSET;			//�I�t�Z�b�g�̒ǉ���
	const int			 TEXT_FONT_SIZE;		//�\�����Ă���e�L�X�g�̕����T�C�Y
	static constexpr int FONT_COLOR = 0xcccccc;	//�t�H���g�J���[
	static constexpr int BACK_COLOR = 0x333333;	//�w�i�J���[
	static constexpr int WINDOW_HEIGHT = 1080;	//�E�B���h�E�̍���
	static constexpr int WINDOW_WIDTH = 1920;	//�E�B���h�E�̕�

	/*�����o�ϐ�*/
	bool isOpenCredit;	//�I�����Ă��邩
	int  yOffset;		//Y���W�̃I�t�Z�b�g�l
	int  fontHandle;	//�t�H���g�n���h��
};

