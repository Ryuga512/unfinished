
//=============================================================================
//
// ���f���̏��� [model.h]
// Author : GP11A132 12 �F�J����
//
//=============================================================================
#pragma once

#include "main.h"
#include "renderer.h"
#include "model.h"
#define MAX_STENCIL_SHADOW  (101)
//*********************************************************
// �\����
//*********************************************************
typedef struct
{
    MODEL       model;
    DX11_MODEL  dx_model;
    D3DXMATRIX	mtxWorld;	// ���[���h�}�g���b�N�X
    D3DXVECTOR3 pos;		// �ʒu
    D3DXVECTOR3 rot;		// ��]
    D3DXVECTOR3 scl;		// �X�P�[��
    MATERIAL	material;	// �}�e���A��
    bool		bUse;		// �g�p���Ă��邩�ǂ���
    BBOX        BBox;
} STENCIL_SHADOW;

//*********************************************************
// �O���[�o���ϐ�
//*********************************************************


//*****************************************************************************
// �v���g�^�C�v�錾
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