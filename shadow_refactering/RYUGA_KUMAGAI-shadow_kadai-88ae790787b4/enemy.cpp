//=============================================================================
//
// エネミーモデル処理 [enemy.cpp]
// Author : GP11A132 12 熊谷隆我
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "enemy.h"
#include "shadow.h"
#include "renderer.h"
#include "StencilShadow.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MODEL_ENEMY		"data/MODEL/miku_01.obj"		// 読み込むモデル名

#define	VALUE_MOVE		(5.0f)							// 移動量
#define	VALUE_ROTATE	(D3DX_PI * 0.02f)				// 回転量

#define ENEMY_SHADOW_SIZE	(0.4f)						// 影の大きさ

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ENEMY			g_Enemy[MAX_ENEMY];				// エネミー
static DX11_MODEL       g_Shadow[MAX_ENEMY];


// エネミーの線形移動用の移動座標テーブル
static D3DXVECTOR3	g_MoveTbl[] = {
    D3DXVECTOR3(0.0f, 7.0f, 0.0f),
    D3DXVECTOR3(-200.0f, 7.0f, 0.0f),
    D3DXVECTOR3(-200.0f, 7.0f, 200.0f),
};


// エネミーの線形移動用の移動スピードテーブル
static float	g_MoveSpd[] = {
    0.01f,
    0.1f,
    0.005f,
};
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitEnemy(void)
{
    float x, z;
	for (int i = 0; i < MAX_ENEMY; i++)
	{
        z = rand() % (WALL_UP - WALL_DOWN) + WALL_DOWN;
        x = rand() % (WALL_RIGHT - WALL_LEFT) + WALL_LEFT;
		LoadModel(MODEL_ENEMY, &g_Enemy[i].model);
        InitStencilShadow(MODEL_ENEMY, &g_Shadow[i]);

		g_Enemy[i].pos = D3DXVECTOR3(x, 7.0f, z);
		g_Enemy[i].rot = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);
		g_Enemy[i].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
        
		g_Enemy[i].spd = 0.0f;		// 移動スピードクリア

		D3DXVECTOR3 pos      = g_Enemy[i].pos;
		pos.y                = 0.0f;
        g_Enemy[i].use       = true;
        g_Enemy[i].stop      = false;

		g_Enemy[i].shadowIdx = CreateShadow(pos, ENEMY_SHADOW_SIZE, ENEMY_SHADOW_SIZE);

        g_Enemy[i].radius    = 10.0;
        g_Enemy[i].time      = 0.0f;
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEnemy(void)
{
	// モデルの解放処理
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		UnloadModel(&g_Enemy[i].model);
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateEnemy(void)
{

    for (int i = 0; i < MAX_ENEMY; i++)
    {
        if (!g_Enemy[i].use) continue;
        //エネミーを動かす場合は影も合わせて動かす
        D3DXVECTOR3 pos = g_Enemy[i].pos;
        pos.y = 0.0f;
        SetPositionShadow(g_Enemy[i].shadowIdx, pos);
        /*LIGHT *Light = GetLight();*/
        //CreateStencilShadow(MODEL_ENEMY, &g_Enemy[i].model, *Light);

    }
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEnemy(void)
{
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

	// カリング無効
	SetCullingMode(CULL_MODE_NONE);

	for (int i = 0; i < MAX_ENEMY; i++)
	{
        if (g_Enemy[i].use)
        {
            if (g_Enemy[i].stop)
            {
                SetBlendState(BLEND_MODE_SUBTRACT);
            }
            // ワールドマトリックスの初期化
            D3DXMatrixIdentity(&g_Enemy[i].mtxWorld);

            // スケールを反映
            D3DXMatrixScaling(&mtxScl, g_Enemy[i].scl.x, g_Enemy[i].scl.y, g_Enemy[i].scl.z);
            D3DXMatrixMultiply(&g_Enemy[i].mtxWorld, &g_Enemy[i].mtxWorld, &mtxScl);

            // 回転を反映
            D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Enemy[i].rot.y, g_Enemy[i].rot.x, g_Enemy[i].rot.z);
            D3DXMatrixMultiply(&g_Enemy[i].mtxWorld, &g_Enemy[i].mtxWorld, &mtxRot);

            // 移動を反映
            D3DXMatrixTranslation(&mtxTranslate, g_Enemy[i].pos.x, g_Enemy[i].pos.y, g_Enemy[i].pos.z);
            D3DXMatrixMultiply(&g_Enemy[i].mtxWorld, &g_Enemy[i].mtxWorld, &mtxTranslate);

            // ワールドマトリックスの設定
            SetWorldMatrix(&g_Enemy[i].mtxWorld);


            // モデル描画
            DrawModel(&g_Enemy[i].model);

            //影描画
            LIGHT *Light = GetLight();
            CreateStencilShadow(g_Enemy[i].mtxWorld, *Light, &g_Shadow[i]);
            DrawStencilShadow(&g_Shadow[i], g_Enemy[i].mtxWorld);
            SetBlendState(BLEND_MODE_ALPHABLEND);
        }
	}

	// カリング設定を戻す
	SetCullingMode(CULL_MODE_BACK);
}

//=============================================================================
// エネミーの取得
//=============================================================================
ENEMY *GetEnemy()
{
	return &(g_Enemy[0]);
}
