//=============================================
// @brief �{�X�p���`�U���A�N�V����
//=============================================
#pragma once

class Boss;
class BossAction;
class BossPunchAction : public BossAction
{
public:
	BossPunchAction();
	virtual ~BossPunchAction();

	void Initialize()				  override;//������
	void Update(Boss& _boss)	override;//�p�����[�^�[�̌v�Z
	void CalcParameter(const Boss& _boss) override;//�p�����[�^�[�̌v�Z

private:

};
