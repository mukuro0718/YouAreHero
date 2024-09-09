#include <DxLib.h>
#include "UseSTL.h"
#include "VECTORtoUseful.h"
#include "DeleteInstance.h"
#include "HPUI.h"
#include "ButtonUI.h"
#include "UIManager.h"
#include "SceneState.h"

/// <summary>
/// コンストラクタ
/// </summary>
UIManager::UIManager()
	:hp(nullptr)
{
	/*インスタンスの生成*/
	this->hp = new HPUI();
	this->button = new ButtonUI();

	/*初期化*/
	Initialize();
}

/// <summary>
/// デストラクタ
/// </summary>
UIManager::~UIManager()
{
	DeleteMemberInstance(this->hp);
	DeleteMemberInstance(this->button);
}

/// <summary>
/// 初期化
/// </summary>
void UIManager::Initialize()
{
	this->hp->Initialize();
	this->button->Initialize();
}

/// <summary>
/// 更新
/// </summary>
void UIManager::Update()
{
	auto& sceneState = Singleton<SceneState>::GetInstance();

	this->hp->Update();
	this->button->Update();
}

/// <summary>
/// 描画
/// </summary>
const void UIManager::Draw()const
{
	auto& sceneState = Singleton<SceneState>::GetInstance();

	this->hp->Draw();
	this->button->Draw();
}