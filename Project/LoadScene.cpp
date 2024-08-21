#include <DxLib.h>
#include "LoadScene.h"

const int LoadScene::FONT_COLOR = GetColor(255, 255, 255);

/// <summary>
/// コンストラクタ
/// </summary>
LoadScene::LoadScene()
	: alpha(MIN_ALPHA)
	, isAddAlpha(false)
{
	Initialize();
}
/// <summary>
/// デストラクタ
/// </summary>
LoadScene::~LoadScene()
{
	Finalize();
}
/// <summary>
/// 初期化
/// </summary>
void LoadScene::Initialize()
{
	SetFontSize(this->FONT_SIZE);
}
/// <summary>
/// 後処理
/// </summary>
void LoadScene::Finalize()
{
}
/// <summary>
/// 更新
/// </summary>
void LoadScene::Update()
{
	//アルファ値の増減
	if (this->isAddAlpha)
	{
		AddAlpha();
	}
	else
	{
		RemoveAlpha();
	}
}
/// <summary>
/// 描画
/// </summary>
const void LoadScene::Draw()const
{
	ClearDrawScreen();
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, this->alpha);
	DrawString(this->TEXT_POS_X, this->TEXT_POS_Y, "Now Loading", this->FONT_COLOR);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, this->alpha);
}

/// <summary>
/// アルファの追加
/// </summary>
void LoadScene::AddAlpha()
{
	/*アルファの追加*/
	this->alpha += this->ADD_ALPHA;

	/*アルファが最大値以下だったらリターン*/
	if (this->alpha <= this->MAX_ALPHA) return;

	/*最大値より大きかったら*/
	this->isAddAlpha = false;			//追加フラグを下す
	this->alpha		 = this->MAX_ALPHA;	//アルファを最大値にする
}

/// <summary>
/// アルファの削除
/// </summary>
void LoadScene::RemoveAlpha()
{
	/*アルファの削除*/
	this->alpha -= this->ADD_ALPHA;

	/*アルファが最小値以上だったらリターン*/
	if (this->alpha >= this->MIN_ALPHA) return;

	/*最小値より小さかったら*/
	this->isAddAlpha = true;			//追加フラグを立てる
	this->alpha		 = this->MIN_ALPHA;	//アルファを最小値にする
}