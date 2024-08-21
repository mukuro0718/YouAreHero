#include <DxLib.h>
#include "UseSTL.h"
#include "Boss.h"
#include "Vector4.h"
#include "Collider.h"
#include "EnemyManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
EnemyManager::EnemyManager()
	:boss(nullptr)
{
	this->boss = new Boss();
}

/// <summary>
/// デストラクタ
/// </summary>
EnemyManager::~EnemyManager()
{

}

/// <summary>
/// 初期化
/// </summary>
void EnemyManager::Initialize()
{
	this->boss->Initialize();
}
/// <summary>
/// 更新
/// </summary>
void EnemyManager::Update()
{
	this->boss->Update();
}
/// <summary>
/// 描画
/// </summary>
const void EnemyManager::Draw()const
{
	this->boss->Draw();
}

/// <summary>
/// 座標の取得
/// </summary>
const VECTOR EnemyManager::GetPosition()const
{
	return this->boss->GetPosition();
}


const Collider EnemyManager::GetCollider()
{
	return this->boss->GetCollider();
}
