//===============================================
// @brief チュートリアルUIクラス
//===============================================
#pragma once

class Image;
class SceneUI;
class TutorialUI : public SceneUI
{
public:
	TutorialUI();
	~TutorialUI();

	void Initialize()		 override;
	void Update()		 override;
	const void Draw()const override;
	const bool IsEnd()const override;
private:
	/*列挙体*/
	enum class FontType
	{
		MAIN = 0,
		SUB = 1
	};

	/*静的定数*/
	static constexpr int TEXT_COLOR = 0xdfffff;

	/*内部処理関数*/
	std::vector<int>  ReturnAddOffset(const std::vector<int> _position, const std::vector<int> _offset);
	bool			  IsPressButton();
	void			  ExtendWindow();
	void			  ReducedWindow();

	/*メンバ変数*/
	//Image*		window;				//window画像
	bool		isPrevPressButton;	//前にボタンが入力されているか
	bool		isExtendWindow;		//画像を拡大するか
	bool		isReducedWindow;	//縮小するか
	bool		isEnd;				//終了するか
	vector<int> fontHandle;			//フォント
};

