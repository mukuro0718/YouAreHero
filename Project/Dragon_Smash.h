//=====================================================
// @brief 叩きつけアクション
//=====================================================
#pragma once

class BehaviorTree;
class ActionNode;
class Character;
class Dragon_Smash : public ActionNode
{
public:
	 Dragon_Smash();//コンストラクタ
	~Dragon_Smash();//デストラクタ

	NodeState Update(BehaviorTree& _tree, Character& _chara)override;//更新
private:
	/*定数*/
	const short USE_COLLIDER_INDEX;		//使用する攻撃の番号
	const short FIX_ROTATE_FRAME;		//回転補正を行うフレーム
	const short SMASH_CANCEL_PLAY_TIME;	//叩きつけ攻撃のキャンセル再生時間

	/*メンバ変数*/
	short frameCount;		 //フレームカウント
	float nowTotalPlayTime;	 //現在の総再生時間
	short currentDragonState;//現在のボスの段階
	vector<float> animationPlayTime;//アニメーション再生時間
};

