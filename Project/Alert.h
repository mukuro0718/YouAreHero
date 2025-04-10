//==================================================================
// @brief ���ӊ��N�N���X
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
	/*�ÓI�萔*/
	static constexpr int TEXT_COLOR = 0xffffff;

	/*�\����*/
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

	/*�����o�ϐ�*/
	bool		isDraw;			//�`�悷�邩
	bool		isCall;			//�Ă΂ꂽ��
	list<Image> images;
	list<Text>	texts;
};

