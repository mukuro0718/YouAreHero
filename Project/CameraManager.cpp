#include <DxLib.h>
#include "CameraManager.h"
#include "DeleteInstance.h"
#include "Camera.h"

//CameraManager* Singleton<CameraManager>::instance = nullptr;

/// <summary>
/// コンストラクタ
/// </summary>
CameraManager::CameraManager()
	:camera(nullptr)
{
	this->camera = new Camera();
}

/// <summary>
/// デストラクタ
/// </summary>
CameraManager::~CameraManager()
{
	DeleteMemberInstance(this->camera);
}

/// <summary>
/// 初期化
/// </summary>
void CameraManager::Initialize()
{
	this->camera->Initialize();
}

/// <summary>
/// 更新
/// </summary>
void CameraManager::Update()
{
	this->camera->Update();
}

/// <summary>
/// 現在のカメラの座標
/// </summary>
const VECTOR CameraManager::GetNowCameraPosition()const
{
	return this->camera->GetNowPosition();
}

/// <summary>
/// 現在のカメラの注視点
/// </summary>
const VECTOR CameraManager::GetNowCameraTarget()const
{
	return this->camera->GetNowTarget();
}

/// <summary>
/// 描画
/// </summary>
const void CameraManager::Draw()const
{
	this->camera->Draw();
}

/// <summary>
/// 現在のカメラの注視点
/// </summary>
const VECTOR CameraManager::GetCameraDirection()const
{
	return this->camera->GetDirection();
}
