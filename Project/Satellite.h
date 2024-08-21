//================================================
// @brief 衛星クラス
// プレイヤーの周りをまわる
//================================================
#pragma once

class Model;
class Satellite
{
public:
	 Satellite();//コンストラクタ
	~Satellite();																						 //デストラクタ

	void		Initialize(const float _angleOffset,const VECTOR _targetPosition, const VECTOR _positionOffset);	//初期化
	void		Update	  (const bool _isLeft, const VECTOR _targetDirection, const VECTOR _targetPosition);//更新
	const void	Draw	  ()const;														//描画
private:
	Model* model;			//モデル
	VECTOR rotationAxis;	//回転軸
	VECTOR positionOffset;	//座標補正値
	VECTOR targetDirection;
	float  angle;			//回転値
	bool isReverse;
};

