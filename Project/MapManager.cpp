#include <DxLib.h>
#include "UseSTL.h"
#include "VECTORtoUseful.h"
#include "DeleteInstance.h"
#include "BossMap.h"
#include "Skydome.h"
#include "MapManager.h"


/// <summary>
/// コンストラクタ
/// </summary>
MapManager::MapManager()
	: ground(nullptr)
	, skydome(nullptr)
{
	this->ground  = new BossMap();
	this->skydome = new Skydome();
}

/// <summary>
/// デストラクタ
/// </summary>
MapManager::~MapManager()
{
	DeleteMemberInstance(this->ground);
	DeleteMemberInstance(this->skydome);
}

/// <summary>
/// 初期化
/// </summary>
void MapManager::Initialize()
{
	this->ground->Initialize();
	this->skydome->Initialize();
}

/// <summary>
/// 後処理
/// </summary>
void MapManager::Finalize()
{
	this->ground->Finalize();
}

/// <summary>
/// 更新
/// </summary>
void MapManager::Update()
{
	this->ground->Update();
	this->skydome->Update();
}

/// <summary>
/// 描画
/// </summary>
const void MapManager::Draw()const
{
	this->ground->Draw();
	this->skydome->Draw();
}


const int MapManager::GetStageModelHandle()const
{
	return this->ground->GetModelHandle();
}