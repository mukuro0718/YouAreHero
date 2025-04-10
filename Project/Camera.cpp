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
#include "MapManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
Camera::Camera()
	: collider				(nullptr)
	, nextTarget			(Gori::ORIGIN)
	, nowTarget				(Gori::ORIGIN)
	, direction				(Gori::ORIGIN)
	, fov					(0.0f)
	, length				(0.0f)
	, yow					(0.0f)
	, pitch					(0.0f)
	, isFightingBoss		(false)
	, BOSS_HEAD_FRAME_INDEX	(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::CAMERA)["BOSS_HEAD_FRAME_INDEX"])
	, ENTRY_TIME			(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::CAMERA)["ENTRY_TIME"])
{
	/*シングルトンクラスのインスタンスを取得*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*コライダーデータの作成*/
	this->collider = new SphereColliderData(ColliderData::Priority::LOW, GameObjectTag::CAMERA);
	this->collider->rigidbody.SetUseGravity(false);
	auto& collider = dynamic_cast<SphereColliderData&>(*this->collider);
	collider.radius = json.GetJson(JsonManager::FileType::CAMERA)["RADIUS"];

	this->nearClip				= json.GetJson(JsonManager::FileType::CAMERA)["NEAR"];
	this->farClip				= json.GetJson(JsonManager::FileType::CAMERA)["FAR"];
	this->fov					= json.GetJson(JsonManager::FileType::CAMERA)["FOV"];
	this->firstLength			= json.GetJson(JsonManager::FileType::CAMERA)["FIRST_LENGTH"];
	this->firstAngle			= json.GetJson(JsonManager::FileType::CAMERA)["FIRST_ANGLE"];
	this->firstDirection		= Gori::Convert(json.GetJson(JsonManager::FileType::CAMERA)["FIRST_DIRECTION"]);
	this->targetOffset			= Gori::Convert(json.GetJson(JsonManager::FileType::CAMERA)["TARGET_OFFSET"]);
	this->firstPosition			= Gori::Convert(json.GetJson(JsonManager::FileType::CAMERA)["FIRST_POSITION"]);
	this->titleTarget			= Gori::Convert(json.GetJson(JsonManager::FileType::CAMERA)["TITLE_TARGET"]);
	this->lerpValueForTarget	= Gori::Convert(json.GetJson(JsonManager::FileType::CAMERA)["LERP_VALUE_TARGET"]);
	this->addAngle				= json.GetJson(JsonManager::FileType::CAMERA)["ADD_ANGLE"];
	this->maxYow				= json.GetJson(JsonManager::FileType::CAMERA)["MAX_YOW"];
	this->minYow				= json.GetJson(JsonManager::FileType::CAMERA)["MIN_YOW"];
	this->maxPitch				= json.GetJson(JsonManager::FileType::CAMERA)["MAX_PITCH"];
	this->minPitch				= json.GetJson(JsonManager::FileType::CAMERA)["MIN_PITCH"];
	this->titleMaxLength		= json.GetJson(JsonManager::FileType::CAMERA)["TITLE_MAX_LENGTH"];
	this->titleMinLength		= json.GetJson(JsonManager::FileType::CAMERA)["TITLE_MIN_LENGTH"];
	this->deathMaxLength		= json.GetJson(JsonManager::FileType::CAMERA)["DEATH_MAX_LENGTH"];
	this->deathMinLength		= json.GetJson(JsonManager::FileType::CAMERA)["DEATH_MIN_LENGTH"];
	this->maxLength				= json.GetJson(JsonManager::FileType::CAMERA)["MAX_LENGTH"];
	this->minLength				= json.GetJson(JsonManager::FileType::CAMERA)["MIN_LENGTH"];
	this->lerpValueForLength	= json.GetJson(JsonManager::FileType::CAMERA)["LERP_VALUE_LENGTH"];
	this->titlePositionOffset	= json.GetJson(JsonManager::FileType::CAMERA)["TITLE_POSITION_OFFSET"];
	this->positionOffset		= Gori::Convert(json.GetJson(JsonManager::FileType::CAMERA)["POSITION_OFFSET"]);
	this->lerpValueForVelocity	= Gori::Convert(json.GetJson(JsonManager::FileType::CAMERA)["LERP_VALUE_VELOCITY"]);
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
	auto& player	= Singleton<PlayerManager>::GetInstance();
	auto& enemy		= Singleton<EnemyManager>::GetInstance();

	/*メンバ変数の初期化*/
	this->entryTime			 = 0;
	this->nowTarget			 = Gori::ORIGIN + this->targetOffset;			//注視点
	this->nextTarget		 = this->nowTarget;													//注視点
	this->direction			 = this->firstDirection;											//カメラの向き
	this->length			 = this->firstLength;												//注視点からの距離
	this->yow				 = 0.0f;															//ヨー
	this->pitch				 = 0.0f;															//ピッチ
	this->collider->rigidbody.SetPosition(this->nowTarget + (this->direction * this->length));	//カメラ座標
	this->collider->rigidbody.SetPosition(this->firstPosition);

	/*カメラの手前クリップ距離と奥クリップ距離を設定する*/
	SetCameraNearFar(this->nearClip, this->farClip);

	/*fovを設定する*/
	SetupCamera_Perspective(DegToRad(this->fov));
}

void Camera::InitializeStaticConst()
{
}

/// <summary>
/// 更新
/// </summary>
void Camera::Update()
{
	auto& map = Singleton<MapManager>::GetInstance();
	if (map.GetMapType() == MapManager::MapType::BOSS)
	{
		this->isFightingBoss = true;
	}
	else
	{
		this->isFightingBoss = false;
	}

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

	/*サウンドのリスナーの位置を向きを設定*/
	Set3DSoundListenerPosAndFrontPos_UpVecY(this->collider->rigidbody.GetPosition(), this->nowTarget);

	auto& debug = Singleton<Debug>::GetInstance();
	if (debug.IsShowDebugInfo(Debug::ItemType::CAMERA))
	{
		VECTOR position = this->collider->rigidbody.GetPosition();
		printfDx("CAMERA_POSITION X:%f,Y:%f,Z:%f\n", position.x, position.y, position.z);
		printfDx("CAMERA_TARGET X:%f,Y:%f,Z:%f\n", this->nowTarget.x, this->nowTarget.y, this->nowTarget.z);
		printfDx("CAMERA_DIRECTION X:%f,Y:%f,Z:%f\n", this->direction.x, this->direction.y, this->direction.z);
		printfDx("CAMERA_LENGTH:%f\n", this->length);
		printfDx("CAMERA_YOW:%f\n", this->yow);
		printfDx("CAMERA_PITCH:%f\n", this->pitch);
	}
}

/// <summary>
/// 注視点の更新
/// </summary>
void Camera::UpdateTarget()
{
	/*シングルトンクラスのインスタンスを取得*/
	auto& enemy = Singleton<EnemyManager>::GetInstance();
	auto& player = Singleton<PlayerManager>::GetInstance();
	auto& sceneChanger = Singleton<SceneChanger>::GetInstance();

	/*シーンの状態で次の注視点を変える*/
	switch (sceneChanger.GetNowSceneType())
	{
	case SceneChanger::SceneType::TITLE:
		this->nextTarget = this->titleTarget;
		break;
	case SceneChanger::SceneType::GAME:
		if (this->entryTime < this->ENTRY_TIME && this->isFightingBoss)
		{
			this->entryTime++;
			this->nextTarget = VAdd(enemy.GetRigidbody().GetPosition(), this->targetOffset);
		}
		else
		{
			int playerHP = player.GetHP();
			int enemyHP = enemy.GetHP();
			if (playerHP <= 0 || enemyHP <= 0 && this->isFightingBoss)
			{
				VECTOR target;
				if (playerHP <= 0)
				{
					target = player.GetRigidbody().GetPosition();
				}
				else
				{
					target = MV1GetFramePosition(enemy.GetModelHandle(), this->BOSS_HEAD_FRAME_INDEX);
				}
				//注視点オフセット
				this->nextTarget = VAdd(target, this->targetOffset);
			}
			else
			{
				if (!player.GetIsLockOn())
				{
					//注視点オフセット
					this->nextTarget = VAdd(player.GetRigidbody().GetPosition(), this->targetOffset);
				}
				else
				{
					//this->nextTarget = VAdd(enemy.GetRigidbody().GetPosition(), this->targetOffset);
					this->nextTarget = VAdd(enemy.GetPositionForLockon(), this->targetOffset);
				}
			}
		}
		break;
	}

	/*現在の注視点を出す*/
	this->nowTarget = Lerp(this->nowTarget, this->nextTarget, this->lerpValueForTarget);
}

/// <summary>
/// カメラ座標の更新
/// </summary>
void Camera::UpdateAngle()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& input  = Singleton<InputManager>	::GetInstance();//インプットマネージャー

	/*変数の準備*/
	const int X_BUF = input.GetRStickState().XBuf;//右スティック入力
	const int Y_BUF = input.GetRStickState().YBuf;//右スティック入力

	/*アングルの更新 Y+:down Y-:up X+:right X-:left*/
	//スティック入力もキー入力もなければ
	if (Y_BUF != 0 || X_BUF != 0 || CheckHitKeyAll(DX_CHECKINPUT_KEY) != 0)
	{
		//左右のスティック入力があれば
		if (X_BUF != 0)
		{
			if (X_BUF < 0)
			{
				this->yow += this->addAngle;
			}
			else if (X_BUF > 0)
			{
				this->yow -= this->addAngle;
			}
		}
		//上下のスティック入力があれば
		if (Y_BUF != 0)
		{
			if (Y_BUF < 0)
			{
				this->pitch -= this->addAngle;
			}
			else if (Y_BUF > 0)
			{
				this->pitch += this->addAngle;
			}
		}
	}
	
	/*アングルが範囲を出ないようにする*/
	if (this->pitch < this->minPitch)
	{
		this->pitch = this->minPitch;
	}
	else if (this->pitch > this->maxPitch)
	{
		this->pitch = this->maxPitch;
	}
	//if (this->yow < this->minYow)
	//{
	//	this->yow += this->maxYow;
	//}
	//else if (this->yow > this->maxYow)
	//{
	//	this->yow -= this->maxYow;
	//}
}

/// <summary>
/// 距離の更新
/// </summary>
void Camera::UpdateLength()
{
	/*シングルトンクラスのインスタンスを取得*/
	auto& player		= Singleton<PlayerManager>::GetInstance();
	auto& enemy			= Singleton<EnemyManager>::GetInstance();
	auto& sceneChanger	= Singleton<SceneChanger>::GetInstance();

	float maxLength = 0.0f;
	float minLength = 0.0f;

	/*シーンの状態で処理を変える*/
	switch (sceneChanger.GetNowSceneType())
	{
	case SceneChanger::SceneType::TITLE:
		maxLength = this->titleMaxLength;
		minLength = this->titleMinLength;
		break;
	case SceneChanger::SceneType::GAME:
		if (this->entryTime < this->ENTRY_TIME && this->isFightingBoss)
		{
			maxLength = this->deathMaxLength * this->ENTRY_MULT;
			minLength = this->deathMinLength * this->ENTRY_MULT;
		}
		else
		{
			if (player.GetHP() <= 0 || enemy.GetHP() <= 0 && this->isFightingBoss)
			{
				maxLength = this->deathMaxLength;
				minLength = this->deathMinLength;
			}
			else
			{
				maxLength = this->maxLength;
				minLength = this->minLength;
			}
		}
		break;
	default:
		maxLength = this->maxLength;
		minLength = this->minLength;
		break;
	}

	/*距離の補正*/
	if (this->length >= maxLength)
	{
		this->length = Lerp(this->length, maxLength, this->lerpValueForLength);
	}
	else if (this->length <= minLength)
	{
		this->length = Lerp(this->length, minLength, this->lerpValueForLength);
	}
}

/// <summary>
/// 移動ベクトルの補正
/// </summary>
void Camera::UpdateVelocity()
{
	auto& player		= Singleton<PlayerManager>::GetInstance();
	auto& enemy			= Singleton<EnemyManager>::GetInstance();
	auto& sceneChanger	= Singleton<SceneChanger>::GetInstance();

	/*次の座標を出す*/
	VECTOR nextPosition = Gori::ORIGIN;

	/*シーンの状態で処理を変える*/
	switch (sceneChanger.GetNowSceneType())
	{
	case SceneChanger::SceneType::TITLE:
		const VECTOR NOW_POSITION	  = this->collider->rigidbody.GetPosition();
		const VECTOR CAMERA_TO_TARGET = VNorm(VSub(this->nowTarget, NOW_POSITION));
		const VECTOR MOVE_VELOCITY	  = VNorm(VCross(CAMERA_TO_TARGET, Gori::UP_VEC));
		nextPosition = VNorm(VAdd(NOW_POSITION, MOVE_VELOCITY));
		nextPosition = VScale(nextPosition, this->length);
		nextPosition.y = this->titlePositionOffset;
		break;
	case SceneChanger::SceneType::GAME:
		if (this->entryTime < this->ENTRY_TIME && this->isFightingBoss)
		{
			VECTOR nowTarget = this->nowTarget;
			nowTarget.y = 0.0f;
			const VECTOR NOW_POSITION = this->collider->rigidbody.GetPosition();
			const VECTOR TARGET_TO_POSITION = VNorm(VSub(NOW_POSITION, this->nowTarget));
			VECTOR newPosition = VAdd(VScale(TARGET_TO_POSITION, this->length), this->nowTarget);
			newPosition.y = 0.0f;
			const VECTOR NEW_POSITION_TO_TARGET = VNorm(VSub(nowTarget, newPosition));
			const VECTOR MOVE_VELOCITY = VNorm(VCross(NEW_POSITION_TO_TARGET, Gori::UP_VEC));
			nextPosition = VAdd(newPosition, MOVE_VELOCITY);
			nextPosition = VAdd(nextPosition, this->positionOffset);
		}
		else
		{
			if (player.GetHP() <= 0 || enemy.GetHP() <= 0 && this->isFightingBoss)
			{
				VECTOR nowTarget = this->nowTarget;
				nowTarget.y = 0.0f;
				const VECTOR NOW_POSITION = this->collider->rigidbody.GetPosition();
				const VECTOR TARGET_TO_POSITION = VNorm(VSub(NOW_POSITION, this->nowTarget));
				VECTOR newPosition = VAdd(VScale(TARGET_TO_POSITION, this->length), this->nowTarget);
				newPosition.y = 0.0f;
				const VECTOR NEW_POSITION_TO_TARGET = VNorm(VSub(nowTarget, newPosition));
				const VECTOR MOVE_VELOCITY = VNorm(VCross(NEW_POSITION_TO_TARGET, Gori::UP_VEC));
				nextPosition = VAdd(newPosition, MOVE_VELOCITY);
				nextPosition = VAdd(nextPosition, this->positionOffset);
			}
			else if (player.GetIsLockOn())
			{
				const VECTOR ENEMY_TO_PLAYER = VNorm(VSub(player.GetRigidbody().GetPosition(), enemy.GetPositionForLockon()));
				nextPosition = VScale(ENEMY_TO_PLAYER, this->length);
				nextPosition = VAdd(player.GetRigidbody().GetPosition(), nextPosition);
				nextPosition = VAdd(nextPosition, this->positionOffset);
			}
			else
			{
				/*アングルの更新*/
				UpdateAngle();
				VECTOR direction = VTransform(this->firstDirection, MGetRotY(this->yow));
				VECTOR axis = VCross(direction, Gori::UP_VEC);
				direction = VTransform(direction, MGetRotAxis(axis, this->pitch));
				nextPosition = VAdd(player.GetRigidbody().GetPosition(), VScale(direction, this->length));
			}
		}
		break;
	default:
		const VECTOR ENEMY_TO_PLAYER = VNorm(VSub(player.GetRigidbody().GetPosition(), enemy.GetRigidbody().GetPosition()));
		nextPosition = VScale(ENEMY_TO_PLAYER, this->length);
		nextPosition = VAdd(player.GetRigidbody().GetPosition(), nextPosition);
		nextPosition = VAdd(nextPosition, this->positionOffset);
		break;
	}

	/*次の座標と今の座標の差を移動ベクトルにする*/
	VECTOR newPosition = Lerp(this->collider->rigidbody.GetPosition(), nextPosition, this->lerpValueForVelocity);
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
	//auto& json = Singleton<JsonManager>::GetInstance();//json
	//auto& player = Singleton<PlayerManager>::GetInstance();//player

	///*定数型に代入*/
	//const float FOV		  = json.GetJson(JsonManager::FileType::CAMERA)["FOV"];
	//const float MOVED_FOV = json.GetJson(JsonManager::FileType::CAMERA)["MOVED_FOV"];
	//const float PERCENT   = json.GetJson(JsonManager::FileType::CAMERA)["FOV_PERCENT"];

	///*プレイヤーが移動状態だったら*/
	////if (player.IsRun())
	////{
	////	this->fov = Lerp(this->fov, MOVED_FOV, PERCENT);
	////}
	/////*移動していなかったら*/
	////else
	////{
	//	this->fov = Lerp(this->fov, FOV, PERCENT);
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