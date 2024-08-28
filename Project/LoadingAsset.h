//================================================================
// @brief アセットのロードクラス
//================================================================
#pragma once
#include "Singleton.h"

class LoadingAsset : public Singleton<LoadingAsset>
{
public:
	friend class Singleton<LoadingAsset>;

	/*列挙体*/
	//モデルの種類
	enum class ModelType
	{
		FINALY_BOSS_STAGE,
		PLAYER,
		SATELLITE,
		SHIELD,
		ENEMY,
		ENEMY_BODY,
		SKYDOME,
	};
	//画像の種類
	enum class ImageType
	{
		B,
		X,
		Y,
		LB,
		LS,
		LT,
		RB,
		RS,
		BOSS_HP_CENTER,
		BOSS_HP_RIGHT,
		BOSS_HP_LEFT,
		BOSS_HP_CENTER_HOLDER,
		BOSS_HP_RIGHT_HOLDER,
		BOSS_HP_LEFT_HOLDER,
		BOSS_HP_ICON_HOLDER,
		PLAYER_HP_CENTER,
		PLAYER_HP_RIGHT,
		PLAYER_HP_LEFT,
		PLAYER_HP_CENTER_HOLDER,
		PLAYER_HP_RIGHT_HOLDER,
		PLAYER_HP_LEFT_HOLDER,
		PLAYER_HP_ICON_HOLDER,
		STAMINA_CENTER,
		STAMINA_RIGHT,
		STAMINA_LEFT,
		STAMINA_CENTER_HOLDER,
		STAMINA_RIGHT_HOLDER,
		STAMINA_LEFT_HOLDER,
		STAMINA_ICON_HOLDER,
		HP_ICON,
		STAMINA_ICON,
	};
	//フォントのタイプ
	enum class FontType
	{
		ICON_UI,
		OPERATION_UI,
	};
	enum class FontName
	{
		Honoka,
	};
	//画像のタイプ
	enum class SoundType
	{

	};

	/*getter*/
	const int GetModel(const ModelType _index) { return this->modelHandle[static_cast<int>(_index)]; }//モデルの取得
	const int GetImage(const ImageType _index) { return this->imageHandle[static_cast<int>(_index)]; }//画像の取得
	const int GetFont (const FontType _index)  { return this->fontHandle [static_cast<int>(_index)]; }//フォントの取得
	const int GetSound(const SoundType _index) { return this->soundHandle[static_cast<int>(_index)]; }//サウンドハンドルの取得
private:
	/*内部処理関数*/
			 LoadingAsset();//コンストラクタ
	virtual ~LoadingAsset();//デストラクタ

	std::vector<int> modelHandle;//モデルハンドル
	std::vector<int> imageHandle;//画像ハンドル
	std::vector<int> fontHandle ;//フォントハンドル
	std::vector<int> soundHandle;//サウンドハンドル
};

