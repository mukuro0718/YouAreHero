//=============================================
// @brief カメラマネージャークラス
//=============================================
#pragma once
#include "Singleton.h"

class Camera;
class CameraManager : public Singleton<CameraManager>
{
public:
	friend class Singleton<CameraManager>;

	void Initialize();		//初期化
	void Update	   ();		//更新
	const void Draw()const;	//描画

	/*getter*/
	const VECTOR GetNowCameraPosition()const;//現在のカメラの座標
	const VECTOR GetNowCameraTarget()const;//現在のカメラの注視点
	const VECTOR GetCameraDirection()const;//現在のカメラの注視点
private:
	/*内部処理関数*/
	~CameraManager();//デストラクタ
	 CameraManager();//コンストラクタ

	/*メンバ変数*/
	Camera* camera;
};

