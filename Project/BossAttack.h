//===========================================
// @brief ボス攻撃
//===========================================
#pragma once

namespace GoriLib
{
	class Physics;
	class Collidable;
	class ColliderData;
	class BossAttack abstract : public GoriLib::Collidable
	{
	public:
		BossAttack(Priority _priority, GameObjectTag _tag, ColliderData::Kind _colliderKind, bool _isTrigger);//コンストラクタ
		~BossAttack();//デストラクタ

		virtual void	Initialize(GoriLib::Physics* _physics) abstract;//初期化
		virtual void	Finalize(GoriLib::Physics* _physics);//後処理
		virtual void	Update(GoriLib::Physics* _physics) abstract;//更新
		virtual void	OnCollide(const Collidable& _colider) override;//衝突したとき
		virtual const void	Draw()const						 abstract;//描画
		void	OnIsStart() { this->isStartHitCheck = true; }	  //当たり判定開始フラグを立てる
		const	VECTOR	GetPosition()const;							  //座標の取得
	protected:
		/*内部処理関数*/
		virtual void		SetRadius(const float _radius) abstract;//半径のセット
		virtual const float GetRadius() const			   abstract;//半径の取得
		const VECTOR Convert(std::vector<float> _in);

		/*メンバ変数*/
		int		frameCount;			 //フレームカウント
		int		damage;				 //ダメージ
		int		attackIndex;		 //攻撃番号
		float	stackSpeed;			 //蓄積したスピード
		VECTOR	basePosition;		 //元の座標
		VECTOR	direction;			 //方向
		bool	isStartHitCheck;	 //当たり判定を行うか
		bool	isDontStartPrevFrame;//前フレームで当たり判定が行われていたか
		//当たり判定のスフィアと座標はCollidableが持つため、発生タイミングを管理する
	};
}
