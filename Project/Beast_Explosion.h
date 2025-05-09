//=====================================================
// @brief 爆発攻撃
//=====================================================
#pragma once

class BehaviorTree;
class ActionNode;
class Character;
class AttackSphereColliderData;
class Beast_Explosion : public ActionNode
{
public:
	 Beast_Explosion();//コンストラクタ
	~Beast_Explosion();//デストラクタ

	void		Initialize	();													//初期化
	NodeState	Update		(BehaviorTree& _tree, Character& _chara)override;	//更新
	const void	Draw		()const;											//描画
private:
	short						roarSoundStartCount;			//咆哮の音開始フレーム
	short						effectStartCount;				//エフェクト開始フレーム
	short						attackStartCount;				//攻撃開始フレーム
	short						attackEndCount;					//攻撃終了フレーム
	short						frameCount;						//フレームカウント
	short						frameIndexUsedSpherePosition;	//開く初の中心を決めるためのフレーム番号
	AttackSphereColliderData*	collider;						//攻撃コライダー
};

