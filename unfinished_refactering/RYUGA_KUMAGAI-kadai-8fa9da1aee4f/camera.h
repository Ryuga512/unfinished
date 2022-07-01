//=============================================================================
//
// �J�����̏��� [camera.h]
// Author : GP11A132 12 �F�J����
//
//=============================================================================
#pragma once



//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitCamera(void);
void UninitCamera(void);
void UpdateCamera(void);
void SetCamera(void);

D3DXMATRIX GetCameraViewMatrix();
D3DXMATRIX GetCameraInvViewMatrix();
D3DXMATRIX GetCameraProjectionMatrix();
D3DXVECTOR3 GetCameraPosition();
