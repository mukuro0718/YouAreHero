//===================================================
// @brief �h���S���_���W����
//===================================================
#pragma once

class Dungeon;
class DragonDungeon : public Dungeon
{
public:
			 DragonDungeon();//�R���X�g���N�^
	virtual ~DragonDungeon();//�f�X�g���N�^

		  void	Initialize() override;		//������
		  void	Update	  () override;		//�X�V
	const void	Draw	  () const override;//�`��
private:
	MV1_REF_POLYGONLIST refPoly;
};

