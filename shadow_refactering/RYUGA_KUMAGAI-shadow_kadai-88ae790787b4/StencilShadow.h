
//=============================================================================
//
// モデルの処理 [model.h]
// Author : GP11A132 12 熊谷隆我
//
//=============================================================================
#pragma once

#include "main.h"
#include "renderer.h"
#include "model.h"
#define MAX_STENCIL_SHADOW  (101)
//*********************************************************
// 構造体
//*********************************************************
typedef struct
{
    MODEL       model;
    DX11_MODEL  dx_model;
    D3DXMATRIX	mtxWorld;	// ワールドマトリックス
    D3DXVECTOR3 pos;		// 位置
    D3DXVECTOR3 rot;		// 回転
    D3DXVECTOR3 scl;		// スケール
    MATERIAL	material;	// マテリアル
    bool		bUse;		// 使用しているかどうか
    BBOX        BBox;
} STENCIL_SHADOW;

//*********************************************************
// グローバル変数
//*********************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
bool InitStencilShadow(char* FileName, STENCIL_SHADOW* shadow);
bool InitStencilShadow(void);
void UnloadStencilShadow(STENCIL_SHADOW* shadow);
void DrawStencilShadow(STENCIL_SHADOW* shadow, D3DXMATRIX mtxWorld);
bool CreateStencilShadow(D3DXMATRIX world, LIGHT Light, STENCIL_SHADOW* shadow);
void polygon(void);
void Peshanko(DX11_MODEL *Model);
void SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos);
void ReleaseShadow(int nIdxShadow);
STENCIL_SHADOW* GetStencilShadow(void);