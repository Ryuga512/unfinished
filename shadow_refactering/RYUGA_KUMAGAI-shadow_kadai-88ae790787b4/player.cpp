//=============================================================================
//
// モデル処理 [player.cpp]
// Author : GP11A132 12 熊谷隆我
//
//=============================================================================
#pragma once

#include "main.h"
#include "input.h"
#include "camera.h"
#include "player.h"
#include "shadow.h"
#include "renderer.h"
#include "bullet.h"
#include "StencilShadow.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MODEL_PLAYER		"data/MODEL/miku_01.obj"		// 読み込むモデル名
#define	MODEL_PLAYER_PARTS	"data/MODEL/miku_02.obj"		// 読み込むモデル名

#define	VALUE_MOVE			(2.0f)							// 移動量
#define VALUE_MOVE_ZOOM     (1.0f)                          // ズーム時の移動量
#define	VALUE_ROTATE		(D3DX_PI * 0.02f)				// 回転量

#define PLAYER_SHADOW_SIZE	(0.4f)							// 影の大きさ

#define PLAYER_PARTS_MAX	(2)								// プレイヤーのパーツの数

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static PLAYER				g_Player;	// プレイヤー
static PLAYER				g_Parts[PLAYER_PARTS_MAX];		// プレイヤーのパーツ用
float roty = D3DX_PI;


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitPlayer(void)
{
	LoadModel(MODEL_PLAYER, &g_Player.model);
    InitStencilShadow(MODEL_PLAYER, &g_shadow[MAX_STENCIL_SHADOW]);
	g_Player.pos = D3DXVECTOR3(0.0f, 7.0f, -50.0f);
	g_Player.rot = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);
	g_Player.scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
    g_Player.axis_x = D3DXVECTOR3(1, 0, 0);
    g_Player.axis_y = D3DXVECTOR3(0, 1, 0);
    g_Player.axis_y = D3DXVECTOR3(0, 0, 1);
	g_Player.spd = 0.0f;		// 移動スピードクリア

	D3DXVECTOR3 pos = g_Player.pos;
	pos.y = 0.0f;
    g_Player.shadowIdx = MAX_STENCIL_SHADOW;

    g_Player.use = true;

    g_Player.radius = 3.5;


	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPlayer(void)
{
	// モデルの解放処理
	UnloadModel(&g_Player.model);
    UnloadStencilShadow(&g_shadow[MAX_STENCIL_SHADOW]);
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePlayer(void)
{
	CAMERA *cam = GetCamera();

    

	g_Player.spd *= 0.0f;
    if (!IsMouseRightPressed())
    {
        // 移動処理
        if (GetKeyboardPress(DIK_A) || IsButtonPressed(0, BUTTON_LEFT))
        {
            g_Player.spd = VALUE_MOVE;
            roty = D3DX_PI / 2.0f;
        }

        else if (GetKeyboardPress(DIK_D) || IsButtonPressed(0, BUTTON_RIGHT))
        {
            g_Player.spd = VALUE_MOVE;
            roty = D3DX_PI * 1.5f;
        }
        else if (GetKeyboardPress(DIK_W) || IsButtonPressed(0, BUTTON_UP))
        {
            g_Player.spd = VALUE_MOVE;
            roty = D3DX_PI;
        }
        else if (GetKeyboardPress(DIK_S) || IsButtonPressed(0, BUTTON_DOWN))
        {
            g_Player.spd = VALUE_MOVE;
            roty = 0.0f;
        }
        if (GetKeyboardPress(DIK_W) && GetKeyboardPress(DIK_A) || IsButtonPressed(0, BUTTON_UP) && IsButtonPressed(0, BUTTON_LEFT))
        {
            g_Player.spd = VALUE_MOVE;
            roty = D3DX_PI - D3DX_PI * 0.25f;
        }
        if (GetKeyboardPress(DIK_W) && GetKeyboardPress(DIK_D) || IsButtonPressed(0, BUTTON_UP) && IsButtonPressed(0, BUTTON_RIGHT))
        {
            g_Player.spd = VALUE_MOVE;
            roty = D3DX_PI + D3DX_PI * 0.25f;
        }
        if (GetKeyboardPress(DIK_S) && GetKeyboardPress(DIK_A) || IsButtonPressed(0, BUTTON_DOWN) && IsButtonPressed(0, BUTTON_LEFT))
        {
            g_Player.spd = VALUE_MOVE;
            roty = D3DX_PI * 0.25f;
        }
        if (GetKeyboardPress(DIK_S) && GetKeyboardPress(DIK_D) || IsButtonPressed(0, BUTTON_DOWN) && IsButtonPressed(0, BUTTON_RIGHT))
        {
            g_Player.spd = VALUE_MOVE;
            roty = - D3DX_PI * 0.25f;
        }


        g_Player.rot.y = roty + cam->rot.y;
        //移動処理
        g_Player.pos.x -= sinf(g_Player.rot.y) * g_Player.spd;
        g_Player.pos.z -= cosf(g_Player.rot.y) * g_Player.spd;

    }
    else
    {
        // 移動処理
        if (GetKeyboardPress(DIK_A) || IsButtonPressed(0, BUTTON_LEFT))
        {
            g_Player.spd = VALUE_MOVE_ZOOM;
            g_Player.pos -= g_Player.axis_x * g_Player.spd;

        }
        if (GetKeyboardPress(DIK_D) || IsButtonPressed(0, BUTTON_RIGHT))
        {
            g_Player.spd = VALUE_MOVE_ZOOM;
            g_Player.pos += g_Player.axis_x * g_Player.spd;

        }
        if (GetKeyboardPress(DIK_W) || IsButtonPressed(0, BUTTON_UP))
        {
            g_Player.spd = VALUE_MOVE_ZOOM;
            g_Player.pos += g_Player.axis_z * g_Player.spd;

        }
        if (GetKeyboardPress(DIK_S) || IsButtonPressed(0, BUTTON_DOWN))
        {
            g_Player.spd = VALUE_MOVE_ZOOM;
            g_Player.pos -= g_Player.axis_z * g_Player.spd;

        }

    }


	if (IsMouseLeftPressed() || GetKeyboardTrigger(DIK_SPACE) || IsButtonTriggered(0, BUTTON_R))
	{
        SetBullet(g_Player.pos, D3DXVECTOR3(cam->rot.x, cam->rot.y, cam->rot.z));
	}

    SetPositionShadow(MAX_STENCIL_SHADOW, g_Player.pos);

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPlayer(void)
{
	// カリング無効
	SetCullingMode(CULL_MODE_NONE);

    D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_Player.mtxWorld);

	// スケールを反映
	D3DXMatrixScaling(&mtxScl, g_Player.scl.x, g_Player.scl.y, g_Player.scl.z);
	D3DXMatrixMultiply(&g_Player.mtxWorld, &g_Player.mtxWorld, &mtxScl);

	// 回転を反映
    D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Player.rot.y + D3DX_PI, g_Player.rot.x, g_Player.rot.z);

    //ローカル座標を用い回転
    D3DXVec3TransformCoord(&g_Player.axis_x, &D3DXVECTOR3(1, 0, 0), &mtxRot);
    D3DXVec3TransformCoord(&g_Player.axis_y, &D3DXVECTOR3(0, 1, 0), &mtxRot);
    D3DXVec3TransformCoord(&g_Player.axis_z, &D3DXVECTOR3(0, 0, 1), &mtxRot);
	D3DXMatrixMultiply(&g_Player.mtxWorld, &g_Player.mtxWorld, &mtxRot);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTranslate, g_Player.pos.x, g_Player.pos.y, g_Player.pos.z);
	D3DXMatrixMultiply(&g_Player.mtxWorld, &g_Player.mtxWorld, &mtxTranslate);


	// ワールドマトリックスの設定
	SetWorldMatrix(&g_Player.mtxWorld);

	// モデル描画
	DrawModel(&g_Player.model);

    LIGHT *Light = GetLight();

    //影描画
    CreateStencilShadow(g_Player.mtxWorld, *Light, &g_shadow[MAX_STENCIL_SHADOW]);
    DrawStencilShadow(&g_shadow[MAX_STENCIL_SHADOW], g_Player.mtxWorld);

    // 階層アニメーション
    for (int i = 0; i < PLAYER_PARTS_MAX; i++)
    {
        // 使われているなら処理する
        if (g_Parts[i].use == false) continue;

        // ワールドマトリックスの初期化
        D3DXMatrixIdentity(&g_Parts[i].mtxWorld);

        // スケールを反映
        D3DXMatrixScaling(&mtxScl, g_Parts[i].scl.x, g_Parts[i].scl.y, g_Parts[i].scl.z);
        D3DXMatrixMultiply(&g_Parts[i].mtxWorld, &g_Parts[i].mtxWorld, &mtxScl);

        // 回転を反映
        D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Parts[i].rot.y, g_Parts[i].rot.x, g_Parts[i].rot.z);
        D3DXMatrixMultiply(&g_Parts[i].mtxWorld, &g_Parts[i].mtxWorld, &mtxRot);

        // 移動を反映
        D3DXMatrixTranslation(&mtxTranslate, g_Parts[i].pos.x, g_Parts[i].pos.y, g_Parts[i].pos.z);
        D3DXMatrixMultiply(&g_Parts[i].mtxWorld, &g_Parts[i].mtxWorld, &mtxTranslate);

        if (g_Parts[i].parent != NULL)	// 子供だったら親と結合する
        {
            D3DXMatrixMultiply(&g_Parts[i].mtxWorld, &g_Parts[i].mtxWorld, &g_Parts[i].parent->mtxWorld);
        }                                                                              //↑g_Player.mtxWorldを指している                         

        // ワールドマトリックスの設定
        SetWorldMatrix(&g_Parts[i].mtxWorld);

        // モデル描画
        DrawModel(&g_Parts[i].model);

    }

    // カリング設定を戻す
    SetCullingMode(CULL_MODE_BACK);

}


//=============================================================================
// プレイヤー情報を取得
//=============================================================================
PLAYER *GetPlayer(void)
{
	return &g_Player;
}

