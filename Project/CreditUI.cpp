#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "CreditUI.h"
#include "LoadingAsset.h"
#include "InputManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
CreditUI::CreditUI()
	: isOpenCredit(false)
	, yOffset (0)
	, ADD_Y_OFFSET(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::UI)["CREDIT_ADD_Y_OFFSET"])
	, TEXT_FONT_SIZE(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::UI)["CREDIT_TEXT_FONT_SIZE"])
{
	auto& asset = Singleton<LoadingAsset>::GetInstance();
	this->fontHandle = asset.GetFont(LoadingAsset::FontType::SELECT_QUEST);
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
CreditUI::~CreditUI()
{

}

/// <summary>
/// �X�V
/// </summary>
void CreditUI::Update()
{
	/*�N���W�b�g���J�����̏���*/
	auto& input = Singleton<InputManager>::GetInstance();
	int nowPad = input.GetNowPadState();
	//�J���Ă��Ȃ��Ƃ���,Start�{�^���������ꂽ��J��
	if (!this->isOpenCredit)
	{
		if (nowPad & InputManager::PAD_START)
		{
			this->isOpenCredit = true;
		}
	}
	//�J���Ă��鎞�ɁAB�{�^���������ꂽ�����
	else
	{
		if (nowPad & InputManager::PAD_B)
		{
			this->isOpenCredit = false;
		}
	}
	/*�N���W�b�g���J���Ă��Ȃ���Α������^�[��*/

	/*���X�e�B�b�N�̓��͏󋵂ŁA��ʂ��X�N���[������i�K�v�Ȃ�j*/
}

/// <summary>
/// �`��
/// </summary>
const void CreditUI::Draw()const
{
	auto& json = Singleton<JsonManager>::GetInstance();
	vector<int> position = json.GetJson(JsonManager::FileType::UI)["CREDIT_POSITION"];
	/*�N���W�b�g���J���Ă��Ȃ����*/
	if (!this->isOpenCredit)
	{
		DrawStringToHandle(position[0], position[1], "START : OPEN CREDIT", this->FONT_COLOR, this->fontHandle);
	}
	else
	{
		/*�w�i�̕`��*/
		DrawBox(0, 0, this->WINDOW_WIDTH, this->WINDOW_HEIGHT, this->BACK_COLOR, TRUE);

		/*�N���W�b�g�̕\��*/
		vector<string> creditText = json.GetJson(JsonManager::FileType::UI)["CREDIT_TEXT"];
		position[1] += this->yOffset;
		for (int i = 0; i < creditText.size(); i++)
		{
			DrawStringToHandle(position[0], position[1], creditText[i].c_str(), this->FONT_COLOR, this->fontHandle);
			position[1] += this->TEXT_FONT_SIZE;
		}
	}
}