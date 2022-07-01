
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
//*********************************************************
// �\����
//*********************************************************


//*********************************************************
// �O���[�o���ϐ�
//*********************************************************



//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
bool InitStencilShadow(char *FileName, DX11_MODEL *Model);
bool InitStencilShadow(void);
void UnloadStencilShadow(DX11_MODEL *Model);
void DrawStencilShadow(DX11_MODEL *Model, D3DXMATRIX mtxWorld);
bool CreateStencilShadow(D3DXMATRIX world, LIGHT Light, DX11_MODEL *Model);
void polygon(void);
void Peshanko(DX11_MODEL *Model);
