//===============================================
// @brief �Z���N�g�V�[��UI�N���X
//===============================================
#pragma once

class Image;
class SceneUI;
class SelectUI : public SceneUI
{
public:
	 SelectUI();//�R���X�g���N�^
	~SelectUI();//�f�X�g���N�^

		  void Initialize()		 override;
		  void Update	 ()		 override;
	const void Draw		 ()const override;
	const bool IsEnd	 ()const override;
	const bool GetIsBack()const { return this->isBackTitle; }
private:
	/*�ÓI�萔*/

	/*�����o�ϐ�*/
	vector<int> enemyImage;		//�G�摜
	int			iconTable;		//�A�C�R���e�[�u��
	int			background;		//�w�i
	int			frame;			//�A�C�R���̃t���[��
	int			provDecide;		//����A�C�R��
	int			nowSelectEnemy;	//���ݑI������Ă���G
	bool		isEnd;			//�I�����邩
	int			headerFont;		//�w�b�_�[�t�H���g
	int			questFont;		//�N�G�X�g�t�H���g
	int			actionFont;		//�A�N�V�����t�H���g
	int			enemyImageTable;//�G�摜�e�[�u��
	int			drawRectBackGround;//�`��͈͔w�i
	int			aButton;
	int			bButton;
	int			lStick;
	bool		isProvDecideForBackTitle;
	bool		isBackTitle;
};

