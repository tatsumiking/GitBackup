#ifndef __STDEF_H
#define __STDEF_H

#include	<math.h>

typedef double DBL;
typedef double FAR* LPDBL;
typedef int FAR* LPINT;
typedef short FAR* LPSHORT;
typedef	unsigned int FAR* LPUINT;
typedef	unsigned char FAR* LPUCHAR;
typedef	unsigned char UCHAR;
typedef unsigned short USHORT; 
typedef unsigned int UINT; 

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif
#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

#define GETSIGNED(a)		((a >= 0) ? (1) : (-1))
#define EQUALPLT(a, b)		(fabs(a-b) < 0.1)
#define EQUAL(a, b)			(fabs(a-b) < 0.0000001)
#define NEQUAL(a, b)		(fabs(a-b) > 0.0000001)
#define FLOOR100(a)			(floor((a) * 100.0 + 0.5) / 100.0);
#define FLOOR10(a)			(floor((a) * 10.0 + 0.5) / 10.0);
#define SWAP(a, b, c)		(c)=(a);(a)=(b);(b)=(c);

#define VER100	100
#define VER101	101
#define VER102	102

#define CLR		0
#define BLACK	1

#define	NOMAL	0
#define	LCLMCLR	1
#define	OGCLR	2

#define	ESC	0x1b
#define	CAN	0x18
#define	FF	0x0c
#define	LF	0x0a
#define	CR	0x0d
#define PAT 0x25
#define	DCT	0x22
#define UVK_DELETE	0x07
#define UVK_LEFT	0x1c
#define UVK_RIGHT	0x1d
#define UVK_UP		0x1e
#define UVK_DOWN	0x1f

#define MODAL		0
#define	MODALESS	1

#define	NONSELECT	0
#define PRINTOUT	1
#define CHANGEFONTOFFOLDER	104
#define CHANGEPAPEROFFOLDER	105
#define ANYSELECT	106
#define CHANGEFONTOFFILE	107
#define PRINTANYOUT	108
#define MINIPRINT	109
#define CSVPRINT	110
#define CSVOUT		111
#define SELECTFOLDER	120

#define GAIJILIST	150

#define	FILDTBLMAX	20
#define FILDNAMEMAX	64

#define	TABLESIZE		128L
#define	TABLESIFT		1
//#define	TABLESIZE		256L
//#define	TABLESIFT		0

#define AREAMAXLIMIT	0x7fff
//#define AREAMAXLIMIT	1000000

//#define	INTMAX	0x7fff
//#define	INTDLIMIT	20000.0
#define	INTMAX		1000000
#define	INTDLIMIT	1000000.0
#define NUMMAX		1000000
#define	LIMITLINE	4
#define	MJFREESPACEPAR	-400
#define	MJFREEFLATPAR	-400
#define	NOSETPAR	-800
#define NOSETCLR	-1
#define	NOSETMM		10000
#define	WAKUNOTEXTSIZE	16

#define	OFF		0
#define	ON		1
#define FAIL	-1
#define	CROSS		1	// �������
#define	INAREA		2	// �͈͓�

#define IMAGESIZE	128

#define NONMODE		0
#define PAPERSLCT	1
#define	FILELOAD	2
#define	INPEDIT		3
#define	FILESAVE	4
#define	FILEDELETE	5
#define	FILELIST	6
#define	FILETEXT	7

#define	DBCSV	1
#define DBXLS	2
#define DBMDB	3

#define	YOKO	0
#define TATE	1
#define	YOKOGAKI	0
#define TATEGAKI	1
#define TATEOKI		2
#define	YOKOOKI		3

#define	XMSURTYPE	1
#define	YMSURTYPE	2

#define TEMPBUFMAX	8192
#define	RECMAX16	4096
#define	RECMAX8		2048
#define	RECMAX4		1024
#define	RECMAX2		512
#define	RECMAXAD	272
#define	RECMAX		256
#define	HRECMAX		128
#define	QRECMAX		64
#define	HQRECMAX	32
#define	NRECMAX		16
#define FNAMEMAX	260
#define FACEMAX		32		// LF_FACESIZE
#define STRGMAX		256+4
#define FACEMAXEX	64		// LF_FACESIZE*2
#define	DBFILDMAX	128
#define	DBWAKUMAX	32
#define	DBNAMEMAX	32

#define	REJOINCNTMAX	4096
#define	PLGNPOINTMAX	0x7fff
#define	POINTMAX		4096
#define TTNAMELISTMAX	138
#define DISPNAMETOP		9
#define ACCESSNAMETOP	74
// �\�o�͂Ŏg�p
#define WAKUNAMETOP	24
#define RECSTRTOP	31

#define	HPLTCNTR	0
#define HPLTLEFT	1
#define HPLTRIGHT	2
#define VPLTCNTR	0
#define VPLTTOP		1
#define VPLTBOTTOM	2

#define	NONRESET	0
#define	FONTRESET	1
#define	FLATRESET	2
#define	SPACERESET	3
#define	ODERRESET	4
#define	TATERESET	5
#define	ROUNDRESET	6
#define	MIRRORRESET	7

#define	ARCSYOTAIMAX	16
// �v���|�[�V���i�������p�̈�̃T�C�Y
#define	FPROPSIZE	256
#define	FPRSPSIZE	128
#define	FDIBCSIZE	512

//#define	FPROPSIZE	128
//#define	FPRSPSIZE	64
//#define	FDIBCSIZE	256
#define	BOLDSIZE	2048
#define	BOLDSPACE	512
//#define	BOLDPAR		0.006
#define	BOLDPAR		0.01

#define	LEFTUP		1
#define	LEFTDOWN	2
#define	LEFTCNTR	3
#define	RIGHTUP		4
#define	RIGHTDOWN	5
#define	RIGHTCNTR	6
#define	CNTRUP		7
#define	CNTRDOWN	8
#define	CNTRCNTR	9

#define	TOP			1
#define	BOTTOM		2
#define	HCNTR		3
#define	LEFT		4
#define	RIGHT		5
#define	VCNTR		6

#define NOMALPRN		0
#define MAXFILLPLOT		1
#define MAXCUTPLOT		2
#define	DETAILPLOT		3

#define ODEREQUAL	0
#define ODERLEFT	1
#define ODERCNTR	2
#define ODERRIGHT	3
#define ODERFITING	4

#define NOMIRROR	0
#define RLMIRROR	0x01
#define	TBMIRROR	0x10
#define PTMIRROR	0x11

#define	SPACEWAKU	0x00
#define	SPACEDATA	0x01
#define	SPACEPROP	0x02
#define SESPOFF		0x00
#define SESPON		0x01

#define	SDRAWFILL	0
#define	SDRAWNON	1
#define	SDRAWLINE	2

#define DRAWNOFILL	0
#define DRAWFILL	1
#define DRAWTURNOVER	2

#define DEFWAKU		0
#define KAMONWAKU	1
#define FIGUOBJ		2
#define LOGOWAKU	3
#define POPWAKU		4
#define MAEWAKU1	10
#define MAEWAKU2	11

// �g�ȊO�̃I�u�W�F�N�g
#define	LINEDATA	0x11
#define BOXSDATA	0x12
#define RBOXSDATA	0x13
#define	CRCLDATA	0x14
#define	NONDATA		0x00ff

#define	UNITPOINT	0
#define UNITKYUSU	1
#define UNITMM		2
#define UNITSUN		3

#define	BKVAL		0
#define	FGVAL		255

#define	NORAG		9999
#define	HALFPAI		90

#define		M_PI	3.14159265358979323846
#define		M_PI_2	1.57079632679489661923
#define		M_PI_4	0.78539816339744830962
#define M_01RAG		0.001745329251994
#define M_1RAG		0.01745329251994
#define	M_2RAG		0.03490658503988
#define	M_5RAG		0.08726646259969
#define	MMPARINTI	25.4
#define	MMTOPOINT	2.8345
#define POINTTOMM	0.3528

#define FNAMETOP	30

#define TTFONT	20
#define TT1FONT	21		//�s�������s����������
#define TT2FONT	22		//�ǉ��s�������s����������
#define TT3FONT	23		//�p���s�������s����������
#define TT4FONT	24		//�V���{���s�������s����������
#define	ARKFONT 31		//�A�[�N�e�s�P�`������
#define	ARCFONT 32		//�A�[�N�e�s�Q�`������
#define	MAXFONT 33		//�A�[�N�l�`�w�`������
#define	MX2FONT 34		//�l�w�Q�`������
#define	SCRFONT 41		//�r�b�q�`������

#define	PRINTVIEW	0
#define	PREVIEW		1
#define	LINEVIEW	2
#define POINTVIEW	3
#define	OUTLINEVIEW	4
#define	BLACKVIEW	5
#define	DIRCTVIEW	6
#define	BITMAPVIEW	7
#define	METAFVIEW	8
#define	PRINTPREVIEW	9

#define STATBARHELP 0
#define STATBARSTAT 1
#define STATBARMODE 2
#define STATBARCMMD 3
#define STATBARCOOD 4

#define NEARLIMIT	32
#define ZNEARLIMIT	16
#define HNEARLIMIT	16
#define QNEARLIMIT	 8
#define SNEARLIMIT	 4
#define LNEARLIMIT	 2

#define	ADDMM	10
#define NORIMM	2

// �V�X�e����ԃt���O
#define EXECFLAG	0x0001
#define STOPFLAG	0x0002

#define	LINEKIND	1
#define	BEZEKIND	2
#define	ARCKIND		3
#define	FREEKIND	4
#define	FCBEZEKIND	5
#define	PLGNKIND	6

// �����t���O
#define	MOJIATRTATEGAKI	0x0001
#define MOJIATRMAXLEGAL	0x0002

// �x�N�^�[�t���O
#define		REJOINFLAG		0x0001
#define		DISPFLAG		0x0002
#define		ACTIVEFLAG		0x0004
#define		VCUTFLAG		0x0008
#define		VSUBFLAG		0x0010
#define		VWAKUFLAG		0x0020
#define		MOVELOCKFLAG	0x0040 // ���g�p
#define		ALLLOCKFLAG		0x0080	// ���Ή�
#define		ATNTION1FLAG	0x0100
#define		ATNTION2FLAG	0x0200
#define		KATAGAKIFLAG	0x0400	// �ȎD����������
#define		UNDERLINEFLAG	0x0800
#define		COPYFLAG		0x1000

// �|�C���g�t���O
#define	PATRMASK	0x0e
#define	BEZEATR		0x08
#define	CURVATR		0x04
#define	CONTATR		0x02
#define PATN0ATR	0x10
#define PATN1ATR	0x20
#define PATN2ATR	0x40
#define PATN3ATR	0x80

/* �v���b�^�[�@���` */
#define MAXRP950 213
#define MAXRP370 214
#define MAXRP350 216
#define MAXRP100 219
#define MAXCM450 221
#define MAXCM510 222

#define NOOPDATAID		0
#define BASEDATAID		1
#define	POINTBLOCKID	10
#define	BASEVECTID		11
#define	VECTLISTID		12
#define DIBCDATAID      21
#define CLIPFDIBID      22
#define CLIPDATAID      23
#define TTEXTID         31
#define TEXTLINEID      32
#define SWAKTEXTID		33
#define KAMONDATAID		41
#define	OOLEDATAID      50
#define	OLEDATAID       51
#define	DATAGROUPID     80
#define	SCRNENVID		90

#define CDNTOBJ		0
#define	CDNTPAGE	1

#define	J0LEVEL0	0
#define	J0LEVEL1	1
#define	J0LEVEL2	2
#define	J0LEVEL3	3

#define	TTFONTBASE	256

#define	ITIRANSLCT	0
#define	ROMAESLCT	1
#define	HUUTOSLCT	2
#define	SYUUKOTUSLCT	3
#define	TUYASLCT	4
#define	HEYASLCT	5
#define	ANNAISLCT	6

#define _AFX_FP_OFF(thing) (*((UINT*)&(thing)))
#define _AFX_FP_SEG(lp) (*((UINT*)&(lp)+1))
#define	NEAR0(len)	(-0.0000001 < len && len < 0.0000001)	

//#define	GAIJIMESH	1024
#define	GAIJIMESH	1000

typedef struct {
	double	a11, a12;
	double	a21, a22;
	double	a31, a32;
} dmtxtype;

typedef	struct {
	DBL	dx, dy;
	DBL	drag;
} DXYRagType;

typedef	struct {
	DBL	x, y;
} DXYType;

typedef	struct {
	int	x, y;
} XYType;

typedef struct {
	int	x, queep;
} QTopType;

typedef struct {
	int	before, next;
	int	x, y;
} QueeType;

typedef struct {
	double	x1, y1;
	double	x2, y2;
	double	x3, y3;
	double	x4, y4;
} dpointtype;

typedef struct {	//s �a�d�y�d�\���i�[�\��
	double	x1, y1;		//sd �a�d�y�d�Ȑ��̎n�_���W
	double	x2, y2;		//sd �a�d�y�d�Ȑ��̒��ԓ_���W
	double	x3, y3;		//sd �a�d�y�d�Ȑ��̒��ԓ_���W
	double	x4, y4;		//sd �a�d�y�d�Ȑ��̏I�_���W
} dbezetype;

typedef struct {	//s �~�ʍ\���i�[�\��
	double	x1, y1;		//sd �~�ʋȐ��̎n�_���W
	double	x2, y2;		//sd �~�ʋȐ��̒��ԓ_���W
	double	x3, y3;		//sd �~�ʋȐ��̏I�_
	double	x4, y4;
} darctype;

typedef struct {	//s ���R�Ȑ��i�[�\��
	int		atr0, atr1, atr2, atr3;	//sd �_����
	double	x0, y0;		//sd ���R�Ȑ��̑O�̓_�̍��W
	double	x1, y1;		//sd ���R�Ȑ��̎n�_���W
	double	x2, y2;		//sd ���R�Ȑ��̏I�_���W
	double	x3, y3;		//sd ���R�Ȑ��̎��̓_�̍��W
} dcurvetype;

typedef struct {	//s �Ȑ�
	int		atr0, atr1, atr2, atr3, atr4;	//sd �_����
	double	x0, y0;		//sd ���R�Ȑ��̑O�̓_�̍��W
	double	x1, y1;		//sd �Ȑ��̂P�_�ڂ̍��W
	double	x2, y2;		//sd �Ȑ��̂Q�_�ڂ̍��W
	double	x3, y3;		//sd �Ȑ��̂R�_�ڂ̍��W
	double	x4, y4;		//sd �Ȑ��̂S�_�ڂ̍��W
} dfigutype;

#define ARCMESH		10000.0

#define	MOVETO		0
#define	LINETO		1
#define	ARCTO		2
#define	ARCEND		3
#define	ENDOF		-1

typedef struct {	//s  �����\���i�[�\��
	double	x1, y1;		//sd �����̎n�_���W
	double	x2, y2;		//sd �����̏I�_���W
} dlinetype;

typedef struct	{
	char		cmdtstr[10];
	int			cmdtype;
	DBL			sizex, sizey;
	DBL			step;
	DBL			sx, sy;
	DBL			xtime, ytime;
	int			arcflg, outflg;
} pltenvtype;

typedef	struct // �Ǘ��t�@�C���E�w�b�_�[
{
	char	szCorp[32];			// ��Ж�
	char	szVersion[16];		// �o�[�W������� ���ӁI:'\0'�ŏI�[���ĂȂ�.
	char	szKamonType[12];	// "DEMO"�ȊO�͐��i��
	DWORD	lKamon;				// �o�^�Ɩ䐔
} KindexHeaderType;

typedef struct // �Ǘ��t�@�C���t�H�[�}�b�g
{
	char	kCode[11];			// �Ɩ�ԍ�		"II001"
	char	kClass[11];			// ���޺���		"�����"
	char	kName[31];			// �Ɩ䖼		"����̊�"
	char	comment[11];		// ����			""
} KindexType;

typedef struct {
	short	x1, y1;				// �������ړ��l
	short	x2, y2;				// �������{�f�B�[�T�C�Y
	short	x3, y3;				// �c�����ړ��l
	short	x4, y4;				// �c�����{�f�B�[�T�C�Y
} archtype;

typedef	struct {
	short	atr;
	short	x;
	short	y;
} arcpointype;

#endif

