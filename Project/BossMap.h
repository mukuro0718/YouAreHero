//===========================================
// @brief 地面クラス
// 地面の描画を行う
//===========================================
#pragma once

class Model;
class BossMap
{
public:
	BossMap();//コンストラクタ
	~BossMap();//デストラクタ

	void Initialize();		//初期化
	void Update();			//更新
	const void Draw()const;	//描画

private:
	/*メンバ変数*/
	Model* model;
};

