//===============================================
// @brief �^�C�g��UI�N���X
//===============================================
#pragma once

class SceneUI;
class CreditUI;
class TitleUI : public SceneUI
{
public:
	TitleUI();
	~TitleUI();

		  void Initialize()		 override;
		  void Update	 ()		 override;
	const void Draw		 ()const override;
	const bool IsEnd	 ()const override;
private:
	/*�\���́E�񋓑�*/
	//���S�̎��
	enum class LogoType
	{
		TITLE,
		PRESS,
	};
	//�摜�f�[�^�\����
	struct ImageData
	{
		int handle;
		int alpha;
		int alphaIncrease;
		int alphaReduction;
		int x;
		int y;
		double rate;
		double angle;
	};

	/*���������֐�*/
	bool IsPressButton();

	/*�����o�ϐ�*/
	CreditUI* credit;
	ImageData titleLogo;
	ImageData pressLogo;
	bool	  isFadeIn;
	bool	  isPrevPressButton;
};

