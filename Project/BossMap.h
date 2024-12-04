//===========================================
// @brief 地面クラス
// 地面の描画を行う
//===========================================
#pragma once

class ColliderData;
class BossMap
{
public:
	BossMap();//コンストラクタ
	~BossMap();//デストラクタ

		  void	Initialize		();		//初期化
		  void	Finalize		();		//後処理
		  void	Update			();		//更新
	const void	Draw			()const;//描画

	/*getter*/
	const int	GetModelHandle()const;

private:
	/*メンバ変数*/
	ColliderData* collider;
	int modelHandle;
};

