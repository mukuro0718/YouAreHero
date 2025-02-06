#include <assert.h>
#include <DxLib.h>
#include "UseSTL.h"
#include "VECTORtoUseful.h"
#include "DeleteInstance.h"
#include "SceneUI.h"
#include "TitleUI.h"
#include "SelectUI.h"
#include "GameUI.h"
#include "GameOverUI.h"
#include "GameClearUI.h"
#include "UIManager.h"
#include "SceneChanger.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
UIManager::UIManager()
	: scene(nullptr)
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
UIManager::~UIManager()
{
	DeleteMemberInstance(this->scene);
}

/// <summary>
/// ������
/// </summary>
void UIManager::Initialize()
{
	auto& sceneChanger = Singleton<SceneChanger>::GetInstance();

	if (this->scene != nullptr)
	{
		DeleteMemberInstance(this->scene);
	}

	switch (sceneChanger.GetNextSceneType())
	{
	case SceneChanger::SceneType::TITLE:
		this->scene = new TitleUI();
		break;
	case SceneChanger::SceneType::GAME:
		this->scene = new GameUI();
		break;
	case SceneChanger::SceneType::SELECT:
		this->scene = new SelectUI();
		break;
	}
	this->scene->Initialize();
}

/// <summary>
/// �X�V
/// </summary>
void UIManager::Update()
{
	//int startTime = GetNowCount();
	this->scene->Update();
	//int endTime = GetNowCount();
	//this->frameTime = endTime - startTime;
}

/// <summary>
/// �`��
/// </summary>
const void UIManager::Draw()const
{
	this->scene->Draw();
	//printfDx("UI_FRAMETIME:%d\n", this->frameTime);
}

/// <summary>
/// �`�悵�Ă��邩
/// </summary>
const bool UIManager::IsDraw()const
{
	return this->scene->IsEnd();
}

/// <summary>
/// �`�悵�Ă��邩
/// </summary>
const bool UIManager::GetIsBackTitle()const
{
	auto& selectScene = dynamic_cast<SelectUI&>(*this->scene);
	return selectScene.GetIsBack();
}