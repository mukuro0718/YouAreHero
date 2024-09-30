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
	/*列挙体*/
	enum class PositionType
	{
		ENTRY,//登場時
		NORMAL,//通常時
	};

	/*内部処理関数*/
	void AddTextIndex();
	void ChangePositionType();

	/*静的定数*/
	static constexpr int TEXT_COLOR = 0xdfffff;

	/*メンバ変数*/
	vector<int> fontHandle;
	int positionType;//座標の種類
	int drawTextIndex;//描画する文字の配列
	int addTextIndexInterval;
	int entryInterval;
};

