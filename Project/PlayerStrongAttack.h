//=============================================================
// @brief プレイヤー強攻撃アクションクラス
//=============================================================
#pragma once

class PlayerAction;
class AttackSphereColliderData;
class PlayerStrongAttack : public PlayerAction
{
public:
	 PlayerStrongAttack();	//コンストラクタ
	~PlayerStrongAttack();	//デストラクタ
	void Initialize	() override;				//初期化
	void Finalize	() override;				//後処理
	void Update		(Player& _player) override;	//更新
private:
	AttackSphereColliderData* collider;			 //コライダー
	float					  firstPlayTime;	 //最初のアニメーション再生時間	
	int						  damage;			 //ダメージ
	short					  cancelableFrame;	 //キャンセル可能フレーム
	int						  startHitCheckFrame;//当たり判定開始フレーム
	int						  endHitCheckFrame;	 //当たり判定終了フレーム
	float					  positionOffset;	 //座標オフセット
	float					  yOffset;			 //Yオフセット
	VECTOR					  firstDirection;	 //最初の向き
};

