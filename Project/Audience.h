//======================================================
// @brief 観客クラス
//======================================================

#pragma once
class Audience
{
public:
	Audience();		//コンストラクタ
	~Audience();		//デストラクタ
	void Initialize();//初期化
	void Update();
	void Draw();	//描画
private:
	/*静的定数*/
	static constexpr int VERTEX_NUM = 4;
	static constexpr int INDEX_NUM = 6;
	static constexpr int POLYGON_NUM = 2;

	/*内部処理関数*/
	int		ConvertColor(const std::vector<int> _color);
	VECTOR	Convert		(const std::vector<int> _in);
	/*メンバ変数*/
	VERTEX3D vertex[VERTEX_NUM];
	WORD index[INDEX_NUM];
	int imageHandle;
};
