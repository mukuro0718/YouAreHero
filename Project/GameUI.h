//===============================================
// @brief ゲームUIクラス
//===============================================
#pragma once

class SceneUI;
class Image;
class HPUI;
class ButtonUI;
class BossNameUI;
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
	/*列挙体*/
	enum class Type
	{
		WIN,//タイトルロゴ
		LOSE,//「PressA」ロゴ
	};

	/*静的定数*/
	static constexpr int TEXT_COLOR = 0xffffff;

	/*内部処理関数*/
	void SetType();

	/*メンバ変数*/
	HPUI* hp;
	ButtonUI* button;
	BossNameUI* bossName;
	int imageHandle;
	bool isEnd;
	int alpha;
	int fontHandle;
	int frameCount;
	int type;
};

