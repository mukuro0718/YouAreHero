#include <DxLib.h>
#include "EffekseerForDXLib.h"
#include "UseJson.h"
#include "ScreenSetup.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
ScreenSetup::ScreenSetup()
{
    Initialize();
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
ScreenSetup::~ScreenSetup()
{
}

int ScreenSetup::Initialize()
{
    /*Json�}�l�[�W���[�̃C���X�^���X�̎擾*/
    auto& json = Singleton<JsonManager>::GetInstance();

    /*���̕ϐ��ɑ��*/
    int windowHeight = json.GetJson(JsonManager::FileType::SCREEN_SETUP)["WINDOW_HEIGHT"];
    int windowWidth  = json.GetJson(JsonManager::FileType::SCREEN_SETUP)["WINDOW_WIDTH"];
    int colorBit     = json.GetJson(JsonManager::FileType::SCREEN_SETUP)["COLOR_BIT"];

    //�w�i�F�̐ݒ�
    //SetBackgroundColor(100, 100, 100);
    //��ʃ��[�h���E�B���h�E���[�h�ŕ`�悷�邩
#if _DEBUG
    ChangeWindowMode(TRUE);
#else
    ChangeWindowMode(FALSE);
#endif// _DEBUG
    //�`���𗠉�ʂɂ���
    SetDrawScreen(DX_SCREEN_BACK);
    //DirectX11���g�p����悤�ɂ���
    SetUseDirect3DVersion(DX_DIRECT3D_11);
	/*��ʂ̍쐬*/
    SetGraphMode(windowWidth, windowHeight, colorBit);
    //�t���O�𗧂Ă邱�ƂŔ񓯊����[�h���\�ɂȂ�
    SetUseASyncLoadFlag(FALSE);
    //ScreenFlip�����s���Ă����������M����҂��Ȃ�
    SetWaitVSyncFlag(FALSE);
    //1���[�g���ɑ�������l��ݒ肷��
    Set3DSoundOneMetre(json.GetJson(JsonManager::FileType::SCREEN_SETUP)["ONE_METRE"]);
    // �c�w���C�u�����̏�����
    if (DxLib_Init() == -1)
    {
        // �G���[�����������璼���ɏI��
        return -1;
    }
    // �c�w���C�u�����̏�����
    if (Effekseer_Init(5000) == -1)
    {
        // �G���[�����������璼���ɏI��
        return -1;
    }
    // �t���X�N���[���E�C���h�E�̐؂�ւ��Ń��\�[�X��������̂�h���B
    // Effekseer���g�p����ꍇ�͕K���ݒ肷��B
    SetChangeScreenModeGraphicsSystemResetFlag(FALSE);

    // DX���C�u�����̃f�o�C�X���X�g�������̃R�[���o�b�N��ݒ肷��B
    // �E�C���h�E�ƃt���X�N���[���̐؂�ւ�����������ꍇ�͕K�����s����B
    // �������ADirectX11���g�p����ꍇ�͎��s����K�v�͂Ȃ��B
    Effekseer_SetGraphicsDeviceLostCallbackFunctions();

    // Z�o�b�t�@��L���ɂ���B
    // Effekseer���g�p����ꍇ�A2D�Q�[���ł�Z�o�b�t�@���g�p����B
    SetUseZBuffer3D(TRUE);

    // Z�o�b�t�@�ւ̏������݂�L���ɂ���B
    // Effekseer���g�p����ꍇ�A2D�Q�[���ł�Z�o�b�t�@���g�p����B
    SetWriteZBuffer3D(TRUE);

    return 0;
}