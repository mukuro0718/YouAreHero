#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "SceneBase.h"
#include "Image.h"
#include "Character.h"
#include "Player.h"
#include "Enemy.h"
#include "TitleScene.h"
#include "CameraManager.h"
#include "MapManager.h"
#include "UIManager.h"
#include "InputManager.h"
#include "SceneChanger.h"
#include "CollisionManager.h"
#include "SoundManager.h"
#include "PlayerManager.h"
#include "EnemyManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
TitleScene::TitleScene()
{
	Initialize();
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
TitleScene::~TitleScene()
{
}

/// <summary>
/// ������
/// </summary>
void TitleScene::Initialize()
{
	/*�V���O���g���N���X�̃C���X�^���X���擾*/
	auto& map			= Singleton<MapManager>::GetInstance();
	auto& camera		= Singleton<CameraManager>::GetInstance();
	auto& player		= Singleton<PlayerManager>::GetInstance();
	auto& enemy			= Singleton<EnemyManager>::GetInstance();
	auto& ui			= Singleton<UIManager>::GetInstance();
	auto& sound			= Singleton<SoundManager>::GetInstance();

	/*������*/
	//camera.Initialize();
	player.Initialize();
	enemy.Initialize();
	map.Initialize();
	ui.Initialize();
	sound.OnIsPlayBgm(SoundManager::BgmType::TITLE_BGM);
}

/// <summary>
/// �ŏI����
/// </summary>
void TitleScene::Finalize()
{
	/*�V���O���g���N���X�̃C���X�^���X���擾*/
	auto& map = Singleton<MapManager>::GetInstance();

	map.Finalize();
}

/// <summary>
/// �X�V
/// </summary>
void TitleScene::Update()
{
	/*�V���O���g���N���X�̃C���X�^���X���擾*/
	auto& input			= Singleton<InputManager>::GetInstance();
	auto& camera		= Singleton<CameraManager>	::GetInstance();
	auto& map			= Singleton<MapManager>		::GetInstance();
	auto& ui			= Singleton<UIManager>		::GetInstance();
	auto& sceneChanger	= Singleton<SceneChanger>	::GetInstance();
	auto& collision = Singleton<CollisionManager>::GetInstance();
	auto& sound = Singleton<SoundManager>::GetInstance();

	/*�X�V����*/
	collision.Update();
	input.Update();
	camera.Update();
	map.Update();
	ui.Update();
	sound.Update();

	/*�V�[���̏I������*/
	if (this->IsEnd())
	{
		sceneChanger.ChangeScene(SceneChanger::SceneType::SELECT);
	}
}

/// <summary>
/// �`��
/// </summary>
const void TitleScene::Draw()const
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
bool TitleScene::IsEnd()
{
	/*�V���O���g���N���X�̃C���X�^���X���擾*/
	auto& ui = Singleton<UIManager>::GetInstance();

	/*�^�C�g���V�[�����I���\���I���{�^����������Ă�����true��Ԃ�*/
	if (ui.IsDraw())
	{
		return true;
	}
	return false;
}