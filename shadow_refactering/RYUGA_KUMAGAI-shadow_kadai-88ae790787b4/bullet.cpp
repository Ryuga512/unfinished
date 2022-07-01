//=============================================================================
//
// �e���� [Bullet.cpp]
// Author : GP11A132 12 �F�J����
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "camera.h"
#include "shadow.h"
#include "bullet.h"
#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_MAX			(1)				// �e�N�X�`���̐�

#define	BULLET_WIDTH	    (50.0f)			// ���_�T�C�Y
#define	BULLET_HEIGHT		(80.0f)			// ���_�T�C�Y

//*****************************************************************************
// �\���̒�`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexBullet(void);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer					*g_VertexBuffer = NULL;	// ���_�o�b�t�@
static ID3D11ShaderResourceView		*g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static BULLET				g_aBullet[MAX_BULLET];	// �e���[�N
static int					g_TexNo;			    // �e�N�X�`���ԍ�
static bool					g_bAlpaTest;		    // �A���t�@�e�X�gON/OFF
static int					g_nAlpha;			    // �A���t�@�e�X�g��臒l

static char *g_TextureName[] =
{
    "data/TEXTURE/bullet000.png",
};

//=============================================================================
// ����������
//=============================================================================
HRESULT InitBullet(void)
{
    MakeVertexBullet();

    // �e�N�X�`������
    for (int i = 0; i < TEXTURE_MAX; i++)
    {
        D3DX11CreateShaderResourceViewFromFile(GetDevice(),
            g_TextureName[i],
            NULL,
            NULL,
            &g_Texture[i],
            NULL);
    }

    g_TexNo = 0;

    // �e���[�N�̏�����
    for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
    {
        ZeroMemory(&g_aBullet[nCntBullet].material, sizeof(g_aBullet[nCntBullet].material));
        g_aBullet[nCntBullet].material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

        g_aBullet[nCntBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_aBullet[nCntBullet].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
        g_aBullet[nCntBullet].fWidth = BULLET_WIDTH;
        g_aBullet[nCntBullet].fHeight = BULLET_HEIGHT;
        g_aBullet[nCntBullet].bUse = false;
        g_aBullet[nCntBullet].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_aBullet[nCntBullet].speed = 1.5f;
        g_aBullet[nCntBullet].radius = 3;
    }



    return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitBullet(void)
{
    for (int nCntTex = 0; nCntTex < TEXTURE_MAX; nCntTex++)
    {
        if (g_Texture[nCntTex] != NULL)
        {// �e�N�X�`���̉��
            g_Texture[nCntTex]->Release();
            g_Texture[nCntTex] = NULL;
        }
    }

    if (g_VertexBuffer != NULL)
    {// ���_�o�b�t�@�̉��
        g_VertexBuffer->Release();
        g_VertexBuffer = NULL;
    }
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateBullet(void)
{

    for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
    {
        if (g_aBullet[nCntBullet].bUse)
        {
            //�e�̈ړ�����
            g_aBullet[nCntBullet].pos.x += sinf(g_aBullet[nCntBullet].rot.y) * g_aBullet[nCntBullet].speed;
            g_aBullet[nCntBullet].pos.z += cosf(g_aBullet[nCntBullet].rot.y) * g_aBullet[nCntBullet].speed;
            g_aBullet[nCntBullet].pos.y += sinf(g_aBullet[nCntBullet].rot.x) * g_aBullet[nCntBullet].speed;

            // �e�̈ʒu�ݒ�
            SetPositionShadow(g_aBullet[nCntBullet].nIdxShadow, D3DXVECTOR3(g_aBullet[nCntBullet].pos.x, 0.1f, g_aBullet[nCntBullet].pos.z));
        
            if (g_aBullet[nCntBullet].pos.x < WALL_LEFT
                || g_aBullet[nCntBullet].pos.x > WALL_RIGHT
                || g_aBullet[nCntBullet].pos.z < WALL_DOWN
                || g_aBullet[nCntBullet].pos.z > WALL_UP
                || g_aBullet[nCntBullet].pos.y < (GROUND - 1)
                )
            {
                g_aBullet[nCntBullet].bUse = false;
                ReleaseShadow(g_aBullet[nCntBullet].nIdxShadow);
            }
        }
    }
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawBullet(void)
{

    // ���C�e�B���O�𖳌�
    SetLightEnable(false);

    D3DXMATRIX mtxView, mtxScale, mtxTranslate, mtxRot;

    // ���_�o�b�t�@�ݒ�
    UINT stride = sizeof(VERTEX_3D);
    UINT offset = 0;
    GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

    // �v���~�e�B�u�g�|���W�ݒ�
    GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

    for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
    {
        if (g_aBullet[nCntBullet].bUse)
        {
            // ���[���h�}�g���b�N�X�̏�����
            D3DXMatrixIdentity(&g_aBullet[nCntBullet].mtxWorld);

            // �r���[�}�g���b�N�X���擾
            CAMERA *cam = GetCamera();
            mtxView = cam->mtxView;

            // �|���S���𐳖ʂɌ�����

            // �t�s����v�Z�ŋ��߂�
            D3DXMatrixInverse(&g_aBullet[nCntBullet].mtxWorld, NULL, &mtxView);
            g_aBullet[nCntBullet].mtxWorld._41 = 0.0f;
            g_aBullet[nCntBullet].mtxWorld._42 = 0.0f;
            g_aBullet[nCntBullet].mtxWorld._43 = 0.0f;

            // �X�P�[���𔽉f
                D3DXMatrixScaling(&mtxScale, g_aBullet[nCntBullet].scl.x,
                    g_aBullet[nCntBullet].scl.y,
                    g_aBullet[nCntBullet].scl.z);
            D3DXMatrixMultiply(&g_aBullet[nCntBullet].mtxWorld, &g_aBullet[nCntBullet].mtxWorld, &mtxScale);

            ////��]�𔽉f
            //D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aBullet[nCntBullet].rot.y, g_aBullet[nCntBullet].rot.x, g_aBullet[nCntBullet].rot.z);
            //D3DXMatrixMultiply(&g_aBullet[nCntBullet].mtxWorld, &g_aBullet[nCntBullet].mtxWorld, &mtxRot);

            // �ړ��𔽉f
            D3DXMatrixTranslation(&mtxTranslate, g_aBullet[nCntBullet].pos.x,
                g_aBullet[nCntBullet].pos.y,
                g_aBullet[nCntBullet].pos.z);
            D3DXMatrixMultiply(&g_aBullet[nCntBullet].mtxWorld, &g_aBullet[nCntBullet].mtxWorld, &mtxTranslate);


            // ���[���h�}�g���b�N�X�̐ݒ�
            SetWorldMatrix(&g_aBullet[nCntBullet].mtxWorld);

            // �}�e���A���ݒ�
            SetMaterial(g_aBullet[nCntBullet].material);

            // �e�N�X�`���ݒ�
            GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[nCntBullet % TEXTURE_MAX]);

            // �|���S���̕`��
            GetDeviceContext()->Draw(4, 0);
        }
    }

    // ���C�e�B���O��L����
    SetLightEnable(true);
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
HRESULT MakeVertexBullet(void)
{
    // ���_�o�b�t�@����
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DYNAMIC;
    bd.ByteWidth = sizeof(VERTEX_3D) * 4;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);

    // ���_�o�b�t�@�ɒl���Z�b�g����
    D3D11_MAPPED_SUBRESOURCE msr;
    GetDeviceContext()->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

    VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

    float fWidth = 20.0f;
    float fHeight = 20.0f;

    // ���_���W�̐ݒ�
    vertex[0].Position = D3DXVECTOR3(-fWidth / 2.0f, 0.1f, fHeight / 2.0f);
    vertex[1].Position = D3DXVECTOR3(fWidth  / 2.0f, 0.1f, fHeight / 2.0f);
    vertex[2].Position = D3DXVECTOR3(-fWidth / 2.0f, 0.1f, -fHeight / 2.0f);
    vertex[3].Position = D3DXVECTOR3(fWidth  / 2.0f, 0.1f, -fHeight / 2.0f);
    
    // �g�U���̐ݒ�
    vertex[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    vertex[1].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    vertex[2].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    vertex[3].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

    // �e�N�X�`�����W�̐ݒ�
    vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);
    vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);
    vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);
    vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);

    GetDeviceContext()->Unmap(g_VertexBuffer, 0);

    return S_OK;
}

//=============================================================================
// �e�̃p�����[�^���Z�b�g
//=============================================================================
int SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
    int nIdxBullet = -1;

    for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
    {
        if (!g_aBullet[nCntBullet].bUse)
        {
            //g_aBullet[nCntBullet].rot.x = ;
            g_aBullet[nCntBullet].pos = pos;
            g_aBullet[nCntBullet].rot = rot;
            g_aBullet[nCntBullet].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
            g_aBullet[nCntBullet].bUse = true;

            // �e�̐ݒ�
            g_aBullet[nCntBullet].nIdxShadow = CreateShadow(g_aBullet[nCntBullet].pos, 0.5f, 0.5f);

            nIdxBullet = nCntBullet;

            break;
        }
    }

    return nIdxBullet;
}

BULLET *GetBullet(void)
{
    return g_aBullet;
}
