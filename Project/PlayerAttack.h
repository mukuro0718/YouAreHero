//===========================================
// @brief プレイヤー攻撃
//===========================================
#pragma once

class ColliderData;
class PlayerAttack
{
public:
	PlayerAttack();//コンストラクタ
	~PlayerAttack();//デストラクタ

		  void Initialize	();													//初期化
		  void Finalize		();													//後処理
		  void Update		(const VECTOR _position, const VECTOR _direction);	//更新
	const void	Draw		()const;											//描画

	/*setter/getter*/
		  void OnIsStart	() { this->isStartHitCheck = true; this->isNotOnHit = false;}//当たり判定開始フラグを立てる
		  void SetDamage	(const float _damage);										 //ダメージの設定
	const bool GetIsStart	()const { return this->isStartHitCheck; }					 //当たり判定開始フラグを取得
private:


	/*メンバ変数*/
	ColliderData* collider;			//コライダー
	bool		  isStartHitCheck;	//ヒットチェック
	bool		  isNotOnHit;		//当たり判定フラグが立っているか
	int			  frameCount;		//フレームカウント
	//当たり判定のスフィアと座標はCollidableが持つため、発生タイミングを管理する
};

