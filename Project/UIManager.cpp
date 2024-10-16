#include <assert.h>
#include <DxLib.h>
#include "UseSTL.h"
#include "VECTORtoUseful.h"
#include "DeleteInstance.h"
#include "HPUI.h"
#include "ButtonUI.h"
#include "BossNameUI.h"
#include "SceneUI.h"
#include "TitleUI.h"
#include "TutorialUI.h"
#include "GameUI.h"
#include "GameOverUI.h"
#include "GameClearUI.h"
#include "Audience.h"
#include "UIManager.h"
#include "SceneChanger.h"

/// <summary>
/// コンストラクタ
/// </summary>
UIManager::UIManager()
	: hp		(nullptr)
	, button	(nullptr)
	, bossName	(nullptr)
{
	/*インスタンスの生成*/
	this->hp		= new HPUI();
	this->button	= new ButtonUI();
	this->bossName	= new BossNameUI();
	this->audience	= new Audience();
	this->scene.emplace_back(new TitleUI());
	this->scene.emplace_back(new TutorialUI());
	this->scene.emplace_back(new GameUI());
	this->scene.emplace_back(new GameClearUI());
	this->scene.emplace_back(new GameOverUI());
}

/// <summary>
/// デストラクタ
/// </summary>
UIManager::~UIManager()
{
	DeleteMemberInstance(this->hp);
	DeleteMemberInstance(this->button);
	DeleteMemberInstance(this->bossName);
	DeleteMemberInstance(this->audience);
	for (int i = 0; i < this->scene.size(); i++)
	{
		DeleteMemberInstance(this->scene[i]);
	}
}

/// <summary>
/// 初期化
/// </summary>
void UIManager::Initialize()
{
	auto& sceneChanger = Singleton<SceneChanger>::GetInstance();

	switch (sceneChanger.GetNextSceneType())
	{
	case SceneChanger::SceneType::TITLE:
		this->scene[static_cast<int>(SceneChanger::SceneType::TITLE)]->Initialize();
		break;
	case SceneChanger::SceneType::TUTORIAL:
		this->scene[static_cast<int>(SceneChanger::SceneType::TUTORIAL)]->Initialize();
		break;
	case SceneChanger::SceneType::GAME:
		this->hp->Initialize();
		this->button->Initialize();
		this->bossName->Initialize();
		this->audience->Initialize();
		this->scene[static_cast<int>(SceneChanger::SceneType::GAME)]->Initialize();
		break;
	case SceneChanger::SceneType::GAME_CLEAR:
		this->scene[static_cast<int>(SceneChanger::SceneType::GAME_CLEAR)]->Initialize();
		break;
	case SceneChanger::SceneType::GAME_OVER:
		this->scene[static_cast<int>(SceneChanger::SceneType::GAME_OVER)]->Initialize();
		break;
	}
}

/// <summary>
/// 更新
/// </summary>
void UIManager::Update()
{
	auto& sceneChanger = Singleton<SceneChanger>::GetInstance();

	switch (sceneChanger.GetNowSceneType())
	{
	case SceneChanger::SceneType::TITLE:
		this->scene[static_cast<int>(SceneChanger::SceneType::TITLE)]->Update();
		break;
	case SceneChanger::SceneType::TUTORIAL:
		this->scene[static_cast<int>(SceneChanger::SceneType::TUTORIAL)]->Update();
		break;
	case SceneChanger::SceneType::GAME:
		this->hp->Update();
		this->button->Update();
		this->bossName->Update();
		this->scene[static_cast<int>(SceneChanger::SceneType::GAME)]->Update();
		break;
	case SceneChanger::SceneType::GAME_CLEAR:
		this->scene[static_cast<int>(SceneChanger::SceneType::GAME_CLEAR)]->Update();
		break;
	case SceneChanger::SceneType::GAME_OVER:
		this->scene[static_cast<int>(SceneChanger::SceneType::GAME_OVER)]->Update();
		break;
	}
}

/// <summary>
/// 描画
/// </summary>
const void UIManager::Draw()const
{
	auto& sceneChanger = Singleton<SceneChanger>::GetInstance();

	switch (sceneChanger.GetNowSceneType())
	{
	case SceneChanger::SceneType::TITLE:
		this->scene[static_cast<int>(SceneChanger::SceneType::TITLE)]->Draw();
		break;
	case SceneChanger::SceneType::TUTORIAL:
		this->scene[static_cast<int>(SceneChanger::SceneType::TUTORIAL)]->Draw();
		break;
	case SceneChanger::SceneType::GAME:
		this->audience->Draw();
		this->hp->Draw();
		this->button->Draw();
		this->bossName->Draw();
		this->scene[static_cast<int>(SceneChanger::SceneType::GAME)]->Draw();
		break;
	case SceneChanger::SceneType::GAME_CLEAR:
		this->scene[static_cast<int>(SceneChanger::SceneType::GAME_CLEAR)]->Draw();
		break;
	case SceneChanger::SceneType::GAME_OVER:
		this->scene[static_cast<int>(SceneChanger::SceneType::GAME_OVER)]->Draw();
		break;
	}
}

const bool UIManager::IsDraw()const
{
	auto& sceneChanger = Singleton<SceneChanger>::GetInstance();

	switch (sceneChanger.GetNowSceneType())
	{
	case SceneChanger::SceneType::TITLE:
		return this->scene[static_cast<int>(SceneChanger::SceneType::TITLE)]->IsEnd();
	case SceneChanger::SceneType::TUTORIAL:
		return this->scene[static_cast<int>(SceneChanger::SceneType::TUTORIAL)]->IsEnd();
	case SceneChanger::SceneType::GAME:
		return this->scene[static_cast<int>(SceneChanger::SceneType::GAME)]->IsEnd();
	case SceneChanger::SceneType::GAME_CLEAR:
		return this->scene[static_cast<int>(SceneChanger::SceneType::GAME_CLEAR)]->IsEnd();
	case SceneChanger::SceneType::GAME_OVER:
		return this->scene[static_cast<int>(SceneChanger::SceneType::GAME_OVER)]->IsEnd();
	}
	return false;
}

const bool UIManager::IsContinue()const
{
	int type = static_cast<int>(SceneChanger::SceneType::GAME_OVER);
	auto& scene = dynamic_cast<GameOverUI&>(*this->scene[type]);

	//assert(this->scene[type] != GameOverUI && "scene pointer is null");
	bool isContinue = scene.IsContinue();
	return isContinue;
}