#pragma once

class PlayerAttack;
class PlayerAttackManager
{
public:
	PlayerAttackManager ();//コンストラクタ
	~PlayerAttackManager();//デストラクタ

	void		Initialize();				 //初期化
	void		Finalize  ();				 //後処理
	void		Update	  ();				 //更新
	const void	Draw	  ()const;			 //描画
	void		OnIsStart ();//開始フラグを立てる
private:
	enum class AttackType
	{
		NONE = -1,
		MAIN_1 = 0,//詠唱
		MAIN_2 = 1,//詠唱
		SPECIAL = 2,//コンボ
	};
	PlayerAttack* attack;
	int hitNumber;
};

