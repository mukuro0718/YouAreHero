//================================================
// @brief ボスアクションの基底クラス
//================================================
#pragma once

class Character;
class Rigidbody;
class ActionParameter;
class BossAction abstract
{
public:
			 BossAction();	//コンストラクタ
	virtual ~BossAction(){}	//デストラクタ

	virtual void		Initialize	 ()																abstract;//初期化
	virtual void		Finalize	 ()																abstract;//後処理
	virtual Rigidbody&	Update		 (Character& _boss)												abstract;//更新
	virtual void		CalcParameter(const int _hp, const int _angryValue, const float _distance)	abstract;//パラメータの計算

	/*getter*/
	const int GetDesireValue() const;
protected:
	ActionParameter* parameter;//パラメータ
};

