//====================================================
// @brief トランスフォームクラス
// 拡大率・回転率・座標を保持するデータ型クラス
//====================================================
#pragma once

class Transform
{
public:
	Transform();			//コンストラクタ
	virtual ~Transform(){}	//デストラクタ

	const float DegreeToRadian(const float _degree)const;//度数をラジアンに変換する
	
	/*getter*/
	const VECTOR GetPosition()const { return this->position; }//座標の取得
	const VECTOR GetRotation()const { return this->rotation; }//回転率の取得
	const VECTOR GetScale	()const { return this->scale;	 }//拡大率の取得

	/*setter*/
	void SetPosition		(const VECTOR _position ) { this->position  = _position; }//座標の設定
	void SetScale			(const VECTOR _scale	) { this->scale		= _scale;	 }//拡大率の設定
	void SetRotation		(const VECTOR _rotation);								  //回転率の設定
	void SetRotationOfRadian(const VECTOR _rotation);								  //回転率の設定
	void SetRotation		(const std::vector<float> _rotation);					  //回転率の設定
	void SetRotation		(const float _x, const float _y, const float _z);		  //回転率の設定

private:

	/*メンバ変数*/
	VECTOR position;//座標
	VECTOR rotation;//回転率
	VECTOR scale;	//拡大率
};

