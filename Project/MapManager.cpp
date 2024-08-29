#include <DxLib.h>
#include "UseSTL.h"
#include "GoriLib.h"
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
void MapManager::Initialize(GoriLib::Physics* _physics)
{
	this->ground->Initialize(_physics);
	this->skydome->Initialize();
}

/// <summary>
/// 後処理
/// </summary>
void MapManager::Finalize(GoriLib::Physics* _physics)
{
	this->ground->Finalize(_physics);
}

/// <summary>
/// 更新
/// </summary>
void MapManager::Update(GoriLib::Physics* _physics)
{
	this->ground->Update(_physics);
}

/// <summary>
/// 移動ベクトルの補正
/// </summary>
void MapManager::OnCollide(const GoriLib::Collidable& _colider)
{
	this->ground->OnCollide(_colider);
}

/// <summary>
/// 描画
/// </summary>
const void MapManager::Draw()const
{
	this->ground->Draw();
	this->skydome->Draw();
}