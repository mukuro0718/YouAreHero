//===============================================
// @brief リザルトUIクラス
//===============================================
#pragma once

class Image;
class SceneUI;
class GameOverUI : public SceneUI
{
public:
	GameOverUI();
	~GameOverUI();

	void Initialize()		 override;
	void Update()		 override;
	const void Draw()const override;
	const bool IsEnd()const override;
	const bool IsContinue()const;
private:
	/*静的定数*/
	static constexpr int TEXT_COLOR = 0xdfffff;

	/*列挙体*/
	//画像の種類
	enum class ImageType
	{
		CONTINUE,
		END,
	};
	//フォントの種類
	enum class FontType
	{
		MAIN,
		SUB,
	};

	void SetType();
	bool IsPressButton();

	std::vector<Image*> image;
	std::vector<int> fontHandle;
	bool isPrevPressButton;
	bool isContinue;
	bool isEnd;
	bool isEndExtend;
	int  type;//ゲームオーバー時のタイプ（Continue/End）
};

