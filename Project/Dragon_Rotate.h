//=====================================================
// @brief 回転攻撃アクション
//=====================================================
#pragma once

class ActionNode;
class Dragon_Rotate : public ActionNode
{
public:
	 Dragon_Rotate();//コンストラクタ
	~Dragon_Rotate();//デストラクタ

	NodeState Update()override;//更新
private:
	/*定数*/
	const short USE_COLLIDER_INDEX;		//使用する攻撃の番号

	/*メンバ変数*/
	float			nowTotalPlayTime;	//現在の総再生時間
	short			currentDragonState;	//現在のボスの段階
	vector<float>	animationPlayTime;	//アニメーション再生時間
	short			frameCount;			//フレームカウント
};

