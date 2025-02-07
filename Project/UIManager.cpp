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
{
	this->scene.emplace_back(new TitleUI());
	this->scene.emplace_back(new SelectUI());
	this->scene.emplace_back(new GameUI());
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
UIManager::~UIManager()
{
	for (int i = 0; i < this->scene.size(); i++)
	{
		DeleteMemberInstance(this->scene[i]);
	}
}

/// <summary>
/// ������
/// </summary>
void UIManager::Initialize()
{
	auto& sceneChanger = Singleton<SceneChanger>::GetInstance();
	this->sceneType = static_cast<int>(sceneChanger.GetNextSceneType());
	
	this->scene[this->sceneType]->Initialize();
}

/// <summary>
/// �X�V
/// </summary>
void UIManager::Update()
{
	//int startTime = GetNowCount();
	this->scene[this->sceneType]->Update();
	//int endTime = GetNowCount();
	//this->frameTime = endTime - startTime;
}

/// <summary>
/// �`��
/// </summary>
const void UIManager::Draw()const
{
	this->scene[this->sceneType]->Draw();
	//printfDx("UI_FRAMETIME:%d\n", this->frameTime);
}

/// <summary>
/// �`�悵�Ă��邩
/// </summary>
const bool UIManager::IsDraw()const
{
	return this->scene[this->sceneType]->IsEnd();
}

/// <summary>
/// �`�悵�Ă��邩
/// </summary>
const bool UIManager::GetIsBackTitle()const
{
	auto& selectScene = dynamic_cast<SelectUI&>(*this->scene[this->SELECT_UI_INDEX]);
	return selectScene.GetIsBack();
}