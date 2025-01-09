//=====================================================
// @brief 爆発攻撃
//=====================================================
#pragma once

class ActionNode;
class AttackSphereColliderData;
class Beast_Explosion : public ActionNode
{
public:
	 Beast_Explosion();//コンストラクタ
	~Beast_Explosion();//デストラクタ

	NodeState Update()override;//更新
	const void Draw()const;	//描画
private:
	short						effectStartCount;				//エフェクト開始フレーム
	short						attackStartCount;				//攻撃開始フレーム
	short						attackEndCount;					//攻撃終了フレーム
	short						frameCount;						//フレームカウント
	short						frameIndexUsedSpherePosition;	//開く初の中心を決めるためのフレーム番号
	AttackSphereColliderData*	collider;						//攻撃コライダー
};

