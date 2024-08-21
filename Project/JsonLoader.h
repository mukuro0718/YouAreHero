//================================================================
// @brief json読み込みクラス
// 指定されたjsonファイルを読み込む
//================================================================
#pragma once
#include "Singleton.h"

class JsonLoader : public Singleton<JsonLoader>
{
public:
	friend class Singleton<JsonLoader>;
	
	/*指定されたJsonファイルを読み込みデータを返す*/
	const nlohmann::json GetJsonData(std::string _jsonName)const;
private:
	/*内部処理関数*/
			 JsonLoader();//コンストラクタ
	virtual ~JsonLoader();//デストラクタ
};

