//=====================================================
// @brief ブレスアクション
//=====================================================
#pragma once

class ActionNode;
class Dragon_Breath : public ActionNode
{
public:
	 Dragon_Breath();//コンストラクタ
	~Dragon_Breath();//デストラクタ

	NodeState Update()override;//更新
private:
	/*定数*/
	const short USE_COLLIDER_INDEX;		//使用する攻撃の番号
	const short FIX_ROTATE_FRAME;		//回転補正を行うフレーム

	/*メンバ変数*/
	short frameCount;		 //フレームカウント
	float nowTotalPlayTime;	 //現在の総再生時間
	short currentDragonStage;//現在のボスの段階
	vector<float> animationPlayTime;//アニメーション再生時間
};

