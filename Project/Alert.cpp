#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "Alert.h"
#include "LoadingAsset.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Alert::Alert()
	: isDraw (false)
	, isCall (false)
{
	auto& asset = Singleton<LoadingAsset>::GetInstance();
	auto& json = Singleton<JsonManager>	 ::GetInstance();
	vector<int> tableDrawRect			= json.GetJson(JsonManager::FileType::UI)["GAME_PAUSE_TABLE_DRAW_RECT"];
	vector<int> decideButtonDrawRect	= json.GetJson(JsonManager::FileType::UI)["GAME_PAUSE_DECIDE_DRAW_RECT"];
	vector<int> backButtonDrawRect		= json.GetJson(JsonManager::FileType::UI)["GAME_PAUSE_BACK_DRAW_RECT"];
	vector<int> alertTextPosition		= json.GetJson(JsonManager::FileType::UI)["GAME_PAUSE_HEADER_TEXT_POSITION"];
	vector<int> actionTextPosition		= json.GetJson(JsonManager::FileType::UI)["GAME_PAUSE_ACTION_TEXT_POSITION"];
	this->images.push_back(Image(asset.GetImage(LoadingAsset::ImageType::SELECT_IMAGE_TABLE), tableDrawRect));
	this->images.push_back(Image(asset.GetImage(LoadingAsset::ImageType::B_BUTTON), decideButtonDrawRect));
	this->images.push_back(Image(asset.GetImage(LoadingAsset::ImageType::A_BUTTON), backButtonDrawRect));
	this->texts.push_back(Text(alertTextPosition, "�{�X�ɒ��킵�܂���", this->TEXT_COLOR, asset.GetFont(LoadingAsset::FontType::SELECT_HEADER)));
	this->texts.push_back(Text(actionTextPosition, ":�͂�       :������", this->TEXT_COLOR, asset.GetFont(LoadingAsset::FontType::SELECT_QUEST)));
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Alert::~Alert() 
{
}

/// <summary>
/// �X�V
/// </summary>
void Alert::Update()
{
	//�Ă΂�Ă�����`��t���O�𗧂ĂČĂ΂ꂽ�t���O�͍~�낷
	if (this->isCall)
	{
		this->isDraw = true;
		this->isCall = false;
	}
	else
	{
		this->isDraw = false;
	}
}

/// <summary>
/// �`��
/// </summary>
const void Alert::Draw()const
{
	if (!this->isDraw)return;
	for (auto& image : this->images)
	{
		image.Draw();
	}
	for (auto& text : this->texts)
	{
		text.Draw();
	}
}
const void Alert::Image::Draw()const
{
	DrawExtendGraph(this->drawRect[0], this->drawRect[1], this->drawRect[2], this->drawRect[3], this->handle, TRUE);
}
const void Alert::Text::Draw()const
{
	DrawStringToHandle(this->position[0], this->position[1], this->text.c_str(), this->color, this->handle);
}