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
	HPUI*		hp;			//バーUI
	ButtonUI*	button;		//ボタンUI
	BossNameUI* bossName;	//ボスの名前UI
	int			imageHandle;//画像ハンドル
	bool		isEnd;		//終了フラグ
	int			alpha;		//アルファ値
	int			fontHandle;	//フォントハンドル
	int			pauseFontHandle;//中断用フォントハンドル
	int			frameCount;	//フレームカウント
	int			type;		//ロゴの種類
	bool		isPause;	//中断フラグ

	short maxAlpha;//最大アルファ
	short addAlpha;//増加アルファ
	short logoDrawTime;//ロゴ描画時間
	vector<int> tableDrawRect;//テーブル描画範囲
	vector<int> pauseTableDrawRect;//中断テーブル描画範囲
	vector<int> pauseTextPosition;//中断文字座標
	vector<int> destroyTextPosition;//討伐文字座標
	vector<int> resultTextPosition;//結果文字座標
};

