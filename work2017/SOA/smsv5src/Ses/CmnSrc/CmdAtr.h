
#pragma once


//G�@�T�v
//	�ړ��A���ʁA�ό`�p�̃I�u�W�F�N�g�ɑ΂���
//�@�x�_��ݒ肷��@�\��񋟂���N���X
//

//C �x�_�ݒ�R�}���h�����N���X
class CCmdAtr : public CCmdBase
{
public:
	CCmdAtr(CScrollView* pcview);
	UINT ExecAtrConer(UINT event, DBL dx, DBL dy);
	UINT ExecAtrWakuConer(UINT event, DBL dx, DBL dy);
	UINT ExecAtrEditConer(UINT event, DBL dx, DBL dy);
	UINT ExecAtrMojiConer(UINT event, DBL dx, DBL dy);
public:

private:

};

