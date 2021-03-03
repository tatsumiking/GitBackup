/*
	KGCOMMON.H
*/
#ifndef KGCOMMON_H
#define KGCOMMON_H

//definition
#define DATAMAX		11
#define FIRSTMAX	28

//command
#define FIRSTSTAGE	1

//ｴﾗｰｺｰﾄﾞ
#define J_OK		0
#define J_ERROR		1

#define FOPEN_ER	12
#define ON		1
#define OFF		0
#define ADRS_ERROR	0xFFFE;

#define	 ER_JSPRESET	  110	//βの初期化エラー
#define	 ER_JSPFIRST	  111	//暗号のエラー
#define	 ER_NOVXDACCESS	  117	//VXD ハンドルの取得に失敗（VXDファイルがない）
#define	 ER_KEYGET	  118
#define	 ER_DLL32LOAD	  119

//次のエラーはマルチタスクＯＳにおいて、プリンタ資源がプリンタ
//スプーラ等において独占されて、設定待ち時間を超えた場合です。
#define	 ER_TIME_OUT	  117	//プリンタ資源の確保タイムアウト

typedef unsigned int   uint ;
typedef unsigned char  uchar;
typedef unsigned short ushort;

typedef struct	_KUSERPACKET{
	ushort adrs;
	ushort data;
}KUSERPACKET;

typedef struct _SETKUSERPACKET{
	struct	_KUSERPACKET*	pin;
	struct	_KUSERPACKET*	pout;
	short	incnt;
	short	outcnt;
	short	key;
	//	次の2byteを入れることにより構造体のアライメントが
	//	1byteでも8byteでも16byte size の構造体になる
	short	dummy;
}SETKUSERPACKET;

#if	 defined(WIN95)
#pragma warning (disable : 4244 4245 4761)
#endif

//....................................................................
#if	  defined(CPLUS)
	extern "C"
#endif
short __cdecl
KJSPCHK(ushort command, struct _SETKUSERPACKET* pset,
		ushort wait_time);

//....................................................................
#if	  defined(CPLUS)
	  extern "C"
#endif
short __cdecl
_KJSPCHK(ushort command, struct _SETKUSERPACKET* pset,
		 ushort wait_time);

//....................................................................
#if	  defined(CPLUS)
	  extern "C"
#endif
short __cdecl
KJSPCHK1(ushort command, struct _SETKUSERPACKET* pset,
			   ushort wait_time);

//....................................................................
#if	  defined(CPLUS)
	  extern "C"
#endif
short __cdecl
_KJSPCHK1(ushort command, struct _SETKUSERPACKET* pset,
			   ushort wait_time);


#endif

//------------------------< E.O.F >---------------------------------------


