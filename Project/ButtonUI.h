//======================================================
// @brief ButtonUIクラス
//======================================================

#pragma once

class ButtonUI
{
public:
	ButtonUI();		//コンストラクタ
	~ButtonUI();		//デストラクタ
	void Initialize();//初期化
	void Update();
	void Draw();	//描画
private:
	enum class TextType 
	{
		W_ATTACK,
		HEAL,
		S_ATTACK,
		AVOID,
		GUARD,
	};	

	/*静的定数*/
	static constexpr int TEXT_COLOR = 0xffffff;
	static constexpr int MAX_CHAR_INDEX = 256;

	/*内部処理関数*/
	int ConvertColor(const std::vector<int> _color);
	void DrawItem();
	void DrawTextImage();
	void DrawButton();

	/*メンバ変数*/
	int table;
	int potion;
	int operationFont;
	int itemFont;
	vector<int> button;
	int	operationTable; //説明背景
	vector<int>	buttonList;//画像のリスト
	map<int, vector<int>> displayTextMap;
	map<int, vector<int>> displayButtonMap;
	vector<int> changeDisplayIndexType;
	vector<string> operationText;
	int displayIndex;
};