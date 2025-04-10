//==========================================
// @brief ダンジョン基底クラス
//==========================================
#pragma once

class ModelColliderData;
class Dungeon abstract
{
public:
			 Dungeon();//コンストラクタ
	virtual ~Dungeon();//デストラクタ

	virtual void	   Initialize() abstract;//初期化
	virtual void	   Update	 () abstract;//更新
	virtual const void Draw		 () const;	 //描画

	/*getter*/
	const int	GetModelHandle()const;
		  void	OffIsDoHitCheck();

protected:
	int				modelHandle;//モデルハンドル
	ModelColliderData*	collider;	//コライダー
};

