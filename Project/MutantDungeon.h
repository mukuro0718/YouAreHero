//===================================================
// @brief �~���[�^���g�_���W����
//===================================================
#pragma once

class Dungeon;
class MutantDungeon : public Dungeon
{
	public:
			 MutantDungeon();//�R���X�g���N�^
	virtual ~MutantDungeon();//�f�X�g���N�^

		  void Initialize() override;		//������
		  void Update	 () override;		//�X�V
	const void Draw		 () const override;	//�`��
private:
	MV1_REF_POLYGONLIST refPoly;
};

