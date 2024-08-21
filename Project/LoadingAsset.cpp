#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "LoadingAsset.h"

//LoadingAsset* Singleton<LoadingAsset>::instance = nullptr;

/// <summary>
/// �R���X�g���N�^
/// </summary>
LoadingAsset::LoadingAsset()
{
	/*json�ǂݍ��݃N���X�̃C���X�^���X���擾*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*json���̃p�X���擾����*/
	vector<string> modelPath = json.GetJson(JsonManager::FileType::MODEL_PATH)["PATH"];
	vector<string> imagePath = json.GetJson(JsonManager::FileType::IMAGE_PATH)["PATH"];
	vector<string> fontPath  = json.GetJson(JsonManager::FileType::FONT_PATH )["PATH"];

	/*���f���̃��[�h*/
	for (int i = 0; i < modelPath.size(); i++)
	{
		this->modelHandle.emplace_back(MV1LoadModel(modelPath[i].c_str()));
	}

	/*�摜�̃��[�h*/
	for (int i = 0; i < imagePath.size(); i++)
	{
		this->imageHandle.emplace_back(LoadGraph(imagePath[i].c_str()));
	}

	/*�t�H���g�̃��[�h*/
	for (int i = 0; i < fontPath.size(); i++)
	{
		this->fontHandle.emplace_back(CreateFontToHandle(fontPath[i].c_str(), 32, 16));
	}
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
LoadingAsset::~LoadingAsset()
{
	/*�A�Z�b�g�n���h���̉��*/
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

	/*���I�z��̊J��*/
	this->fontHandle.clear();
	this->imageHandle.clear();
	this->modelHandle.clear();
	this->soundHandle.clear();
}