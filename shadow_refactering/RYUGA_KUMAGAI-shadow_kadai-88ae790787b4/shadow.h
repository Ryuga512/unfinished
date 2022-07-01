//=============================================================================
//
// 影処理 [shadow.h]
// Author : GP11A132 12 熊谷隆我
//
//=============================================================================
#pragma once
#include "main.h"
#include "shadow.h"
#include "renderer.h"

typedef struct
{
    D3DXMATRIX	mtxWorld;	// ワールドマトリックス
    D3DXVECTOR3 pos;		// 位置
    D3DXVECTOR3 rot;		// 回転
    D3DXVECTOR3 scl;		// スケール
    MATERIAL	material;	// マテリアル
    bool		bUse;		// 使用しているかどうか
} SHADOW;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitShadow(void);
void UninitShadow(void);
void UpdateShadow(void);
void DrawShadow(void);

int CreateShadow(D3DXVECTOR3 pos, float fSizeX, float fSizeZ);
void ReleaseShadow(int nIdxShadow);
void SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos);
void SetColorShadow(int nIdxShadow, D3DXCOLOR col);
SHADOW* GetShadow(void);