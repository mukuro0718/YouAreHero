//=====================================================
// @brief 弱ブレスアクション
//=====================================================
#pragma once

class ActionNode;
class AttackCapsuleColliderData;
class Beast_WeakBreath : public ActionNode
{
public:
	 Beast_WeakBreath();//コンストラクタ
	~Beast_WeakBreath();//デストラクタ

	void		Initialize	();			//初期化
	NodeState	Update		()override;	//更新
	const void	Draw		()const;	//描画
private:
	short						attackStartCount;				//攻撃開始フレーム
	short						attackEndCount;					//攻撃終了フレーム
	short						frameCount;						//フレームカウント
	short						frameIndexUsedCapsuleDirection1;//ブレスの方向を決めるためのフレーム番号
	short						frameIndexUsedCapsuleDirection2;//ブレスの方向を決めるためのフレーム番号
	AttackCapsuleColliderData*	collider;						//攻撃コライダー
	float						breathLength;					//ブレスの長さ
};

