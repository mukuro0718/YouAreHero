#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "SceneBase.h"
#include "Image.h"
#include "SceneUI.h"
#include "CreditUI.h"
#include "TitleUI.h"
#include "CameraManager.h"
#include "MapManager.h"
#include "UIManager.h"
#include "InputManager.h"
#include "LoadingAsset.h"
#include "SoundManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
TitleUI::TitleUI()
{
	/*�摜�N���X�C���X�^���X�̍쐬*/
	auto& asset = Singleton<LoadingAsset>::GetInstance();
	this->titleLogo.handle = asset.GetImage(LoadingAsset::ImageType::TITLE_LOGO);
	this->pressLogo.handle = asset.GetImage(LoadingAsset::ImageType::PRESS_BUTTON_LOGO);

	/*�����o�ϐ��̏�����*/
	auto& json = Singleton<JsonManager>	::GetInstance();
	this->titleLogo.x				= json.GetJson(JsonManager::FileType::UI)["TITLE_LOGO_POSITION"][0];
	this->titleLogo.y				= json.GetJson(JsonManager::FileType::UI)["TITLE_LOGO_POSITION"][1];
	this->titleLogo.rate			= json.GetJson(JsonManager::FileType::UI)["TITLE_LOGO_RATE"];
	this->titleLogo.alphaIncrease	= json.GetJson(JsonManager::FileType::UI)["TITLE_LOGO_ALPHA_INCREASE"];
	this->titleLogo.alphaReduction	= json.GetJson(JsonManager::FileType::UI)["TITLE_LOGO_ALPHA_REDUCTION"];
	this->titleLogo.alpha			= 0.0f;
	this->titleLogo.angle			= 0.0f;
	this->pressLogo.x				= json.GetJson(JsonManager::FileType::UI)["PRESS_A_POSITION"][0];
	this->pressLogo.y				= json.GetJson(JsonManager::FileType::UI)["PRESS_A_POSITION"][1];
	this->pressLogo.rate			= json.GetJson(JsonManager::FileType::UI)["PRESS_A_RATE"];
	this->pressLogo.alphaIncrease	= json.GetJson(JsonManager::FileType::UI)["PRESS_A_ALPHA_INCREASE"];
	this->pressLogo.alphaReduction	= json.GetJson(JsonManager::FileType::UI)["PRESS_A_ALPHA_REDUCTION"];
	this->pressLogo.alpha			= 0.0f;
	this->pressLogo.angle			= 0.0f;

	this->credit = new CreditUI();

	/*������*/
	Initialize();
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
TitleUI::~TitleUI()
{
}

/// <summary>
/// ������
/// </summary>
void TitleUI::Initialize()
{
	/*�V���O���g���N���X�̃C���X�^���X���擾*/
	auto& json = Singleton<JsonManager>	 ::GetInstance();
	this->titleLogo.alpha			= 0.0f;
	this->pressLogo.alpha			= 0.0f;
	this->isFadeIn					= true;
	this->alphaForTransition		= this->MAX_ALPHA;
	this->isTransition				= true;
	this->isEndFadeInForTransition	= false;
}


/// <summary>
/// �X�V
/// </summary>
void TitleUI::Update()
{
	/*��ʑJ�ڗp�̃A���t�@�l���c���Ă�����*/
	if (this->isTransition)
	{
		//�J�ڗp�̃t�F�[�h�C�����I����ċ��Ȃ�������
		if (!this->isEndFadeInForTransition)
		{
			this->alphaForTransition -= this->ALPHA_REDUCTION;
			if (this->alphaForTransition <= 0)
			{
				this->isEndFadeInForTransition = true;
				this->isTransition = false;
			}
		}
		else
		{
			this->alphaForTransition += this->ALPHA_INCREASE;
		}
	}
	else
	{
		/*�N���W�b�g�̍X�V����*/
		this->credit->Update();

		/*�N���W�b�g���J���Ă����炻�̑������͍s��Ȃ�*/
		if (this->credit->GetIsOpen())return;

		/*pad����*/
		bool isPressButton = IsPressButton();

		/*�A���t�@����*/
		//�^�C�g�����S�̃A���t�@���ő�A���t�@������������A���t�@�𑝉�������
		if (this->titleLogo.alpha < this->MAX_ALPHA)
		{
			this->titleLogo.alpha += this->titleLogo.alphaIncrease;
		}
		//����ȊO�Ȃ�v���X�{�^�����t�F�[�h�C���A�E�g����
		else
		{
			if (this->isFadeIn)
			{
				this->pressLogo.alpha += this->pressLogo.alphaIncrease;
				if (this->pressLogo.alpha >= this->MAX_ALPHA)
				{
					this->isFadeIn = false;
				}
			}
			else
			{
				this->pressLogo.alpha -= this->pressLogo.alphaReduction;
				if (this->pressLogo.alpha <= 0)
				{
					this->isFadeIn = true;
				}
			}
			//�{�^����������Ă�����A���t�@���ő�A���t�@�ɂ���
			if (isPressButton)
			{
				auto& sound = Singleton<SoundManager>::GetInstance();
				sound.OnIsPlayEffect(SoundManager::EffectType::CONFIRMATION_SOUND);
				this->isTransition = true;
				this->pressLogo.alpha = 0;
			}
		}
	}
}

/// <summary>
/// �`��
/// </summary>
const void TitleUI::Draw()const
{
	/*�^�C�g�����S�̕`��*/
	SetDrawBlendMode(DX_BLENDMODE_ALPHA,this->titleLogo.alpha);
	DrawRotaGraph(this->titleLogo.x, this->titleLogo.y, this->titleLogo.rate, this->titleLogo.angle, this->titleLogo.handle, TRUE);;
	/*�v���X�{�^���̕`��*/
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, this->pressLogo.alpha);
	DrawRotaGraph(this->pressLogo.x, this->pressLogo.y, this->pressLogo.rate, this->pressLogo.angle, this->pressLogo.handle, TRUE);;
	/*�V�[���J�ڂ̕`��*/
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, this->alphaForTransition);
	DrawBox(0, 0, this->MAX_X, this->MAX_Y, 0, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, this->MAX_ALPHA);
	/*�N���W�b�g�̕\��*/
	this->credit->Draw();
}

/// <summary>
/// �V�[�����I�����邩
/// </summary>
const bool TitleUI::IsEnd()const
{
	/*PRESS�̃A���t�@���ϓ����Ă�����\�����Ă���*/
	if (this->isEndFadeInForTransition && this->alphaForTransition >= this->MAX_ALPHA)
	{
		return true;
	}
	return false;
}

/// <summary>
/// �{�^���̓���
/// </summary>
bool TitleUI::IsPressButton()
{
	/*�V���O���g���N���X�̃C���X�^���X���擾*/
	auto& input = Singleton<InputManager>::GetInstance();
	auto& json = Singleton<JsonManager>	 ::GetInstance();

	/*pad����*/
	bool isPressButton = false;

	/*PRESS���S���\������Ă�����*/
	if (this->pressLogo.alpha > 0)
	{
			isPressButton = input.GetNowPad(InputManager::PAD_A);
			//�O�Ƀ{�^�����͂��Ȃ�&���{�^�����͂�����
			if (!this->isPrevPressButton && isPressButton)
			{
				this->isPrevPressButton = true;
			}
			//�O�Ƀ{�^�����͂�����
			else if (this->isPrevPressButton)
			{
				//���{�^�����͂��Ȃ�
				if (!isPressButton)
				{
					this->isPrevPressButton = false;
				}
				isPressButton = false;
			}
	}

	/*�����L�[�������ꂽ��A��������true��Ԃ�*/
	//if (CheckHitKeyAll()) return true;


	return isPressButton;
}