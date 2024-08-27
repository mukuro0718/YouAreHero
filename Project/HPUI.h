//======================================================
// @brief HPUIクラス
// ボスとプレイヤーのHPの表示を行う
// （HPの直接管理は行わない）
//======================================================

#pragma once
class HPUI
{
public:
	HPUI();		//コンストラクタ
	~HPUI();		//デストラクタ
	void Initialize();//初期化
	void Update();
	const void Draw()const;	//描画
private:
	/*構造体*/
	struct RangeNum
	{
	public:
		void SetRange(const int _max, const int _min, const int _now);
		void Add(const int _add);
		void PrevDecrease();
		const float GetMax()const { return this->max; }
		const float GetNow()const { return this->now; }
		const float GetPrev()const { return this->prev; }
		void SetNow(const int _hp);
	private:
		float max, min, now, prev;
	};
	struct Vec2d
	{
		void Set(const int _x, const int _y) { this->x = _x; this->y = _y; }
		void Set(const std::vector<int> _pos) { this->x = _pos[0]; this->y = _pos[1]; }
		int x, y;
	};
	struct Box
	{
		int lx, ly, rx, ry;
	};

	/*内部処理関数*/
	int ConvertColor(const std::vector<int> _color);

	/*メンバ変数*/
	int playerHPColor;
	int bossHPColor;
	int prevPlayerHPColor;
	int prevBossHPColor;
	RangeNum playerHP;	//プレイヤーHP
	RangeNum bossHP;	//ボスHP
	Vec2d playerHPPosition;
	Vec2d bossHPPosition;
};

