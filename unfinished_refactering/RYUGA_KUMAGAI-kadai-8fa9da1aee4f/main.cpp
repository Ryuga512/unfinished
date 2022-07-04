//=============================================================================
//
// Main���� [main.cpp]
// Author : GP11A132 12 �F�J����
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "camera.h"

#include "player.h"
#include "enemy.h"
#include "input.h"
#include "attack.h"
#include "bg.h"
#include "sound.h"

#include "game.h"
#include "title.h"
#include "result.h"
#include "fade.h"
#include "object.h"
#include "effect.h"
#include "tutorial.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define CLASS_NAME			"AppClass"						// �E�C���h�E�̃N���X��
#define WINDOW_NAME			"Unfinished"	// �E�C���h�E�̃L���v�V������

//*****************************************************************************
// �\���̒�`
//*****************************************************************************



//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);
void HitCheck(void);
bool HitCheckBB(D3DXVECTOR3 m_pos, float m_hw, float m_hh,
    D3DXVECTOR3 y_pos, float y_hw, float y_hh);

//*****************************************************************************
// �O���[�o���ϐ�:
//*****************************************************************************
long g_MouseX = 0;
long g_MouseY = 0;
int  stop     = 0;
MODE g_Mode = MODE_TITLE;

#ifdef _DEBUG
int		g_CountFPS;							// FPS�J�E���^
char	g_DebugStr[2048] = WINDOW_NAME;		// �f�o�b�O�����\���p

#endif

//=============================================================================
// ���C���֐�
//=============================================================================
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//UNREFERENCED_PARAMETER(hPrevInstance);	// �����Ă��ǂ����ǁA�x�����o��i���g�p�錾�j
	//UNREFERENCED_PARAMETER(lpCmdLine);		// �����Ă��ǂ����ǁA�x�����o��i���g�p�錾�j

	// ���Ԍv���p
	DWORD dwExecLastTime;
	DWORD dwFPSLastTime;
	DWORD dwCurrentTime;
	DWORD dwFrameCount;
	
	WNDCLASSEX	wcex = {
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hInstance,
		NULL,
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW+1),
		NULL,
		CLASS_NAME,
		NULL
	};
	HWND		hWnd;
	MSG			msg;
	
	// �E�B���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	// �E�B���h�E�̍쐬
	hWnd = CreateWindow(CLASS_NAME,
						WINDOW_NAME,
						WS_OVERLAPPEDWINDOW,
						CW_USEDEFAULT,																		// �E�B���h�E�̍����W
						CW_USEDEFAULT,																		// �E�B���h�E�̏���W
						SCREEN_WIDTH + GetSystemMetrics(SM_CXDLGFRAME)*2,									// �E�B���h�E����
						SCREEN_HEIGHT + GetSystemMetrics(SM_CXDLGFRAME)*2+GetSystemMetrics(SM_CYCAPTION),	// �E�B���h�E�c��
						NULL,
						NULL,
						hInstance,
						NULL);

    bool mode = true;

    int id = MessageBox(NULL, "Window���[�h�Ńv���C���܂����H", "�N�����[�h", MB_YESNOCANCEL | MB_ICONQUESTION);

    switch (id)
    {
    case IDYES:                                                   //YES�Ȃ�Window���[�h�ŋN��
        mode = true;
        break;

    case IDNO:                                                    //NO�Ȃ�t���X�N���[�����[�h�ŋN��
        mode = false;
        break;

    case IDCANCEL:                                                //CANCEL�Ȃ�I��
    default:
        return -1;
        break;
    }
	
	if(FAILED(Init(hInstance, hWnd, mode)))                       // DirectX�̏�����(�E�B���h�E���쐬���Ă���s��)
	{
		return -1;
	}

	                                                              // �t���[���J�E���g������
	timeBeginPeriod(1);                         	              // ����\��ݒ�
	dwExecLastTime = dwFPSLastTime = timeGetTime();	              // �V�X�e���������~���b�P�ʂŎ擾
	dwCurrentTime = dwFrameCount = 0;
	
	                                                
	ShowWindow(hWnd, nCmdShow);                                  // �E�C���h�E�̕\��(Init()�̌�ɌĂ΂Ȃ��Ƒʖ�)
	UpdateWindow(hWnd);
	
	
	while(1)                                                     // ���b�Z�[�W���[�v
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
			{                                                     // PostQuitMessage()���Ă΂ꂽ�烋�[�v�I��
				break;
			}
			else
			{
				                                    
				TranslateMessage(&msg);                            // ���b�Z�[�W�̖|��ƃf�B�X�p�b�`
				DispatchMessage(&msg);
			}
		}
		else
		{
			dwCurrentTime = timeGetTime();					        // �V�X�e���������擾

			if ((dwCurrentTime - dwFPSLastTime) >= 1000)        	// 1�b���ƂɎ��s
			{
#ifdef _DEBUG
				g_CountFPS = dwFrameCount;
#endif
				dwFPSLastTime = dwCurrentTime;				        // FPS�𑪒肵��������ۑ�
				dwFrameCount = 0;							        // �J�E���g���N���A
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))	// 1/60�b���ƂɎ��s
			{
				dwExecLastTime = dwCurrentTime;	                    // ��������������ۑ�

#ifdef _DEBUG	                                                    // �f�o�b�O�ł̎�����FPS��\������
				wsprintf(g_DebugStr, WINDOW_NAME);
				wsprintf(&g_DebugStr[strlen(g_DebugStr)], " FPS:%d", g_CountFPS);
#endif

				Update();			                                // �X�V����
				Draw();				                                // �`�揈��

#ifdef _DEBUG	                                                    // �f�o�b�O�ł̎������\������
				wsprintf(&g_DebugStr[strlen(g_DebugStr)], " MX:%d MY:%d", GetMousePosX(), GetMousePosY());
				SetWindowText(hWnd, g_DebugStr);
#endif

				dwFrameCount++;		                                // �����񐔂̃J�E���g�����Z
			}
		}
	}
	
	timeEndPeriod(1);				                                // ����\��߂�

	                                                                
	UnregisterClass(CLASS_NAME, wcex.hInstance);                    // �E�B���h�E�N���X�̓o�^������

	
	Uninit();                                                       // �I������

	return (int)msg.wParam;
}

//=============================================================================
// �v���V�[�W��
//=============================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch( message )
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_ESCAPE:					// [ESC]�L�[�������ꂽ
			DestroyWindow(hWnd);		// �E�B���h�E��j������悤�w������
			break;
		}
		break;

	case WM_MOUSEMOVE:
		g_MouseX = LOWORD(lParam);
		g_MouseY = HIWORD(lParam);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
    // �����_�����O�����̏�����
    InitRenderer(hInstance, hWnd, bWindow);

    // �J���������̏�����
    InitCamera();

    // ���͏����̏�����
    InitInput(hInstance, hWnd);

    // �T�E���h�����̏����� 
    InitSound(hWnd);

    // �t�F�[�h�̏�����
    InitFade();

    // �ŏ��̃��[�h���Z�b�g
    SetMode(g_Mode);	// ������SetMode�̂܂܂ŁI

	return S_OK;
}


//=============================================================================
// �I������
//=============================================================================
void Uninit(void)
{
    // �I���̃��[�h���Z�b�g
    SetMode(MODE_MAX);

    // �t�F�[�h�̏I������
    UninitFade();

    // �T�E���h�I������
    UninitSound();

    // ���͂̏I������
    UninitInput();

    // �J�����̏I������
    UninitCamera();

    // �����_�����O�̏I������
    UninitRenderer();

}

//=============================================================================
// �X�V����
//=============================================================================
void Update(void)
{
    // ���͂̍X�V����
    UpdateInput();

    // �J�����̍X�V����
    UpdateCamera();

#ifdef _DEBUG
    if (GetKeyboardPress(DIK_P))
    {
        stop++;
    }

    if (stop % 10 > 1)
    {
        return;
    }
#endif

    // ���[�h�ɂ���ď����𕪂���
    switch (g_Mode)
    {
    case MODE_TITLE:		// �^�C�g����ʂ̍X�V
        UpdateTitle();
        break;

    case MODE_TUTORIAL:     // �`���[�g���A����ʂ̍X�V
        UpdateTutorial();
        break;

    case MODE_GAME:			// �Q�[����ʂ̍X�V
        UpdateGame();
        break;

    case MODE_RESULT:		// ���U���g��ʂ̍X�V
        UpdateResult();
        break;
    }

    // �t�F�[�h�����̍X�V
    UpdateFade();
}

//=============================================================================
// �`�揈��
//=============================================================================
void Draw(void)
{
    // �o�b�N�o�b�t�@�N���A
    Clear();

    // �J�������Z�b�g
    SetCamera();

    // 2D�`��Ȃ̂Ő[�x����
    SetDepthEnable(false);

    // ���[�h�ɂ���ď����𕪂���
    switch (g_Mode)
    {
    case MODE_TITLE:		// �^�C�g����ʂ̕`��
        DrawTitle();
        break;

    case MODE_TUTORIAL:     // �`���[�g���A����ʂ̕`��
        DrawTutorial();
        break;

    case MODE_GAME:			// �Q�[����ʂ̕`��
        DrawGame();
        break;

    case MODE_RESULT:		// ���U���g��ʂ̕`��
        DrawResult();
        break;
    }

    // �t�F�[�h�`��
    DrawFade();

    // �o�b�N�o�b�t�@�A�t�����g�o�b�t�@����ւ�
    Present();

}


long GetMousePosX(void)
{
	return g_MouseX;
}


long GetMousePosY(void)
{
	return g_MouseY;
}


#ifdef _DEBUG
char *GetDebugStr(void)
{
	return g_DebugStr;
}
#endif



void CollisionCheckPlayer(int j)
{
    OBJECT *object = GetObject_p();
    PLAYER *player = GetPlayer_p();

    for (int i = 0; i < 4; i++)
    {
        player[j].hit[i] = false;
    }

    for (int i = 0; i < OBJECT_MAX; i++)
    {
        if (object[i].use == false)continue;    //�I�u�W�F�N�g�������Ă���s��Ȃ�

        if (player[j].use == false)continue;    //�v���C���[������ł���s��Ȃ�

        CollisionCheckBB(player[j].pos, player[j].w * 0.5, player[j].h * 0.5,
                         object[i].pos, object[i].w * 0.5, object[i].h * 0.5, j);
        CollisionCheckBB(player[j].pre_pos, player[j].w * 0.5, player[j].h * 0.5,
            object[i].pos, object[i].w * 0.5, object[i].h * 0.5, j);

    }
}

void CollisionCheckBB(D3DXVECTOR3 m_pos, float m_hw, float m_hh,
                      D3DXVECTOR3 y_pos, float y_hw, float y_hh, int j)
{
    PLAYER *player = GetPlayer_p();

    //�I�u�W�F�N�g����ɂ���Ƃ�
    if ((m_pos.y > y_pos.y) && (m_pos.x > y_pos.x - y_hw) && (m_pos.x < y_pos.x + y_hw))
    {
    //�v���C���[�̏�[���I�u�W�F�N�g�̉��[�𒴂����Ƃ�
        if (m_pos.y - m_hh <= y_pos.y + y_hh)
        {
            player[j].hit[UP] = true;
            if (m_pos.y - m_hh < y_pos.y + y_hh)
            {
                player[j].pos.y = y_pos.y + y_hh + m_hh;
            }
        }
        else
        {
            player[j].hit[UP] = false;
        }
    }

    //�I�u�W�F�N�g�����ɂ���Ƃ�
    if ((m_pos.y < y_pos.y) && (m_pos.x > y_pos.x - y_hw) && (m_pos.x < y_pos.x + y_hw))
    {
    //�v���C���[�̉��[���I�u�W�F�N�g�̏�[�𒴂����Ƃ�
        if (m_pos.y + m_hh >= y_pos.y - y_hh)
        {
            player[j].hit[DOWN] = true;
            if (m_pos.y + m_hh > y_pos.y - y_hh)
            {
                player[j].pos.y = y_pos.y - y_hh - m_hh;
            }
        }
        else
        {
            player[j].hit[DOWN] = false;
        }
    }

    //�I�u�W�F�N�g���E���ɂ��鎞
    if ((m_pos.x < y_pos.x) && (m_pos.y > y_pos.y - y_hh) && (m_pos.y < y_pos.y + y_hh))
    {
    //�v���C���[�̉E�[���I�u�W�F�N�g�̍��[�𒴂����Ƃ�
        if (m_pos.x + m_hw >= y_pos.x - y_hw)
        {
            player[j].hit[RIGHT] = true;
            if (m_pos.x + m_hw > y_pos.x - y_hw)
            {
                player[j].pos.x = y_pos.x - y_hw - m_hw;
            }
        }
        else
        {
            player[j].hit[RIGHT] = false;
        }
    }

    //�I�u�W�F�N�g�������ɂ��鎞
    if ((m_pos.x > y_pos.x) && (m_pos.y > y_pos.y - y_hh) && (m_pos.y < y_pos.y + y_hh))
    {
    //�v���C���[�̍��[���I�u�W�F�N�g�̉E�[�𒴂����Ƃ�
        if (m_pos.x - m_hw <= y_pos.x + y_hw)
        {
            player[j].hit[LEFT] = true;
            if (m_pos.x - m_hw < y_pos.x + y_hw)
            {
                player[j].pos.x = y_pos.x + y_hw + m_hw;
            }

        }
        else
        {
            player[j].hit[LEFT] = false;
        }
    }
}


void HitCheck(void)
{
    BULLET* bullet = GetBullet();
    ENEMY * enemy  = GetEnemy_p();
    PLAYER* player = GetPlayer_p();

    //�G�l�~�[�ƃv���C���[�̏Փˏ���
    for (int i = 0; i < ENEMY_MAX; i++)
    {
        if (enemy[i].use == false)continue;     //�G�l�~�[������ł���s��Ȃ�

        for (int j = 0; j < PLAYER_MAX; j++)
        {

            if (player[j].use == false)continue;//�v���C���[������ł���s��Ȃ�

            //�U���͈̔�
            bool ans_atk = false;
            //�U�����̓����蔻��
            if (enemy[i].texNo == OTHER_FRONT && enemy[i].patternAnim == 4 && enemy[i].countAnim >= enemy[i].atk_delay)
            {
                if (HitCheckBB(player[j].pos, player[j].w * 0.2, player[j].h * 0.3,
                                enemy[i].pos,  enemy[i].w * 0.2,  enemy[i].h * 0.45))
                {
                    player[j].hp -= 20;
                    enemy[i].atk_delay = enemy[i].countAnim + 32;

                    //�G�l�~�[���㑤�ɂ��鎞
                    if (player[j].pos.y > enemy[i].pos.y)
                    {
                        player[j].pos.y += 20;
                    }
                    else
                    {
                        player[j].pos.y -= 20;
                    }
                }
            }
            else if (enemy[i].texNo == OTHER_SIDE && enemy[i].patternAnim == 4 && enemy[i].countAnim >= enemy[i].atk_delay)
            {
                if (HitCheckBB(player[j].pos, player[j].w * 0.2, player[j].h * 0.3,
                    enemy[i].pos, enemy[i].w * 0.5, enemy[i].h * 0.1))
                {
                    player[j].hp -= 20;
                    enemy[i].atk_delay = enemy[i].countAnim + 32;

                    //�G�l�~�[���E���ɂ��鎞
                    if (player[j].pos.x < enemy[i].pos.x)
                    {
                        player[j].pos.x -= 20;
                    }
                    else
                    {
                        player[j].pos.x += 20;
                    }
                }
            }
            else if (enemy[i].texNo == OTHER_DEFAULT && enemy[i].countAnim >= enemy[i].atk_delay)
            {
                if (HitCheckBB(player[j].pos, player[j].w * 0.2, player[j].h * 0.3,
                    enemy[i].pos, enemy[i].w * 0.2, enemy[i].h * 0.3))
                {
                    player[j].hp -= 10;
                    enemy[i].atk_delay = enemy[i].countAnim + 30;

                    //�G�l�~�[���㑤�ɂ��鎞
                    if (player[j].pos.y > enemy[i].pos.y)
                    {
                        player[j].pos.y += 20;
                    }
                    //�G�l�~�[�������ɂ��鎞
                    else if (player[j].pos.y < enemy[i].pos.y)
                    {
                        player[j].pos.y -= 20;
                    }
                    //�G�l�~�[���E���ɂ��鎞
                    else if (player[j].pos.x < enemy[i].pos.x)
                    {
                        player[j].pos.x -= 20;
                    }
                    //�G�l�~�[�������ɂ��鎞
                    else if (player[j].pos.x > enemy[i].pos.x)
                    {
                        player[j].pos.x += 20;
                    }
                }
            }
            else if (enemy[i].texNo == OTHER_ROLLING && enemy[i].patternAnim == 4 && enemy[i].countAnim >= enemy[i].atk_delay)
            {
                if (HitCheckBB(player[j].pos, player[j].w * 0.2, player[j].h * 0.3,
                    enemy[i].pos, enemy[i].w * 0.45, enemy[i].h * 0.45))
                {
                    player[j].hp -= 10;
                    enemy[i].atk_delay = enemy[i].countAnim + 30;
                }
            }
        }
    }

    bool ans = false;

    //�G�l�~�[�ƍU���̏Փˏ���
    for (int i = 0; i < ENEMY_MAX; i++)
    {
        if (enemy[i].use == false)continue;

        for (int j = 0; j < PLAYER_MAX; j++)
        {
            if ((player[j].dir == UP    && player[j].texNo  != 17) ||      //�U��������Ȃ������珈�����Ƃ΂�
                (player[j].dir == DOWN  && player[j].texNo  != 18) ||
                (player[j].dir == LEFT  && player[j].texNo  != 19) ||
                (player[j].dir == RIGHT && player[j].texNo  != 20)) 
                continue;


            if (player[j].dir == RIGHT)
            {
                ans = HitCheckBB(D3DXVECTOR3(player[j].pos.x + player[j].w + player[j].atk_w, player[j].pos.y, 0), player[j].atk_w, player[j].atk_h, enemy[i].pos, enemy[i].w * 0.3, enemy[i].h * 0.3);
            }
            else if (player[j].dir == LEFT)
            {
                ans = HitCheckBB(D3DXVECTOR3(player[j].pos.x - player[j].w - player[j].atk_w, player[j].pos.y, 0), player[j].atk_w, player[j].atk_h, enemy[i].pos, enemy[i].w * 0.3, enemy[i].h * 0.3);
            }
            else if (player[j].dir == UP)
            {
                ans = HitCheckBB(D3DXVECTOR3(player[j].pos.x, player[j].pos.y - player[j].h - player[j].atk_h, 0), player[j].atk_w, player[j].atk_h, enemy[i].pos, enemy[i].w * 0.3, enemy[i].h * 0.3);
            }
            else if (player[j].dir == DOWN)
            {
                ans = HitCheckBB(D3DXVECTOR3(player[j].pos.x, player[j].pos.y + player[j].h + player[j].atk_h, 0), player[j].atk_w, player[j].atk_h, enemy[i].pos, enemy[i].w * 0.3, enemy[i].h * 0.3);
            }

            if (ans == true)
            {
                if (player[j].atk_delay <= player[j].countAnim)
                {
                    enemy[i].hp        -= player[j].attack;
                    player[j].atk_delay = player[j].countAnim + ATACK_DELAY;
                }
                if (enemy[i].hp <= 0)
                {
                    enemy[i].use = false;
                }
            }
        }
        UpdateEffect(enemy[i].pos, i, ans);
    }

    // �G�l�~�[���S�����S�������ԑJ��
    int enemy_count = 0;
    for (int i = 0; i < ENEMY_MAX; i++)
    {
        if (enemy[i].use == false) continue;
        enemy_count++;
    }

    // �G�l�~�[���O�C�H
    if (enemy_count == 0 && g_Mode != MODE_TUTORIAL)
    {
        SetFade(FADE_OUT, MODE_RESULT);
    }

    // �v���C���[���S�����S�������ԑJ��
    int player_count = 0;
    for (int i = 0; i < PLAYER_MAX; i++)
    {
        if (player[i].use == false) continue;
        player_count++;
    }

    // �v���C���[���O�C�H
    if (player_count == 0 && g_Mode == MODE_GAME)
    {
        SetFade(FADE_OUT, MODE_TUTORIAL);
    }
    else if (player_count == 0 && g_Mode == MODE_TUTORIAL)
    {
        SetFade(FADE_OUT, MODE_GAME);
    }

}

bool HitCheckBB(D3DXVECTOR3 m_pos, float m_hw, float m_hh, 
                D3DXVECTOR3 y_pos, float y_hw, float y_hh)
{
    if ((m_pos.x + m_hw >= y_pos.x - y_hw) &&    //m�̉E�� > y�̍���
        (m_pos.x - m_hw <= y_pos.x + y_hw) &&    //m�̍��� < y�̉E��
        (m_pos.y + m_hh >= y_pos.y - y_hh) &&    //m�̉��� > y�̏㑤
        (m_pos.y - m_hh <= y_pos.y + y_hh)   )   //m�̏㑤 < y�̉���
    {
        return true;                             //���������ꍇ
    }
    return false;                                //�O�ꂽ�ꍇ
}

//=============================================================================
// ���[�h�̐ݒ�
//=============================================================================
void SetMode(MODE mode)
{
    // ���[�h��ς���O�ɑS������������������Ⴄ
    // �^�C�g����ʂ̏I������
    UninitTitle();

    //�`���[�g���A����ʂ̏I������
    UninitTutorial();

    // �Q�[����ʂ̏I������
    UninitGame();

    // ���U���g��ʂ̏I������
    UninitResult();


    g_Mode = mode;

    switch (g_Mode)
    {
    case MODE_TITLE:
    // �^�C�g����ʂ̏�����
        InitTitle();
        break;

    case MODE_TUTORIAL:     
    // �`���[�g���A����ʂ̏�����
        InitTutorial();
        break;

    case MODE_GAME:
    // �Q�[����ʂ̏�����
        InitGame();
        break;

    case MODE_RESULT:
    // ���U���g��ʂ̏�����
        InitResult();
        break;

    // �Q�[���I�����̏���
    case MODE_MAX:
        break;
    }
}

//=============================================================================
// ���[�h�̎擾
//=============================================================================
MODE GetMode(void)
{
    return g_Mode;
}

