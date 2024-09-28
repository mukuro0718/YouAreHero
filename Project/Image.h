//===================================================
// @brief 画像クラス
//===================================================
#pragma once
class Image final
{
public:
	 Image(int _imageHandle);//コンストラクタ
	~Image(){}//デストラクタ

	const void	Draw()const;						//描画
	void		SetPosition(std::vector<int> _position);	//座標の設定

	bool ExtendGraph(const std::vector<int> _targetPosition, const std::vector<int> _addValue);
	void ScalingGraph(const std::vector<int> _extendPosition, const std::vector<int> _reducedPosition, const std::vector<int> _scalingValue);
	void		FadeIn(const int _addAlpha);//フェードイン
	void		FadeOut(const int _addAlpha);//フェードアウト
	void		FadeInOut(const int _addAlpha);//フェードインアウト
	
	/*メンバ変数*/
	static constexpr int MAX_ALPHA = 255;//最大アルファ
	vector<int>  position;//座標
	int  alpha;			//アルファ値
	bool isAddAlpha;	//アルファを追加するか（false	なら減少のみ許可される）
	int interval;		//インターバル
	int  imageHandle;	//画像ハンドル
	bool isExtend;
};

