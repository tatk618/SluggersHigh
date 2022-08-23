#pragma once



//===============================================================
//
// �l�X�ȃo�b�t�@���쐬�E���삷��N���X
//
// Direct3D11��ID3D11Buffer���ȒP�ɑ���ł���悤�ɂ܂Ƃ߂����̂ł�
//  (��)���_�o�b�t�@�A�C���f�b�N�X�o�b�t�@�A�萔�o�b�t�@�Ȃ�
//
//===============================================================
class KdBuffer {
public:

	//=================================================
	//
	// �擾
	//
	//=================================================

	// �o�b�t�@�C���^�[�t�F�C�X���擾
	ID3D11Buffer*			GetBuffer() const { return m_pBuffer; }
	ID3D11Buffer* const*	GetAddress() const { return &m_pBuffer; }

	// �o�b�t�@�̃T�C�Y���擾
	UINT					GetBufferSize() const { return m_bufSize; }

	//=================================================
	//
	// �쐬�E���
	//
	//=================================================

	// �o�b�t�@���쐬
	// �EbindFlags		�c �ǂ̃o�b�t�@�Ƃ���Direct3D�փo�C���h���邩�̃t���O�@D3D11_BIND_FLAG�萔���w�肷��
	// �EbufferSize		�c �쐬����o�b�t�@�̃T�C�Y(byte)
	// �EbufferUsage	�c �o�b�t�@�̎g�p�@�@D3D11_USAGE�萔���w�肷��
	// �EinitData		�c �쐬���ɏ������ރf�[�^ nullptr���Ɖ����������܂Ȃ�
	bool Create(UINT bindFlags, UINT bufferSize, D3D11_USAGE bufferUsage, const D3D11_SUBRESOURCE_DATA* initData);
	
	// ���
	void Release()
	{
		KdSafeRelease(m_pBuffer);
		m_bufSize = 0;
		m_bufUsage = D3D11_USAGE_DEFAULT;
	}

	//=================================================
	//
	// ����
	//
	//=================================================

	// �o�b�t�@�֎w��f�[�^����������
	// Dynamic�̏ꍇ��D3D11_MAP_WRITE_DISCARD��Map�B
	// Default�̏ꍇ��UpdateSubResource�B
	// Staging�̏ꍇ��D3D11_MAP_READ_WRITE��Map�B
	// �EpSrcData		�c �������݂����f�[�^�̐擪�A�h���X
	// �Esize			�c �������ރT�C�Y(byte)
	void WriteData(const void* pSrcData, UINT size);

	// GPU��Ńo�b�t�@�̃R�s�[�����s����
	// ���ڍׂ�DeviceContext��CopyResource()�Q�� https://msdn.microsoft.com/ja-jp/library/ee419574(v=vs.85).aspx
	//  �ꕔ����
	//  ���قȂ郊�\�[�X�ł���K�v������܂��B
	//  ��������ނł���K�v������܂��B
	//  ������ (���A�����A�[�x�A�T�C�Y�Ȃ�) �������ł���K�v������܂��B
	//  ���P���ȃR�s�[�݂̂��s���܂��BCopyResource �ł́A�����L�΂��A�J���[ �L�[�A�u�����h�A�t�H�[�}�b�g�ϊ��̓T�|�[�g����܂���B
	//  ��DXGI�t�H�[�}�b�g�̌݊������K�v�ł��B
	// �EsrcBuffer		�c �R�s�[���o�b�t�@
	void CopyFrom(const KdBuffer& srcBuffer);

	//=================================================

	//
	KdBuffer() {}
	// 
	~KdBuffer() {
		Release();
	}

protected:

	// �o�b�t�@�{��
	ID3D11Buffer*		m_pBuffer = nullptr;

	// �o�b�t�@�̃T�C�Y(byte)
	UINT				m_bufSize = 0;

	// �o�b�t�@�̎g�p�@
	D3D11_USAGE			m_bufUsage = D3D11_USAGE_DEFAULT;

private:
	// �R�s�[�֎~�p
	KdBuffer(const KdBuffer& src) = delete;
	void operator=(const KdBuffer& src) = delete;
};

//========================================================
//
// ��ƃf�[�^�t�� �萔�o�b�t�@�N���X
//
//  Buffer�ɁA�o�b�t�@�Ɠ����T�C�Y�̍�ƃf�[�^������������A
//  �X�V�����������o�b�t�@�ɏ������݂��s������ƁA�Ǘ����y�ɂ����N���X
//
//========================================================
template<class DataType>
class KdConstantBuffer {
public:

	//=================================================
	//
	// �擾�E�ݒ�
	//
	//=================================================

	// ��Ɨ̈�擾�@���ύX�t���O��ON�ɂȂ�܂�
	DataType& Work()
	{
		m_isDirty = true;	// �ύX�t���OON
		return m_work;
	}

	// ��Ɨ̈�擾�@���ǂݎ���p�@�ύX�t���O�͕ω����܂���
	const DataType&			GetWork() const { return m_work; }

	// �o�b�t�@�A�h���X�擾
	ID3D11Buffer* const*	GetAddress() const { return m_buffer.GetAddress(); }


	// m_work��萔�o�b�t�@�֏�������
	// ��m_isDirty��true�̎��̂݁A�o�b�t�@�ɏ������܂��
	void Write()
	{
		// �ύX�����鎞������������
		if (m_isDirty)
		{
			m_buffer.WriteData(&m_work, m_buffer.GetBufferSize());
			m_isDirty = false;
		}
	}


	//=================================================
	//
	// �쐬�E���
	//
	//=================================================

	// DataType�^�̃T�C�Y�̒萔�o�b�t�@���쐬
	// �EinitData		�c �쐬���Ƀo�b�t�@�ɏ������ރf�[�^�@nullptr�ŉ����������܂Ȃ�
	bool Create(const DataType* initData = nullptr)
	{
		if (initData)
		{
			m_work = *initData;
		}

		//----------------------------------
		// �o�b�t�@�쐬
		//----------------------------------
		// �������ރf�[�^
		D3D11_SUBRESOURCE_DATA srd;
		srd.pSysMem = &m_work;
		srd.SysMemPitch = 0;
		srd.SysMemSlicePitch = 0;
		// �쐬
		return m_buffer.Create(	D3D11_BIND_CONSTANT_BUFFER, sizeof(DataType), D3D11_USAGE_DYNAMIC, &srd);
	}

	// ���
	void Release()
	{
		m_buffer.Release();
		m_isDirty = true;
	}

	// 
	~KdConstantBuffer()
	{
		Release();
	}

	KdConstantBuffer() = default;

private:

	// �萔�o�b�t�@
	KdBuffer			m_buffer;

	// ��Ɨp �萔�o�b�t�@
	// ���̓��e��Write�֐��Œ萔�o�b�t�@�{�̂ɕ��ɏ������܂��
	DataType			m_work;

	// �f�[�^�X�V�t���O�@�p�t�H�[�����X����̂��߁A���ꂪtrue�̎�����Write()�Ńf�[�^�������ݎ��s�����悤�ɂ��Ă��܂�
	bool				m_isDirty = true;

private:
	// �R�s�[�֎~�p
	KdConstantBuffer(const KdConstantBuffer& src) = delete;
	void operator=(const KdConstantBuffer& src) = delete;
};


