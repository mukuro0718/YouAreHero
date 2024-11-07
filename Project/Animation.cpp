#include <DxLib.h>
#include "UseSTL.h"
#include "Animation.h"

/// <summary>
/// コンストラクタ
/// </summary>
Animation::Animation()
	: animationTotalTime		(0.0f)
	, animationPlayTime			(0.0f)
	, prevAnimationPlayTime		(0.0f)
	, animationRate				(0.0f)
	, addAnimationRate			(0.0f)
	, isChange					(false)
	, animationAttachIndex		(0)
	, prevAnimationAttachIndex	(0)
	, prevAnimation				(0)
	, nowAnimation				(0)
{
	this->addAnimationRate = this->ADD_ANIMATION_RATE;
}

/// <summary>
/// デストラクタ
/// </summary>
Animation::~Animation()
{
	this->animationHandle.clear();
	this->animationIndex.clear();
}

/// <summary>
/// アニメーションの追加
/// </summary>
void Animation::Add(const int _animationHandle,const int _animatinoIndex)
{
	this->animationHandle.emplace_back(_animationHandle);
	this->animationIndex.emplace_back(_animatinoIndex);
}

/// <summary>
/// アニメーションのアタッチ
/// </summary>
void Animation::Attach(int* modelHandle)
{
	/*アニメーションのデタッチ*/
	MV1DetachAnim(*modelHandle, this->prevAnimationAttachIndex);
	MV1DetachAnim(*modelHandle, this->animationAttachIndex);

	/*アニメーションのアタッチ*/
	this->prevAnimationAttachIndex = MV1AttachAnim(*modelHandle, this->animationIndex[this->prevAnimation], this->animationHandle[this->prevAnimation], TRUE);
	this->animationAttachIndex = MV1AttachAnim(*modelHandle, this->animationIndex[this->nowAnimation], this->animationHandle[this->nowAnimation], TRUE);
	
	/*アニメーションの総再生時間を設定*/
	this->animationTotalTime = MV1GetAttachAnimTotalTime(*modelHandle, this->animationAttachIndex);
	
	/*アニメーションのブレンド率を初期化*/
	this->animationRate = 0.0f;

	/*アニメーション再生時間を設定*/
	MV1SetAttachAnimTime(*modelHandle, this->animationAttachIndex, this->animationPlayTime);
}
/// <summary>
/// アニメーションの再生
/// </summary>
void Animation::Play(int* _modelHandle, const int _nextAnimation, const float _animationPlayTime)
{
	/*もし今までアタッチしていたアニメーションと次のアニメーションが違うなら*/
	if (this->nowAnimation != _nextAnimation)
	{
		this->prevAnimation = this->nowAnimation;
		this->nowAnimation = _nextAnimation;
		this->isChange = false;
		this->prevAnimationPlayTime = this->animationPlayTime;
		this->animationPlayTime = 0.0f;
		//アニメーションのアタッチ
		Attach(_modelHandle);
	}

	/*再生時間がアニメーションの総再生時間に達したら再生時間を０に戻す*/
	if (this->animationPlayTime >= this->animationTotalTime)
	{
		this->animationPlayTime = 0.0f;
		MV1SetAttachAnimTime(*_modelHandle, this->animationAttachIndex, this->animationPlayTime);
	}

	/*アニメーション変更許可フラグが立っていたら下す（本来ならすでに降りているはずだが、同じアニメーションを再生しようとすると立ったままになる）*/
	if (this->isChange)
	{
		this->isChange = false;
	}


	/*アニメーションのブレンド率をセット*/
	//ブレンド率が1以上だったら
	if (this->animationRate >= 1.0f)
	{
		//前のアタッチしていたアニメーションをデタッチする
		MV1DetachAnim(*_modelHandle, this->prevAnimationAttachIndex);
	}
	else
	{
		//ブレンド率を増加
		this->animationRate += this->addAnimationRate;
		MV1SetAttachAnimBlendRate(*_modelHandle, this->prevAnimationAttachIndex, 1.0f - this->animationRate);
		MV1SetAttachAnimBlendRate(*_modelHandle, this->animationAttachIndex, this->animationRate);
		MV1SetAttachAnimTime(*_modelHandle, this->prevAnimationAttachIndex, this->prevAnimationPlayTime);
	}

	/*アニメーション再生時間を進める*/
	this->animationPlayTime += _animationPlayTime;
	MV1SetAttachAnimTime(*_modelHandle, this->animationAttachIndex, this->animationPlayTime);


	/*再生時間がアニメーションの総再生時間に達したら変更フラグを立てる*/
	if (this->animationPlayTime >= this->animationTotalTime)
	{
		this->isChange = true;
	}
}
