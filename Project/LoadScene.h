//=============================================
//ロード画面クラス
// 5つの頂点を保持し、DrawLineで星を描画する
// また頂点の位置を移動させ、動いていることを表す。
//=============================================
#pragma once
#include "SceneBase.h"

class LoadScene : public SceneBase
{
public:
	LoadScene();//コンストラクタ
	~LoadScene();//デストラクタ

	void		Initialize	() override;		//初期化
	void		Finalize	() override;		//最終処理
	void		Update		() override;		//更新
	const void	Draw		() const override;	//描画
private:
	/*静的定数*/
	static constexpr int MAX_ALPHA  = 255;	//最大アルファ
	static constexpr int MIN_ALPHA  = 0;	//最小アルファ
	static constexpr int ADD_ALPHA  = 1;	//アルファの増加値
	static constexpr int TEXT_POS_X = 1620;	//文字を描画するX座標
	static constexpr int TEXT_POS_Y = 980;	//文字を描画するY座標
	static constexpr int FONT_SIZE  = 32;	//フォントのサイズ
	static const	 int FONT_COLOR;		//フォントの色

	/*内部処理関数*/
	void AddAlpha	();//アルファの追加
	void RemoveAlpha();//アルファの削除

	/*メンバ変数*/
	int  alpha;		//アルファ
	bool isAddAlpha;//値が増加しているか(TRUE:追加 FALSE:削除 )
};

