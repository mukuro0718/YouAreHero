//=====================================================
// @brief シーンベースクラス
// 各シーンで必ず実装したい関数を純粋仮想関数で実装
//=====================================================
#pragma once

class SceneBase abstract
{
public:
	SceneBase();			//コンストラクタ
	virtual ~SceneBase();	//デストラクタ

	/*純粋仮想関数（継承先で実装）*/
	virtual void		Initialize	() abstract;		//初期化
	virtual void		Finalize	() abstract;		//後処理
	virtual void		Update		() abstract;		//更新
	virtual const void	Draw		() const abstract;	//描画
};

