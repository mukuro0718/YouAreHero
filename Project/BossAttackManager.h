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
	//攻撃の種類
	enum class AttackType
	{
		NONE			= -1,
		SLASH			= 0,//パンチ
		STAB			= 1,//突き刺し攻撃
		ROTATE_PUNCH	= 2,//回転パンチ
		SLAP			= 3,
		MELEE			= 4,
		KICK			= 5,
		MELEE_COMBO_3	= 6,
		SLASH_COMBO_2	= 7
	};
private:
	std::vector<BossAttack*> attack;
	int hitNumber;
};

