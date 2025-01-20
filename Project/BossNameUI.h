//===================================================
// @brief ボスの名前を表示するクラス
// エントリー時にボスのHPを表示し、
// その後、ボスのHPの上に名前を表示する
//===================================================
#pragma once

class Image;
class BossNameUI
{
public:
	BossNameUI();//コンストラクタ
	~BossNameUI();//デストラクタ

		  void Initialize();	 //初期化
		  void Update	 ();	 //更新
	const void Draw		 ()const;//描画
private:
	/*静的定数*/
	static constexpr int TEXT_COLOR = 0xffffff;

	/*メンバ変数*/
	int			fontHandle;			//フォント
	int			imageHandle;		//背景
	vector<int> namePosition;		//名前の座標
	vector<int> backgroundDrawRect;	//背景の描画範囲
	string		bossName;			//ボスの名前
};

