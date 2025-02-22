//===========================================
// @brief クレジット表記クラス
//===========================================
#pragma once
class CreditUI
{
public:
	 CreditUI();//コンストラクタ
	~CreditUI();//デストラクタ

	void Update();//更新
	const void Draw()const;//描画

	const bool GetIsOpen()const { return isOpenCredit; }//選択されているか取得
private:
	/*定数*/
	const int			 ADD_Y_OFFSET;			//オフセットの追加量
	const int			 TEXT_FONT_SIZE;		//表示しているテキストの文字サイズ
	static constexpr int FONT_COLOR = 0xcccccc;	//フォントカラー
	static constexpr int BACK_COLOR = 0x333333;	//背景カラー
	static constexpr int WINDOW_HEIGHT = 1080;	//ウィンドウの高さ
	static constexpr int WINDOW_WIDTH = 1920;	//ウィンドウの幅

	/*メンバ変数*/
	bool isOpenCredit;	//選択しているか
	int  yOffset;		//Y座標のオフセット値
	int  fontHandle;	//フォントハンドル
};

