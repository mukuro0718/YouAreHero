//=====================================================
// @brief 右足溜め攻撃アクション
//=====================================================
#pragma once

class ActionNode;
class AttackCapsuleColliderData;
class Beast_ChargeRightFootAttack : public ActionNode
{
public:
	Beast_ChargeRightFootAttack();//コンストラクタ
	~Beast_ChargeRightFootAttack();//デストラクタ

	void		Initialize	();			//初期化
	NodeState	Update		()override;	//更新
	const void	Draw		()const;	//描画
private:
	short						attackStartCount;				//攻撃開始フレーム
	short						attackEndCount;					//攻撃終了フレーム
	short						frameCount;						//フレームカウント
	short						frameIndexUsedCapsuleDirection1;//ブレスの方向を決めるためのフレーム番号
	short						frameIndexUsedCapsuleDirection2;//ブレスの方向を決めるためのフレーム番号
	AttackCapsuleColliderData*  collider;						//攻撃コライダー
};

