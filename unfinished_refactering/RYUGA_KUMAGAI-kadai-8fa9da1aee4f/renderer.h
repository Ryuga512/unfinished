//=============================================================================
//
// �����_�����O���� [renderer.h]
// Author : GP11A132 12 �F�J����
//
//=============================================================================
#pragma once

#include "main.h"

//*********************************************************
// �\����
//*********************************************************

// ���_�\����
struct VERTEX_3D
{
    D3DXVECTOR3 Position;	// �\�����W�ix, y, z�j
    D3DXVECTOR3 Normal;		// �@��/�|���S���̌����ix, y, z�j
    D3DXCOLOR   Diffuse;	// ���_�J���[(R, G, B, A)
    D3DXVECTOR2 TexCoord;	// Tex���W�ix, y�j���iU, V�j
};


// �}�e���A���\����
struct MATERIAL
{
	D3DXCOLOR	Ambient;
	D3DXCOLOR	Diffuse;
	D3DXCOLOR	Specular;
	D3DXCOLOR	Emission;
	float		Shininess;
	float		Dummy[3];//16bit���E�p
};


//*****************************************************************************
// �v���g�^�C�v�錾
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


