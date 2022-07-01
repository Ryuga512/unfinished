//=============================================================================
//
// プレイヤー処理 [player.h]
// Author : GP11A132 12 熊谷隆我
//
//=============================================================================
#pragma once

#include "main.h"
#include "renderer.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************



//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void SetSpriteColorRotation(ID3D11Buffer *buf, float X, float Y, float Width, float Height, float U, float V, float UW, float VH, D3DXCOLOR Color, float Rot);
void SetSprite(ID3D11Buffer *buf, float X, float Y, float Width, float Height, float U, float V, float UW, float VH);
void SetSpriteColor(int i, ID3D11Buffer *buf, float X, float Y, float Width, float Height,float U, float V, float UW, float VH, D3DXCOLOR color);
