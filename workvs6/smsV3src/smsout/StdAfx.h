// stdafx.h : �W���̃V�X�e�� �C���N���[�h �t�@�C���A
//            �܂��͎Q�Ɖ񐔂������A�����܂�ύX����Ȃ�
//            �v���W�F�N�g��p�̃C���N���[�h �t�@�C�����L�q���܂��B
//

#if !defined(AFX_STDAFX_H__2D225333_631B_4625_85F5_6032071C2127__INCLUDED_)
#define AFX_STDAFX_H__2D225333_631B_4625_85F5_6032071C2127__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Windows �w�b�_�[����w�ǎg�p����Ȃ��X�^�b�t�����O���܂��B

#include <afxwin.h>         // MFC �̃R�A����ѕW���R���|�[�l���g
#include <afxext.h>         // MFC �̊g������
#include <afxdisp.h>        // MFC �̃I�[�g���[�V���� �N���X
#include <afxdtctl.h>		// MFC �� Internet Explorer 4 �R���� �R���g���[�� �T�|�[�g
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

#include <imm.h>

#include "..\stdef.h"
#include "..\defenv.h"

#include "Resource.h"

#include "smsout.h"
#include "sysenv.h"

#include "smsoutDoc.h"
#include "smsoutView.h"
#include "MainFrm.h"
// CG: ���̍s�́uIME �T�|�[�g�v�R���|�[�l���g�ɂ��ǉ�����Ă��܂��B
#pragma comment(lib, "imm32.lib")

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_STDAFX_H__2D225333_631B_4625_85F5_6032071C2127__INCLUDED_)
