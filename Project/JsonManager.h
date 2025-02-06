//================================================================
// @brief json管理クラス
// JsonLoaderクラスを使用してJsonファイルを読み込み保持する
//================================================================
#pragma once
#include "Singleton.h"

class JsonManager : public Singleton<JsonManager>
{
public:
	friend class Singleton<JsonManager>;

	/*ファイルの種類*/
	enum class FileType
	{
		FPS_CONTROLLER	= 0,
		SCREEN_SETUP	= 1,
		LOAD_SCENE		= 2,
		MODEL_PATH		= 3,
		IMAGE_PATH		= 4,
		FONT_PATH		= 5,
		SOUND_PATH		= 6,
		MAP				= 7,
		CAMERA			= 8,
		PLAYER			= 9,
		ENEMY			= 10,
		DEBUG			= 11,
		UI				= 12,
		EFFECT_PATH		= 13,
		EFFECT			= 14,
		BOT				= 15,
		RANKING			= 16,
		DRAGON			= 17,
		BEAST			= 18,
		SOUND			= 19,
		DEMON_PANDA		= 20,
	};

	void ReloadingJsonFile(const FileType _type);

	/*getter*/
	const nlohmann::json& GetJson(const FileType _index)const { return json[static_cast<int>(_index)]; }//jsonデータのgetter
protected:
	/*内部処理関数*/
			 JsonManager();//コンストラクタ
	virtual ~JsonManager();//デストラクタ
private:
	void	LoadJsonFile();//jsonファイルの読み込み

	/*メンバ変数*/
	std::vector<nlohmann::json> json;
};

