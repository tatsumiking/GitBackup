// Machine generated IDispatch wrapper class(es) created with ClassWizard

#include "stdafx.h"
#include "excel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// --------------------------------------------------------------------------------
// [���O] FindFile
// [�T�v] �t�@�C���I������
// [����] �Ȃ�
// [�ߒl] �Ȃ�
// [�X�V�� / �X�V�� / �X�V���e]
// 2007/11/01 STAN �R�����g�쐬
// --------------------------------------------------------------------------------
void _Application::FindFile()
{
	InvokeHelper(0x42c, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

// --------------------------------------------------------------------------------
// [���O] FindFileV9
// [�T�v] �t�@�C���I������
// [����] �Ȃ�
// [�ߒl] BOOL	// �����̉�
// [�X�V�� / �X�V�� / �X�V���e]
// 2007/11/01 STAN �R�����g�쐬
// --------------------------------------------------------------------------------
BOOL _Application::FindFileV9()
{
	BOOL result;
	InvokeHelper(0x6eb, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
	return result;
}

// --------------------------------------------------------------------------------
// [���O] GetWorkbooks
// [�T�v] �u�b�N�R���N�V�����̎擾
// [����] �Ȃ�
// [�ߒl] LPDISPATCH	// �I�u�W�F�N�g�A�h���X
// [�X�V�� / �X�V�� / �X�V���e]
// 2007/11/01 STAN �R�����g�쐬
// --------------------------------------------------------------------------------
LPDISPATCH _Application::GetWorkbooks()
{
	LPDISPATCH result;
	InvokeHelper(0x23c, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

// --------------------------------------------------------------------------------
// [���O] GetWorksheets
// [�T�v] �V�[�g�R���N�V�����̎擾
// [����] �Ȃ�
// [�ߒl] LPDISPATCH	// �I�u�W�F�N�g�A�h���X
// [�X�V�� / �X�V�� / �X�V���e]
// 2007/11/01 STAN �R�����g�쐬
// --------------------------------------------------------------------------------
LPDISPATCH _Application::GetWorksheets()
{
	LPDISPATCH result;
	InvokeHelper(0x1ee, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

// --------------------------------------------------------------------------------
// [���O] GetActiveWorkbook
// [�T�v] �A�v���P�[�V�����I�u�W�F�N�g���擾
// [����] �Ȃ�
// [�ߒl] LPDISPATCH	// �I�u�W�F�N�g�A�h���X
// [�X�V�� / �X�V�� / �X�V���e]
// 2007/11/01 STAN �R�����g�쐬
// --------------------------------------------------------------------------------
LPDISPATCH _Application::GetActiveWorkbook()
{
	LPDISPATCH result;
	InvokeHelper(0x134, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

// --------------------------------------------------------------------------------
// [���O] Quit
// [�T�v] �A�v���P�[�V�����I������
// [����] �Ȃ�
// [�ߒl] �Ȃ�
// [�X�V�� / �X�V�� / �X�V���e]
// 2007/11/01 STAN �R�����g�쐬
// --------------------------------------------------------------------------------
void _Application::Quit()
{
	InvokeHelper(0x12e, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

// --------------------------------------------------------------------------------
// [���O] OpenBook
// [�T�v] �u�b�N�R���N�V�����N�I�[�v������
// [����] LPCTSTR Filename	// �I�[�v���Ώۃt�@�C����
// [����] const VARIANT& UpdateLinks	// �I�[�v�����
// [�ߒl] LPDISPATCH	// �I�u�W�F�N�g�A�h���X
// [�X�V�� / �X�V�� / �X�V���e]
// 2007/11/01 STAN �R�����g�쐬
// --------------------------------------------------------------------------------
LPDISPATCH Workbooks::OpenBook(LPCTSTR Filename, const VARIANT& UpdateLinks)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR VTS_VARIANT;
	InvokeHelper(0x2aa, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, Filename, &UpdateLinks);
	return result;
}

// --------------------------------------------------------------------------------
// [���O] Close
// [�T�v] �u�b�N�R���N�V�����N���[�Y����
// [����] �Ȃ�
// [�ߒl] �Ȃ�
// [�X�V�� / �X�V�� / �X�V���e]
// 2007/11/01 STAN �R�����g�쐬
// --------------------------------------------------------------------------------
void Workbooks::Close()
{
	InvokeHelper(0x115, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

// --------------------------------------------------------------------------------
// [���O] GetCount
// [�T�v] �u�b�N���̎擾
// [����] �Ȃ�
// [�ߒl] long	// �u�b�N��
// [�X�V�� / �X�V�� / �X�V���e]
// 2007/11/01 STAN �R�����g�쐬
// --------------------------------------------------------------------------------
long Workbooks::GetCount()
{
	long result;
	InvokeHelper(0x76, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

// --------------------------------------------------------------------------------
// [���O] GetWorksheets
// [�T�v] �V�[�g�R���N�V�����̎擾
// [����] �Ȃ�
// [�ߒl] LPDISPATCH	// �I�u�W�F�N�g�A�h���X
// [�X�V�� / �X�V�� / �X�V���e]
// 2007/11/01 STAN �R�����g�쐬
// --------------------------------------------------------------------------------
LPDISPATCH _Workbook::GetWorksheets()
{
	LPDISPATCH result;
	InvokeHelper(0x1ee, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

// --------------------------------------------------------------------------------
// [���O] GetPath
// [�T�v] �u�b�N�p�X���̎擾
// [����] �Ȃ�
// [�ߒl] CString	// �u�b�N�p�X��
// [�X�V�� / �X�V�� / �X�V���e]
// 2007/11/01 STAN �R�����g�쐬
// --------------------------------------------------------------------------------
CString _Workbook::GetPath()
{
	CString result;
	InvokeHelper(0x123, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

// --------------------------------------------------------------------------------
// [���O] GetFullName
// [�T�v] �t���u�b�N���̎擾
// [����] �Ȃ�
// [�ߒl] CString	// �t���u�b�N��
// [�X�V�� / �X�V�� / �X�V���e]
// 2007/11/01 STAN �R�����g�쐬
// --------------------------------------------------------------------------------
CString _Workbook::GetFullName()
{
	CString result;
	InvokeHelper(0x121, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

// --------------------------------------------------------------------------------
// [���O] GetName
// [�T�v] �u�b�N���̎擾
// [����] �Ȃ�
// [�ߒl] CString	// �u�b�N��
// [�X�V�� / �X�V�� / �X�V���e]
// 2007/11/01 STAN �R�����g�쐬
// --------------------------------------------------------------------------------
CString _Workbook::GetName()
{
	CString result;
	InvokeHelper(0x6e, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

// --------------------------------------------------------------------------------
// [���O] GetItem
// [�T�v] �V�[�g�R���N�V��������Ή�����V�[�g���擾����
// [����] const VARIANT& Index	// �V�[�g�ԍ�
// [�ߒl] LPDISPATCH	// �I�u�W�F�N�g�A�h���X
// [�X�V�� / �X�V�� / �X�V���e]
// 2007/11/01 STAN �R�����g�쐬
// --------------------------------------------------------------------------------
LPDISPATCH Worksheets::GetItem(const VARIANT& Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0xaa, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms,
		&Index);
	return result;
}

// --------------------------------------------------------------------------------
// [���O] Select
// [�T�v] �V�[�g��I����Ԃɂ���
// [����] �Ȃ�
// [�ߒl] �Ȃ�
// [�X�V�� / �X�V�� / �X�V���e]
// 2007/11/01 STAN �R�����g�쐬
// --------------------------------------------------------------------------------
void _Worksheet::Select() // added
{
	InvokeHelper(0xeb, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

// --------------------------------------------------------------------------------
// [���O] GetName
// [�T�v] �V�[�g���̎擾
// [����] �Ȃ�
// [�ߒl] CString	// �V�[�g��
// [�X�V�� / �X�V�� / �X�V���e]
// 2007/11/01 STAN �R�����g�쐬
// --------------------------------------------------------------------------------
CString _Worksheet::GetName()
{
	CString result;
	InvokeHelper(0x6e, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

// --------------------------------------------------------------------------------
// [���O] GetRange
// [�T�v] ��������̈�
// [����] const VARIANT& Cell1	// �擾����Z��������
// [�ߒl] LPDISPATCH	// �I�u�W�F�N�g�A�h���X
// [�X�V�� / �X�V�� / �X�V���e]
// 2007/11/01 STAN �R�����g�쐬
// --------------------------------------------------------------------------------
LPDISPATCH _Worksheet::GetRange(const VARIANT& Cell1) // added
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0xc5, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, 
		parms,&Cell1);
	return result;
}

// --------------------------------------------------------------------------------
// [���O] GetValue
// [�T�v] �̈�ɒl���擾����
// [����] �Ȃ�
// [�ߒl] VARIANT	// �擾�����l
// [�X�V�� / �X�V�� / �X�V���e]
// 2007/11/01 STAN �R�����g�쐬
// --------------------------------------------------------------------------------
VARIANT Range::GetValue()
{
	VARIANT result;
	InvokeHelper(0x6, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
	return result;
}

// --------------------------------------------------------------------------------
// [���O] SetValue
// [�T�v] �̈�ɒl���Z�b�g����
// [����] const VARIANT& newValue	// �Z�b�g����l
// [�ߒl] �Ȃ�
// [�X�V�� / �X�V�� / �X�V���e]
// 2007/11/01 STAN �R�����g�쐬
// --------------------------------------------------------------------------------
void Range::SetValue(const VARIANT& newValue)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 &newValue);
}

