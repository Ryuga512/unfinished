//=============================================================================
//
// ���f������ [player.h]
// Author : GP11A132 12 �F�J����
//
//=============================================================================
#pragma once

#include "main.h"
#include "model.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_PLAYER		(1)					// �v���C���[�̐�

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
struct PLAYER
{
    D3DXVECTOR3         axis_x;             // ���[�J����x
    D3DXVECTOR3         axis_y;             // ���[�J����y
    D3DXVECTOR3         axis_z;             // ���[�J����z

	D3DXVECTOR3			pos;				// ���f���̈ʒu
	D3DXVECTOR3			rot;				// ���f���̌���(��])
	D3DXVECTOR3			scl;				// ���f���̑傫��(�X�P�[��)

	float				spd;				// �ړ��X�s�[�h
	
	DX11_MODEL			model;				// ���f�����

	D3DXMATRIX			mtxWorld;			// ���[���h�}�g���b�N�X

	int					shadowIdx;			// �e�̃C���f�b�N�X�ԍ�

    float               radius;


    bool                use;

    // �K�w�A�j���[�V�����p�̃����o�[�ϐ�
    INTERPOLATION_DATA	*tbl_adr;           //  �����Ƀe�[�u���̐擪�A�h���X������
    float				move_time;

    PLAYER				*parent;            // �e�́ANULL�A�q���͐e�̃A�h���X������

};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);

PLAYER *GetPlayer(void);

