#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "VECTORtoUseful.h"
#include "DeleteInstance.h"
#include "LoadingAsset.h"
#include "Gate.h"
#include "Rigidbody.h"
#include "Character.h"
#include "Player.h"
#include "PlayerManager.h"
#include "InputManager.h"
#include "MapManager.h"
#include "UIManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Gate::Gate()
	: ACTION_LENGTH(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::MAP)["GATE_ACTION_LENGTH"])
	, isNearPlayer(false)
{
	/*���f���n���h���̎擾*/
	auto& asset = Singleton<LoadingAsset>::GetInstance();
	this->modelHandle = MV1DuplicateModel(asset.GetModel(LoadingAsset::ModelType::GATE));
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Gate::~Gate()
{
	MV1DeleteModel(this->modelHandle);
}

/// <summary>
/// ������
/// </summary>
void Gate::Initialize(const int _bossType)
{
	/*���W�b�h�{�f�B�̏�����*/
	auto& json = Singleton<JsonManager>::GetInstance();
	VECTOR position   = Gori::Convert(json.GetJson(JsonManager::FileType::MAP)["GATE_POSITION"][_bossType]);
	VECTOR scale	  = Gori::Convert(json.GetJson(JsonManager::FileType::MAP)["GATE_SCALE"]);
	VECTOR rotation	  = Gori::Convert(json.GetJson(JsonManager::FileType::MAP)["GATE_ROTATION"][_bossType]);
		   rotation.y = rotation.y * (DX_PI_F / 180.0f);
	this->position	  = position;
	MV1SetPosition	 (this->modelHandle, position);
	MV1SetRotationXYZ(this->modelHandle, rotation);
	MV1SetScale		 (this->modelHandle, scale);
	this->isNearPlayer = false;
}

/// <summary>
/// �X�V
/// </summary>
void Gate::Update()
{
	/*�v���C���[�̍��W���擾*/
	auto& player = Singleton<PlayerManager>::GetInstance();
	VECTOR playerPosition = player.GetRigidbody().GetPosition();

	/*�v���C���[�ƃQ�[�g�̋��������*/
	float lengthSquare = VSquareSize(VSub(this->position, playerPosition));
	if (this->ACTION_LENGTH > lengthSquare)
	{
		auto& ui = Singleton<UIManager>::GetInstance();
		ui.OnIsCallAlert();
		this->isNearPlayer = true;
		/*�{�^�����͂����邩�𒲂ׂ�*/
		auto& input = Singleton<InputManager>::GetInstance();
		if (input.GetNowPad(InputManager::PAD_B) && input.GetPrevPad(InputManager::PAD_B))
		{
			auto& map = Singleton<MapManager>::GetInstance();
			map.OnIsChangeStage();
		}
	}
	else
	{
		this->isNearPlayer = false;
	}
}

/// <summary>
/// �`��
/// </summary>
const void Gate::Draw()const
{
	MV1DrawModel(this->modelHandle);
}