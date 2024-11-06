//===========================================================================
//@brief アニメーションクラス
//===========================================================================

#pragma once

class Animation
{
public:
    Animation();    //コンストラクタ
    ~Animation();   //デストラクタ

    void Add        (const int _animationHandle, const int _animatinoIndex);                          //アニメーションの追加
    void Attach     (int* _modelHandle);                                                              //アニメーションのアタッチ
    void Play       (int* _modelHandle, const int _nextAnimation, const float _animationPlayTime);;   //アニメーションの再生時間
    void SetAddRate (const float _animationRate) { this->addAnimationRate = _animationRate; }         //アニメーションブレンド増加量の設定
    
    /*getter*/
    const bool GetIsChangeAnim()const { return isChange; }//アニメーションの変更フラグのgetter
private:
    /*静的定数*/
    const float ADD_ANIMATION_RATE = 0.05f;//アニメーションブレンド増加量

    /*メンバ変数*/
    std::vector<int> animationHandle;           //アニメーションのモデルのハンドル
    std::vector<int> animationIndex;            //使用したいアニメーションの番号
    float            animationTotalTime;        //アニメーション総再生時間
    float            animationPlayTime;         //アニメーション再生時間
    float            animationRate;             //アニメーションのブレンド率
    float            addAnimationRate;          //アニメーションブレンド率増加量
    bool             isChange;                  //アニメーション変更フラグ（true:変更可能 false:不可）
    int              animationAttachIndex;      //アニメーションアタッチ番号
    int              prevAnimationAttachIndex;  //以前のアニメーションアタッチ番号
    int              prevAnimation;             //前のアニメーション
    int              nowAnimation;              //現在のアニメーション
};