#include <DxLib.h>
#include "EffekseerForDXLib.h"
#include "UseSTL.h"
#include "UseJson.h"
#include "LoadingAsset.h"

//LoadingAsset* Singleton<LoadingAsset>::instance = nullptr;

/// <summary>
/// コンストラクタ
/// </summary>
LoadingAsset::LoadingAsset()
{
	/*json読み込みクラスのインスタンスを取得*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*json内のパスを取得する*/
	vector<string> modelPath  = json.GetJson(JsonManager::FileType::MODEL_PATH)["PATH"];
	vector<string> imagePath  = json.GetJson(JsonManager::FileType::IMAGE_PATH)["PATH"];
	vector<string> effectPath = json.GetJson(JsonManager::FileType::EFFECT_PATH)["PATH"];
	vector<string> soundPath = json.GetJson(JsonManager::FileType::SOUND_PATH)["PATH"];
	vector<int> soundFlag = json.GetJson(JsonManager::FileType::SOUND_PATH)["FLAG"];

	vector<string> fontPath = json.GetJson(JsonManager::FileType::FONT_PATH )["PATH"];
	vector<string> fontName = json.GetJson(JsonManager::FileType::FONT_PATH)["NAME"];

	vector<int> fontSize  = json.GetJson(JsonManager::FileType::FONT_PATH)["SIZE"];
	vector<int> fontThick = json.GetJson(JsonManager::FileType::FONT_PATH)["THICK"];
	vector<int> fontType  = json.GetJson(JsonManager::FileType::FONT_PATH)["TYPE"];
	vector<int> fontEdge  = json.GetJson(JsonManager::FileType::FONT_PATH)["EDGE"];
	/*モデルのロード*/
	for (int i = 0; i < modelPath.size(); i++)
	{
		this->modelHandle.emplace_back(MV1LoadModel(modelPath[i].c_str()));
	}

	/*画像のロード*/
	for (int i = 0; i < imagePath.size(); i++)
	{
		this->imageHandle.emplace_back(LoadGraph(imagePath[i].c_str()));
	}

	/*フォントのロード*/
	for (int i = 0; i < fontPath.size(); i++)
	{
		AddFontResourceEx(fontPath[i].c_str(), FR_PRIVATE, NULL);
	}
	for (int i = 0; i < fontSize.size(); i++)
	{
		if (fontType[i] == static_cast<int>(FontName::Aihara))
		{
			this->fontHandle.emplace_back(CreateFontToHandle("コーポレート明朝 ver3 Medium", fontSize[i], fontThick[i], DX_FONTTYPE_EDGE, DX_CHARSET_DEFAULT, fontEdge[i]));
		}
		else
		{
			this->fontHandle.emplace_back(CreateFontToHandle(fontName[fontType[i]].c_str(), fontSize[i], fontThick[i], DX_FONTTYPE_EDGE, DX_CHARSET_DEFAULT, fontEdge[i]));
		}
	}
	/*エフェクトのロード*/
	for (int i = 0; i < effectPath.size(); i++)
	{
		this->effectHandle.emplace_back(LoadEffekseerEffect(effectPath[i].c_str(), 1.0f));
	}
	/*サウンドのロード*/
	for (int i = 0; i < soundPath.size(); i++)
	{
		SetCreate3DSoundFlag(soundFlag[i]);
		this->soundHandle.emplace_back(LoadSoundMem(soundPath[i].c_str()));
	}
	SetCreate3DSoundFlag(FALSE);
	//フラグを立てることで非同期ロードが可能になる
	SetUseASyncLoadFlag(TRUE);
}

/// <summary>
/// デストラクタ
/// </summary>
LoadingAsset::~LoadingAsset()
{
	/*フォントのアンロード*/
	auto& json = Singleton<JsonManager>::GetInstance();
	vector<string> fontPath = json.GetJson(JsonManager::FileType::FONT_PATH)["PATH"];
	for (int i = 0; i < fontPath.size(); i++)
	{
		RemoveFontResourceExA(fontPath[i].c_str(), FR_PRIVATE, NULL);
	}

	/*アセットハンドルの解放*/
	for (int i = 0; i < this->modelHandle.size(); i++)
	{
		MV1DeleteModel(this->modelHandle[i]);
	}
	for (int i = 0; i < this->imageHandle.size(); i++)
	{
		DeleteGraph(this->imageHandle[i]);
	}
	for (int i = 0; i < this->soundHandle.size(); i++)
	{
		DeleteSoundMem(this->soundHandle[i]);
	}
	for (int i = 0; i < fontHandle.size(); i++)
	{
		DeleteFontToHandle(this->fontHandle[i]);
	}

	/*動的配列の開放*/
	this->fontHandle.clear();
	this->imageHandle.clear();
	this->modelHandle.clear();
	this->soundHandle.clear();
}
