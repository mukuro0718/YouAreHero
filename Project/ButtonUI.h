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
	/*構造体*/
	struct Vec2d
	{
		void Set(const int _x, const int _y) { this->x = _x; this->y = _y; }
		void Set(const std::vector<int> _pos) { this->x = _pos[0]; this->y = _pos[1]; }
		int x, y;
	};
	struct Box
	{
		void Set(const std::vector<int> _pos) { this->lx = _pos[0]; this->ly = _pos[1]; this->rx = _pos[2]; this->ry = _pos[3];}
		int lx, ly, rx, ry;
	};

	/*静的定数*/
	static constexpr int TEXT_COLOR = 0xffffff;

	/*内部処理関数*/
	int ConvertColor(const std::vector<int> _color);
	void DrawIcon();
	void DrawFont();
	void DrawButton();

	/*メンバ変数*/
	int table;
	int potion;
	int iconFont, operationFont;
	vector<int> button;
	int buttonFont;
};