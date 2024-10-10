//========================================
// @brief 影クラス
// プレイヤーとエネミーに対して丸影の描画を行う
//========================================
#pragma once
#include "Singleton.h"

class Shadow : public Singleton<Shadow>
{
public:
	friend class Singleton<Shadow>;

	void Final();//最終処理
	void Draw(const int _stageModelHandle, const VECTOR _targetPos, const float _shadowHeight, const float _shadowSize);//影を描画
private:
	/*内部処理関数*/
			 Shadow();	//コンストラクタ
	virtual ~Shadow();	//デストラクタ

	/*メンバ変数*/
	int imageHandle;//影画像ハンドル
};

