//=====================================================================
// @brief ビットフラグクラス
//=====================================================================
#pragma once
class BitFlag
{
public:
	void				SetFlag(const unsigned int _flag);		 //フラグをonにする
	void				ClearFlag(const unsigned int _flag);	 //フラグをoffにする
	const bool			CheckFlag(const unsigned int _flag)const;//フラグの確認
	const unsigned int	GetFlag()const { return this->flags; }	 //フラグの取得
private:
	unsigned int flags;//ビットフラグ
};

