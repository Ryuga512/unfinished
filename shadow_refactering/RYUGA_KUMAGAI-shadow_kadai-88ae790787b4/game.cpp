//=============================================================================
//
// ゲーム画面処理 [game.cpp]
// Author : 
//
//=============================================================================
#include "game.h"
#include "renderer.h"
#include "camera.h"
#include "input.h"

#include "player.h"
#include "enemy.h"
#include "bullet.h"
#include "score.h"
#include "sound.h"
#include "meshfield.h"
#include "meshwall.h"
#include "shadow.h"
#include "tree.h"
#include "particle.h"
#include "StencilShadow.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************



//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
float x = 0.0f; //ライトの座標
float y = 0.0f;
float z = 0.0f;
bool flg = true; //足すときtrue
int time = 0;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitGame(void)
{
    time = 0;
    x = 0;
    y = 0;
    z = 0;
    InitStencilShadow();

    InitScore();

    //弾の初期化
    InitBullet();

    //影の初期化
    InitShadow();

    // プレイヤーの初期化
    InitPlayer();

    // エネミーの初期化
    InitEnemy();

    InitTree();

    //InitParticle();

    // フィールドの初期化
    InitMeshField(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 16, 16, 80.0f, 80.0f);

    // 壁の初期化
    InitMeshWall(D3DXVECTOR3(0.0f, 0.0f, WALL_UP), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
        D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, 80.0f, 80.0f);
    InitMeshWall(D3DXVECTOR3(WALL_LEFT, 0.0f, 0.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.50f, 0.0f),
        D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, 80.0f, 80.0f);
    InitMeshWall(D3DXVECTOR3(WALL_RIGHT, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.50f, 0.0f),
        D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, 80.0f, 80.0f);
    InitMeshWall(D3DXVECTOR3(0.0f, 0.0f, WALL_DOWN), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
        D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, 80.0f, 80.0f);

    // 壁(裏側用の半透明)
    InitMeshWall(D3DXVECTOR3(0.0f, 0.0f, 640.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
        D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 16, 2, 80.0f, 80.0f);
    InitMeshWall(D3DXVECTOR3(-640.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.50f, 0.0f),
        D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 16, 2, 80.0f, 80.0f);
    InitMeshWall(D3DXVECTOR3(640.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.50f, 0.0f),
        D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 16, 2, 80.0f, 80.0f);
    InitMeshWall(D3DXVECTOR3(0.0f, 0.0f, -640.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
        D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 16, 2, 80.0f, 80.0f);

    //地面
    //InitMeshWall(D3DXVECTOR3(0.0f, 0.0f, -640.0f), D3DXVECTOR3(D3DX_PI * 0.5f, 0.0f, 0.0f),
     //   D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 16, 16, 80.0f, 80.0f);


    // ライトを有効化
    SetLightEnable(true);

    // 背面ポリゴンをカリング
    SetCullingMode(CULL_MODE_BACK);

    return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitGame(void)
{
    // エネミーの終了処理
    UninitEnemy();

    UninitScore();

    // 地面の終了処理
    UninitMeshField();

    // 壁の終了処理
    UninitMeshWall();

    //UninitParticle();

    UninitTree();

    UninitBullet();

    // プレイヤーの終了処理
    UninitPlayer();

    // 影の終了処理
    UninitShadow();

    // カメラの終了処理
    UninitCamera();
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateGame(void)
{
    time++;
    if (x >= 80)
    {
        flg = false;
    }
    else if (x <= -80)
    {
        flg = true;
    }

    if (flg)
    {
        x += 0.001f;
        y += 0.001f;
        z += 0.001f;
    }
    else
    {
        x -= 0.001f;
        //y -= 0.001f;
        z -= 0.001f;
    }

    UpdateScore();

    // エネミーの更新処理
    UpdateEnemy();

    // 地面処理の更新
    UpdateMeshField();

    // 壁処理の更新
    UpdateMeshWall();

    UpdateTree();

    UpdateBullet();

    // 影の更新処理
    UpdateShadow();

    UpdateRenderer(x, y, z);

    // プレイヤーの更新処理
    UpdatePlayer();


    HitCheck();

    if (time / 60 == 30)
    {
        SetMode(MODE_RESULT);
    }
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawGame(void)
{
    // 地面の描画処理
    DrawMeshField();

    DrawBullet();


    // 壁の描画処理
    DrawMeshWall();

    DrawTree();
    DrawShadow();
    // プレイヤーの描画処理
    DrawPlayer();

    // エネミーの描画処理
    DrawEnemy();

    polygon();

    //DrawParticle();

    //2Dの描画処理をする（ライティングを無効）
    SetLightEnable(false);

    //Z比較なし
    SetDepthEnable(false);

    //この間に2Dの処理を書く
    DrawScore();

    //
    SetLightEnable(true);

    SetDepthEnable(true);

}
