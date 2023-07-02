#pragma once
class Framework
{
public://メンバ関数
	virtual ~Framework() = default;
	//初期化
	virtual void Initialize();
	//終了
	virtual	void Finalize();
	//更新
	virtual	void Update();
	//描画
	virtual	void Draw() = 0;
	//実行
	void Run();
	//終了チェック
	virtual	bool IsEndRequst() { return isEndRequst; }
private:
	//終了フラグ
	bool isEndRequst = false;
};

