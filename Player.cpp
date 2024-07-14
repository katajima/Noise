#include <Player.h>
#include <cassert>

// 自キャラの初期化
void Player::Initialize(Model* model, uint32_t texturHandle, ViewProjection* viewProjection) {
	// NULLポインタチェック
	assert(model);

	// 引数として受け取ったデータをメンバ変数に記録する
	model_ = model;
	textureHandle_ = texturHandle;
	viewProjection_ = viewProjection;

	// ワールド変換の初期化
	worldTransform_.Initialize();
}

void Player::Update() { 
	worldTransform_.UpdateMatrix();
}

void Player::Draw() {

	// 3Dモデルを描画
	model_->Draw(worldTransform_, *viewProjection_, textureHandle_);
}