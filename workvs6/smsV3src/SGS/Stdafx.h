// stdafx.h : �W���̃V�X�e�� �C���N���[�h �t�@�C���A
//            �܂��͎Q�Ɖ񐔂������A�����܂�ύX����Ȃ�
//            �v���W�F�N�g��p�̃C���N���[�h �t�@�C�����L�q���܂��B
//

#if !defined(AFX_STDAFX_H__D2ED8FA8_417C_11D2_99B2_004026171D2B__INCLUDED_)
#define AFX_STDAFX_H__D2ED8FA8_417C_11D2_99B2_004026171D2B__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define VC_EXTRALEAN		// Windows �w�b�_�[����w�ǎg�p����Ȃ��X�^�b�t�����O���܂��B

#include <afxwin.h>         // MFC �̃R�A����ѕW���R���|�[�l���g
#include <afxext.h>         // MFC �̊g������
#include <afxole.h>         // MFC �� OLE �N���X
#include <afxodlgs.h>       // MFC �� OLE �_�C�A���O �N���X
#include <afxdisp.h>        // MFC �� OLE �I�[�g���[�V���� �N���X
#include <afxdlgs.h>        // MFC �� �_�C�A���O�N���X
#include <afxdtctl.h>		// MFC �� Internet Explorer 4 �R���� �R���g���[�� �T
#include <afxtempl.h>		// MFC �̃e���v���[�g
#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>			// MFC ODBC database classes
#endif // _AFX_NO_DB_SUPPORT
#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>			// MFC DAO database classes
#endif // _AFX_NO_DAO_SUPPORT
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC �� Windows �R���� �R���g���[�� �T�|�[�g
#endif // _AFX_NO_AFXCMN_SUPPORT


//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#include <imm.h>

#include "..\stdef.h"
#include "..\defenv.h"

#include "resource.h"
#include "sysenv.h"
#include "sgs.h"

#include "..\cmnsrc\mydialog.h"
#include "..\cmnsrc\mydlgbar.h"
#include "..\cmnsrc\myedit.h"
#include "..\cmnsrc\myfiledl.h"
#include "..\cmnsrc\mybutton.h"
#include "sgsDoc.h"
#include "sgsView.h"
#include "MainFrm.h"
#include "MainFrm.h"

// CG: ���̍s�́uIME �T�|�[�g�v�R���|�[�l���g�ɂ��ǉ�����Ă��܂��B
#pragma comment(lib, "imm32.lib")
#endif // !defined(AFX_STDAFX_H__D2ED8FA8_417C_11D2_99B2_004026171D2B__INCLUDED_)
