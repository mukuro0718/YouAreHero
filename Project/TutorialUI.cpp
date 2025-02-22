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
#include "TutorialManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
TutorialUI::TutorialUI()
	: alpha				(0)
	, backGround		(-1)
	, headerFontHandle	(-1)
	, bodyFontHandle	(-1)
	, frameCount		(0)
	, isFadeIn			(true)
{
	/*�摜�N���X�C���X�^���X�̍쐬*/
	auto& asset = Singleton<LoadingAsset>::GetInstance();
	this->backGround		= asset.GetImage(LoadingAsset::ImageType::SELECT_IMAGE_TABLE);
	this->headerFontHandle	= asset.GetFont(LoadingAsset::FontType::SELECT_HEADER);
	this->bodyFontHandle	= asset.GetFont(LoadingAsset::FontType::SELECT_QUEST);

	auto& json = Singleton<JsonManager>	 ::GetInstance();
	vector<int> backGroundDrawRect  = json.GetJson(JsonManager::FileType::UI)["TUTORIAL_BACK_DRAW_RECT"];
	vector<int> headerTextPosition	= json.GetJson(JsonManager::FileType::UI)["TUTORIAL_HEADER_TEXT_POSITION"];
	vector<int> bodyTextPosition	= json.GetJson(JsonManager::FileType::UI)["TUTORIAL_BODY_TEXT_POSITION"];
	this->backGroundDrawRect		= backGroundDrawRect;
	this->headerTextPosition		= headerTextPosition;
	this->bodyTextPosition			= bodyTextPosition;

	this->headerText = "�`���[�g���A��";
	this->bodyText.emplace_back("�E�X�e�B�b�N�ŃJ��������\n�������݂Ń��b�N�I��\n");
	this->bodyText.emplace_back("���X�e�B�b�N�ŕ���\n�����ړ�����RB�Ń_�b�V��\n");
	this->bodyText.emplace_back("B�ŃR���{�U��\nY�ŉ�]�U��\n");
	this->bodyText.emplace_back("A�ŉ��\nLT�Ŗh��\n");
	this->bodyText.emplace_back("X�ŉ�\n");
	this->bodyText.emplace_back("�{�X��|��\n");

	Initialize();
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
TutorialUI::~TutorialUI()
{
}

/// <summary>
/// ������
/// </summary>
void TutorialUI::Initialize()
{
	/*�ϐ��̏�����*/
	this->alpha		 = 0;
	this->frameCount = 0;
	this->isFadeIn	 = true;
}


/// <summary>
/// �X�V
/// </summary>
void TutorialUI::Update()
{
	auto& tutorialManager = Singleton<TutorialManager>::GetInstance();
	tutorialManager.Update();

	/*�t���O�������Ă�����*/
	if (this->isFadeIn)
	{
		this->alpha += this->ADD_ALPHA;
		if (this->alpha >= this->MAX_ALPHA)
		{
			this->alpha = this->MAX_ALPHA;
			this->isFadeIn = false;
		}
	}
}

/// <summary>
/// �`��
/// </summary>
const void TutorialUI::Draw()const
{
	auto& tutorialManager = Singleton<TutorialManager>::GetInstance();
	int currentStage = tutorialManager.GetStage();
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, this->alpha);
	DrawExtendGraph(this->backGroundDrawRect[0], this->backGroundDrawRect[1], this->backGroundDrawRect[2], this->backGroundDrawRect[3], this->backGround, TRUE);
	DrawStringToHandle(this->headerTextPosition[0], this->headerTextPosition[1], this->headerText.c_str(), this->TEXT_COLOR, this->headerFontHandle);
	DrawStringToHandle(this->bodyTextPosition[0], this->bodyTextPosition[1], this->bodyText[currentStage].c_str(), this->TEXT_COLOR, this->bodyFontHandle);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, this->MAX_ALPHA);
}