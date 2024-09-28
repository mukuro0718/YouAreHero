//===============================================
// @brief ゲームUIクラス
//===============================================
#pragma once

class Image;
class SceneUI;
class GameUI : public SceneUI
{
public:
	GameUI();
	~GameUI();

	void Initialize()		 override;
	void Update()		 override;
	const void Draw()const override;
	const bool IsEnd()const override;
private:
	/*列挙体*/
	enum class Type
	{
		WIN,//タイトルロゴ
		LOSE,//「PressA」ロゴ
	};

	/*内部処理関数*/
	void SetType();

	/*メンバ変数*/
	std::vector<Image*> image;
	bool isPrevPressAButton;
	bool isEndExtend;
	bool isEnd;
	int frameCount;
	int type;
};

