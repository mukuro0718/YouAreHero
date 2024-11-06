//===============================================
// @brief プレイヤー攻撃マネージャークラス
//===============================================
#pragma once

class PlayerAttack;
class PlayerAttackManager
{
public:
	 PlayerAttackManager();//コンストラクタ
	~PlayerAttackManager();//デストラクタ

	void		Initialize();					//初期化
	void		Finalize  ();					//後処理
	void		Update	  ();					//更新
	const void	Draw	  ()const;				//描画

	/*setter*/
	void OnIsStart ();					 //開始フラグを立てる
	void SetDamage (const float _damage);//ダメージのセット
private:
	/*メンバ変数*/
	PlayerAttack* attack;//攻撃クラス
};

