#pragma once

class Character;
class Rigidbody;
class BossAction;
class BossDeath : public BossAction
{
public:
			 BossDeath();//コンストラクタ
	virtual ~BossDeath();//デストラクタ

	void		Initialize	 ()																override;//初期化
	void		Finalize	 ()																override;//後処理
	Rigidbody&	Update		 (Character& _boss)												override;//更新
	void		CalcParameter(const int _hp, const int _angryValue, const float _distance)	override;//パラメータの計算
private:

};

