//============================================================
// @brief アクション用UIクラス
//============================================================
#pragma once
class Action
{
public:
	Action();//コンストラクタ
	~Action();//デストラクタ

	const void Draw()const;//描画
private:
	/*静的定数*/
	static constexpr int ELEMENT_NUM = 5;

	/*構造体*/
	struct Vector2
	{
		int x, y;
	};

	/*メンバ変数*/
	int elementImageHandle[ELEMENT_NUM];//属性アイコン画像ハンドル
};

