//==========================================
// カメラクラス
//==========================================
#pragma once


class Camera
{
public:
	 Camera();//コンストラクタ
	~Camera();//デストラクタ

	void Initialize();		//初期化
	void Update();			//更新
	const void Draw()const;	//描画

	/*getter*/
	const VECTOR GetNowPosition() { return this->nowPosition; }//現在の座標
	const VECTOR GetNowTarget() { return this->nowTarget; }//現在の注視点
	const VECTOR GetDirection() { return this->direction; }//現在の注視点
private:
	/*内部処理関数*/
	void   UpdateDirection	();																//カメラの向きの更新
	void   UpdateAngle		();																//回転率の更新
	void   UpdateLength		();																//距離の更新
	void   UpdateTarget		();																//注視点の更新
	void   FixAngle			();																//回転値の修正
	void   UpdatePosition	();																//カメラ座標の更新
	void   UpdateFoV		();																//Fovの更新
	float  Lerp				(const float _start, const float _end, const float _percent);	//らーぷ関数
	VECTOR Lerp				(const VECTOR _start, const VECTOR _end, const VECTOR _percent);//らーぷ関数
	float  DegToRad			(const float _deg) { return _deg * (DX_PI_F / 180.0f); }		//度数をラジアンに変換

	/*メンバ変数*/
	VECTOR nextPosition;	//次の座標
	VECTOR nowPosition;		//今の座標
	VECTOR nextTarget;		//次の注視点
	VECTOR nowTarget;		//今の注視点
	VECTOR direction;		//カメラから注視点の方向
	VECTOR firstDirection;	//カメラの初期方向
	//HACK:注視点からlength分だけ離した位置にカメラの座標を設定するため、初期directionを反転させた値を保持する
	float  fov;				//field of view
	float  length;			//長さ
	float  horizonAngle;	//水平回転値
	float  verticalAngle;	//垂直回転値
};

