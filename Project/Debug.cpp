#include <DxLib.h>
#include "USEJson.h"
#include "UseSTL.h"
#include "Debug.h"
#include "InputManager.h"
#include "BitFlag.h"

/// <summary>
/// コンストラクタ
/// </summary>
Debug::Debug()
	: isReloadingJsonFile(false)
{
	this->jsonIndexMap.emplace(static_cast<int>(ItemType::CAMERA)	, static_cast<int>(JsonManager::FileType::CAMERA));
	this->jsonIndexMap.emplace(static_cast<int>(ItemType::PLAYER)	, static_cast<int>(JsonManager::FileType::PLAYER));
	this->jsonIndexMap.emplace(static_cast<int>(ItemType::FPS)		, static_cast<int>(JsonManager::FileType::FPS_CONTROLLER));
	this->jsonIndexMap.emplace(static_cast<int>(ItemType::ENEMY)	, static_cast<int>(JsonManager::FileType::ENEMY));
}

/// <summary>
/// デストラクタ
/// </summary>
Debug::~Debug() 
{
	this->jsonIndexMap.clear();
}

/// <summary>
/// 初期化
/// </summary>
void Debug::Initialize()
{
}

/// <summary>
/// 更新
/// </summary>
void Debug::Update()
{
	/*Jsonファイルを再読み込み*/
	this->ReloadingJsonFile();
}

/// <summary>
/// Jsonファイルを再読み込み
/// </summary>
void Debug::ReloadingJsonFile()
{
	/*シングルトンクラスのインスタンスを取得*/
	auto& input = Singleton<InputManager>::GetInstance();
	auto& json = Singleton<JsonManager>::GetInstance();

	/*もしJsonファイルを再読み込みしていなかったら（押し続けによる連続読み込みを防ぐ）*/
	if (!this->isReloadingJsonFile)
	{
		if (CheckHitKey(KEY_INPUT_R))
		{
			//読み込みフラグを立てる
			this->isReloadingJsonFile = true;
			//デバックJsonファイルを読み込み
			json.ReloadingJsonFile(JsonManager::FileType::DEBUG);
			//デバックを使用しないになっていたら早期リターン
			if (!json.GetJson(JsonManager::FileType::DEBUG)["IS_USE_DEBUG_MODE"])return;
			//フラグが立っているもののみを再読み込み
			vector<bool> isShowDebugInfo = json.GetJson(JsonManager::FileType::DEBUG)["SHOW_DEBUG_INFO"];
			for (int i = 0; i < isShowDebugInfo.size(); i++)
			{
				//フラグが立っていなければ早期リターン
				if (!isShowDebugInfo[i]) return;
				//指定されたJsonファイルを再読み込み
				static_cast<JsonManager::FileType>(i);
				json.ReloadingJsonFile(static_cast<JsonManager::FileType>(this->jsonIndexMap[i]));
			}
		}
	}
	else
	{
		if (!CheckHitKey(KEY_INPUT_R))
		{
			this->isReloadingJsonFile = false;
		}
	}

}

/// <summary>
/// デバックモードを使用するか
/// </summary>
const bool Debug::IsUseDebugMode() const
{
	/*シングルトンクラスのインスタンスを取得*/
	auto& json = Singleton<JsonManager>::GetInstance();

	return json.GetJson(JsonManager::FileType::DEBUG)["IS_USE_DEBUG_MODE"];
}

/// <summary>
/// デバックモードを使用するか
/// </summary>
const bool Debug::IsShowDebugInfo(const ItemType _type) const
{
	/*シングルトンクラスのインスタンスを取得*/
	auto& json = Singleton<JsonManager>::GetInstance();

	const int TYPE = static_cast<int>(_type);
	return json.GetJson(JsonManager::FileType::DEBUG)["SHOW_DEBUG_INFO"][TYPE];
}