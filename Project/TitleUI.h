//===============================================
// @brief タイトルUIクラス
//===============================================
#pragma once

class Image;
class SceneUI;
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
	/*列挙体*/
	enum class Type
	{
		TITLE,//タイトルロゴ
		PRESS,//「PressA」ロゴ
	};

	/*内部処理関数*/
	bool IsPressButton();

	/*メンバ変数*/
	std::vector<Image*> image;
	bool isPrevPressButton;
	bool isFadeOut;
};

