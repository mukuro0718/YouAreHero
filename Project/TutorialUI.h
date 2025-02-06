//===============================================
// @brief �Q�[��UI�N���X
//===============================================
#pragma once

class Image;
class HPUI;
class ButtonUI;
class BossNameUI;
class GameClearUI;
class GameOverUI;
class TutorialUI
{
public:
	TutorialUI();
	~TutorialUI();

		  void Initialize()		;
		  void Update	 ()		;
	const void Draw		 ()const;
private:
	/*�ÓI�萔*/
	static constexpr int	TEXT_COLOR = 0xffffff;
	static constexpr short	MAX_ALPHA = 255;
	static constexpr short	ADD_ALPHA = 5;

	/*�����o�ϐ�*/
	int			 alpha;				//�A���t�@�l
	int			 backGround;		//�w�i
	int			 fontHandle;		//���f�p�t�H���g�n���h��
	int			 frameCount;		//�t���[���J�E���g
	short		 textDrawTime;		//���S�`�掞��
	bool		 isFadeIn;			//�t�F�[�h�C�����邩
	vector<int>  backGroundDrawRect;//�w�i�`��͈�
	vector<int>  textPosition;		//�������W
};

