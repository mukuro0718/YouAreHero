//=====================================================
// @brief 咆哮アクション
// 通常→怒りへの状態変化を行うときの咆哮
//=====================================================
#pragma once

class ActionNode;
class AttackCapsuleColliderData;
class Beast_FootComboAttack : public ActionNode
{
public:
	 Beast_FootComboAttack();//コンストラクタ
	~Beast_FootComboAttack();//デストラクタ

	void		Initialize	();			//初期化
	NodeState	Update		()override;	//更新
	const void	Draw		()const;	//描画
private:
	vector<short>				attackStartCount;				//攻撃開始フレーム
	vector<short>				attackEndCount;					//攻撃終了フレーム
	vector<short>				moveStartCount;					//攻撃開始フレーム
	vector<short>				moveEndCount;					//攻撃終了フレーム
	short						frameCount;						//フレームカウント
	short						maxAttackCount;					//攻撃の最大回数
	short						attackCount;					//攻撃の回数
	vector<short>				frameIndexUsedCapsuleDirection1;//ブレスの方向を決めるためのフレーム番号
	vector<short>				frameIndexUsedCapsuleDirection2;//ブレスの方向を決めるためのフレーム番号
	AttackCapsuleColliderData*  collider;						//攻撃コライダー
};

