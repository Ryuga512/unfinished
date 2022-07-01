//=============================================================================
//
// ���f���̏��� [model.cpp]
// Author : 
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "main.h"
#include "StencilShadow.h"
#include "camera.h"
#include "renderer.h"
#include "main.h"
#include "player.h"
#include "sprite.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
//#define MODEL_FILENAME		"data/MODEL/miku_01.obj"
#define	VALUE_MOVE_MODEL	(0.50f)					// �ړ����x
#define	RATE_MOVE_MODEL		(0.20f)					// �ړ������W��
#define	VALUE_ROTATE_MODEL	(PI * 0.05f)			// ��]���x
#define	RATE_ROTATE_MODEL	(0.20f)					// ��]�����W��
#define	SCALE_MODEL			(10.0f)					// ��]�����W��


//*****************************************************************************
// �\���̒�`
//*****************************************************************************
struct CBUFF_PER_MESH//���b�V���P�ʂœn�����
{
    D3DXMATRIX mW;//���[���h�s��
    D3DXMATRIX mWVP;//���[���h����ˉe�܂ł̕ϊ��s��
};
struct CBUFF_PER_MATERIAL//�}�e���A���P�ʂœn�����
{
    D3DXVECTOR4 vAmbient;//�A���r�G���g��
    D3DXVECTOR4 vDiffuse;//�f�B�t���[�Y�F
    D3DXVECTOR4 vSpecular;//���ʔ���
};

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
ID3D11DeviceContext* ImmediateContext;
ID3D11Device* D3DDevice;
MODEL g_model;
ID3D11DepthStencilState* m_pDSState;
ID3D11DepthStencilState* m_pDSState2;
ID3D11BlendState* m_pBlendState;
ID3D11Buffer* m_pVertexBuffer;//��ʂ�����|���S���p
ID3D11Buffer* VertexBuffer;
ID3D11Buffer* m_pCBuffPerMaterial;
D3DXVECTOR3 pos;
D3DXVECTOR3 rot;
D3DXVECTOR3 scl;
DWORD id[2][2];
int NumVertices = 0;

bool InitStencilShadow(char *FileName, DX11_MODEL *Model)
{
    LoadObj(FileName, &g_model);

    // ���_�o�b�t�@����
    {
        //�{�����[�����b�V���p�̃o�[�e�b�N�X�o�b�t�@�[���쐬
        D3D11_BUFFER_DESC bd;
        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.ByteWidth = sizeof(VERTEX_3D) * 100000;
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bd.CPUAccessFlags = 0;
        bd.MiscFlags = 0;

        //D3D11_SUBRESOURCE_DATA sd;
        //ZeroMemory(&sd, sizeof(sd));
        //sd.pSysMem = g_model.VertexArray;

        D3DDevice->CreateBuffer(&bd, /*&sd*/NULL, &Model->VertexBuffer);
    }
    //    // ���_�o�b�t�@����
    //{
    //    D3D11_BUFFER_DESC bd;
    //    ZeroMemory(&bd, sizeof(bd));
    //    bd.Usage = D3D11_USAGE_DEFAULT;
    //    bd.ByteWidth = sizeof(VERTEX_3D) * g_model.VertexNum;
    //    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    //    bd.CPUAccessFlags = 0;

    //    D3D11_SUBRESOURCE_DATA sd;
    //    ZeroMemory(&sd, sizeof(sd));
    //    sd.pSysMem = g_model.VertexArray;

    //    GetDevice()->CreateBuffer(&bd, &sd, &Model->VertexBuffer);
    //}

    // �C���f�b�N�X�o�b�t�@����
    {
        D3D11_BUFFER_DESC bd;
        ZeroMemory(&bd, sizeof(bd));
        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.ByteWidth = sizeof(unsigned short) * g_model.IndexNum;
        bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
        bd.CPUAccessFlags = 0;

        D3D11_SUBRESOURCE_DATA sd;
        ZeroMemory(&sd, sizeof(sd));
        sd.pSysMem = g_model.IndexArray;

        GetDevice()->CreateBuffer(&bd, &sd, &Model->IndexBuffer);
    }

    // �T�u�Z�b�g�ݒ�
    {
        Model->SubsetArray = new DX11_SUBSET[g_model.SubsetNum];
        Model->SubsetNum = g_model.SubsetNum;

        for (unsigned short i = 0; i < g_model.SubsetNum; i++)
        {
            Model->SubsetArray[i].StartIndex = g_model.SubsetArray[i].StartIndex;
            Model->SubsetArray[i].IndexNum = g_model.SubsetArray[i].IndexNum;

            Model->SubsetArray[i].Material.Material = g_model.SubsetArray[i].Material.Material;
            Model->SubsetArray[i].Material.Material.Diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

            D3DX11CreateShaderResourceViewFromFile(GetDevice(),
                                                   g_model.SubsetArray[i].Material.TextureName,
                                                   NULL,
                                                   NULL,
                                                   &Model->SubsetArray[i].Material.Texture,
                                                   NULL);
        }
    }

    return true;
}

bool InitStencilShadow(void)
{
    ImmediateContext = GetDeviceContext();
    D3DDevice = GetDevice();


    // �X�e���V���������ݎ��̋������`���郌���_�����O�X�e�[�g�̃I�u�W�F�N�g���쐬
    D3D11_DEPTH_STENCIL_DESC dc;
    //�[�x�e�X�g
    dc.DepthEnable = true;//�����|���S�����͂���ł��锻�f�ɕK�v
    dc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;//�������������g��Z�e�X�g���Ȃ����߂ɏ������݂̓[��
    dc.DepthFunc = D3D11_COMPARISON_LESS;
    //�X�e���V���e�X�g
    dc.StencilEnable = true;
    dc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
    dc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
    //�\�ʂ��ǂ����邩
    dc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
    dc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_ZERO;
    dc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
    dc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
    //���ʂ��ǂ����邩
    dc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
    dc.BackFace.StencilFailOp = D3D11_STENCIL_OP_ZERO;
    dc.BackFace.StencilPassOp = D3D11_STENCIL_OP_DECR;
    dc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    D3DDevice->CreateDepthStencilState(&dc, &m_pDSState);

    // �X�e���V�����p���̋������`���郌���_�����O�X�e�[�g�̃I�u�W�F�N�g���쐬
    dc.DepthEnable = false;
    dc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
    dc.DepthFunc = D3D11_COMPARISON_LESS;
    //�X�e���V���e�X�g
    dc.StencilEnable = true;
    dc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
    dc.StencilWriteMask = 0x00;
    //�\�ʂ��ǂ����邩
    dc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
    dc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_ZERO;
    dc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    dc.FrontFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL;
    //���ʂ��ǂ����邩
    dc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
    dc.BackFace.StencilFailOp = D3D11_STENCIL_OP_ZERO;
    dc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    dc.BackFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL;

    D3DDevice->CreateDepthStencilState(&dc, &m_pDSState2);

    //�A���t�@�u�����h�p�u�����h�X�e�[�g�쐬
    D3D11_BLEND_DESC bld;
    ZeroMemory(&bld, sizeof(D3D11_BLEND_DESC));
    bld.IndependentBlendEnable = false;
    bld.AlphaToCoverageEnable = false;
    bld.RenderTarget[0].BlendEnable = true;
    bld.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    bld.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    bld.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    bld.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    bld.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    bld.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    bld.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    if (FAILED(D3DDevice->CreateBlendState(&bld, &m_pBlendState)))
    {
        return E_FAIL;
    }

    UINT mask = 0xffffffff;
    ImmediateContext->OMSetBlendState(m_pBlendState, NULL, mask);

    //��ʑO�ʂ̃|���S���p�@�o�[�e�b�N�X�o�b�t�@�[���쐬
    VERTEX_3D VertexData[] =
    {
        D3DXVECTOR3(-SCREEN_WIDTH, -SCREEN_HEIGHT, 0), D3DXVECTOR3(0,0,0), D3DXCOLOR(0,0,0,1), D3DXVECTOR2(0,0),
        D3DXVECTOR3( SCREEN_WIDTH, -SCREEN_HEIGHT, 0), D3DXVECTOR3(0,0,0), D3DXCOLOR(0,0,0,1), D3DXVECTOR2(0,0),
        D3DXVECTOR3(-SCREEN_WIDTH,  SCREEN_HEIGHT, 0), D3DXVECTOR3(0,0,0), D3DXCOLOR(0,0,0,1), D3DXVECTOR2(0,0),
        D3DXVECTOR3( SCREEN_WIDTH,  SCREEN_HEIGHT, 0), D3DXVECTOR3(0,0,0), D3DXCOLOR(0,0,0,1), D3DXVECTOR2(0,0),

    };
    D3D11_BUFFER_DESC bd;

    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(VERTEX_3D) * 4;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;
    bd.MiscFlags = 0;
    D3D11_SUBRESOURCE_DATA InitData;
    InitData.pSysMem = VertexData;
    D3DDevice->CreateBuffer(&bd, &InitData, &m_pVertexBuffer);


    return true;
}

bool CreateStencilShadow(D3DXMATRIX world, LIGHT Light, DX11_MODEL *Model)
{
    //D3D11_MAPPED_SUBRESOURCE SubR;
    D3DXVECTOR3 vertex0, vertex1, vertex2, vertex3;
    NumVertices = 0;
    //ImmediateContext->Map(Model->VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &SubR);
    //VERTEX_3D *model = (VERTEX_3D*)SubR.pData;
    VERTEX_3D *model = new VERTEX_3D[100000];

    D3DXVECTOR3 dir = D3DXVECTOR3(Light.Direction.x, Light.Direction.y, Light.Direction.z);

    D3DXMATRIX inv;
    D3DXMatrixInverse(&inv, NULL, &world);
    inv._41 = inv._42 = inv._43 = 0;

    D3DXVec3TransformCoord(&dir, &dir, &inv);

    //DX11_SUBSET *SubsetArray;
    //SubsetArray = new DX11_SUBSET[g_model.SubsetNum * 4];
    //unsigned short	SubsetNum = g_model.SubsetNum * 4;

    int *edge = new int[g_model.VertexNum * 4];
    int Edges = 0;
    /**********************************************************************/
    //�p�^�[���P�`��`�����L�΂��ҁ`
    /**********************************************************************/
    //�S���_�ɂ��āA�������΂�������
    for (int i = 0; i < g_model.VertexNum - 3; i += 3)
    {
        //���_���R�g���A�ʂ̖@�������߂�
        vertex0 = g_model.VertexArray[i + 0].Position;
        vertex1 = g_model.VertexArray[i + 1].Position;
        vertex2 = g_model.VertexArray[i + 2].Position;


        D3DXVECTOR3 Normal;
        D3DXVECTOR3 cross0(vertex1 - vertex0);
        D3DXVECTOR3 cross1(vertex2 - vertex1);
        D3DXVec3Cross(&Normal, &cross0, &cross1);
        D3DXVec3Normalize(&Normal, &Normal);

        // �@���̐ݒ�
        model[i + 0].Normal = Normal;
        model[i + 1].Normal = Normal;
        model[i + 2].Normal = Normal;

        // �g�U���̐ݒ�
        model[i + 0].Diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
        model[i + 1].Diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
        model[i + 2].Diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

        //�|���S���̗����Ȃ̂��𔻒�&�ӂ𒊏o
        if (D3DXVec3Dot(&Normal, &dir) <= 0 && D3DXVec3Dot(&Normal, &dir) > -1.0)
        {
            edge[Edges + 0] = g_model.IndexArray[i + 0];
            edge[Edges + 1] = g_model.IndexArray[i + 1];

            edge[Edges + 2] = g_model.IndexArray[i + 1];
            edge[Edges + 3] = g_model.IndexArray[i + 2];

            edge[Edges + 4] = g_model.IndexArray[i + 2];
            edge[Edges + 5] = g_model.IndexArray[i + 0];

            Edges += 6;

        }
    }

    dir = -dir;
    for (DWORD i = 0; i < Edges - 6; i += 2)
    {
        vertex0 = g_model.VertexArray[edge[i + 0]].Position + g_model.VertexArray[edge[i + 0]].Normal * 0.05f;
        vertex1 = g_model.VertexArray[edge[i + 1]].Position + g_model.VertexArray[edge[i + 1]].Normal * 0.05f;
        vertex2 = vertex0 + dir * 10000;
        vertex3 = vertex1 + dir * 10000;

        model[NumVertices + 0].Position = vertex0;
        model[NumVertices + 1].Position = vertex1;
        model[NumVertices + 2].Position = vertex2;

        model[NumVertices + 3].Position = vertex1;
        model[NumVertices + 4].Position = vertex3;
        model[NumVertices + 5].Position = vertex2;

        //model[NumVertices + 0].Position *= 1.001f;
        //model[NumVertices + 1].Position *= 1.001f;
        //model[NumVertices + 2].Position *= 1.001f;
        //model[NumVertices + 3].Position *= 1.001f;
        //model[NumVertices + 4].Position *= 1.001f;
        //model[NumVertices + 5].Position *= 1.001f;

        NumVertices += 6;
    }

    ImmediateContext->UpdateSubresource(Model->VertexBuffer, 0, NULL, model, 0, 0);


    /**********************************************************************/
    //�p�^�[���P�`��`�����L�΂��œK����ҁ`�i���j
    /**********************************************************************/
    ////�S���_�ɂ��āA�������΂�������
    //for (int i = 0; i < g_model.VertexNum - 3; i += 3)
    //{
    //    //���_���R�g���A�ʂ̖@�������߂�
    //    vertex0 = g_model.VertexArray[i + 0].Position;
    //    vertex1 = g_model.VertexArray[i + 1].Position;
    //    vertex2 = g_model.VertexArray[i + 2].Position;


    //    D3DXVECTOR3 Normal;
    //    D3DXVECTOR3 cross0(vertex1 - vertex0);
    //    D3DXVECTOR3 cross1(vertex2 - vertex1);
    //    D3DXVec3Cross(&Normal, &cross0, &cross1);
    //    D3DXVec3Normalize(&Normal, &Normal);

    //    // �@���̐ݒ�
    //    model[i + 0].Normal = Normal;
    //    model[i + 1].Normal = Normal;
    //    model[i + 2].Normal = Normal;

    //    // �g�U���̐ݒ�
    //    model[i + 0].Diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
    //    model[i + 1].Diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
    //    model[i + 2].Diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

    //    //�|���S���̗����Ȃ̂��𔻒�&�ӂ𒊏o
    //    if (D3DXVec3Dot(&Normal, &dir) <= 0 && D3DXVec3Dot(&Normal, &dir) > -1.0)
    //    {
    //        edge[Edges + 0] = g_model.IndexArray[i + 0];
    //        edge[Edges + 1] = g_model.IndexArray[i + 1];

    //        edge[Edges + 2] = g_model.IndexArray[i + 1];
    //        edge[Edges + 3] = g_model.IndexArray[i + 2];

    //        edge[Edges + 4] = g_model.IndexArray[i + 2];
    //        edge[Edges + 5] = g_model.IndexArray[i + 0];

    //        Edges += 6;

    //    }
    //}

    //dir = -dir;
    //for (DWORD i = 0; i < Edges - 6; i += 2)
    //{
    //    vertex0 = g_model.VertexArray[edge[i + 0]].Position + g_model.VertexArray[edge[i + 0]].Normal * 0.05f;
    //    vertex1 = g_model.VertexArray[edge[i + 1]].Position + g_model.VertexArray[edge[i + 1]].Normal * 0.05f;
    //    vertex2 = vertex0 + dir * 10000;
    //    vertex3 = vertex1 + dir * 10000;

    //    model[NumVertices + 0].Position = vertex0;
    //    model[NumVertices + 1].Position = vertex1;
    //    model[NumVertices + 2].Position = vertex2;

    //    model[NumVertices + 3].Position = vertex1;
    //    model[NumVertices + 4].Position = vertex3;
    //    model[NumVertices + 5].Position = vertex2;

    //    //model[NumVertices + 0].Position *= 1.001f;
    //    //model[NumVertices + 1].Position *= 1.001f;
    //    //model[NumVertices + 2].Position *= 1.001f;
    //    //model[NumVertices + 3].Position *= 1.001f;
    //    //model[NumVertices + 4].Position *= 1.001f;
    //    //model[NumVertices + 5].Position *= 1.001f;

    //    NumVertices += 6;
    //}
    ////ImmediateContext->Unmap(Model->VertexBuffer, 0);

    ImmediateContext->UpdateSubresource(Model->VertexBuffer, 0, NULL, model, 0, 0);

    delete[] edge;
    delete[] model;

    return true;
}


//=============================================================================
// �I������
//=============================================================================
void UnloadStencilShadow(DX11_MODEL *Model)
{
    if (Model->VertexBuffer)	Model->VertexBuffer->Release();
    if (Model->IndexBuffer)		Model->IndexBuffer->Release();
    if (Model->SubsetArray)		delete[] Model->SubsetArray;
}


//=============================================================================
// �`�揈��
//=============================================================================
void DrawStencilShadow(DX11_MODEL *Model, D3DXMATRIX mtxWorld)
{
    PLAYER *Player = GetPlayer();
    LIGHT *Light = GetLight();

    //���݂̃����_�����O�X�e�[�g��ޔ������Ă����i��Ŗ߂����߁j
    ID3D11DepthStencilState* pCurrentDSState = NULL;
    UINT Num = 0;
    ImmediateContext->OMGetDepthStencilState(&pCurrentDSState, &Num);
    // �����_�����O�X�e�[�g�̕ύX
    ImmediateContext->OMSetDepthStencilState(m_pDSState, 1);

    //�o�[�e�b�N�X�o�b�t�@�[���Z�b�g
    UINT stride = sizeof(VERTEX_3D);
    UINT offset = 0;

    // ���[���h�}�g���b�N�X�̐ݒ�
    SetWorldMatrix(&mtxWorld);
    //SetMaterial(Model->SubsetArray->Material.Material);
    //DrawModel(Model);
    ImmediateContext->IASetVertexBuffers(0, 1, &Model->VertexBuffer, &stride, &offset);
    ImmediateContext->Draw(NumVertices, 0);

    // �����_�����O�X�e�[�g�����ɖ߂�
    ImmediateContext->OMSetDepthStencilState(pCurrentDSState, Num);

}

void polygon(void)
{
    //���݂̃����_�����O�X�e�[�g��ޔ������Ă����i��Ŗ߂����߁j
    ID3D11DepthStencilState* pCurrentDSState = NULL;
    UINT Num = 0;
    ImmediateContext->OMGetDepthStencilState(&pCurrentDSState, &Num);

    D3DXMATRIX mtxWorld, mtxView, mtxTranslate, mtxScl, mtxRot;
    // �����_�����O�X�e�[�g�̕ύX
    ImmediateContext->OMSetDepthStencilState(m_pDSState2, 0);
    // �v���~�e�B�u�g�|���W�ݒ�
    ImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

    // ���[���h�}�g���b�N�X�̏�����
    D3DXMatrixIdentity(&mtxWorld);


    // �r���[�}�g���b�N�X���擾
    CAMERA *cam = GetCamera();
    mtxView = cam->mtxView;
    PLAYER *player = GetPlayer();
    // �|���S���𐳖ʂɌ�����


       // ���_�o�b�t�@�ݒ�
    UINT stride = sizeof(VERTEX_3D);
    UINT offset = 0;
    ImmediateContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

    // �}�g���N�X�ݒ�
    SetWorldViewProjection2D();

    ImmediateContext->Draw(4, 0);


    // �����_�����O�X�e�[�g�����ɖ߂�
    ImmediateContext->OMSetDepthStencilState(pCurrentDSState, Num);
}

void Peshanko(DX11_MODEL *Model)
{
    PLAYER *Player = GetPlayer();


    D3DXMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

    // ���[���h�}�g���b�N�X�̏�����
    D3DXMatrixIdentity(&mtxWorld);

    // �X�P�[���𔽉f
    D3DXMatrixScaling(&mtxScl, Player->scl.x, Player->scl.y, Player->scl.z);
    D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScl);

    // ��]�𔽉f
    D3DXMatrixRotationYawPitchRoll(&mtxRot, Player->rot.y, Player->rot.x, Player->rot.z);

    D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);


    //�ړ��𔽉f
    D3DXMatrixTranslation(&mtxTranslate, Player->pos.x, Player->pos.y, Player->pos.z);
    D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);

    // //�ړ��𔽉f
    //D3DXMatrixTranslation(&mtxTranslate, pos.x, pos.y, pos.z);
    //D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);


    D3DXMATRIX matShadow = D3DXMATRIX(
        1, 0,     0, 0,
        0, 0,     0, 0,
        0, 0,     1, 0,
        0, 0.01f, 0, 1);

    D3DXMATRIX mat;
    D3DXMatrixMultiply(&mat, &mtxWorld, &matShadow);
    //D3DXMatrixMultiply(&mat, &Player->mtxWorld, &matShadow);

    //for (int i = 0; i < Model->SubsetNum; i++)
    //{
    //    Model->SubsetArray[i].Material.Material.Diffuse = (0.2, 0.2, 0.2, 0.6);
    //}

    // ���[���h�}�g���b�N�X�̐ݒ�
    SetWorldMatrix(&mat);
    
    DrawModel(Model);

    //DrawModel(&Player->model);
    return;
}