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
	
		  void Initialize();		//初期化
		  void Update	 ();		//更新
	const void Draw		 ()const;	//描画
private:
	/*構造体*/
	//範囲
	struct RangeNum
	{
	public:
		RangeNum()
		{
			this->max = this->min = this->now = this->prev = 0.0f;
		}
		void SetRange	 (const int _max, const int _min, const int _now);	//範囲の設定
		void Add		 (const int _add);									//追加
		void PrevDecrease();												//前の値を減少
		
		/*getter/setter*/
		const float GetMax ()const { return this->max; } //最大値の取得
		const float GetNow ()const { return this->now; } //今の値の取得
		const float GetPrev()const { return this->prev; }//前の値の取得
			  void  SetNow (const int _hp);				 //今の値を設定
	private:
		/*メンバ変数*/
		float max, min, now, prev;//範囲
	};
	//二次元ベクトル構造体
	struct Vec2d
	{
		Vec2d()
		{
			this->x = this->y = 0;
		}
		void Set(const int _x, const int _y) { this->x = _x; this->y = _y; }
		void Set(const std::vector<int> _pos) { this->x = _pos[0]; this->y = _pos[1]; }
		int x, y;
	};
	//ボックス構造体
	struct Box
	{
		void Set(const std::vector<int> _pos) { this->lx = _pos[0]; this->ly = _pos[1]; this->rx = _pos[2]; this->ry = _pos[3]; }
		int lx, ly, rx, ry;
	};

	/*静的定数*/
	static constexpr int WEAK_ENEMY_Y_OFFSET = 11;

	/*内部処理関数*/
	int ConvertColor(const std::vector<int> _color);

	/*メンバ変数*/
	int				 backgroundColor;		//
	int				 playerHPColor;			//
	int				 bossHPColor;			//
	int				 prevPlayerHPColor;		//
	int				 prevBossHPColor;		//
	int				 staminaColor;			//
	RangeNum		 playerHP;				//プレイヤーHP
	RangeNum		 playerStamina;			//プレイヤーHP
	RangeNum		 bossHP;				//ボスHP
	vector<RangeNum> weakEnemyHP;			//雑魚敵のHP
	Vec2d			 playerHPPosition;		//プレイヤーHP座標
	Vec2d			 bossHPPosition;		//ボスHP座標
	int				 barTable;				//バーテーブル
	int				 playerHPBar;			//HPバー
	int				 playerStaminaBar;		//スタミナバー
	int				 bossHPBar;				//ボスHP
	int				 bossHPBarFrame;		//ボスHPフレーム
};