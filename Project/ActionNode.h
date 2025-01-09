//===========================================
// @brief アクションノード
// ゲームキャラクターの実際のふるまいを書く
//===========================================
#pragma once

class BehaviorTreeNode;
class ActionNode abstract : public BehaviorTreeNode
{
public:
			 ActionNode();//コンストラクタ
	virtual ~ActionNode(){}//デストラクタ

	virtual NodeState Update() abstract;//更新
protected:
	short rotateAnimationType;	//回転用アニメーション（これは共通）
	short animationType;		//アニメーションの種類
	short actionType;			//アクションの種類
	short interval;				//インターバル
	float animationPlayTime;	//アニメーション再生時間
	float rotatePlayTime;		//回転アニメーション再生時間
	float maxSpeed;				//最大速度
	float accel;				//加速度
	float decel;				//減速度
	bool  isFixRotate;			//回転率の修正（アクション開始時にプレイヤーのほうを向くようにする）

};

