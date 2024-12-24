#include <DxLib.h>
#include "UseJson.h"
#include "UseSTL.h"
#include "VECTORtoUseful.h"
#include "DeleteInstance.h"
#include "Rigidbody.h"
#include "ColliderData.h"
#include "SphereColliderData.h"
#include "Character.h"
#include "Player.h"
#include "Camera.h"
#include "PlayerManager.h"
#include "EnemyManager.h"
#include "InputManager.h"
#include "Debug.h"
#include "SceneChanger.h"

/// <summary>
/// コンストラクタ
/// </summary>
Camera::Camera()
	: collider	(nullptr)
	, nextTarget(Gori::ORIGIN)
	, nowTarget	(Gori::ORIGIN)
	, direction	(Gori::ORIGIN)
	, fov		(0.0f)
	, length	(0.0f)
	, yow		(0.0f)
	, pitch		(0.0f)
{
	/*シングルトンクラスのインスタンスを取得*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*コライダーデータの作成*/
	this->collider = new SphereColliderData(ColliderData::Priority::LOW, GameObjectTag::CAMERA);
	this->collider->rigidbody.SetUseGravity(false);
	auto& collider = dynamic_cast<SphereColliderData&>(*this->collider);
	collider.radius = json.GetJson(JsonManager::FileType::CAMERA)["RADIUS"];
}

/// <summary>
/// デストラクタ
/// </summary>
Camera::~Camera()
{
	DeleteMemberInstance(this->collider);
}

/// <summary>
/// 初期化
/// </summary>
void Camera::Initialize()
{
	/*シングルトンクラスのインスタンスを取得*/
	auto& json   = Singleton<JsonManager>::GetInstance();
	auto& player = Singleton<PlayerManager>::GetInstance();
	auto& enemy	 = Singleton<EnemyManager>::GetInstance();

	/*使用するjsonデータを定数型に代入*/
	const  float  NEAR_CLIP		  = json.GetJson(JsonManager::FileType::CAMERA)["NEAR"];				  //手前クリップ距離
	const  float  FAR_CLIP		  = json.GetJson(JsonManager::FileType::CAMERA)["FAR"];					  //奥クリップ距離
	const  float  FOV			  = json.GetJson(JsonManager::FileType::CAMERA)["FOV"];					  //field of view
	const  float  FIRST_LENGTH	  = json.GetJson(JsonManager::FileType::CAMERA)["FIRST_LENGTH"];					  //奥クリップ距離
	const  float  FIRST_ANGLE	  = json.GetJson(JsonManager::FileType::CAMERA)["FIRST_ANGLE"];//カメラ座標オフセット
	vector<float> FIRST_DIRECTION = json.GetJson(JsonManager::FileType::CAMERA)["FIRST_DIRECTION"];//カメラ座標オフセット
	vector<float> TARGET_OFFSET   = json.GetJson(JsonManager::FileType::CAMERA)["TARGET_OFFSET"];//カメラ座標オフセット
	vector<float> FIRST_POSITION  = json.GetJson(JsonManager::FileType::CAMERA)["FIRST_POSITION"];//カメラ座標オフセット


	/*メンバ変数の初期化*/
	this->nowTarget			 = enemy.GetRigidbody().GetPosition() + Gori::Convert(TARGET_OFFSET);	//注視点
	this->nextTarget		 = this->nowTarget;												//注視点
	this->direction			 = Gori::Convert(FIRST_DIRECTION);									//カメラの向き
	this->length			 = FIRST_LENGTH;												//注視点からの距離
	this->fov				 = FOV;															//field of view
	this->yow				 = 0.0f;														//ヨー
	this->pitch				 = 0.0f;														//ピッチ
	this->collider->rigidbody.SetPosition(this->nowTarget + (this->direction * this->length));	//カメラ座標
	this->collider->rigidbody.SetPosition(Gori::Convert(FIRST_POSITION));

	/*カメラの手前クリップ距離と奥クリップ距離を設定する*/
	SetCameraNearFar(NEAR_CLIP, FAR_CLIP);

	/*fovを設定する*/
	SetupCamera_Perspective(DegToRad(this->fov));
}

/// <summary>
/// 更新
/// </summary>
void Camera::Update()
{
	/*シングルトンクラスのインスタンスを取得*/
	auto& json	 = Singleton<JsonManager>::GetInstance();
	auto& player = Singleton<PlayerManager>::GetInstance();

	/*注視点の更新*/
	UpdateTarget();

	/*距離の更新*/
	UpdateLength();

	/*移動ベクトルの更新*/
	UpdateVelocity();

	/*FoVの更新*/
	UpdateFoV();

	/*カメラの向きの更新*/
	UpdateDirection();		
}

/// <summary>
/// 描画
/// </summary>
const void Camera::Draw()const
{
	/*カメラの座標、注視点を設定する（上方向はＹ軸から算出）*/
	SetCameraPositionAndTarget_UpVecY(this->collider->rigidbody.GetPosition(), this->nowTarget);

	auto& debug = Singleton<Debug>::GetInstance();
	if (debug.IsShowDebugInfo(Debug::ItemType::CAMERA))
	{
		VECTOR position = this->collider->rigidbody.GetPosition();
		printfDx("CAMERA_POSITION X:%f,Y:%f,Z:%f\n", position.x, position.y, position.z);
		printfDx("CAMERA_TARGET X:%f,Y:%f,Z:%f\n", this->nowTarget.x, this->nowTarget.y, this->nowTarget.z);
		printfDx("CAMERA_DIRECTION X:%f,Y:%f,Z:%f\n", this->direction.x, this->direction.y, this->direction.z);
		printfDx("CAMERA_LENGTH:%f\n", this->length);
	}
}

/// <summary>
/// 注視点の更新
/// </summary>
void Camera::UpdateTarget()
{
	/*シングルトンクラスのインスタンスを取得*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& enemy = Singleton<EnemyManager>::GetInstance();
	auto& player = Singleton<PlayerManager>::GetInstance();
	auto& sceneChanger = Singleton<SceneChanger>::GetInstance();

	/*シーンの状態で次の注視点を変える*/
	switch (sceneChanger.GetNowSceneType())
	{
	case SceneChanger::SceneType::TITLE:
		this->nextTarget = Gori::Convert(json.GetJson(JsonManager::FileType::CAMERA)["TITLE_TARGET"]);
		break;
	case SceneChanger::SceneType::GAME:
		if (player.GetHP() < 0 || !player.GetIsLockOn())
		{
			//注視点オフセット
			const VECTOR TARGET_OFFSET = Gori::Convert(json.GetJson(JsonManager::FileType::CAMERA)["TARGET_OFFSET"]);
			this->nextTarget = VAdd(player.GetRigidbody().GetPosition(), TARGET_OFFSET);
		}
		else
		{
			this->nextTarget = enemy.GetRigidbody().GetPosition();
		}
		break;
	case SceneChanger::SceneType::GAME_OVER:
		this->nextTarget = Gori::Convert(json.GetJson(JsonManager::FileType::CAMERA)["TITLE_TARGET"]);
		break;
	case SceneChanger::SceneType::GAME_CLEAR:
		this->nextTarget = Gori::Convert(json.GetJson(JsonManager::FileType::CAMERA)["TITLE_TARGET"]);
		break;
	}

	/*現在の注視点を出す*/
	const VECTOR LERP_PERCENT = Gori::Convert(json.GetJson(JsonManager::FileType::CAMERA)["LERP_VALUE_TARGET"]);	//線形補完のパーセント
	this->nowTarget = Lerp(this->nowTarget, this->nextTarget, LERP_PERCENT);
}

/// <summary>
/// カメラ座標の更新
/// </summary>
void Camera::UpdateAngle()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& input  = Singleton<InputManager>	::GetInstance();//インプットマネージャー
	auto& json   = Singleton<JsonManager>	::GetInstance();//json
	auto& player = Singleton<PlayerManager>::GetInstance();//json

	/*変数の準備*/
	const int	X_BUF		= input.GetRStickState().XBuf;								//右スティック入力
	const int	Y_BUF		= input.GetRStickState().YBuf;								//右スティック入力
	const float ADD_ANGLE	= json.GetJson(JsonManager::FileType::CAMERA)["ADD_ANGLE"];	//アングル増加量
	const float MAX_YOW		= json.GetJson(JsonManager::FileType::CAMERA)["MAX_YOW"];	//アングル増加量
	const float MIN_YOW		= json.GetJson(JsonManager::FileType::CAMERA)["MIN_YOW"];	//アングル増加量
	const float MAX_PITCH	= json.GetJson(JsonManager::FileType::CAMERA)["MAX_PITCH"];	//アングル増加量
	const float MIN_PITCH	= json.GetJson(JsonManager::FileType::CAMERA)["MIN_PITCH"];	//アングル増加量

	/*アングルの更新 Y+:down Y-:up X+:right X-:left*/
	//スティック入力もキー入力もなければ
	if (Y_BUF != 0 || X_BUF != 0 || CheckHitKeyAll(DX_CHECKINPUT_KEY) != 0)
	{
		//左右のスティック入力があれば
		if (X_BUF != 0)
		{
			if (X_BUF < 0)
			{
				this->yow += ADD_ANGLE;
			}
			else if (X_BUF > 0)
			{
				this->yow -= ADD_ANGLE;
			}
		}
		//上下のスティック入力があれば
		if (Y_BUF != 0)
		{
			if (Y_BUF < 0)
			{
				this->pitch -= ADD_ANGLE;
			}
			else if (Y_BUF > 0)
			{
				this->pitch += ADD_ANGLE;
			}
		}
	}
	
	/*アングルが範囲を出ないようにする*/
	if (this->pitch < MIN_PITCH)
	{
		this->pitch = MIN_PITCH;
	}
	else if (this->pitch > MAX_PITCH)
	{
		this->pitch = MAX_PITCH;
	}

	if (this->yow < MIN_YOW)
	{
		this->yow += MAX_YOW;
	}
	else if (this->yow > MAX_YOW)
	{
		this->yow -= MIN_YOW;
	}
}

/// <summary>
/// 距離の更新
/// </summary>
void Camera::UpdateLength()
{
	/*シングルトンクラスのインスタンスを取得*/
	auto& json			= Singleton<JsonManager>::GetInstance();
	auto& player		= Singleton<PlayerManager>::GetInstance();
	auto& enemy			= Singleton<EnemyManager>::GetInstance();
	auto& sceneChanger	= Singleton<SceneChanger>::GetInstance();

	float maxLength = 0.0f;
	float minLength = 0.0f;

	/*シーンの状態で処理を変える*/
	switch (sceneChanger.GetNowSceneType())
	{
	case SceneChanger::SceneType::TITLE:
		maxLength = json.GetJson(JsonManager::FileType::CAMERA)["TITLE_MAX_LENGTH"];
		minLength = json.GetJson(JsonManager::FileType::CAMERA)["TITLE_MIN_LENGTH"];
		break;
	case SceneChanger::SceneType::GAME:
		if (player.GetHP() < 0 || enemy.GetHP() < 0)
		{
			maxLength = json.GetJson(JsonManager::FileType::CAMERA)["DEATH_MAX_LENGTH"];
			minLength = json.GetJson(JsonManager::FileType::CAMERA)["DEATH_MIN_LENGTH"];
		}
		else
		{
			maxLength = json.GetJson(JsonManager::FileType::CAMERA)["MAX_LENGTH"];
			minLength = json.GetJson(JsonManager::FileType::CAMERA)["MIN_LENGTH"];
		}
		break;
	default:
		maxLength = json.GetJson(JsonManager::FileType::CAMERA)["MAX_LENGTH"];
		minLength = json.GetJson(JsonManager::FileType::CAMERA)["MIN_LENGTH"];
		break;
	}
	const float LERP_VALUE = json.GetJson(JsonManager::FileType::CAMERA)["LERP_VALUE_LENGTH"];

	/*距離の補正*/
	if (this->length >= maxLength)
	{
		this->length = Lerp(this->length, maxLength, LERP_VALUE);
	}
	else if (this->length <= minLength)
	{
		this->length = Lerp(this->length, minLength, LERP_VALUE);
	}
}

/// <summary>
/// 移動ベクトルの補正
/// </summary>
void Camera::UpdateVelocity()
{
	/*シングルトンクラスのインスタンスを取得*/
	auto& json			= Singleton<JsonManager>::GetInstance();
	auto& player		= Singleton<PlayerManager>::GetInstance();
	auto& enemy			= Singleton<EnemyManager>::GetInstance();
	auto& sceneChanger	= Singleton<SceneChanger>::GetInstance();

	/*次の座標を出す*/
	VECTOR nextPosition = Gori::ORIGIN;

	/*シーンの状態で処理を変える*/
	switch (sceneChanger.GetNowSceneType())
	{
	case SceneChanger::SceneType::TITLE:
		const VECTOR NOW_POSITION = this->collider->rigidbody.GetPosition();
		const VECTOR CAMERA_TO_TARGET = VNorm(VSub(this->nowTarget, NOW_POSITION));
		const VECTOR MOVE_VELOCITY = VNorm(VCross(CAMERA_TO_TARGET, Gori::UP_VEC));
		nextPosition = VNorm(VAdd(NOW_POSITION, MOVE_VELOCITY));
		nextPosition = VScale(nextPosition, this->length);
		nextPosition.y = json.GetJson(JsonManager::FileType::CAMERA)["TITLE_POSITION_OFFSET"];

		break;
	case SceneChanger::SceneType::GAME:
		if (player.GetIsLockOn())
		{
			const VECTOR ENEMY_TO_PLAYER = VNorm(VSub(player.GetRigidbody().GetPosition(), enemy.GetRigidbody().GetPosition()));
			const VECTOR POSITION_OFFSET = Gori::Convert(json.GetJson(JsonManager::FileType::CAMERA)["POSITION_OFFSET"]);
			nextPosition = VScale(ENEMY_TO_PLAYER, this->length);
			nextPosition = VAdd(player.GetRigidbody().GetPosition(), nextPosition);
			nextPosition = VAdd(nextPosition, POSITION_OFFSET);
		}
		else
		{
			/*アングルの更新*/
			UpdateAngle();
			const VECTOR FIRST_DIRECTION = Gori::Convert(json.GetJson(JsonManager::FileType::CAMERA)["FIRST_DIRECTION"]);
			VECTOR direction = VTransform(FIRST_DIRECTION, MGetRotY(this->yow));
			VECTOR axis = VCross(direction, Gori::UP_VEC);
			direction = VTransform(direction, MGetRotAxis(axis, this->pitch));
			nextPosition = VAdd(player.GetRigidbody().GetPosition(), VScale(direction, this->length));
		}
		break;
	default:
		const VECTOR ENEMY_TO_PLAYER = VNorm(VSub(player.GetRigidbody().GetPosition(), enemy.GetRigidbody().GetPosition()));
		const VECTOR POSITION_OFFSET = Gori::Convert(json.GetJson(JsonManager::FileType::CAMERA)["POSITION_OFFSET"]);
		nextPosition = VScale(ENEMY_TO_PLAYER, this->length);
		nextPosition = VAdd(player.GetRigidbody().GetPosition(), nextPosition);
		nextPosition = VAdd(nextPosition, POSITION_OFFSET);
		break;
	}

	/*補正値*/
	const VECTOR LERP_VALUE = Gori::Convert(json.GetJson(JsonManager::FileType::CAMERA)["LERP_VALUE_VELOCITY"]);

	/*次の座標と今の座標の差を移動ベクトルにする*/
	VECTOR newPosition = Lerp(this->collider->rigidbody.GetPosition(), nextPosition, LERP_VALUE);
	VECTOR newVelocity = VSub(newPosition,this->collider->rigidbody.GetPosition());

	this->collider->rigidbody.SetVelocity(newVelocity);
}

/// <summary>
/// カメラの向きの更新
/// </summary>
void Camera::UpdateDirection()
{
	/*シングルトンクラスのインスタンスを取得*/
	auto& json = Singleton<JsonManager>::GetInstance();//json

	/*カメラ→注視点がカメラの向きになる（正規化）*/
	this->direction = VSub(this->nowTarget, this->collider->rigidbody.GetPosition());
	this->direction = VNorm(this->direction);
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
	VECTOR out = (Gori::ORIGIN);
	out.x = Lerp(_start.x, _end.x, _percent.x);
	out.y = Lerp(_start.y, _end.y, _percent.y);
	out.z = Lerp(_start.z, _end.z, _percent.z);
	return out;
}

/// <summary>
/// 現在の座標の取得
/// </summary>
const VECTOR Camera::GetNowPosition()const
{
	return this->collider->rigidbody.GetPosition();
}