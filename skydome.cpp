#include<skydome.h>
#include <cassert>

//初期化
void Skydome::Initialize(Model* model ,ViewProjection* viewProjection) {
	// NULLポインタチェック
	//assert(model);
	


	// 引数として受け取ったデータをメンバ変数に記録する
	model_ = model;
	
	// ビュープロジェクション
	viewProjection_ = viewProjection;
	
	//ワールドトランスフォーム
	worldTransform_.Initialize();
}
//更新
void Skydome::Update() { worldTransform_.TransferMatrix(); }
//描画
void Skydome::Draw() {model_->Draw(worldTransform_, *viewProjection_); }
