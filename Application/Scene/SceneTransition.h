#pragma once
#include<memory>
#include"Sprite.h"
/**
 * @file SceneTransition.h
 * @brief シーン遷移
 */
class SceneTransition{
public://静的メンバ関数
	static SceneTransition* GetInstance();
public://メンバ関数
	//初期化
	void Initialize();
	/// <summary>
	/// リセット
	/// </summary>
	/// <param name="isScaling_">拡縮フラグ</param>
	//void Reset(bool isScaling_);
	//更新
	void Updata();
	//描画
	void Draw();
	//演出をtrueにする
	void IsBlackOutTrue();
	//フラグの取得
	bool GetIsBlackOut() {
		return isBlackOut;
	}
	//フラグの取得
	bool GetIsLightChange() {
		return isLightChange;
	}
private:
	//演出フラグ
	//暗転
	float blackOutTime;
	const float blackOutTimer = 2.0f;
	float alpha;
	bool isBlackOut;
	bool isLightChange;
	std::unique_ptr<Sprite> transSprite;

};

