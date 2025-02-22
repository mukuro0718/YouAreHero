#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "ButtonUI.h"
#include "Character.h"
#include "Player.h"
#include "LoadingAsset.h"
#include "InputManager.h"
#include "PlayerManager.h"
#include "ScreenSetup.h"
#include "PlayerController.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
ButtonUI::ButtonUI()
	: displayIndex(0)
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& asset = Singleton<LoadingAsset>::GetInstance();

	//�t�H���g
	this->operationFont	 = asset.GetFont(LoadingAsset::FontType::BUTTON_OPERATION);
	this->itemFont		 = asset.GetFont(LoadingAsset::FontType::ITEM_NUM);
	//�񕜃|�[�V�����p�摜
	this->table			 = asset.GetImage(LoadingAsset::ImageType::POTION_TABLE);
	this->potion		 = asset.GetImage(LoadingAsset::ImageType::POTION);
	//�����w�i
	this->operationTable = asset.GetImage(LoadingAsset::ImageType::OPERATION_TABLE);
	//��������
	this->operationText.emplace_back("�ړ�");		//0
	this->operationText.emplace_back("�_�b�V��");	//1
	this->operationText.emplace_back("���");		//2
	this->operationText.emplace_back("�K�[�h");		//3
	this->operationText.emplace_back("�U��");		//4
	this->operationText.emplace_back("��]�؂�");	//5
	this->operationText.emplace_back("�A�C�e���g�p");//6
	this->operationText.emplace_back("�[��");		//7
	this->operationText.emplace_back("����");		//8
	this->operationText.emplace_back("���b�N�I��");	//9
	//�\������{�^���̃��X�g
	this->buttonList.emplace_back(asset.GetImage(LoadingAsset::ImageType::A_BUTTON));		 //0
	this->buttonList.emplace_back(asset.GetImage(LoadingAsset::ImageType::B_BUTTON));		 //1
	this->buttonList.emplace_back(asset.GetImage(LoadingAsset::ImageType::X_BUTTON));		 //2
	this->buttonList.emplace_back(asset.GetImage(LoadingAsset::ImageType::Y_BUTTON));		 //3
	this->buttonList.emplace_back(asset.GetImage(LoadingAsset::ImageType::LEFT_STICK));	 //4
	this->buttonList.emplace_back(asset.GetImage(LoadingAsset::ImageType::RIGHT_TRIGGER));	 //5
	this->buttonList.emplace_back(asset.GetImage(LoadingAsset::ImageType::RIGHT_BUMPER));	 //6
	this->buttonList.emplace_back(asset.GetImage(LoadingAsset::ImageType::BUTTON_SET_1));	 //7
	this->buttonList.emplace_back(asset.GetImage(LoadingAsset::ImageType::BUTTON_SET_2));	 //8
	this->buttonList.emplace_back(asset.GetImage(LoadingAsset::ImageType::PUSH_RIGHT_STICK));//9

	auto& json = Singleton<JsonManager>::GetInstance();
	this->changeDisplayIndexType.emplace_back(static_cast<int>(PlayerController::PlayerState::COMBO_1));
	this->changeDisplayIndexType.emplace_back(static_cast<int>(PlayerController::PlayerState::COMBO_2));
	this->changeDisplayIndexType.emplace_back(static_cast<int>(PlayerController::PlayerState::COMBO_3));
	this->changeDisplayIndexType.emplace_back(static_cast<int>(PlayerController::PlayerState::STRONG_ATTACK));
	this->changeDisplayIndexType.emplace_back(static_cast<int>(PlayerController::PlayerState::BLOCK));
	this->changeDisplayIndexType.emplace_back(static_cast<int>(PlayerController::PlayerState::DRAW_SWORD_1));

	for (int i = 0; i < this->changeDisplayIndexType.size(); i++)
	{
		this->displayTextMap.emplace(this->changeDisplayIndexType[i], json.GetJson(JsonManager::FileType::UI)["DISPLAY_TEXT_INDEX"][i]);
		this->displayButtonMap.emplace(this->changeDisplayIndexType[i], json.GetJson(JsonManager::FileType::UI)["DISPLAY_BUTTON_INDEX"][i]);
	}
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
ButtonUI::~ButtonUI()
{

}

/// <summary>
/// ������
/// </summary>
void ButtonUI::Initialize()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*������*/
	this->displayIndex = static_cast<int>(PlayerController::PlayerState::DRAW_SWORD_1);
}

/// <summary>
/// �X�V
/// </summary>
void ButtonUI::Update()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& player = Singleton<PlayerManager>::GetInstance();
	int playerNowState = player.GetNowState();
	for (int i = 0; i < this->changeDisplayIndexType.size(); i++)
	{
		if (this->changeDisplayIndexType[i] == playerNowState)
		{
			this->displayIndex = playerNowState;
			return;
		}
	}
	
}

/// <summary>
/// �`��
/// </summary>
void ButtonUI::Draw()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*�A�C�R���̕`��*/
	DrawItem();

	/*�{�^���̕`��*/
	DrawButton();

	/*�A�C�R�����Ƃ̕����̕`��*/
	DrawTextImage();
}


/// <summary>
/// �A�C�R���̕`��
/// </summary>
void ButtonUI::DrawItem()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& player = Singleton<PlayerManager>::GetInstance();

	/*�ϐ��̏���*/
	vector<int> potionPositon	 = json.GetJson(JsonManager::FileType::UI)["ITEM_POSITION"];
	vector<int> tablePosition	 = json.GetJson(JsonManager::FileType::UI)["ITEM_TABLE_POSITION"];
	vector<int> buttonPosition	 = json.GetJson(JsonManager::FileType::UI)["ITEM_BUTTON_POSITION"];
	vector<int> numPosition		 = json.GetJson(JsonManager::FileType::UI)["ITEM_NUM_POSITION"];
	int			buttonType		 = json.GetJson(JsonManager::FileType::UI)["ITEM_BUTTON_TYPE"];

	/*json�f�[�^�̑��*/
	const int itemNum = player.GetHealOrbNum();

	/*�e�[�u���̕`��*/
	DrawExtendGraph(tablePosition[0], tablePosition[1], tablePosition[2], tablePosition[3], this->table, TRUE);
	
	/*�A�C�e���̕`��*/
	DrawExtendGraph(potionPositon[0], potionPositon[1], potionPositon[2], potionPositon[3], this->potion, TRUE);
	
	/*�{�^���̕`��*/
	DrawExtendGraph(buttonPosition[0], buttonPosition[1], buttonPosition[2], buttonPosition[3], this->buttonList[buttonType], TRUE);

	/*�A�C�e���̐�*/
	DrawFormatStringToHandle(numPosition[0], numPosition[1], this->TEXT_COLOR, this->itemFont, "�~%d", itemNum);
}

void ButtonUI::DrawButton()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& input = Singleton<InputManager>::GetInstance();

	vector<int> position = json.GetJson(JsonManager::FileType::UI)["DISPLAY_BUTTON_POSITION"];
	int width = 0;
	int height = json.GetJson(JsonManager::FileType::UI)["DISPLAY_BUTTON_HEIGHT"];
	vector<int> drawList = this->displayButtonMap[this->displayIndex];
	for (int i = 0; i < drawList.size(); i++)
	{
		int buttonType = drawList[i];
		width = json.GetJson(JsonManager::FileType::UI)["DISPLAY_BUTTON_WIDTH"][buttonType];
		DrawExtendGraph(position[0], position[1], position[0] + width, position[1] + height, this->buttonList[buttonType], TRUE);
		position[1] += height;
	}
}

/// <summary>
/// �����̕`��
/// </summary>
void ButtonUI::DrawTextImage()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();

	vector<int> position	 = json.GetJson(JsonManager::FileType::UI)["OPERATION_TEXT_TABLE_POSITION"];
	int			tableYOffset = json.GetJson(JsonManager::FileType::UI)["OPERATION_TABLE_Y_OFFSET"];
	int			textYOffset	 = json.GetJson(JsonManager::FileType::UI)["OPERATION_TEXT_Y_OFFSET"];
	vector<int>	textXOffset	 = json.GetJson(JsonManager::FileType::UI)["OPERATION_TEXT_X_OFFSET"];
	int			height		 = json.GetJson(JsonManager::FileType::UI)["OPERATION_TABLE_HEIGHT"];
	int			width		 = json.GetJson(JsonManager::FileType::UI)["OPERATION_TABLE_WIDTH"];
	vector<int> drawList	 = this->displayTextMap[this->displayIndex];
	for (int i = 0; i < drawList.size(); i++)
	{
		int textType = drawList[i];
		DrawExtendGraph(position[0], position[1], position[0] + width, position[1] + height, this->operationTable, TRUE);
		DrawStringToHandle(position[0] + textXOffset[textType], position[1] + textYOffset, this->operationText[textType].c_str(), this->TEXT_COLOR, this->operationFont);
		position[1] += tableYOffset;
	}
}
/// <summary>
/// �F�擾
/// </summary>
/// <param name="_color"></param>
int ButtonUI::ConvertColor(const std::vector<int> _color)
{
	return GetColor(_color[0], _color[1], _color[2]);
}