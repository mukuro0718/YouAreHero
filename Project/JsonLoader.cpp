#include <DxLib.h>
#include <iostream>
#include <fstream>
#include "UseSTL.h"
#include "json.hpp"
#include "JsonLoader.h"

// for convenience
using json = nlohmann::json;
//JsonLoader* Singleton<JsonLoader>::instance = nullptr;

/// <summary>
/// コンストラクタ
/// </summary>
JsonLoader::JsonLoader()
{
	
}

/// <summary>
/// デストラクタ
/// </summary>
JsonLoader::~JsonLoader()
{

}

/// <summary>
/// 指定されたjsonファイルを読み込みデータを返す
/// </summary>
const json JsonLoader::GetJsonData(std::string _jsonName)const
{
    json jsonData;
    /*ファイルを読み込んで内容を画面に表示する*/
    std::ifstream ifs(_jsonName.c_str());
    if (ifs.good())
    {
        ifs >> jsonData;
    }
    /*読み込みに失敗した場合はエラーを表示する*/
    else
    {
        std::cout << "ファイルの読み込みに失敗しました。" << std::endl;
    }
    return jsonData;
}
