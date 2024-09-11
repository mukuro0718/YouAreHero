#include <DxLib.h>
#include "UseJson.h"
#include "UseSTL.h"
#include "VECTORtoUseful.h"
#include "Rigidbody.h"
#include "Camera.h"
#include "PlayerManager.h"
#include "EnemyManager.h"
#include "InputManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
Camera::Camera()
	: nextPosition	 { 0.0f,0.0f,0.0f }
	, firstDirection { 0.0f,0.0f,0.0f }
	, nowPosition	 { 0.0f,0.0f,0.0f }
	, nextTarget	 { 0.0f,0.0f,0.0f }
	, nowTarget		 { 0.0f,0.0f,0.0f }
	, direction		 { 0.0f,0.0f,0.0f }
	, fov			 (0.0f)
	, length		 (0.0f)
	, horizonAngle	 (0.0f)
	, verticalAngle	 (0.0f)
{

}

/// <summary>
/// デストラクタ
/// </summary>
Camera::~Camera()
{

}

/// <summary>
/// 初期化
/// </summary>
void Camera::Initialize()
{
	/*シングルトンクラスのインスタンスを取得*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& player = Singleton<PlayerManager>::GetInstance();

	/*使用するjsonデータを定数型に代入*/
	const  float  NEAR_CLIP		  = json.GetJson(JsonManager::FileType::CAMERA)["NEAR"];				  //手前クリップ距離
	const  float  FAR_CLIP		  = json.GetJson(JsonManager::FileType::CAMERA)["FAR"];					  //奥クリップ距離
	const  float  FOV			  = json.GetJson(JsonManager::FileType::CAMERA)["FOV"];					  //field of view
	const  float  FIRST_LENGTH	  = json.GetJson(JsonManager::FileType::CAMERA)["FIRST_LENGTH"];					  //奥クリップ距離
	const  float  FIRST_ANGLE	  = json.GetJson(JsonManager::FileType::CAMERA)["FIRST_ANGLE"];//カメラ座標オフセット
	vector<float> FIRST_DIRECTION = json.GetJson(JsonManager::FileType::CAMERA)["FIRST_DIRECTION"];//カメラ座標オフセット
	vector<float> TARGET_OFFSET   = json.GetJson(JsonManager::FileType::CAMERA)["TARGET_OFFSET"];//カメラ座標オフセット

	/*メンバ変数の初期化*/
	this->nowTarget		  = player.GetRigidbody().GetPosition() + Convert(TARGET_OFFSET);		//注視点
	this->nextTarget	  = this->nowTarget;										//注視点
	this->direction		  = Convert(FIRST_DIRECTION);							//カメラの向き
	this->length		  = FIRST_LENGTH;											//注視点からの距離
	this->firstDirection  = this->direction * -1.0f;								//反転させたdirection
	this->nowPosition	  = this->nowTarget + (this->firstDirection * this->length);//カメラ座標
	this->nextPosition	  = this->nowPosition;										//カメラ座標
	this->fov			  = FOV;													//field of view
	this->horizonAngle	  = FIRST_ANGLE;											//水平方向回転値
	this->verticalAngle	  = FIRST_ANGLE;											//垂直方向回転値

	/*カメラの手前クリップ距離と奥クリップ距離を設定する*/
	SetCameraNearFar(NEAR_CLIP, FAR_CLIP);

	/*fovを設定する*/
	SetupCamera_Perspective(this->DegToRad(this->fov));
}

/// <summary>
/// 更新
/// </summary>
void Camera::Update()
{

	/*注視点の更新*/
	UpdateTarget();

	/*回転値の更新*/
	UpdateAngle();

	FixAngle();

	/*長さ（距離）の更新*/
	UpdateLength();

	/*FoVの更新*/
	UpdateFoV();

	/*カメラの向きの更新*/
	UpdateDirection();
	
	/*座標の更新*/
	this->nowPosition = this->nowTarget + (this->direction * this->length);
	
	/*カメラの座標、注視点を設定する（上方向はＹ軸から算出）*/
	SetCameraPositionAndTarget_UpVecY(this->nowPosition, this->nowTarget);
}

/// <summary>
/// カメラ座標の更新
/// </summary>
void Camera::UpdatePosition()
{

}

/// <summary>
/// 描画
/// </summary>
const void Camera::Draw()const
{
	//printfDx("CAMERA_POSITION X:%f,Y:%f,Z:%f\n", this->nowPosition.x, this->nowPosition.y, this->nowPosition.z);
	//printfDx("CAMERA_TARGET X:%f,Y:%f,Z:%f\n", this->nowTarget.x, this->nowTarget.y, this->nowTarget.z);
}

/// <summary>
/// 注視点の更新
/// </summary>
void Camera::UpdateTarget()
{
	/*シングルトンクラスのインスタンスを取得*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& player = Singleton<PlayerManager>::GetInstance();

	/*プレイヤーの座標に補正値を足して注視点とする*/
	this->nextTarget = player.GetRigidbody().GetPosition() + Convert(json.GetJson(JsonManager::FileType::CAMERA)["TARGET_OFFSET"]);

	this->nowTarget = Lerp(this->nowTarget, this->nextTarget, VGet(0.3f, 0.3f, 0.3f));

}

/// <summary>
/// カメラ回転値の更新
/// </summary>
void Camera::UpdateAngle()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& input = Singleton<InputManager>::GetInstance();//インプットマネージャー
	auto& json  = Singleton<JsonManager>::GetInstance();//json
	auto& player = Singleton<PlayerManager>::GetInstance();

	/*変数の準備*/
	const int	X_BUF			= input.GetRStickState().XBuf;										  //右スティック入力
	const int	Y_BUF			= input.GetRStickState().YBuf;										  //右スティック入力
	const float KEY_BUF			= json.GetJson(JsonManager::FileType::CAMERA)["KEY_INPUT_BUF"]; //キー入力
	const float ANGLE_MULTIPLY	= json.GetJson(JsonManager::FileType::CAMERA)["ANGLE_MULTIPLY"];//アングル倍率
		  float horizonAngle	= 0.0f;																  //水平回転値
		  float verticalAngle	= 0.0f;																  //垂直回転値
		  float	addAngle		= 0.0f;
		addAngle = json.GetJson(JsonManager::FileType::CAMERA)["ADD_ANGLE"];	//アングル増加量
	/*もしスティック入力もキー入力もなければreturnを返す*/
	if (Y_BUF == 0 && X_BUF == 0 && CheckHitKeyAll(DX_CHECKINPUT_KEY) == 0)return;
	
	/*Y+:down Y-:up X+:right X-:left*/
	/*左右のスティック入力またはがあれば*/
	if (X_BUF != 0 || CheckHitKey(KEY_INPUT_RIGHT) != 0 || CheckHitKey(KEY_INPUT_LEFT) != 0)
	{
		//スティック入力がなければ定数を入れる
		if (X_BUF == 0)
		{
			horizonAngle = KEY_BUF;
			if (CheckHitKey(KEY_INPUT_LEFT))
			{
				horizonAngle *= -1.0f;
			}
		}
		//スティック入力があればその値を入れる
		else
		{
			horizonAngle = static_cast<float>(X_BUF);
		}
		this->horizonAngle += horizonAngle * ANGLE_MULTIPLY;
	}

	/*上下の入力があれば*/
	if (Y_BUF != 0 || CheckHitKey(KEY_INPUT_DOWN) != 0 || CheckHitKey(KEY_INPUT_UP) != 0)
	{
		//スティック入力がなければ定数を入れる
		if (Y_BUF == 0)
		{
			verticalAngle = KEY_BUF;
			if (CheckHitKey(KEY_INPUT_UP))
			{
				verticalAngle *= -1.0f;
			}
		}
		//スティック入力があればその値を入れる
		else
		{
			verticalAngle = static_cast<float>(Y_BUF);
		}
		this->verticalAngle += verticalAngle * ANGLE_MULTIPLY;
	}
}

/// <summary>
/// 回転値の補正
/// </summary>
void Camera::FixAngle()
{
	/*シングルトンクラスのインスタンスを取得*/
	auto& json = Singleton<JsonManager>::GetInstance();//json

	/*使用する変数の準備*/
	const float MAX_HORIZON_ANGLE  = json.GetJson(JsonManager::FileType::CAMERA)["MAX_HORIZON_ANGLE"]; //注視点までの最大長さ
	const float MIN_HORIZON_ANGLE  = json.GetJson(JsonManager::FileType::CAMERA)["MIX_HORIZON_ANGLE"]; //注視点までの最小長さ
	const float MAX_VERTICAL_ANGLE = json.GetJson(JsonManager::FileType::CAMERA)["MAX_VERTICAL_ANGLE"];//注視点までの最大長さ
	const float MIN_VERTICAL_ANGLE = json.GetJson(JsonManager::FileType::CAMERA)["MIX_VERTICAL_ANGLE"];//注視点までの最小長さ

	/*アングルを補正する*/
	if (this->horizonAngle > MAX_HORIZON_ANGLE || this->horizonAngle < MIN_HORIZON_ANGLE)
	{
		this->horizonAngle = 0.0f;
	}
	if (this->verticalAngle > MAX_VERTICAL_ANGLE)
	{
		this->verticalAngle = MAX_VERTICAL_ANGLE;
	}
	else if (this->verticalAngle < MIN_VERTICAL_ANGLE)
	{
		this->verticalAngle = MIN_VERTICAL_ANGLE;
	}
}

/// <summary>
/// 注視点からカメラ座標までの長さの更新
/// </summary>
void Camera::UpdateLength()
{
	/*シングルトンクラスのインスタンスを取得*/
	auto& json = Singleton<JsonManager>::GetInstance();//json
	auto& player = Singleton<PlayerManager>::GetInstance();

	/*使用する変数の準備*/
	float maxLength = 0.0f;
		maxLength = json.GetJson(JsonManager::FileType::CAMERA)["MAX_LENGTH"];	//アングル増加量

	/*長さの補正*/
	this->length = Lerp(this->length, maxLength, 0.05f);
}

/// <summary>
/// カメラの向きの更新
/// </summary>
void Camera::UpdateDirection()
{
	/*シングルトンクラスのインスタンスを取得*/
	auto& json = Singleton<JsonManager>::GetInstance();//json

	/*各回転値をもとにした回転行列を出す*/
	MATRIX horizonRotationMatrix = MGetRotY(this->horizonAngle);
	MATRIX verticalRotationMatrix = MGetRotZ(this->verticalAngle);
	VECTOR firstDirection = Convert(json.GetJson(JsonManager::FileType::CAMERA)["FIRST_DIRECTION"]);

	/*最初の向きベクトルを回転させ現在の向きベクトルを出す*/
	this->direction = VTransform(firstDirection, MMult(verticalRotationMatrix, horizonRotationMatrix));
}

/// <summary>
/// FoVの更新
/// </summary>
void Camera::UpdateFoV()
{
	/*シングルトンクラスのインスタンスを取得*/
	auto& json = Singleton<JsonManager>::GetInstance();//json
	auto& player = Singleton<PlayerManager>::GetInstance();//player

	/*定数型に代入*/
	const float FOV		  = json.GetJson(JsonManager::FileType::CAMERA)["FOV"];
	const float MOVED_FOV = json.GetJson(JsonManager::FileType::CAMERA)["MOVED_FOV"];
	const float PERCENT   = json.GetJson(JsonManager::FileType::CAMERA)["FOV_PERCENT"];

	/*プレイヤーが移動状態だったら*/
	//if (player.IsRun())
	//{
	//	this->fov = Lerp(this->fov, MOVED_FOV, PERCENT);
	//}
	///*移動していなかったら*/
	//else
	//{
		this->fov = Lerp(this->fov, FOV, PERCENT);
	//}

	float fov = this->fov * (DX_PI_F / 180.0f);

	/*fovを設定する*/
	SetupCamera_Perspective(fov);
}

float Camera::Lerp(const float _start,const float _end,const float _percent)
{
	return _start + _percent * (_end - _start);
}
VECTOR Camera::Lerp(const VECTOR _start, const VECTOR _end, const VECTOR _percent)
{
	VECTOR out = { 0.0f,0.0f,0.0f };
	out.x = Lerp(_start.x, _end.x, _percent.x);
	out.y = Lerp(_start.y, _end.y, _percent.y);
	out.z = Lerp(_start.z, _end.z, _percent.z);
	return out;
}