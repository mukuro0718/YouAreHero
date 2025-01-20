//==========================================
// カメラクラス
//==========================================
#pragma once

class ColliderData;
class Rigidbody;
class Camera
{
public:
	 Camera();//コンストラクタ
	~Camera();//デストラクタ

		  void Initialize();		//初期化
		  void Update	 ();		//更新
	const void Draw		 ()const;	//描画
	static void InitializeStaticConst();

	/*getter*/
	const VECTOR GetNowPosition	()const;						//現在の座標（使用していない）
	const VECTOR GetNowTarget	() { return this->nowTarget; }	//現在の注視点（使用していない）
	const VECTOR GetDirection	() { return this->direction; }	//現在の注視点（プレイヤーをTPSで操作するときのみ必要）
private:
	/*定数*/
	const int BOSS_HEAD_FRAME_INDEX;

	/*内部処理関数*/
	void   UpdateDirection	();																//向きの更新
	void   UpdateAngle		();																//座標の更新
	void   UpdateVelocity	();																//移動ベクトル
	void   UpdateTarget		();																//注視点の更新
	void   UpdateLength		();																//座標の修正
	void   UpdateFoV		();																//Fovの更新
	float  Lerp				(const float _start, const float _end, const float _percent);	//らーぷ関数
	VECTOR Lerp				(const VECTOR _start, const VECTOR _end, const VECTOR _percent);//らーぷ関数
	float  DegToRad			(const float _deg) { return _deg * (DX_PI_F / 180.0f); }		//度数をラジアンに変換

	/*メンバ変数*/
	ColliderData*	collider;				//コライダー
	VECTOR			nextTarget;				//次の注視点
	VECTOR			nowTarget;				//今の注視点
	VECTOR			direction;				//カメラから注視点の方向
	float			fov;					//field of view
	float			length;					//長さ
	float			yow;					//ヨー（横回転）
	float			pitch;					//ピッチ（縦回転）
	//TODO:以下の変数はJsonのデータを格納する変数であり、出来れば定数にすべきものである
	float			nearClip;				//手前クリップ距離
	float			farClip;				//奥クリップ距離
	float			firstLength;			//
	float			firstAngle;				//
	VECTOR			firstDirection;			//
	VECTOR			targetOffset;			//
	VECTOR			firstPosition;			//
	VECTOR			titleTarget;			//
	VECTOR			lerpValueForTarget;		//
	VECTOR			lerpValueForVelocity;	//
	float			addAngle;				//
	float			maxYow;					//
	float			minYow;					//
	float			maxPitch;				//
	float			minPitch;				//
	float			titleMaxLength;			//
	float			titleMinLength;			//
	float			deathMaxLength;			//	
	float			deathMinLength;			//
	float			maxLength;				//
	float			minLength;				//
	float			lerpValueForLength;		//
	float			titlePositionOffset;	//
	VECTOR			positionOffset;			//
	//HACK:注視点からlength分だけ離した位置にカメラの座標を設定するため、初期directionを反転させた値を保持する
};
