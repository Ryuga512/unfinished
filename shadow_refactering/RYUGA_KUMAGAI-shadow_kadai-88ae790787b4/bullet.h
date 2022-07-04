#pragma once
//=============================================================================
//
// �؏��� [Bullet.h]
// Author : GP11A132 12 �F�J����
//
//=============================================================================

#define	MAX_BULLET			(10)			// �e�ő吔

typedef struct
{
    D3DXMATRIX	mtxWorld;		// ���[���h�}�g���b�N�X
    D3DXVECTOR3 pos;			// �ʒu
    D3DXVECTOR3 scl;			// �X�P�[��
    MATERIAL	material;		// �}�e���A��
    float		fWidth;			// ��
    float		fHeight;		// ����
    int			nIdxShadow;		// �eID
    bool		bUse;			// �g�p���Ă��邩�ǂ���
    float       speed;
    float       radius;
    D3DXVECTOR3 rot;
} BULLET;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);

//�e����
int SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
BULLET *GetBullet(void);

