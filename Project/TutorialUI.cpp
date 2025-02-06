#include <DxLib.h>
#include "DeleteInstance.h"
#include "UseSTL.h"
#include "UseJson.h"
#include "Rigidbody.h"
#include "SceneBase.h"
#include "Image.h"
#include "HPUI.h"
#include "ButtonUI.h"
#include "BossNameUI.h"
#include "GameClearUI.h"
#include "GameOverUI.h"
#include "SceneUI.h"
#include "TutorialUI.h"
#include "UIManager.h"
#include "InputManager.h"
#include "LoadingAsset.h"
#include "Character.h"
#include "Player.h"
#include "PlayerManager.h"
#include "EnemyManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
TutorialUI::TutorialUI()
	: alpha			(0)
	, backGround	(-1)
	, fontHandle	(-1)
	, frameCount	(0)
	, textDrawTime	(0)
	, isFadeIn		(true)
{
	/*画像クラスインスタンスの作成*/
	auto& asset = Singleton<LoadingAsset>::GetInstance();
	this->backGround		= asset.GetImage(LoadingAsset::ImageType::SELECT_IMAGE_TABLE);
	this->fontHandle = asset.GetFont(LoadingAsset::FontType::SELECT_HEADER);

	auto& json = Singleton<JsonManager>	 ::GetInstance();
	vector<int> backGroundDrawRect  = json.GetJson(JsonManager::FileType::UI)["TUTORIAL_BACK_DRAW_RECT"];
	vector<int> textPosition		= json.GetJson(JsonManager::FileType::UI)["TUTORIAL_TEXT_POSITION"];
	this->backGroundDrawRect		= backGroundDrawRect;
	this->textPosition				= textPosition;
	this->textDrawTime				= json.GetJson(JsonManager::FileType::UI)["TUTORIAL_TEXT_DRAW_TIME"];

	Initialize();
}

/// <summary>
/// デストラクタ
/// </summary>
TutorialUI::~TutorialUI()
{
}

/// <summary>
/// 初期化
/// </summary>
void TutorialUI::Initialize()
{
	/*変数の初期化*/
	this->alpha		 = 0;
	this->frameCount = 0;
	this->isFadeIn	 = true;
}


/// <summary>
/// 更新
/// </summary>
void TutorialUI::Update()
{
	/*フラグが立っていたら*/
	if (this->isFadeIn)
	{
		this->alpha += this->ADD_ALPHA;
		if (this->alpha >= this->MAX_ALPHA)
		{
			this->alpha = this->MAX_ALPHA;
			this->isFadeIn = false;
		}
	}
	else
	{
		if (this->alpha < 0) return;
		this->frameCount++;
		if (this->frameCount >= this->textDrawTime)
		{
			this->alpha -= this->ADD_ALPHA;
		}
	}
}

/// <summary>
/// 描画
/// </summary>
const void TutorialUI::Draw()const
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, this->alpha);
	DrawExtendGraph(this->backGroundDrawRect[0], this->backGroundDrawRect[1], this->backGroundDrawRect[2], this->backGroundDrawRect[3], this->backGround, TRUE);
	DrawStringToHandle(this->textPosition[0], this->textPosition[1], "ここではUI・操作の確認を行えます。\n終了するときは、中央の敵を倒してください。", this->TEXT_COLOR, this->fontHandle);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, this->MAX_ALPHA);
}