#include <DxLib.h>
#include "UseSTL.h"
#include "VECTORtoUseful.h"
#include "DeleteInstance.h"
#include "MapManager.h"
#include "BossMap.h"
#include "Skydome.h"


/// <summary>
/// コンストラクタ
/// </summary>
MapManager::MapManager()
	: ground(nullptr)
	, skydome(nullptr)
{
	this->ground  = new BossMap();
	this->skydome = new Skydome();

	Initialize();
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
/// 更新
/// </summary>
void MapManager::Update() 
{
	this->ground->Update();
}

/// <summary>
/// 描画
/// </summary>
const void MapManager::Draw()const
{
	this->ground->Draw();
	this->skydome->Draw();
}