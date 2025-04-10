//==================================================================
// @brief 注意喚起クラス
//==================================================================
#pragma once

class Alert
{
public:
	 Alert();
	~Alert();

		  void Update	();
	const void Draw		()const;
		  void OnIsCall	() { this->isCall = true; }

private:
	/*静的定数*/
	static constexpr int TEXT_COLOR = 0xffffff;

	/*構造体*/
	struct Image
	{
	public:
		Image(int _handle, vector<int> _drawRect)
		{
			this->handle	= _handle;
			this->drawRect	= _drawRect;
		}
		const void Draw()const;
	private:
		int handle;
		vector<int> drawRect;
	};
	struct Text
	{
	public:
		Text(vector<int> _position, string _text, int _color, int _handle) 
		{
			this->position	= _position;
			this->text		= _text;
			this->color		= _color;
			this->handle	= _handle;
		}
		const void Draw()const;
	private:
		vector<int> position;
		string text;
		int color;
		int handle;
	};

	/*メンバ変数*/
	bool		isDraw;			//描画するか
	bool		isCall;			//呼ばれたか
	list<Image> images;
	list<Text>	texts;
};

