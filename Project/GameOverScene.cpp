#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "SceneBase.h"
#include "Image.h"
#include "GameOverScene.h"
#include "Character.h"
#include "Player.h"
#include "PlayerManager.h"
#include "EnemyManager.h"
#include "CameraManager.h"
#include "MapManager.h"
#include "UIManager.h"
#include "InputManager.h"
#include "SceneChanger.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
GameOverScene::GameOverScene()
{
	Initialize();
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
GameOverScene::~GameOverScene()
{
}

/// <summary>
/// ������
/// </summary>
void GameOverScene::Initialize()
{
	/*�V���O���g���N���X�̃C���X�^���X���擾*/
	auto& camera = Singleton<CameraManager>::GetInstance();
	auto& map = Singleton<MapManager>::GetInstance();
	auto& ui = Singleton<UIManager>::GetInstance();
	auto& player = Singleton<PlayerManager>::GetInstance();
	auto& enemy = Singleton<EnemyManager>::GetInstance();

	/*������*/
	//camera.Initialize();
	map.Initialize();
	ui.Initialize();

	this->isPrevPressAnyButton = false;
	if (player.GetHP() < 0)
	{
		this->isGameOver = true;
		this->isGameClear = false;
	}
	else
	{
		this->isGameOver = false;
		this->isGameClear = true;
	}
}

/// <summary>
/// �ŏI����
/// </summary>
void GameOverScene::Finalize()
{
	/*�V���O���g���N���X�̃C���X�^���X���擾*/
	auto& map = Singleton<MapManager>::GetInstance();

	map.Finalize();
}

/// <summary>
/// �X�V
/// </summary>
void GameOverScene::Update()
{
	/*�V���O���g���N���X�̃C���X�^���X���擾*/
	/*auto& camera = Singleton<CameraManager>	::GetInstance();
	auto& map = Singleton<MapManager>		::GetInstance();
	auto& ui = Singleton<UIManager>		::GetInstance();*/
	auto& sceneChanger = Singleton<SceneChanger>	::GetInstance();
	//auto& input = Singleton<InputManager>::GetInstance();

	///*�X�V����*/
	//input.Update();
	//camera.Update();
	//map.Update();
	//ui.Update();

	///*�V�[���̏I������*/
	//if (this->IsEnd())
	//{
		sceneChanger.ChangeScene(SceneChanger::SceneType::SELECT);
	//}
}

/// <summary>
/// �`��
/// </summary>
const void GameOverScene::Draw()const
{
	/*�V���O���g���N���X�̃C���X�^���X���擾*/
	auto& map = Singleton<MapManager>		 ::GetInstance();
	auto& camera = Singleton<CameraManager>	 ::GetInstance();
	auto& ui = Singleton<UIManager>::GetInstance();

	/*�`��*/
	camera.Draw();
	map.Draw();
	ui.Draw();
}

/// <summary>
/// �V�[�����I�����邩
/// </summary>
bool GameOverScene::IsEnd()
{
	/*�V���O���g���N���X�̃C���X�^���X���擾*/
	auto& input = Singleton<InputManager>::GetInstance();
	auto& ui = Singleton<UIManager>::GetInstance();

	/*�����{�^���������ꂽ��*/
	int pad = input.GetNowPadState();
	bool isPressAnyButton = (pad & InputManager::PAD_A);

	/*�^�C�g���V�[�����I���\���I���{�^����������Ă�����true��Ԃ�*/
	if (isPressAnyButton && ui.IsDraw())
	{
		return true;
	}
	return false;
}