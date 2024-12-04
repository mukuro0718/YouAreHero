#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "SceneBase.h"
#include "Image.h"
#include "SelectScene.h"
#include "MapManager.h"
#include "UIManager.h"
#include "SceneChanger.h"
#include "InputManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
SelectScene::SelectScene()
{
	Initialize();
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
SelectScene::~SelectScene()
{
}

/// <summary>
/// ������
/// </summary>
void SelectScene::Initialize()
{
	/*�V���O���g���N���X�̃C���X�^���X���擾*/
	auto& ui = Singleton<UIManager>::GetInstance();

	/*������*/
	ui.Initialize();
}

/// <summary>
/// �ŏI����
/// </summary>
void SelectScene::Finalize()
{
}

/// <summary>
/// �X�V
/// </summary>
void SelectScene::Update()
{
	/*�V���O���g���N���X�̃C���X�^���X���擾*/
	auto& ui			= Singleton<UIManager>		::GetInstance();
	auto& sceneChanger  = Singleton<SceneChanger>	::GetInstance();
	auto& input = Singleton<InputManager>::GetInstance();

	/*�X�V����*/
	input.Update();
	ui.Update();

	/*�V�[���̏I������*/
	if (this->IsEnd())
	{
		sceneChanger.ChangeScene(SceneChanger::SceneType::GAME);
	}
}

/// <summary>
/// �`��
/// </summary>
const void SelectScene::Draw()const
{
	/*UI�̕`��*/
	auto& ui	 = Singleton<UIManager>		::GetInstance();
	ui.Draw();
}

/// <summary>
/// �V�[�����I�����邩
/// </summary>
bool SelectScene::IsEnd()
{
	/*�V���O���g���N���X�̃C���X�^���X���擾*/
	auto& ui = Singleton<UIManager>::GetInstance();

	/*�^�C�g���V�[�����I���\��������true��Ԃ�*/
	if (ui.IsDraw())
	{
		return true;
	}
	return false;
}