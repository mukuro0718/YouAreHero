//=============================================================
// @brief 基本ステートクラス
//=============================================================
#pragma once
class State abstract
{
public:
			 State(){}//コンストラクタ
	virtual ~State(){}//デストラクタ

	virtual void Entry () abstract;//登録
	virtual void Update() abstract;//更新
	virtual void Exit  () abstract;//解除
	virtual int  GetId () abstract;//IDの取得
};

