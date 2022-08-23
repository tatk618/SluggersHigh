#include "define.h"
#include "main.h"
#include "Scene/Title/TitleScene.h"
#include "Manager/Scene/SceneManager.h"
#include "Manager/Enemy/EnemyManager.h"

//===================================================================
// ���C��
//===================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpszArgs, int nWinMode)
{
	// ���������[�N��m�点��
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	// COM������
	CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	// mbstowcs_s�֐��œ��{��Ή��ɂ��邽�߂ɌĂ�
	setlocale(LC_ALL, "japanese");

	//===================================================================
	// ���s
	//===================================================================

	APP.Execute();

	// COM���
	CoUninitialize();

	return 0;
}


// �A�v���P�[�V���������ݒ�
bool Application::Init(int w, int h)
{

	//===================================================================
	// �E�B���h�E�쐬
	//===================================================================
	if (m_window.Create(w, h, "Slugger's High", "Window") == false) {
		MessageBoxA(nullptr, "�E�B���h�E�쐬�Ɏ��s", "�G���[", MB_OK);
		return false;
	}

	//===================================================================
	// �t���X�N���[���m�F
	//===================================================================
	bool bFullScreen = false;
	//if (MessageBoxA(m_window.GetWndHandle(), "�t���X�N���[���ɂ��܂����H", "�m�F", MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2) == IDYES) {
	//	bFullScreen = true;
	//}


	//===================================================================
	// Direct3D������
	//===================================================================

	// �f�o�C�X�̃f�o�b�O���[�h��L���ɂ���
	bool deviceDebugMode = false;
	#ifdef _DEBUG
	deviceDebugMode = true;
	#endif

	// Direct3D������
	std::string errorMsg;
	if (D3D.Init(m_window.GetWndHandle(), w, h, deviceDebugMode, errorMsg) == false) {
		MessageBoxA(m_window.GetWndHandle(), errorMsg.c_str(), "Direct3D���������s", MB_OK | MB_ICONSTOP);
		return false;
	}

	// �t���X�N���[���ݒ�
	if (bFullScreen) {
		D3D.GetSwapChain()->SetFullscreenState(TRUE, 0);
	}

	// �V�F�[�_�[������
	SHADER.Init();

	//===================================================================
	// �T�E���h������
	//===================================================================
	if (AUDIO.Init() == false)
	{
		MessageBoxA(m_window.GetWndHandle(), errorMsg.c_str(), "�T�E���h���������s", MB_OK | MB_ICONSTOP);
		return false;
	}




	return true;
}

// �A�v���P�[�V�����I��
void Application::Release()
{

	// �V�F�[�_���
	SHADER.Release();

	// Direct3D���
	D3D.Release();

	// XAudio2���
	AUDIO.Release();

	// �E�B���h�E�폜
	m_window.Release();

}

// �A�v���P�[�V�������s
void Application::Execute()
{
	//===================================================================
	// �����ݒ�(�E�B���h�E�쐬�ADirect3D�������Ȃ�)
	//===================================================================
	if (APP.Init(ScrW, ScrH) == false) {
		return;
	}

	//===================================================================
	// �Q�[�����[�v
	//===================================================================

	// ����
	DWORD baseTime = timeGetTime();
	int count = 0;

	// �Q�[���C���X�^���X
	SCENE.ChangeScene(new TitleScene());

	srand(timeGetTime());

	// ���[�v
	while (1)
	{

		// �����J�n����Get
		DWORD st = timeGetTime();

		// �Q�[���I���w�肪����Ƃ��̓��[�v�I��
		if (m_endFlag)
		{ 
			break;
		}

		//=========================================
		//
		// �E�B���h�E�֌W�̏���
		//
		//=========================================

		// �E�B���h�E�̃��b�Z�[�W����������
		m_window.ProcessMessage();

		// �E�B���h�E���j������Ă�Ȃ烋�[�v�I��
		if (m_window.IsCreated() == false)
		{
			break;
		}

		//=========================================
		//
		// �I�[�f�B�I����
		//
		//=========================================
		AUDIO.Update();

		//=========================================
		//
		// �Q�[������
		//
		//=========================================
		SCENE.DoScene();

		D3D.GetSwapChain()->Present(0, 0);

		//=========================================
		//
		// �t���[�����[�g����
		//
		//=========================================
		// �����I������Get
		DWORD et = timeGetTime();
		// Fps����
		DWORD ms = 1000 / m_maxFps;
		if (et - st < ms)
		{
			Sleep(ms - (et - st));	// ����������҂�
		}

		// FPS�v��
		count++;
		if (st - baseTime >= 500)
		{
			m_fps = (count * 1000) / (st - baseTime);
			baseTime = st;
			count = 0;
		}

	}

	//===================================================================
	// �A�v���P�[�V�������
	//===================================================================
	Release();
}