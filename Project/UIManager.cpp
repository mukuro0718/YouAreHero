#include <DxLib.h>
#include "UseSTL.h"
#include "VECTORtoUseful.h"
#include "DeleteInstance.h"
#include "HP.h"
#include "UIManager.h"


/// <summary>
/// コンストラクタ
/// </summary>
UIManager::UIManager()
	:hp(nullptr)
{
	/*インスタンスの生成*/
	this->hp = new HP();

	/*初期化*/
	Initialize();
}

/// <summary>
/// デストラクタ
/// </summary>
UIManager::~UIManager()
{
	DeleteMemberInstance(this->hp);
}

/// <summary>
/// 初期化
/// </summary>
void UIManager::Initialize()
{
	this->hp->Initialize();
}

/// <summary>
/// 更新
/// </summary>
void UIManager::Update()
{
	this->hp->Update();
}

/// <summary>
/// 描画
/// </summary>
const void UIManager::Draw()const
{
	this->hp->Draw();
}