#pragma once

class CCrctWaku :
	public CObject
{
public:
	CCrctWaku();
	~CCrctWaku();
public:
	long m_nCode; //1 ���R�[�h�ԍ�
	CString	m_csFlag; //8 "0"�@"1"
	CString m_csMiSumi; //6 "��"�@"��"
	CString m_csGroupName; //2 �g�O���[�v��
	CString m_csYousiName; //3 �p����
	CString m_csFileName; //4 �g�t���p�X��
	CString m_csCount; //7 ����
	CString m_csStrings; //5 /�ŋ�؂����g������
};

