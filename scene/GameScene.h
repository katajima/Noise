#pragma once

#include "Audio.h"
#include "DebugCamera.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Player.h"
#include "skydome.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include<vector>
#include"Noise.h"
#include"camera.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	
	//プレイヤー
	Player* player_ = nullptr;
	
	//プレイヤーモデル
	Model* model_ = nullptr;
	
	//プレイヤーテクスチャ
	uint32_t textureHandle_ = 0;

	// スプライト
	Sprite* sprite_ = nullptr;

	// ブロックの3Dモデル
	Model* blockModel_ = nullptr;

	// ワールドトランスフォーム
	WorldTransform worldTransform_;

	// ビュープロジェクション
	ViewProjection viewProjection_;

	// ブロック用のワールドトランスフォーム。
	// 複数並べるために配列にする
	std::vector<std::vector<std::vector<WorldTransform*>>> worldTransformBlocks_;
	//std::vector<std::vector<WorldTransform*>> worldTransformBlocksWater_;

	//デバックカメラ有効
	bool isDebugCameraActive_ = false;

	// デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;

	// 天球の3Dモデル
	Model* modelSkydome_ = nullptr;
	// 天球クラス
	Skydome* skydome_ = nullptr;


	// ブロック1個分の横幅
	const float kBlockWidth = 1.0f;
	const float kBlockHeight = 1.0f;
	// 要素数
	const uint32_t kNumBlockVertical = 50;   // 例として20を使用
	const uint32_t kNumBlockHorizontal = 50; // 横方向の要素数（Z軸）
	const uint32_t kNumBlockDepth = 20;      // 深さ方向の要素数（Y軸）

	int countX = 0;
	int countY = 0;
	int countZ = 0;


	Noise* noise = nullptr; 

	float perlin_grid[50][50];
	float perlin_grid_size[50][50];

	float perlin_grid_ringt[50][50];
	float perlin_grid_left[50][50];
	float perlin_grid_front[50][50];
	float perlin_grid_back[50][50];

	std::unique_ptr<Model> blockGrass;
	std::unique_ptr<Model> blockStone;
	std::unique_ptr<Model> blockWater;
	//int Seed = 1;

	Frustum frustum;
};
