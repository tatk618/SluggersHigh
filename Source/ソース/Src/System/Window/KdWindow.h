#pragma once

//=====================================================
//
// �E�B���h�E
//
//=====================================================
class KdWindow {
public:

	//===================================
	// �擾�E�ݒ�
	//===================================

	// �E�B���h�E�n���h���擾
	HWND GetWndHandle() const { return m_hWnd; }
	// �E�B���h�E�����݂���H
	bool IsCreated() const { return m_hWnd ? true : false; }
	// �}�E�X�z�C�[���̕ω��ʂ��擾
	int GetMouseWheelVal() const { return m_mouseWheelVal; }
	// �C���X�^���X�n���h���擾
	HINSTANCE GetInstanceHandle() const { return GetModuleHandle(0); }

	// �N���C�A���g�T�C�Y�̐ݒ�
	void SetClientSize(int w, int h);

	//===================================
	// �������E���
	//===================================

	// �E�B���h�E�쐬
	bool Create(int clientWidth, int clientHeight, const std::string& titleName, const std::string& windowClassName);

	// ���
	void Release();

	//
	~KdWindow() {
		Release();
	}

	//===================================
	// ����
	//===================================

	// �E�B���h�E���b�Z�[�W����������
	//  �߂�l�F�I�����b�Z�[�W��������false���Ԃ�
	bool ProcessMessage();

private:

	// �E�B���h�E�n���h��
	HWND	m_hWnd = nullptr;

	// �}�E�X�z�C�[���l
	int		m_mouseWheelVal = 0;

	// �E�B���h�E�֐�
	static LRESULT CALLBACK callWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


};

