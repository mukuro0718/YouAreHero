//===========================================
// @brief ボスの状態を表すUI
//===========================================
#pragma once
class BossStateUI
{
public:
	 BossStateUI();//コンストラクタ
	~BossStateUI();//デストラクタ

	const void	Draw		()const;//描画
private:
	vector<int> imageHandle;
};

