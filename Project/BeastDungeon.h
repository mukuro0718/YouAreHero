//===================================================
// @brief �r�[�X�g�_���W����
//===================================================
#pragma once

class Dungeon;
class BeastDungeon : public Dungeon
{
public:
			 BeastDungeon();//�R���X�g���N�^
	virtual ~BeastDungeon();//�f�X�g���N�^

		  void	Initialize() override;		//������
		  void	Update	  () override;		//�X�V
	const void	Draw	  () const override;//�`��
private:
	MV1_REF_POLYGONLIST refPoly;
};

