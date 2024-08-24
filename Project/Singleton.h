//===========================================================
// @brief　シングルトンテンプレートクラス
// 参考:https://qiita.com/narumi_/items/b205e59e7fc81695f380
//===========================================================
#pragma once
template <typename T>
class Singleton
{
public:
	/*インスタンスの取得*/
	static T& GetInstance()
	{
		static T instance;
		return instance;
	}
protected:
	Singleton() { }
	virtual ~Singleton(){}

private:
	void operator=(const Singleton& _object){}//代入演算子禁止
	Singleton(const Singleton& _object){}//コピーコンストラクタ禁止
};

//#pragma once
//template <typename T>
//class Singleton
//{
//public:
//	/*インスタンスの作成*/
//	static void CreateInstance()
//	{
//		instance = new T();
//	}
//
//	/*インスタンスの削除*/
//	static void DeleteInstance()
//	{
//		if (instance != nullptr)
//		{
//			delete(instance);
//			instance = nullptr;
//		}
//	}
//
//	/*インスタンスを取得*/
//	static T& GetInstance()
//	{
//		return *instance;
//	}
//
//protected:
//	/*内部処理関数*/
//	Singleton() { }
//	virtual ~Singleton() {}
//	/*メンバ変数*/
//	static T* instance;
//};

