//========================================
// @brief �}�b�v�}�l�[�W���[�N���X
// �}�b�v�̊Ǘ����s��
//========================================
#pragma once
#include "Singleton.h"

class SceneUI;
class Alert;
class UIManager : public Singleton<UIManager>
{
public:
	friend class Singleton<UIManager>;
	virtual ~UIManager();//�f�X�g���N�^

		  void Initialize();	 //������
		  void Update	 ();	 //�X�V
	const void Draw		 ()const;//�`��
	const bool IsDraw	 ()const;//�`�悵�Ă��邩
	const bool GetIsBackTitle()const;
		  void OnIsCallAlert();//�x�����Ă΂ꂽ��
private:
	/*���������֐�*/
	UIManager();//�R���X�g���N�^

	/*�ÓI�萔*/
	static constexpr int SELECT_UI_INDEX = 1;

	/*�����o�ϐ�*/
	Alert*			 alert;			//�x���N���X
	vector<SceneUI*> scene;			//�V�[��
	int				 sceneType;		//�V�[���̎��
	int				 frameTime;		//�����܂łɂ�����������
};

