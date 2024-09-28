#include <DxLib.h>
#include "UseSTL.h"
#include "Image.h"

/// <summary>
/// コンストラクタ
/// </summary>
Image::Image(int _imageHandle)
	: imageHandle(_imageHandle)
	, alpha		 (0)
	, isAddAlpha (false)
	, interval	 (0)
{

}

/// <summary>
/// 描画
/// </summary>
const void Image::Draw()const
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, this->alpha);
	DrawExtendGraph(this->position[0], this->position[1], this->position[2], this->position[3], this->imageHandle, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, this->MAX_ALPHA);
}

/// <summary>
/// 座標の設定
/// </summary>
void Image::SetPosition(std::vector<int> _position)
{
	this->position = _position;
}

/// <summary>
/// フェードイン
/// </summary>
void Image::FadeIn(const int _addAlpha)
{
	/*アルファ増加フラグが立っていなければ早期リターン*/
	if (!this->isAddAlpha)return;

	/*アルファの増加*/
	this->alpha += _addAlpha;

	/*最大を超えないようにする*/
	if (this->alpha >= this->MAX_ALPHA)
	{
		this->alpha = this->MAX_ALPHA;
		this->isAddAlpha = false;
	}
}

/// <summary>
/// フェードアウト
/// </summary>
void Image::FadeOut(const int _decreaseAlpha)
{
	/*アルファ増加フラグが立っていたら早期リターン*/
	if (this->isAddAlpha)return;

	/*アルファの減少*/
	this->alpha -= _decreaseAlpha;

	/*０を下回らないようにする*/
	if (this->alpha < 0)
	{
		this->alpha = 0;
		this->isAddAlpha = true;
	}
}

/// <summary>
/// フェードインアウト
/// </summary>
void Image::FadeInOut(const int _addAlpha)
{

	if (this->isAddAlpha)
	{
		this->FadeIn(_addAlpha);
	}
	else
	{
		this->FadeOut(_addAlpha);
	}
}

/// <summary>
/// 画像の拡大
/// </summary>
bool Image::ExtendGraph(const std::vector<int> _targetPosition, const std::vector<int> _addValue)
{
	//拡大が収容した数
	int endCount = 0;

	for (int i = 0; i < position.size(); i++)
	{
		/*座標が目標地点になっていなかったら*/
		if (this->position[i] != _targetPosition[i])
		{
			//指定した増加量を座標に足す
			this->position[i] += _addValue[i];
			//増加量がプラスだったら
			if (_addValue[i] > 0)
			{
				//最大値を超えないようにする
				if (this->position[i] >= _targetPosition[i])
				{
					this->position[i] = _targetPosition[i];
					endCount++;
				}
			}
			//増加量がマイナスだったら
			else if (_addValue[i] < 0)
			{
				//０を下回らないようにする
				if (this->position[i] < _targetPosition[i])
				{
					this->position[i] = _targetPosition[i];
					endCount++;
				}
			}
		}
		else
		{
			endCount++;
		}
	}


	/*終了カウントが座標のサイズ+1と同じだったらすべての座標の移動が終了しているの*/
	if (endCount == this->position.size())
	{
		return true;
	}
	return false;
}

/// <summary>
/// 画像の拡大
/// </summary>
void Image::ScalingGraph(const std::vector<int> _extendPosition, const std::vector<int> _reducedPosition, const std::vector<int> _scalingValue)
{ 
	int  endCount		= 0;	//拡大が収容した数
	int  scalingValue	= 0;	//拡大値
	int  targetPosition	= 0;	//目標座標
	bool setFlag		= false;//endCountが最大の時にセットするフラグ

	for (int i = 0; i < position.size(); i++)
	{
		/*拡大するか*/
		if (this->isExtend)
		{
			scalingValue	= _scalingValue[i];
			targetPosition	= _extendPosition[i];
			setFlag			= false;
		}
		else
		{
			scalingValue	= -_scalingValue[i];
			targetPosition	= _reducedPosition[i];
			setFlag			= true;
		}
		//指定した増加量を座標に足す
		this->position[i] += scalingValue;
		//増加量がプラスだったら
		if (scalingValue > 0)
		{
			//最大値を超えないようにする
			if (this->position[i] >= targetPosition)
			{
				this->position[i] = targetPosition;
				endCount++;
			}
		}
		//増加量がマイナスだったら
		else if (scalingValue < 0)
		{
			//０を下回らないようにする
			if (this->position[i] < targetPosition)
			{
				this->position[i] = targetPosition;
				endCount++;
			}
		}
	}


	/*終了カウントが座標のサイズ+1と同じだったらすべての座標の移動が終了しているの*/
	if (endCount == this->position.size())
	{
		this->isExtend = setFlag;
	}
}