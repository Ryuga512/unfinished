//=============================================================================
//
// レンダリング処理 [renderer.h]
// Author : GP11A132 12 熊谷隆我
//
//=============================================================================
#pragma once

#include "main.h"

//*********************************************************
// 構造体
//*********************************************************

// 頂点構造体
struct VERTEX_3D
{
    D3DXVECTOR3 Position;	// 表示座標（x, y, z）
    D3DXVECTOR3 Normal;		// 法線/ポリゴンの向き（x, y, z）
    D3DXCOLOR   Diffuse;	// 頂点カラー(R, G, B, A)
    D3DXVECTOR2 TexCoord;	// Tex座標（x, y）→（U, V）
};


// マテリアル構造体
struct MATERIAL
{
	D3DXCOLOR	Ambient;
	D3DXCOLOR	Diffuse;
	D3DXCOLOR	Specular;
	D3DXCOLOR	Emission;
	float		Shininess;
	float		Dummy[3];//16bit境界用
};


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitRenderer(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void UninitRenderer(void);

void Clear(void);
void Present(void);

ID3D11Device *GetDevice( void );
ID3D11DeviceContext *GetDeviceContext( void );

void SetDepthEnable( bool Enable );

void SetWorldViewProjection2D( void );
void SetWorldMatrix( D3DXMATRIX *WorldMatrix );
void SetViewMatrix( D3DXMATRIX *ViewMatrix );
void SetProjectionMatrix( D3DXMATRIX *ProjectionMatrix );


void SetMaterial( MATERIAL Material );


