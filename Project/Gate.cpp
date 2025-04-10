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
/// コンストラクタ
/// </summary>
Gate::Gate()
	: ACTION_LENGTH(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::MAP)["GATE_ACTION_LENGTH"])
	, isNearPlayer(false)
{
	/*モデルハンドルの取得*/
	auto& asset = Singleton<LoadingAsset>::GetInstance();
	this->modelHandle = MV1DuplicateModel(asset.GetModel(LoadingAsset::ModelType::GATE));
}

/// <summary>
/// デストラクタ
/// </summary>
Gate::~Gate()
{
	MV1DeleteModel(this->modelHandle);
}

/// <summary>
/// 初期化
/// </summary>
void Gate::Initialize(const int _bossType)
{
	/*リジッドボディの初期化*/
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
/// 更新
/// </summary>
void Gate::Update()
{
	/*プレイヤーの座標を取得*/
	auto& player = Singleton<PlayerManager>::GetInstance();
	VECTOR playerPosition = player.GetRigidbody().GetPosition();

	/*プレイヤーとゲートの距離を取る*/
	float lengthSquare = VSquareSize(VSub(this->position, playerPosition));
	if (this->ACTION_LENGTH > lengthSquare)
	{
		auto& ui = Singleton<UIManager>::GetInstance();
		ui.OnIsCallAlert();
		this->isNearPlayer = true;
		/*ボタン入力があるかを調べる*/
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
/// 描画
/// </summary>
const void Gate::Draw()const
{
	MV1DrawModel(this->modelHandle);
}