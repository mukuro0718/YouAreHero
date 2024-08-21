//============================================
// @brief モデルクラス
// モデルを使用するときに使用するデータ型クラス
//============================================

#pragma once

class Transform;
class Animation;
class Model
{
public:
	 Model(const int _modelHandle);//コンストラクタ
	~Model();//デストラクタ

	const void Draw	  ()const;																				//描画
	void AddAnimation (const std::vector<string> _animationHandle, const std::vector<int> _animationIndex);	//アニメーションの追加
	void PlayAnimation(const int _nowAnimation, const float _animationPlayTime);							//アニメーションの再生

	/*getter*/
	const VECTOR GetPosition    ()const;								//座標の取得
	const VECTOR GetRotation    ()const;								//回転率の取得
	const VECTOR GetScale	    ()const;								//拡大率の取得
	const int	 GetModelHandle ()const { return this->modelHandle; }	//モデルハンドルの取得
	const bool	 GetIsChangeAnim()const;

	/*setter*/
	void SetPosition		 (const VECTOR	   _position );											//座標の設定
	void SetRotation		 (const VECTOR	   _rotation );											//回転率の設定
	void SetRotationOfRadian (const VECTOR	   _rotation);											//回転率の設定
	void SetScale			 (const VECTOR	   _scale	 );											//拡大率の設定
	void SetTransform		 (const VECTOR _position, const VECTOR _rotation, const VECTOR _scale);	//トランスフォームの設定

private:
	/*メンバ変数*/
	int			modelHandle;//モデルハンドル
	Transform*	transform;	//トランスフォーム
	Animation*	animation;	//アニメーション
};

