//================================================
// @brief ボスアクションの基底クラス
//================================================
#pragma once

class Boss;
class ActionParameter;
class BossAttack;
class HitStop;
class BossAction abstract
{
public:
			 BossAction();	//コンストラクタ
	virtual ~BossAction(){}	//デストラクタ
	virtual void Initialize	  ()					abstract;//初期化
	virtual void Update		  (Boss& _boss)			abstract;//更新
	virtual void CalcParameter(const Boss& _boss)	abstract;//パラメーターの計算
	const void Draw()const;

	/*getter/setter*/
	const int  GetDesireValue()	const;
	const bool GetIsSelect	 ()	const { return this->isSelect; }
		  void OnIsSelect	 () { this->isSelect = true; }
	const int  GetWeight	 (const int _sum)const;
	const int GetBaseWeight	 ()const;
	const bool GetIsPriority ()const { return this->isPriority; }
protected:
	/*内部処理関数*/
	bool FrameCount(const int _maxFrame);
	void OffIsSelect(const int _maxInterval);

	/*メンバ変数*/
	BossAttack*				 attack;		//ボス攻撃クラス
	HitStop*				 hitStop;		//ヒットストップ
	ActionParameter*		 parameter;		//パラメータ
	bool					 isSelect;		//アクションが選択されたか
	int						 frameCount;	//フレームカウント
	bool					 isInitialize;	//初期化されたか
	bool					 isPriority;
};

