//========================================
// @brief �}�b�v�}�l�[�W���[�N���X
// �}�b�v�̊Ǘ����s��
//========================================
#pragma once
#include "Singleton.h"

class SceneUI;
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
private:
	/*���������֐�*/
	UIManager();//�R���X�g���N�^

	/*�����o�ϐ�*/
	SceneUI* scene;
	int		 frameTime;//�����܂łɂ�����������
};

