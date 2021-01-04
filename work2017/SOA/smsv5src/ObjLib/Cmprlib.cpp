//
// Easy Scanner System Ver1.0
//
// ess  cmprlib.c  旧ロゴデータ圧縮復元関数
//
//  Copyright (c) 1998 by THEON

#include	"stdafx.h"
#include	"Cmprlib.h"

BYTE	*ibuf1, *ibuf2;		//ds イメージ保持バッファ
BYTE	*basebuf;			//ds １つ前のイメージ保持バッファ
BYTE	*cmpbuf;			//ds 処理対象イメージ保持バッファ
int		ibufp;				//ds 圧縮データ処理ポインタ
int		ibuflen;			//ds 圧縮データ長
int		ibufflg;			//ds ibuf1,ibuf2選択フラグ

DllExport void cmprlibrstorinit(int byte, BYTE* cmprbuf)
{
	ibuf1 = &(cmprbuf[4000]);
	ibuf2 = &(cmprbuf[6000]);
	ibuflen = byte;
	ibufflg = 0;
	memset(ibuf1, 0, 1024);
}

DllExport void cmprlibrstor1(BYTE *buf, int l1flg)
{
	int	l1, l1mask;
	int	l2, l2flg, l2mask;
	int	l3, l3flg, l3mask;
	int	i;

	cmprlibsetbuffirst();
	l1mask = 0x80;
	for(l1 = 0; l1 < 8; l1++){
	    if(l1flg & l1mask){
			l2flg = *buf++;
			l2mask = 0x80;
			for(l2 = 0; l2 < 8; l2++){
			    if(l2flg & l2mask){
					l3flg = *buf++;
					l3mask = 0x80;
					for(l3 = 0; l3 < 8; l3++){
					    if(l3flg & l3mask){
							if(cmprlibsetibuf((int)(*buf))){
								return;
							}
							buf++;
					    }
					    else{
							if(cmprlibsetibuf(0)){
								return;
							}
					    }
				    l3mask >>= 1;
					}
			    }
			    else{
					for(i = 0; i < 8; i++){
					    if(cmprlibsetibuf(0)){
							return;
					    }
					}
			    }
			    l2mask >>= 1;
			}
		}
		else{
			for(i = 0; i < 64; i++){
			    if(cmprlibsetibuf(0)){
					return;
			    }
			}
	    }
	    l1mask >>= 1;
	}
}

DllExport void cmprlibrstor2(BYTE linebuf[])
{
	int	i;

	switch(ibufflg){
	case 0:
		basebuf = ibuf1;
		cmpbuf = ibuf2;
		ibufflg = 1;
		break;
	case 1:
		basebuf = ibuf2;
		cmpbuf = ibuf1;
		ibufflg = 0;
		break;
	}
	for(i = 0; i < ibuflen; i++){
		cmpbuf[i] = basebuf[i] ^ cmpbuf[i];
		linebuf[i] = ~(cmpbuf[i]);
	}
}

void cmprlibsetbuffirst()
{
	ibufp = 0;
	switch(ibufflg){
	case 0:
		cmpbuf = ibuf2;
		break;
	case 1:
		cmpbuf = ibuf1;
		break;
	}
}

int cmprlibsetibuf(int ch)
{
	int ret;
	cmpbuf[ibufp] = ch;
	ibufp++;
	if(ibufp >= ibuflen){
		ret = ON;
	}
	else {
		ret = OFF;
	}
	return(ret);
}

DllExport void cmprlibsinit(int byte, BYTE *sp)
{
	ibuf1 = &(sp[4000]);
	ibuf2 = &(sp[6000]);
	ibuflen = byte;
	ibufflg = 0;
	memset(ibuf1, 0, 2048);
}

DllExport void cmprlibs1(BYTE buf[])
{
	int	i;

	switch(ibufflg){
	case 0:
		basebuf = ibuf1;
		cmpbuf = ibuf2;
		break;
	case 1:
		basebuf = ibuf2;
		cmpbuf = ibuf1;
		break;
	}
	for(i = 0; i < ibuflen; i++){
		cmpbuf[i] = ~buf[i];
		basebuf[i] = basebuf[i] ^ cmpbuf[i];
	}
}

DllExport int cmprlibs2(BYTE buf[])
{
	int	l1flg;
	int	l1, l1mask;
	int	l2, l2flg, l2mask;
	int	l3, l3flg, l3mask, l3cnt;
	UCHAR	tmpbuf[10];
	int	i, j;

	l1 = 0;
	l1flg = 0;
	l1mask = 0x80;
	l2 = 1;
	l2flg = 0;
	l2mask = 0x80;
	l3 = 2;
	l3flg = 0;
	l3mask = 0x80;
	l3cnt = 0;
	for(i = 0; i < ibuflen; i++){
		if(basebuf[i] != 0){
			l3flg |= l3mask;
			tmpbuf[l3cnt] = basebuf[i];
			l3cnt++;
		}
		l3mask  >>= 1;
		if(l3mask == 0){
			if(l3flg != 0){
				l2flg |= l2mask;
				buf[l3] = l3flg;
				l3++;
				for(j = 0; j < l3cnt; j++){
					buf[l3] = tmpbuf[j];
					l3++;
				}
			}
			l3flg = 0;
			l3mask = 0x80;
			l3cnt = 0;

			l2mask >>= 1;
			if(l2mask == 0){
				if(l2flg != 0){
					l1flg |= l1mask;
					buf[l2] = l2flg;
					l2 = l3;
					l3++;
				}
				l2flg = 0;
				l2mask = 0x80;
				l1mask >>= 1;
			}
		}
	}
	if(l3flg != 0){
		l2flg |= l2mask;
		buf[l3] = l3flg;
		l3++;
		for(j = 0; j < l3cnt; j++){
			buf[l3] = tmpbuf[j];
			l3++;
		}
	}
	if(l2flg != 0){
		l1flg |= l1mask;
		buf[l2] = l2flg;
	}
	buf[l1] = l1flg;
	if(ibufflg == 0){
		ibufflg = 1;
	}
	else{
		ibufflg = 0;
	}
	if(l1flg == 0){
		return(1);
	}
	else{
		return(l3);
	}
}

