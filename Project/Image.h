#pragma once
class Image
{
public:
	 Image(const int _imageHandle);//ƒRƒ“ƒXƒgƒ‰ƒNƒ^
	~Image(){}//ƒfƒXƒgƒ‰ƒNƒ^

	/*•`‰æ*/
	const void Draw			()const;//’Êí•`‰æ
	const void DrawExtend	()const;//Šg‘å‚µ‚Ä•`‰æ
	const void DrawBillboard()const;//‚RD‹óŠÔ‚É•`‰æ

	/*\‘¢‘Ì*/
	//’Êí•`‰æ‚Ég—p‚·‚é\‘¢‘Ì
	struct Normal
	{
		int x, y;
	};
	//Šg‘å•`‰æ‚Ég—p‚·‚é\‘¢‘Ì
	struct Extend
	{
		int leftX, leftY, rightX, rightY;
	};
	//‚RD‹óŠÔ•`‰æ‚Ég—p‚·‚é\‘¢‘Ì
	struct Billboard
	{
		VECTOR position;
		float centerX, centerY;
		float size;
		float angle;
	};

	/*getter*/
	const Normal	GetNormal	()const { return this->normal; }	//’Êí
	const Extend	GetExtend	()const { return this->extend; }	//Šg‘å
	const Billboard GetBillboard()const { return this->billboard; }	//‚RD‹óŠÔ
	
	/*setter*/
	void SetNormal	 (const Normal _normal)		  { this->normal = _normal; }		//’Êí
	void SetExtend	 (const Extend _extend)		  { this->extend = _extend; }		//Šg‘å
	void SetBillboard(const Billboard _billboard) { this->billboard = _billboard; }	//‚RD‹óŠÔ	
private:
	/*Ã“I’è”*/
	static constexpr int MAX_ALPHA = 255;
	static constexpr int MIN_ALPHA = 0;

	/*ƒƒ“ƒo•Ï”*/
	Normal		normal;
	Extend		extend;
	Billboard	billboard;
	int			imageHandle;
	int			alpha;
};

