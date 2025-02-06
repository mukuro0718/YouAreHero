//===============================================
// @brief �Q�[��UI�N���X
//===============================================
#pragma once

class SceneUI;
class Image;
class HPUI;
class ButtonUI;
class BossNameUI;
class GameClearUI;
class GameOverUI;
class TutorialUI;
class GameUI : public SceneUI
{
public:
	GameUI();
	~GameUI();

		  void Initialize()		 override;
		  void Update	 ()		 override;
	const void Draw		 ()const override;
	const bool IsEnd	 ()const override;
private:
	/*�񋓑�*/
	enum class ResultType
	{
		NONE,
		WIN,
		LOSE,
	};

	/*�ÓI�萔*/
	static constexpr int TEXT_COLOR = 0xffffff;
	static constexpr short MAX_ALPHA = 255;
	const float LOCK_ON_UI_OFFSET;
	const float LOCK_ON_UI_SIZE;

	/*���������֐�*/
	void SetType();

	/*�����o�ϐ�*/
	HPUI*		 hp;				//�o�[UI
	ButtonUI*	 button;			//�{�^��UI
	BossNameUI*  bossName;			//�{�X�̖��OUI
	GameClearUI* clearUI;			//�N�G�X�g�N���AUI
	GameOverUI*	 overUI;			//�N�G�X�g���sUI
	TutorialUI*  tutorialUI;		//�`���[�g���A��UI
	ResultType	 type;				//���S�̎��
	bool		 isEnd;				//�I���t���O
	int			 alpha;				//�A���t�@�l
	int			 backGround;		//�w�i
	int			 lockOnImage;		//���b�N�I��UI�摜
	int			 pauseFontHandle;	//���f�p�t�H���g�n���h��
	int			 pauseActionHandle; //���f�p�t�H���g�n���h��
	int			 frameCount;		//�t���[���J�E���g
	bool		 isPause;			//���f�t���O
	short		 maxAlpha;			//�ő�A���t�@
	short		 addAlpha;			//�����A���t�@
	short		 logoDrawTime;		//���S�`�掞��
	int			 decideButton;		//����{�^��
	int			 backButton;		//�߂�{�^��
	vector<int> pauseTableDrawRect;			//���f�e�[�u���`��͈�
	vector<int> pauseHeaderTextPosition;	//���f�w�b�_�[�������W
	vector<int> pauseActionTextPosition;	//���f�A�N�V�����������W
	vector<int> decideButtonDrawRect;		//����{�^���`��͈�
	vector<int> backButtonDrawRect;			//�߂�`��͈�
};

