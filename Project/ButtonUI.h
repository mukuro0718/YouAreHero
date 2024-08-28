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

	/*内部処理関数*/
	int ConvertColor(const std::vector<int> _color);
	void DrawIcon();
	void DrawFont();

	/*メンバ変数*/
	int specialAttackSize;
	int b, x, y, lb, ls, lt, rb, rs;
	int iconFont, operationFont;
};

