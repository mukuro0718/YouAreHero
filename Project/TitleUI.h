//===============================================
// @brief タイトルUIクラス
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
	/*構造体・列挙体*/
	//ロゴの種類
	enum class LogoType
	{
		TITLE,
		PRESS,
	};
	//画像データ構造体
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

	/*内部処理関数*/
	bool IsPressButton();

	/*メンバ変数*/
	CreditUI* credit;
	ImageData titleLogo;
	ImageData pressLogo;
	bool	  isFadeIn;
	bool	  isPrevPressButton;
};

