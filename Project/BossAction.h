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
	virtual ~BossAction();	//デストラクタ
	virtual void Initialize	  ()					abstract;//初期化
	virtual void Update		  (Boss& _boss)			abstract;//更新
	virtual void CalcParameter(const Boss& _boss)	abstract;//パラメーターの計算
	const void Draw()const;

	/*getter/setter*/
	const int  GetWeight	 (const int _sum)const;
	const int  GetDesireValue()	const;
	const int  GetBaseWeight ()	const;
	const bool GetIsSelect	 () const { return this->isSelect; }
	const bool GetIsPriority () const { return this->isPriority; }
		  void OnIsSelect	 (){ this->isSelect = true; }
protected:
	/*列挙体*/
	/*内部処理関数*/
		  bool		FrameCount		(const int _maxFrame);																						//フレームカウント
		  void		OffIsSelect		(const int _maxInterval);																					//選択フラグの初期化
		  VECTOR	GetLerpRotation (Boss& _boss, const VECTOR _positionToTargetVector, const VECTOR _nowRotation, const VECTOR _lerpValue);	//補完した回転率の取得
		  VECTOR	CalcVelocity	(const VECTOR _prevVelocity, const VECTOR _nowRotation, const float _speed);								//移動ベクトルの計算
		  float		Lerp			(const float _start, const float _end, const float _percent);												//らーぷ関数
		  COLOR_F	LerpColor		(const COLOR_F _start, const COLOR_F _end, const COLOR_F _percent);											//らーぷ関数
	const COLOR_F	ColorConvert	(const std::vector<float> _value);																			//vector<float>をCOLOR_Fに変換

	/*メンバ変数*/
	BossAttack*				attack;				 //ボス攻撃クラス
	HitStop*				hitStop;			 //ヒットストップ
	ActionParameter*		parameter;			 //パラメータ
	VECTOR					moveTarget;			 //移動目標
	bool					isSelect;			 //アクションが選択されたか
	short					frameCount;			 //フレームカウント
	bool					isInitialize;		 //初期化されたか
	bool					isPriority;			 //優先フラグ
	bool					isAllowAction;		 //アクションを許可する
	int						frameTime;			 //フレーム時間
	short					nextAnimation;		 //次のアニメーション
	float					animationPlayTime;	 //アニメーション再生時間
	short					maxDesireValue;		 //最大欲求値
	float					nowTotalAnimPlayTime;//現在の総再生時間
	short					interval;			 //インターバル
};

