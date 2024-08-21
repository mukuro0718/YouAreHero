#include <DxLib.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include "json.hpp"
#include "JsonLoader.h"
#include "JsonManager.h"

using json = nlohmann::json;

//JsonManager* Singleton<JsonManager>::instance = nullptr;

/// <summary>
/// コンストラクタ
/// </summary>
JsonManager::JsonManager()
{
	LoadJsonFile();
}

/// <summary>
/// デストラクタ
/// </summary>
JsonManager::~JsonManager()
{
	this->json.clear();
}

/// <summary>
/// jsonファイルの読み込み
/// </summary>
void JsonManager::LoadJsonFile()
{
	/*json読み込みクラスのインスタンスを取得*/
	auto& loader = JsonLoader::GetInstance();

	/*ファイルの名前を保持しているJsonファイルを読み込む*/
	nlohmann::json baseJson = loader.GetJsonData("Data/Json/LoadJsonFile.json");

	/*読み込んだファイルのデータを保持*/
	std::vector<std::string> fileName = baseJson["FILE_NAME"];

	/*保持したデータの要素分だけループを回す*/
	for (int i = 0; i < fileName.size(); i++)
	{
		//対応するファイル名のjsonを読み込み、保持する
		this->json.push_back(loader.GetJsonData(fileName[i]));
	}
}