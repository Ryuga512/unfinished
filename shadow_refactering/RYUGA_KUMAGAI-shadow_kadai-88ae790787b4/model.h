//=============================================================================
//
// モデルの処理 [model.h]
// Author : GP11A132 12 熊谷隆我
//
//=============================================================================
#pragma once

#include "main.h"
#include "renderer.h"

//*********************************************************
// 構造体
//*********************************************************

// マテリアル構造体
struct DX11_MODEL_MATERIAL
{
	MATERIAL					Material;
	ID3D11ShaderResourceView	*Texture;
};

// 描画サブセット構造体
struct DX11_SUBSET
{
	unsigned short	StartIndex;
	unsigned short	IndexNum;
	DX11_MODEL_MATERIAL	Material;
};

struct DX11_MODEL
{
	ID3D11Buffer*	VertexBuffer;
	ID3D11Buffer*	IndexBuffer;

	DX11_SUBSET		*SubsetArray;
	unsigned short	SubsetNum;
};

// マテリアル構造体
struct MODEL_MATERIAL
{
    char						Name[256];
    MATERIAL					Material;
    char						TextureName[256];
};

// 描画サブセット構造体
struct SUBSET
{
    unsigned short	StartIndex;
    unsigned short	IndexNum;
    MODEL_MATERIAL	Material;
};

// モデル構造体
struct MODEL
{
    VERTEX_3D		*VertexArray;
    unsigned short	VertexNum;
    unsigned short	*IndexArray;
    unsigned short	IndexNum;
    SUBSET			*SubsetArray;
    unsigned short	SubsetNum;
};



//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void LoadModel( char *FileName, DX11_MODEL *Model );
void UnloadModel( DX11_MODEL *Model );
void DrawModel( DX11_MODEL *Model );
void LoadObj(char *FileName, MODEL *Model);
void LoadMaterial(char *FileName, MODEL_MATERIAL **MaterialArray, unsigned short *MaterialNum);

