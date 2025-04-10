//===========================================
// @brief アクションノード
// ゲームキャラクターの実際のふるまいを書く
//===========================================
#pragma once

class BehaviorTree;
class BehaviorTreeNode;
class Character;
class ActionNode abstract : public BehaviorTreeNode
{
public:
			 ActionNode();//コンストラクタ
	virtual ~ActionNode(){}//デストラクタ

	virtual NodeState Update(BehaviorTree& _tree, Character& _chara) abstract;//更新
protected:
	static constexpr short BREATH_HIT_COUNT = 10;//ブレス攻撃のヒットフラグを再起的に立てるカウント

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

