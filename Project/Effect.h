//========================================
// @brief エフェクトベースクラス
//========================================
#pragma once

class Transform;
class Effect abstract
{
public:
	Effect(const int _effectResourceHandle);//コンストラクタ
	virtual ~Effect();//デストラクタ

		  void Initialize	 ();		//初期化
		  void Update		 ();		//更新
	const void Draw			 ()const;	//描画
	
	/*setter*/
	void OnIsPlayEffect	() { this->isPlayEffect = true; }	//再生フラグを立てる
	void SetPosition	(const VECTOR _position);			//座標の設定
	/*純粋仮想関数*/
	virtual void SetInfoToPlayTheEffect() abstract;//エフェクトの再生に必要な情報の設定
protected:
	/*メンバ変数*/
	Transform*	transform;			 //トランスフォーム
	vector<int> playingEffectHandle; //再生エフェクトハンドル
	bool		isPlayEffect;		 //エフェクトを再生するか
	int			effectResourceHandle;//エフェクトリソースハンドル
	int			frameCount;			 //再生フレームカウント
	int			startFrame;			 //再生開始フレーム
	int			endFrame;			 //再生終了フレーム
	int			firstFrame;			 //最初のフレーム
};

