#include "System/KdSystem.h"

#include "KdBuffer.h"



bool KdBuffer::Create(UINT bindFlags, UINT bufferSize, D3D11_USAGE bufferUsage, const D3D11_SUBRESOURCE_DATA* initData)
{
	Release();

	//----------------------------------
	// �萔�o�b�t�@�Ƃ��č쐬����ꍇ�̓T�C�Y�`�F�b�N���s��
	// ���萔�o�b�t�@��16�o�C�g�A���C�����g(16�̔{��)�ɂ���K�v������
	//----------------------------------
	if (bindFlags == D3D11_BIND_CONSTANT_BUFFER)
	{

		if (bufferSize % 16 != 0) {
			assert(0 && "�R���X�^���g�o�b�t�@�쐬�̃T�C�Y��16�̔{���o�C�g�łȂ���΂Ȃ�Ȃ���!!(Buffer)");
			return false;
		}

		// �T�C�Y��16�̔{���o�C�g�ɂ���
//		bufferSize = ((bufferSize + 16 - 1) / 16) * 16;

	}

	//--------------------------------
	// �o�b�t�@�쐬�̂��߂̏ڍ׃f�[�^
	//--------------------------------
	D3D11_BUFFER_DESC desc;
	desc.BindFlags			= bindFlags;		// �f�o�C�X�Ƀo�C���h����Ƃ��̎��(���_�o�b�t�@�A�C���f�b�N�X�o�b�t�@�A�萔�o�b�t�@�Ȃ�)
	desc.ByteWidth			= bufferSize;		// �쐬����o�b�t�@�̃o�C�g�T�C�Y
	desc.MiscFlags			= 0;				// ���̑��̃t���O
	desc.StructureByteStride= 0;				// �\�����o�b�t�@�̏ꍇ�A���̍\���̂̃T�C�Y

	desc.Usage				= bufferUsage;		// �쐬����o�b�t�@�̎g�p�@

	// ���I�r�f�I�������o�b�t�@
	//  GPU����Write�~ Read��
	//  CPU����Write�� Read�~
	// �p�ɂɍX�V�����悤�ȃo�b�t�@�͂��ꂪ�����ǂ����ADEFAULT�ɔ�ׂ��班�����x�͗�����
	if (desc.Usage == D3D11_USAGE_DYNAMIC)
	{
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	// �ÓI�r�f�I�������o�b�t�@
	//  GPU����Write�� Read��
	//  CPU����Write�~ Read�~�@(������UpdateSubresource()�ōX�V�͉\)
	// �r�f�I�������[�̃o�b�t�@�@�p�ɂɍX�V����ɂ͌����Ă��Ȃ����`�悪����
	else if(desc.Usage == D3D11_USAGE_DEFAULT)
	{
		desc.CPUAccessFlags = 0;
	}
	// �X�e�[�W���O�o�b�t�@
	//  GPU����Write�~ Read�~
	//  CPU����Write�� Read��
	// Direct3D�փo�C���h�͏o���Ȃ����ADEFAULT��DYNAMIC�̃o�b�t�@�ɑ΂��ēǂݏ����]�����\
	// (��)DEFAULT�o�b�t�@�̓��e���擾�������I -> STAGING�o�b�t�@���쐬���ADEFAULT�o�b�t�@����R�s�[���Ă���(CopyResource�֐�)�B������STAGING�o�b�t�@�ɃA�N�Z�X(Map/Unmap)����B
	else if (desc.Usage == D3D11_USAGE_STAGING)
	{
		desc.BindFlags = 0;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
	}
	
	//--------------------------------
	// �o�b�t�@�쐬
	//--------------------------------
	if (FAILED(D3D.GetDev()->CreateBuffer(&desc, initData, &m_pBuffer)))
	{
		assert(0 && "�o�b�t�@�쐬���s(Buffer)");
		return false;
	}

	// 
	m_bufUsage = bufferUsage;
	m_bufSize = bufferSize;

	return true;
}

void KdBuffer::WriteData(const void* pSrcData, UINT size)
{
	// ���I�o�b�t�@�̏ꍇ
	if (m_bufUsage == D3D11_USAGE_DYNAMIC)
	{
		D3D11_MAPPED_SUBRESOURCE pData;
		// �������ݐ�p
		if (SUCCEEDED(D3D.GetDevContext()->Map(m_pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
		{
			memcpy_s(pData.pData, m_bufSize, pSrcData, size);

			D3D.GetDevContext()->Unmap(m_pBuffer, 0);
		}
	}
	// �ÓI�o�b�t�@�̏ꍇ
	else if (m_bufUsage == D3D11_USAGE_DEFAULT)
	{
		// �o�b�t�@�ɏ�������
		D3D.GetDevContext()->UpdateSubresource(m_pBuffer, 0, 0, pSrcData, 0, 0);
	}
	// �X�e�[�W���O�o�b�t�@�̏ꍇ
	else if (m_bufUsage == D3D11_USAGE_STAGING)
	{
		// �ǂݏ�������
		D3D11_MAPPED_SUBRESOURCE pData;
		if (SUCCEEDED(D3D.GetDevContext()->Map(m_pBuffer, 0, D3D11_MAP_READ_WRITE, 0, &pData)))
		{
			memcpy_s(pData.pData, m_bufSize, pSrcData, size);

			D3D.GetDevContext()->Unmap(m_pBuffer, 0);
		}
	}

}

void KdBuffer::CopyFrom(const KdBuffer& srcBuffer)
{
	if (m_pBuffer == nullptr)return;
	if (srcBuffer.GetBuffer() == nullptr)return;

	D3D.GetDevContext()->CopyResource(m_pBuffer, srcBuffer.GetBuffer());
}
