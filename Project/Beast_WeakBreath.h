//=====================================================
// @brief 弱ブレスアクション
//=====================================================
#pragma once

class BehaviorTree;
class ActionNode;
class Character;
class AttackCapsuleColliderData;
class Beast_WeakBreath : public ActionNode
{
public:
	 Beast_WeakBreath();//コンストラクタ
	~Beast_WeakBreath();//デストラクタ

	void		Initialize	();													//初期化
	NodeState	Update		(BehaviorTree& _tree, Character& _chara)override;	//更新
	const void	Draw		()const;											//描画
private:
	/*定数*/
	const int FIX_ROTATE_COUNT;//回転補正数

	/*メンバ変数*/
	short						attackStartCount;				//攻撃開始フレーム
	short						attackEndCount;					//攻撃終了フレーム
	short						frameCount;						//フレームカウント
	short						frameIndexUsedCapsuleDirection1;//ブレスの方向を決めるためのフレーム番号
	short						frameIndexUsedCapsuleDirection2;//ブレスの方向を決めるためのフレーム番号
	AttackCapsuleColliderData*	collider;						//攻撃コライダー
	float						breathLength;					//ブレスの長さ
};

