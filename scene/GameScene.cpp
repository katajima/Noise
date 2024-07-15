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
	// ポインタの削除
	for (std::vector<std::vector<WorldTransform*>>& worldTransformBlockLayer : worldTransformBlocks_) {
		for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlockLayer) {
			for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
				delete worldTransformBlock;
			}
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

	// worldTransformBlocks_のサイズをY軸方向にリサイズ
	worldTransformBlocks_.resize(kNumBlockVertical);

	for (uint32_t i = 0; i < kNumBlockVertical; ++i) {
		// 1列の要素数を設定（X方向）
		worldTransformBlocks_[i].resize(kNumBlockHorizontal);
		for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {
			// 各列の奥行き方向の要素数を設定（Z方向）
			worldTransformBlocks_[i][j].resize(kNumBlockDepth);
		}
	}

	// ブロックの生成
	for (uint32_t i = 0; i < kNumBlockVertical; ++i) {

		for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {
			for (uint32_t k = 0; k < kNumBlockDepth; ++k) {
				float xMap = (float)i / kNumBlockVertical;
				float zMap = (float)j / kNumBlockHorizontal;
				perlin_grid[j][i] = noise->perlin_noise2(xMap, zMap);
				
				perlin_grid_size[j][i] = float(int(perlin_grid[j][i]) * 1);

				worldTransformBlocks_[j][i][k] = new WorldTransform();
				worldTransformBlocks_[j][i][k]->Initialize();
				worldTransformBlocks_[j][i][k]->translation_.x = kBlockWidth * j;
				worldTransformBlocks_[j][i][k]->translation_.y = perlin_grid_size[j][i] + -float(k);
				worldTransformBlocks_[j][i][k]->translation_.z = kBlockHeight * i;
			}
		}
	}

	for (uint32_t i = 0; i < kNumBlockVertical; ++i) {

		for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {
			if (j != kNumBlockHorizontal) {

				perlin_grid_ringt[j][i] = noise->neighbor_deviation(perlin_grid_size[j][i], perlin_grid_size[j + 1][i]);
			}
			if (j != 0) {

				perlin_grid_left[j][i] = noise->neighbor_deviation(perlin_grid_size[j][i], perlin_grid_size[j - 1][i]);
			}
			if (i != kNumBlockVertical) {

				perlin_grid_front[j][i] = noise->neighbor_deviation(perlin_grid_size[j][i], perlin_grid_size[j][i + 1]);
			}
			if (i != 0) {

				perlin_grid_back[j][i] = noise->neighbor_deviation(perlin_grid_size[j][i], perlin_grid_size[j][i - 1]);
			}
		}
	}
	// 3Dモデルの生成
	blockModel_ = Model::Create();

	blockGrass.reset(Model::CreateFromOBJ("grass", true));
	blockStone.reset(Model::CreateFromOBJ("stone", true));
	blockWater.reset(Model::CreateFromOBJ("water", true));
	blockSoil.reset(Model::CreateFromOBJ("soil", true));

#pragma endregion

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();

	// ビュープロジェクションの初期化

	// viewProjection_.farZ = 10;
	viewProjection_.Initialize();
	viewProjection_.translation_.x = 25;
	viewProjection_.translation_.y = 55;
	viewProjection_.rotation_.x = 0.73f;
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

#pragma region MyRegion

	ImGui::Begin("noise");
	for (uint32_t i = 0; i < kNumBlockVertical; ++i) {
		for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {
			ImGui::Text("%5.2f", perlin_grid[j][i]);
		}
	}
	ImGui::End();
	ImGui::Begin("noise2");
	for (uint32_t i = 0; i < kNumBlockVertical; ++i) {
		for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {
			ImGui::Text("%5.2f", perlin_grid_size[j][i]);
		}
	}
	ImGui::End();

	/*ImGui::Begin("right");
	for (uint32_t i = 0; i < kNumBlockVertical; ++i) {
	    for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {
	        ImGui::Text("%5.2f", perlin_grid_ringt[j][i]);
	    }
	    ImGui::Text("\n");
	}
	ImGui::End();
	ImGui::Begin("left");
	for (uint32_t i = 0; i < kNumBlockVertical; ++i) {
	    for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {
	        ImGui::Text("%5.2f", perlin_grid_left[j][i]);
	    }
	    ImGui::Text("\n");
	}
	ImGui::End();
	ImGui::Begin("front");
	for (uint32_t i = 0; i < kNumBlockVertical; ++i) {
	    for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {
	        ImGui::Text("%5.2f", perlin_grid_front[j][i]);
	    }
	    ImGui::Text("\n");
	}
	ImGui::End();
	ImGui::Begin("back");
	for (uint32_t i = 0; i < kNumBlockVertical; ++i) {
	    for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {
	        ImGui::Text("%5.2f", perlin_grid_back[j][i]);
	    }
	    ImGui::Text("\n");
	}
	ImGui::End();*/

#pragma endregion

	ImGui::Begin("camera");
	ImGui::DragFloat3("translate", &viewProjection_.translation_.x, 1.0f);
	ImGui::DragFloat3("rotate", &viewProjection_.rotation_.x, 0.01f);
	ImGui::Text("set [R key]\n");
	ImGui::End();

	setTimer--;
	if (Input::GetInstance()->TriggerKey(DIK_R) && setTimer <= 0) {
		//setTimer = 40;
		// ブロックの生成
		for (uint32_t i = 0; i < kNumBlockVertical; ++i) {

			for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {
				for (uint32_t k = 0; k < kNumBlockDepth; ++k) {
					float xMap = (float)i / kNumBlockVertical;
					float zMap = (float)j / kNumBlockHorizontal;
					perlin_grid[j][i] = noise->perlin_noise2(xMap, zMap);
					perlin_grid_size[j][i] = float(int(perlin_grid[j][i]) * 1);
					worldTransformBlocks_[j][i][k]->translation_.x = kBlockWidth * j;
					worldTransformBlocks_[j][i][k]->translation_.y = perlin_grid_size[j][i] + -float(k);
					worldTransformBlocks_[j][i][k]->translation_.z = kBlockHeight * i;
				}
			}
		}

		for (uint32_t i = 0; i < kNumBlockVertical; ++i) {

			for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {
				if (j != kNumBlockHorizontal) {

					perlin_grid_ringt[j][i] = noise->neighbor_deviation(perlin_grid_size[j][i], perlin_grid_size[j + 1][i]);
				}
				if (j != 0) {

					perlin_grid_left[j][i] = noise->neighbor_deviation(perlin_grid_size[j][i], perlin_grid_size[j - 1][i]);
				}
				if (i != kNumBlockVertical) {

					perlin_grid_front[j][i] = noise->neighbor_deviation(perlin_grid_size[j][i], perlin_grid_size[j][i + 1]);
				}
				if (i != 0) {

					perlin_grid_back[j][i] = noise->neighbor_deviation(perlin_grid_size[j][i], perlin_grid_size[j][i - 1]);
				}
			}
		}
	}

	noise->NoiseImGui();

	// プレイヤー更新処理
	//player_->Update();

	// 天球更新処理
	//skydome_->Update();

	// ブロックの更新
	for (std::vector<std::vector<WorldTransform*>>& worldTransformBlockLayer : worldTransformBlocks_) {
		for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlockLayer) {
			for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
				if (!worldTransformBlock)
					continue;

				// アフィン変換
				worldTransformBlock->matWorld_ = MakeAffineMatrixMatrix(worldTransformBlock->scale_, worldTransformBlock->rotation_, worldTransformBlock->translation_);

				// 定数バッファに転送する
				worldTransformBlock->TransferMatrix();
			}
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

	
	frustum.ConstructFrustum(1000.0f, &viewProjection_);

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
	//player_->Draw();

	// スカイドーム描画
	// skydome_->Draw();

	// ブロック描画
	countX = 0;
	countY = 0;
	countZ = 0;
	for (const std::vector<std::vector<WorldTransform*>>& worldTransformBlockLayer : worldTransformBlocks_) {
		++countX;
		for (const std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlockLayer) {
			++countZ;
			for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
				++countY;
				if (!worldTransformBlock)
					continue;

				// if (!((countX == 1 || countX == int(kNumBlockHorizontal)) || (countZ == 1 || countZ == int(kNumBlockVertical)))) {
				if ((countY < perlin_grid_ringt[countX][countZ])) {
				} else if ((countY <= perlin_grid_left[countX][countZ] + 2)) {
				} else if ((countY <= perlin_grid_front[countX][countZ] + 2)) {
				} else if ((countY <= perlin_grid_front[countX][countZ] + 2)) {
				} else {

					continue;
				}
				//}
				if (frustum.CheckCube(worldTransformBlock, kBlockWidth / 2.0f)) {
					// ブロックの描画
					if (countY >= 4) {

						blockStone->Draw(*worldTransformBlock, viewProjection_);
					} else {
						if (countY == 1) {

							blockGrass->Draw(*worldTransformBlock, viewProjection_);
						} else {
							blockSoil->Draw(*worldTransformBlock, viewProjection_);
						}

					}
					if (perlin_grid_size[countZ][countX] <= 0) {
						//blockWater->Draw(*worldTransformBlock, viewProjection_);
					}
				}
			}
			countY = 0;
		}
		countZ = 0;
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
