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
	/*列挙体*/
	enum class ImageType
	{
		CONTINUE,
		END,
		WINDOW,//ウィンドウ
	};

	void SetType();
	bool IsPressButton();

	std::vector<Image*> image;
	bool isPrevPressButton;
	bool isContinue;
	bool isGameClear;
	bool isEnd;
	bool isEndExtend;
	int type;//ゲームオーバー時のタイプ（Continue/End）
};

