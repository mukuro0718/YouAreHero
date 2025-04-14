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

			void		Initialize	();			//初期化
	virtual void		Update		() abstract;//更新
	virtual const void	Draw		() const;	//描画
	
	/*setter*/
	void OnIsPlayEffect	() { this->isPlayEffect = true; }	//再生フラグを立てる
	void SetPosition(const VECTOR _position);
	void SetRotation(const VECTOR _rotation);
protected:
	/*メンバ変数*/
	Transform*	transform;			 //トランスフォーム
	bool		isPlayEffect;		 //エフェクトを再生するか
	int			effectResourceHandle;//エフェクトリソースハンドル
	int			frameCount;			 //再生フレームカウント
	int			startFrame;			 //再生開始フレーム
	int			endFrame;			 //再生終了フレーム
};

