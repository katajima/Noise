#include "GameScene.h"
#include "AxisIndicator.h"
#include "ImGuiManager.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h"
#include <MathFunctions.h>
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {
	// 2D
	delete sprite_;
	// 3Dモデル
	delete blockModel_;
	// 3D天球
	delete modelSkydome_;
	// 天球クラス
	delete skydome_;
	// プレイヤークラス
	delete player_;

	// ワールドトランスフォーム
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();

	// デバックカメラ
	delete debugCamera_;

	delete noise;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

#pragma region block


	noise = new Noise();


	

	// 要素数を変更する
	// 列数を指定（縦方向のブロック数）
	worldTransformBlocks_.resize(kNumBlockVirtical);
	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
		// 1列の要素数を設定（横方向のぶ）
		worldTransformBlocks_[i].resize(kNumBlockHorizontal);
	}

		// ブロックの生成
	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
		for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {
			float xMap = (float)i / kNumBlockVirtical;
			float yMap = (float)j / kNumBlockHorizontal;
			perlin_grid[j][i] = noise->perlin_noise(xMap, yMap);
			//perlin_grid_size[j][i] = Nomalize(perlin_grid[j][i]) * 2.0f;
			perlin_grid_size[j][i] = float(int(perlin_grid[j][i]) * 1);
			worldTransformBlocks_[i][j] = new WorldTransform();
			worldTransformBlocks_[i][j]->Initialize();
			worldTransformBlocks_[i][j]->translation_.x = kBlockWidth * j;
			worldTransformBlocks_[i][j]->translation_.y = perlin_grid_size[j][i];
			worldTransformBlocks_[i][j]->translation_.z = kBlockHeight * i;
		}
	}
	// 3Dモデルの生成
	blockModel_ = Model::Create();

	blockGrass.reset(Model::CreateFromOBJ("grass", true));

#pragma endregion

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();

	// ビュープロジェクションの初期化

	// viewProjection_.farZ = 10;
	viewProjection_.Initialize();
	viewProjection_.translation_.x = 25;
	// プレイヤークラス生成
	player_ = new Player();

	// プレイヤーテクスチャ
	textureHandle_ = TextureManager::Load("sample.png");

	// プレイヤーモデル
	model_ = Model::Create();

	// プレイヤー初期化
	player_->Initialize(model_, textureHandle_, &viewProjection_);

	// 天球クラス生成
	skydome_ = new Skydome();

	// デバックカメラの生成
	debugCamera_ = new DebugCamera(500, 100);

	// 天球3Dモデルの生成
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);

	// 天球初期化
	skydome_->Initialize(modelSkydome_, &viewProjection_);
}

void GameScene::Update() {


	ImGui::Begin("noise");
	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
		for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {
			ImGui::Text("%5.2f", perlin_grid[j][i]);
		}
		ImGui::Text("\n");
	}
	ImGui::End();
	ImGui::Begin("noise2");
	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
		for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {
			ImGui::Text("%5.2f", perlin_grid_size[j][i]);
		}
		ImGui::Text("\n");
	}
	ImGui::End();
	ImGui::Begin("camera");
	ImGui::DragFloat3("translate", &viewProjection_.translation_.x, 1.0f);
	ImGui::DragFloat3("rotate", &viewProjection_.rotation_.x, 0.01f);
	ImGui::End();
	if (Input::GetInstance()->TriggerKey(DIK_0)) {
		// ブロックの生成
		for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
			for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {
				float xMap = (float)i / kNumBlockVirtical;
				float yMap = (float)j / kNumBlockHorizontal;
				perlin_grid[j][i] = noise->perlin_noise(xMap, yMap);
				perlin_grid_size[j][i] = float(int(perlin_grid[j][i]) * 1);

				worldTransformBlocks_[i][j]->translation_.x = kBlockWidth * j;
				worldTransformBlocks_[i][j]->translation_.z = kBlockHeight * i;
				worldTransformBlocks_[i][j]->translation_.y = perlin_grid_size[j][i];
			}
		}
	}

	noise->NoiseImGui();

	// プレイヤー更新処理
	player_->Update();

	// 天球更新処理
	skydome_->Update();

	// ブロックの更新
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)
				continue;
			// アフィン変換
			worldTransformBlock->matWorld_ = MakeAffineMatrixMatrix(worldTransformBlock->scale_, worldTransformBlock->rotation_, worldTransformBlock->translation_);

			// 定数バッファに転送する
			worldTransformBlock->TransferMatrix();
		}
	}

#ifdef _DEBUG
	if (input_->TriggerKey(DIK_SPACE)) {
		if (isDebugCameraActive_ == false) {

			isDebugCameraActive_ = true;

		} else if (isDebugCameraActive_ == true) {

			isDebugCameraActive_ = false;
		}
	}

#endif //  _DEBUG


	

	if (isDebugCameraActive_) {
		// デバックカメラの更新
		debugCamera_->Update();

		viewProjection_.matView = debugCamera_->GetViewProjection().matView;

		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;

		// ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	} else {

		// ビュープロジェクション行列の更新と転送
		viewProjection_.UpdateMatrix();
	}
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// 3Dモデル描画

	// 自キャラの描画
	player_->Draw();

	// スカイドーム描画
	// skydome_->Draw();

	// ブロック描画
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)
				continue;
			//blockModel_->Draw(*worldTransformBlock, viewProjection_);
			blockGrass->Draw(*worldTransformBlock, viewProjection_);
		}
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();

#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
