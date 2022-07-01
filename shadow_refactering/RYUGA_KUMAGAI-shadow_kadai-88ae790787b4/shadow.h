//=============================================================================
//
// �e���� [shadow.h]
// Author : GP11A132 12 �F�J����
//
//=============================================================================
#pragma once
#include "main.h"
#include "shadow.h"
#include "renderer.h"

typedef struct
{
    D3DXMATRIX	mtxWorld;	// ���[���h�}�g���b�N�X
    D3DXVECTOR3 pos;		// �ʒu
    D3DXVECTOR3 rot;		// ��]
    D3DXVECTOR3 scl;		// �X�P�[��
    MATERIAL	material;	// �}�e���A��
    bool		bUse;		// �g�p���Ă��邩�ǂ���
} SHADOW;

//*****************************************************************************
// �v���g�^�C�v�錾
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