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

#define EQUALPLT(a, b)		(fabs(a-b) < 0.1)
#define EQUAL01(a, b)		(fabs(a-b) < 0.1)
#define EQUAL(a, b)			(fabs(a-b) < 0.0000001)
#define NEQUAL(a, b)		(fabs(a-b) > 0.0000001)
#define FLOOR100(a)			(floor((a) * 100.0 + 0.5) / 100.0);
#define FLOOR10(a)			(floor((a) * 10.0 + 0.5) / 10.0);
#define SWAP(a, b, c)		(c)=(a);(a)=(b);(b)=(c);

#define VER100	100
#define VER300	300
#define VER310	310
#define VER350	350

#define CLR		0
#define BLACK	1

#define	NOMAL	0
#define	LCLMCLR	1
#define	OGCLR	2

#define	TAB	0x09
#define	FF	0x0c
#define	LF	0x0a
#define	CR	0x0d
#define	CAN	0x18
#define	ESC	0x1b
#define	DCT	0x22
#define PAT 0x25
#define UVK_DELETE	0x07
#define UVK_LEFT	0x1c
#define UVK_RIGHT	0x1d
#define UVK_UP		0x1e
#define UVK_DOWN	0x1f

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
#define	LIMITLINE	4
#define	MJFREESPACEPAR	-400
#define	MJFREEFLATPAR	-400
#define	NOSETPAR	-800
#define NOSETCLR	-1
#define	NOSETMM		10000
#define	MJEQUALSP	8000
#define	WAKUNOTEXTSIZE	16
#define SLCTNEARLIMIT	4

#define	OFF		0
#define	ON		1
#define FAIL	-1
#define	CROSS		1	// 交差状態
#define	INAREA		2	// 範囲内

#define	YOKO	0
#define TATE	1
#define	YOKOGAKI	0
#define TATEGAKI	1
#define TATEOKI		2
#define	YOKOOKI		3

#define	XMSURTYPE	1
#define	YMSURTYPE	2

#define NOMIRROR	0
#define RLMIRROR	0x01
#define	TBMIRROR	0x10
#define PTMIRROR	0x11

#define TEMPBUFMAX	8192
#define TEMPBUFMAX2	16384
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

#define	DBCSV	1
#define DBXLS	2
#define DBMDBT	3
#define DBMDBQ	4

// プロポーショナル処理用領域のサイズ
#define	FPROPSIZE	128
#define FPRSPSIZE	64
#define	FDIBCSIZE	256
#define	BOLDSIZE	1024

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

#define ODEREQUAL	0
#define ODERLEFT	1
#define ODERCNTR	2
#define ODERRIGHT	3

#define	SPACEWAKU	0x00
#define	SPACEDATA	0x01
#define SESPOFF		0x00
#define SESPON		0x10

#define DEFWAKU		0
#define NAMEWAKU	1
#define BMPWAKU		2
#define LOGOWAKU	3
#define POPWAKU		4
#define MAEWAKU1	10
#define MAEWAKU2	11
// 枠以外のオブジェクト
#define	LINEDATA	0x11
#define BOXSDATA	0x12
#define RBOXSDATA	0x13
#define	CRCLDATA	0x14
#define	NONDATA		0x00ff

#define	UNITPOINT	0
#define UNITKYUSU	1
#define UNITMM		2
#define UNITSUN		3

#define LINEUPUP		1
#define LINEUPUDCNTR	2
#define LINEUPDOWN		3
#define	LINEUPLEFT		4
#define LINEUPLRCNTR	5
#define LINEUPRIGHT		6
#define LINEUPXSIZEEQUL	7
#define LINEUPYSIZEEQUL	8
#define LINEUPXYSIZEEQUL	9
#define LINEUPXCNTR		10
#define LINEUPYCNTR		11
#define LINEUPXYCNTR	12
#define LINEUPXSPEQUL	13
#define LINEUPYSPEQUL	14

#define	BKVAL		0
#define	FGVAL		255

#define	NORAG		9999
#define	HALFPAI		90

#define M_PI		3.14159265358979323846
#define M_PI_2		1.57079632679489661923
#define M_PI_4		0.785398163397
#define M_1RAG		0.01745329251994
#define	M_2RAG		0.03490658503988
#define	M_5RAG		0.08726646259969
#define	MMPARINTI	25.4
#define	MMTOPOINT	2.8345
#define POINTTOMM	0.3528

#define FNAMETOP	30

#define	ES0FONT	10		//３バイトＥＳ書体
#define	ES1FONT	11		//５バイトＥＳ書体
#define ES2FONT	12
#define TTFONT	20
#define TT1FONT	21		//ＴｒｕｅＴｙｐｅ書体
#define TT2FONT	22		//追加ＴｒｕｅＴｙｐｅ書体
#define TT3FONT	23		//英数ＴｒｕｅＴｙｐｅ書体
#define TT4FONT	24		//シンボルＴｒｕｅＴｙｐｅ書体
#define	ARKFONT 31		//アークＦＴ１形式書体
#define	ARCFONT 32		//アークＦＴ２形式書体
#define	MAXFONT 33		//アークＭＡＸ形式書体
#define	MX2FONT 34		//ＭＸ２形式書体
#define	SCRFONT 41		//ＳＣＲ形式書体

#define	PRINTVIEW	0
#define	PREVIEW		1
#define	LINEVIEW	2
#define POINTVIEW	3
#define	BLACKVIEW	5
#define	DIRCTVIEW	6
#define	BITMAPVIEW	7
#define	METAFVIEW	8

#define STATBARHELP 0
#define STATBARSTAT 1
#define STATBARMODE 2
#define STATBARCMMD 3
#define STATBARCOOD 4

#define NEARLIMIT	32
#define HNEARLIMIT	16
#define QNEARLIMIT	 8
#define SNEARLIMIT	 4
#define LNEARLIMIT	 2

#define	ADDMM	10
#define NORIMM	2

// システム状態フラグ
#define EXECFLAG	0x0001
#define STOPFLAG	0x0002

#define	LINEKIND	1
#define	BEZEKIND	2
#define	ARCKIND		3
#define	FREEKIND	4
#define	FCBEZEKIND	5
#define	PLGNKIND	6

// 文字フラグ
#define	MOJIATRTATEGAKI	0x0001
#define MOJIDRAWWINDING	0x0004
// ベクターフラグ
#define		REJIONFLAG		0x0001
#define		VUSEFLAG		0x0002
#define		ACTIVEFLAG		0x0004
#define		VCUTFLAG		0x0008
#define		VSUBFLAG		0x0010
#define		VWAKUFLAG		0x0020
#define		LOCKFLAG		0x0040
#define		ALLLOCKFLAG		0x0080
#define		ATNTION1FLAG	0x0100
#define		ATNTION2FLAG	0x0200
#define		KATAGAKIFLAG	0x0400
#define		UNDERLINEFLAG	0x0800
#define		COPYFLAG		0x1000

#define	PATRMASK		0x0e
#define	BEZEATR		0x08
#define	CURVATR		0x04
#define	CONTATR		0x02

#define PATN0ATR	0x10
#define PATN1ATR	0x20
#define PATN2ATR	0x40
#define PATN3ATR	0x80

/* プログラム制御フラグ */
#define UNDOBIT		0x01	//ON UNDO処理中　OFF 通常処理　
#define CLIPBIT		0x02	//ON ｱｰｶｲﾌﾞﾌｧｲﾙ処理中　OFF 通常処理　
#define MJONEBIT	0x04	//ON １文字編集処理中　OFF 通常処理　

/* プロッター機種定義 */
#define RP395T 210
#define RP9T 211
#define RP6T 212
#define CH37 213
#define RP3T 213
#define RP3L 214
#define RP3LL 215
#define RP35F 216
#define RP93T 217
#define RP93N 218
#define RP1T 219
#define RP95T 220
#define TM13 221
#define RPDL 231
#define LIPS 241
#define MAXP 110
#define MAXC 111
#define GRPH 120
#define MIMK 130
#define RLND 140
#define HPGL 150
#define MEHP 151
#define MHP  152
#define CJHP 153
#define PNCHP 154
#define CMHP 155
#define PCHP 156
#define CM24HP 157
#define CJ2HP 158
#define DYNA 100
#define	NCAMADA	310
#define LASERPRO 320

#define	NOCOMMAND	0
#define	OUTPUTDATA	1
#define	ESCPROGRAM	8
#define	ENDPROGRAM	9

#define	DIZA		0
#define	GOSA1		1
#define	GOSA2		2

#define	IOP		0
#define	WRF		1

#define	CAMJET		0
#define	MIMAKI		1
#define GRAPHTEC	2
#define	CLRCAM		3
#define	HPDJC		5
#define	HPDJET		6
#define	HIFIJET		7
#define MASTERJET	8
#define	HP98JET		9
#define	HPBOSJET	10
#define	RJNX		11
#define	CJ500		12
#define	PM7000C		13
#define	HIFJET36LCLM	21
#define	HIFJET72LCLM	22
#define	HIFJET36OG		23
#define	HIFJET72OG		24
#define	HIFJET36		25
#define	HIFJET72		26
#define	KONICA			30
#define	RJNX192			41
#define	RJNX384			42
#define	RJNX768			43
#define	CJ50036LCLM		51
#define	CJ50054LCLM		52
#define	CJ50072LCLM		53
#define	CJ50036OG		54
#define	CJ50054OG		55
#define	CJ50072OG		56
#define	CJ50036			57
#define	CJ50054			58
#define	CJ50072			59
#define	PM7000C36LCLM	61
#define	PM7000C36		66

#define NOOPDATAID		0
#define BASEDATAID		1
#define	BASEVECTID		2
#define	OLDRULEVECTID	3
#define	MSURVECTID		4
#define	DIBCDATAID		5
#define MOJIVECTID		6
#define	WAKUVECTID		7
#define	STRGVECTID		8
#define	DATALISTID		9
#define	DATAGRUPID		10
#define	DATALOGOID		11
#define	DATAFDIBID		12
#define	SPTNVECTID		13
#define	RULEVECTID		14
#define	SWAKVECTID		15
#define MOJITEXTID		16
#define	STRGTEXTID		18
#define	GRADDATAID		20
#define CLIPDATAID		21
#define	SPTNTEXTID		23
#define	SWAKTEXTID		24
#define	NAMEGRUPID		25
#define	METADATAID		26
#define	OLEDATAID		51
#define SCRNAREAID		99

#define	MOVETO	0
#define LINETO	1
#define BEZETO	2
#define YBEZETO	3
#define VBEZETO	4
#define	CLRBRUSHSET	11
#define	CLRPENSET	12
#define THICKSET	13
#define FILLSET	21
#define FILLLINESET	22
#define	LINESET	23
#define VECTEND 32
#define	BLOCKSTART	33
#define	BLOCKEND	34
#define	ENDDATA 999

#define MODAL		0
#define	MODALESS	1

#define _AFX_FP_OFF(thing) (*((UINT*)&(thing)))
#define _AFX_FP_SEG(lp) (*((UINT*)&(lp)+1))
#define	NEAR0(len)	(-0.0000001 < len && len < 0.0000001)	

#define	GAIJIMESH	1024
#define	MOJISIZE	102.4	// MESHSIZE 1024

#define ALDUS_ID				(0x9AC6CDD7)
#define APMSIZE					(22)
typedef struct{		// WMF file use
	DWORD		key;
	WORD		hmf;
	SMALL_RECT	bbox;
	WORD		inch;
	DWORD		reserved;
	WORD		checksum;
} APMFILEHEADER;
typedef APMFILEHEADER * PAPMFILEHEADER;

typedef struct {
	double	a11, a12;
	double	a21, a22;
	double	a31, a32;
} dmtxtype;

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

typedef struct {	//s ＢＥＺＥ構造格納構造
	double	x1, y1;		//sd ＢＥＺＥ曲線の始点座標
	double	x2, y2;		//sd ＢＥＺＥ曲線の中間点座標
	double	x3, y3;		//sd ＢＥＺＥ曲線の中間点座標
	double	x4, y4;		//sd ＢＥＺＥ曲線の終点座標
} dbezetype;

typedef struct {	//s 円弧構造格納構造
	double	x1, y1;		//sd 円弧曲線の始点座標
	double	x2, y2;		//sd 円弧曲線の中間点座標
	double	x3, y3;		//sd 円弧曲線の終点
	double	x4, y4;
} darctype;

typedef struct {	//s 自由曲線格納構造
	int		atr0, atr1, atr2, atr3;	//sd 点属性
	double	x0, y0;		//sd 自由曲線の前の点の座標
	double	x1, y1;		//sd 自由曲線の始点座標
	double	x2, y2;		//sd 自由曲線の終点座標
	double	x3, y3;		//sd 自由曲線の次の点の座標
} dcurvetype;

typedef struct {	//s 曲線
	int		atr0, atr1, atr2, atr3, atr4;	//sd 点属性
	double	x0, y0;		//sd 自由曲線の前の点の座標
	double	x1, y1;		//sd 曲線の１点目の座標
	double	x2, y2;		//sd 曲線の２点目の座標
	double	x3, y3;		//sd 曲線の３点目の座標
	double	x4, y4;		//sd 曲線の４点目の座標
} dfigutype;

typedef struct {	//s  直線構造格納構造
	double	x1, y1;		//sd 直線の始点座標
	double	x2, y2;		//sd 直線の終点座標
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

//　家紋関係定義
#define REJIONMASK		0x01
#define	KAMONREJION		0x10
#define	KAMONDATAMAX	1000
#define	KAMONPOINTSIZE	4
#define	KAMONPOINTATR	0
#define	KAMONPOINTX		1
#define	KAMONPOINTY		2
#define	KAMONPOINTBIT	3

typedef struct {	//s 家紋ファイルインデックス構造
	short	len;		//sd データサイズ
	long	seekp;		//sd データ格納位置
} kamonidxtype;
#define KAMONIDXSIZE	6

#define	KAMONNAME	12		// 家紋簡略名称文字列サイズ
#define	KAMONFULLNAME	42	// 家紋正式名称文字列サイズ

typedef struct {	//s 家紋データHEAD部構造
	char	fullname[KAMONFULLNAME];	//sd 家紋正式名称文字列サイズ
	char	name[KAMONNAME];			//sd 家紋簡略名称文字列サイズ
} kamonheadtype;
#define KAMONHEADSIZE	54

#endif

