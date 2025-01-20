//=====================================================
// @brief 後ろに下がりながらブレスアクション
//=====================================================
#pragma once

class ActionNode;
class AttackCapsuleColliderData;
class Beast_BackingBreath : public ActionNode
{
public:
	 Beast_BackingBreath();//コンストラクタ
	~Beast_BackingBreath();//デストラクタ

	void		Initialize	();			//初期化
	NodeState	Update		()override;	//更新
	const void	Draw		()const;	//描画
private:
	short						attackStartCount;				//攻撃開始フレーム
	short						frameCount;						//フレームカウント
	short						frameIndexUsedCapsuleDirection1;//ブレスの方向を決めるためのフレーム番号
	short						frameIndexUsedCapsuleDirection2;//ブレスの方向を決めるためのフレーム番号
	AttackCapsuleColliderData*	collider;						//攻撃コライダー
	float						breathLength;					//ブレスの長さ
};

