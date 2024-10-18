#pragma once

class BossAttack;
class BossAttackManager
{
public:
	BossAttackManager();//コンストラクタ
	~BossAttackManager();//デストラクタ

	void		 Initialize	();					//初期化
	void		 Finalize	();					//後処理
	void		 Update		();					//更新
	const void	 Draw		()const;			//描画
	void		 OnIsStart	(const int _index);	//開始フラグを立てる
	const VECTOR GetPosition(const int _index);	//座標の取得
private:
	std::vector<BossAttack*> attack;
	int hitNumber;
};

