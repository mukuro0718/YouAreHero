//=============================================================================================
// @brief シーン系のUIクラス
// タイトルロゴ、ゲームオーバー/クリア
// リザルトの描画を行う
//=============================================================================================
#pragma once


class SceneUI abstract
{
public:
			 SceneUI(){}//コンストラクタ
	virtual ~SceneUI(){}//デストラクタ

	virtual void		Initialize	() abstract;	  //初期化
	virtual	void		Update		() abstract;	  //更新
	virtual const void	Draw		() const abstract;//描画
protected:
	/*構造体*/
	struct Image
	{
	public:
		static constexpr int MAX_ALPHA = 255;//最大アルファ
		int lx, ly, rx, ry;					 //座標
		int imageHandle;					 //画像ハンドル
		int alpha;							 //アルファ値
		bool isAddAlpha;
		const void Draw()const;
		void SetPosition(std::vector<int> _position);
		void AddAlpha(const int _addAlpha);
		void DecreaseAlpha(const int _decreaseAlpha);
		void FlashingAlpha(const int _addAlpha);
	};
};

