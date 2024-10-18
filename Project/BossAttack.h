//===========================================
// @brief ボス攻撃
//===========================================
#pragma once

class ColliderData;
class BossAttack abstract
{
public:
	BossAttack();//コンストラクタ
	virtual ~BossAttack();//デストラクタ

	virtual void		Initialize () abstract;		//初期化
	virtual void		Finalize   ();				//後処理
	virtual void		Update	   () abstract;		//更新
	virtual const void	Draw	   () const abstract;//描画

	/*getter/setter*/
	void OnIsStart() { this->isStartHitCheck = true; this->isNotOnHit = false; }	//当たり判定開始フラグを立てる
	const VECTOR GetPosition() const;					//座標の取得
	const bool GetIsHitAttack();
	void OffIsHitAttack();

	/*攻撃の種類*/
	enum class AttackType
	{
		NONE			= -1,
		SLASH_1			= 0, //スラッシュ
		SLASH_2			= 1, //スラッシュ
		STAB			= 2, //突き刺し攻撃
		ROTATE_SLASH	= 3, //回転スラッシュ
		PUNCH			= 4, //パンチ
		SLASH_COMBO_1	= 5, //スラッシュコンボ１
		SLASH_COMBO_2	= 6, //スラッシュコンボ２
		JUMP_ATTACK		= 7, //ジャンプアタック
	};
protected:
	/*内部処理関数*/
	const VECTOR Convert(std::vector<float> _in);
	/*メンバ変数*/
	ColliderData* collider;			//コライダー
	int			  frameCount;		//フレームカウント
	int			  attackIndex;		//攻撃番号
	bool		  isNotOnHit;		//処理中に当たり判定がオンになったか
	bool		  isStartHitCheck;	//当たり判定を行うか
	//当たり判定のスフィアと座標はCollidableが持つため、発生タイミングを管理する
};
