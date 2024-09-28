//===============================================
// @brief リザルトUIクラス
//===============================================
#pragma once

class Image;
class SceneUI;
class GameClearUI : public SceneUI
{
public:
	GameClearUI();
	~GameClearUI();

		  void Initialize()		 override;
		  void Update	 ()		 override;
	const void Draw		 ()const override;
	const bool IsEnd	 ()const override;
private:
	/*静的定数*/
	static constexpr int INDEX = 2;
	static constexpr int TEXT_COLOR = 0xdfffff;

	enum class FontType
	{
		HEADER = 0,
		MAIN = 1,
	};
	enum class StarType
	{
		NONE = -1,
		SILVER = 0,
		GOLD = 1,
	};

	bool IsPressButton();
	void AddStarIndex();
	vector<int> AddPositionOffset(const vector<int> _position, const vector<int> _offset);
	void SetStarHandle();

	Image* window;
	vector<Image*> star;
	vector<Image*> starFrame;
	vector<int> fontHandle;
	vector<int> starType;
	bool isPrevPressButton;
	bool isEnd;
	bool isEndExtend;
	bool isRedused;
	int starIndex;
	int frameCount;
};

