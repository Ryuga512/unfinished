//=============================================================================
//
// ���f���̏��� [model.h]
// Author : GP11A132 12 �F�J����
//
//=============================================================================
#pragma once

#include "main.h"
#include "renderer.h"

//*********************************************************
// �\����
//*********************************************************

// �}�e���A���\����
struct DX11_MODEL_MATERIAL
{
	MATERIAL					Material;
	ID3D11ShaderResourceView	*Texture;
};

// �`��T�u�Z�b�g�\����
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

// �}�e���A���\����
struct MODEL_MATERIAL
{
    char						Name[256];
    MATERIAL					Material;
    char						TextureName[256];
};

// �`��T�u�Z�b�g�\����
struct SUBSET
{
    unsigned short	StartIndex;
    unsigned short	IndexNum;
    MODEL_MATERIAL	Material;
};

// ���f���\����
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
// �v���g�^�C�v�錾
//*****************************************************************************
void LoadModel( char *FileName, DX11_MODEL *Model );
void UnloadModel( DX11_MODEL *Model );
void DrawModel( DX11_MODEL *Model );
void LoadObj(char *FileName, MODEL *Model);
void LoadMaterial(char *FileName, MODEL_MATERIAL **MaterialArray, unsigned short *MaterialNum);

