/*
	Easy Scanner ÉVÉXÉeÉÄã§í É\Å[ÉX	1992.12.01	by stan
base		mojienv.c	ÉXÉNÉäÅ[Éìä÷åWä÷êî
	Copyright äîÅjÉnÉäÉ}
*/

#include	<windows.h>
#include	<stdio.h>
#include	<math.h>
#include	"..\stdef.h"
#include	"dlldef.h"

#include	"mojienv.fnc"
#include	"senvlib.fnc"
#include	"scrnlib.fnc"

char	szinputstring[RECMAX];
char	szfontpath[FNAMEMAX];
char	szfontname[64];
int		esseisuno, arceisuno;
int		ftategaki;
DBL		dmmfwidth, dmmfhight;
DBL		dmmmojispace, dmmlinespace;
DBL		dmmstringlen;
DBL		dmmstartx, dmmstarty;
int		nspacekind;
int		nyokomojiflag;
int		nstartkind;
int		nconerkind;
DBL		dmojicount;
int		fontmax;
FILE	*m_fpFList;
DBL		dtrnslimit;

int		ngyakukind;	
char	szsmpletext[QRECMAX];
int		narngkind;
int		nstrgorder;
DBL		dstrgslop;
DBL		dstrground;
int		fbold;
int		fitric;
int		funderl;
int		fmaxlegal;

DllExport void mojienvinit()
{
	fbold = OFF;
	fitric = OFF;
	funderl = OFF;
	fmaxlegal = 0;
	nspacekind = 1;
	nyokomojiflag = 0;
}

DllExport void mojienvend()
{
}

DllExport void mojienvsetmaxlegal(int flag)
{
	fmaxlegal = flag;
}

DllExport int mojienvcheckmaxlegal()
{
	return(fmaxlegal);
}

DllExport void mojienvsetbold(int flag)
{
	fbold = flag;
}

DllExport int mojienvcheckbold()
{
	return(fbold);
}

DllExport void mojienvsetitric(int flag)
{
	fitric = flag;
}

DllExport int mojienvcheckitric()
{
	return(fitric);
}

DllExport void mojienvsetunderl(int flag)
{
	funderl = flag;
}

DllExport int mojienvcheckunderl()
{
	return(funderl);
}

DllExport void mojienvsettrnslimit(DBL dlimit)
{
	dtrnslimit = dlimit;
}

DllExport void mojienvgettrnslimit(LPDBL dlimit)
{
	*dlimit = dtrnslimit;
}

DllExport void mojienvsetstring(LPSTR instr)
{
	strcpy(szinputstring, instr);
}

DllExport void mojienvgetstring(LPSTR instr)
{
	strcpy(instr, szinputstring);
}

DllExport void mojienvgetfontpath(LPSTR fontpath)
{
	strcpy(fontpath, szfontpath);
}

DllExport void mojienvsetfontpath(LPSTR fontpath)
{
	strcpy(szfontpath, fontpath);
}

DllExport void mojienvgetfontname(LPSTR fontname)
{
	strcpy(fontname, szfontname);
}

DllExport void mojienvsetfontname(LPSTR fontname)
{
	strcpy(szfontname, fontname);
}

DllExport void mojienvgetesseisuno(LPINT no)
{
	*no = esseisuno;
}

DllExport void mojienvsetesseisuno(int no)
{
	esseisuno = no;
}

DllExport void mojienvgetarceisuno(LPINT no)
{
	*no = arceisuno;
}

DllExport void mojienvsetarceisuno(int no)
{
	arceisuno = no;
}

DllExport void mojienvgettategakiflag(LPINT tateflg)
{
	*tateflg = ftategaki;
}

DllExport void mojienvsettategakiflag(int tateflg)
{
	ftategaki = tateflg;
}

DllExport void mojienvgetspacekind(LPINT spkflg)
{
	*spkflg = nspacekind;
}

DllExport void mojienvsetspacekind(int spkflg)
{
	nspacekind = spkflg;
}

DllExport void mojienvgetyokomojiflag(LPINT yokomoji)
{
	*yokomoji = nyokomojiflag;
}

DllExport void mojienvsetyokomojiflag(int yokomoji)
{
	nyokomojiflag = yokomoji;
}

DllExport void mojienvgetstart(LPDBL dstartx, LPDBL dstarty)
{
	*dstartx = dmmstartx;
	*dstarty = dmmstarty;
}

DllExport void mojienvsetstart(DBL dstartx, DBL dstarty)
{
	dmmstartx = dstartx;
	dmmstarty = dstarty;
}

DllExport void mojienvgetsize(LPDBL dfwidth, LPDBL dfhight)
{
	*dfwidth = dmmfwidth;
	*dfhight = dmmfhight;
}

DllExport void mojienvsetsize(DBL dfwidth, DBL dfhight)
{
	dmmfwidth = dfwidth;
	dmmfhight = dfhight;
}

DllExport void mojienvgetspace(LPDBL dmojispace, LPDBL dlinespace)
{
	*dmojispace = dmmmojispace;
	*dlinespace = dmmlinespace;
}

DllExport void mojienvsetspace(DBL dmojispace, DBL dlinespace)
{
	dmmmojispace = dmojispace;
	dmmlinespace = dlinespace;
}

DllExport void mojienvgetstrlen(LPDBL dstringlen)
{
	*dstringlen = dmmstringlen;
}

DllExport void mojienvsetstrlen(DBL dstringlen)
{
	dmmstringlen = dstringlen;
}

DllExport void mojienvsetstartkind(int nkind)
{
	nstartkind = nkind;
}

DllExport void mojienvgetstartkind(LPINT nkind)
{
	*nkind = nstartkind;
}

DllExport void mojienvsetconerkind(int nkind)
{
	nconerkind = nkind;
}

DllExport void mojienvgetconerkind(LPINT nkind)
{
	*nkind = nconerkind;
}

DllExport void mojienvsetmojicount(DBL dmjcnt)
{
	dmojicount = dmjcnt;
}

DllExport void mojienvgetmojicount(LPDBL dmjcnt)
{
	*dmjcnt= dmojicount;
}

DllExport void mojienvsetgyakukind(int nkind)
{
	ngyakukind = nkind; 

}

DllExport void mojienvgetgyakukind(LPINT nkind)
{
	*nkind = ngyakukind; 
}

DllExport void mojienvsetsmpltext(LPSTR smpl)
{
	strcpy(szsmpletext, smpl); 

}

DllExport void mojienvgetsmpltext(LPSTR smpl)
{
	strcpy(smpl, szsmpletext); 
}

DllExport void mojienvsetarngkind(int nkind)
{
	narngkind = nkind; 

}

DllExport void mojienvgetarngkind(LPINT nkind)
{
	*nkind = narngkind; 
}

DllExport void mojienvsetorder(int nkind)
{
	nstrgorder = nkind; 
}

DllExport void mojienvgetorder(LPINT nkind)
{
	*nkind = nstrgorder; 
}

DllExport void mojienvsetstrgslop(DBL dslop)
{
	dstrgslop = dslop; 

}

DllExport void mojienvgetstrgslop(LPDBL dslop)
{
	*dslop = dstrgslop; 
}

DllExport void mojienvsetmojiround(DBL dround)
{
	dstrground = dround; 
}

DllExport void mojienvgetmojiround(LPDBL dround)
{
	*dround = dstrground; 
}

DllExport void mojienvflistinit()
{
	char	exepath[FNAMEMAX];
	char	flstfname[FNAMEMAX];
	FILE	*fp;

	fontmax = 0;
	senvgetexepath(exepath);
	sprintf(flstfname, "%s\\fontlist.lst", exepath);
	fp = fopen(flstfname, "wb");
	if(fp != NULL){
		fclose(fp);
	}
	m_fpFList = NULL;
}

DllExport void mojienvflistopen(LPSTR lpMode)
{
	char	exepath[FNAMEMAX];
	char	flstfname[FNAMEMAX];

	senvgetexepath(exepath);
	sprintf(flstfname, "%s\\fontlist.lst", exepath);
	m_fpFList = fopen(flstfname, lpMode);
}

DllExport void mojienvflistclose()
{
	fclose(m_fpFList);
}

DllExport int mojienvgetflistmax()
{
	return(fontmax);
}

DllExport void mojienvsetflist(int idx, int type, LPSTR sfname, LPSTR ffname)
{
	char	buf[HRECMAX+16];
	long	seekp;

	if(m_fpFList == NULL){
		return;
	}
	if(idx != fontmax){
		seekp = idx*HRECMAX;
		fseek(m_fpFList, seekp, 0);
		fontmax++;
	}
	memset(buf, '\0', HRECMAX);
	sprintf(buf, "%d", type);
	strcpy(&(buf[3]), sfname);
	strcpy(&(buf[64]), ffname);
	buf[HRECMAX-2] = 0x0d;
	buf[HRECMAX-1] = 0x0a;
	fwrite(buf, 1, HRECMAX, m_fpFList);
	return;
}

DllExport void mojienvgetflist(int idx, int *type, LPSTR sfname, LPSTR ffname)
{
	char	buf[HRECMAX+16];
	long	seekp;
	int		len;

	if(m_fpFList == NULL){
		return;
	}
	sfname[0] = '\0';
	ffname[0] = '\0';
	if(idx >= fontmax){
		return;
	}
	seekp = idx*HRECMAX;
	fseek(m_fpFList, seekp, 0);
	len = fread(buf, 1, HRECMAX, m_fpFList);
	if(len != HRECMAX){
		return;
	}
	sscanf(buf, "%d", type);
	strcpy(sfname, &(buf[3]));
	strcpy(ffname, &(buf[64]));
	return;
}

DllExport void mojienvgetfonttypename(LPSTR fontname, LPINT retftype, LPSTR fontfullname)
{
	char	buf[HRECMAX+16];
	int		len;
	int		num;
	int		type;
	char	sfname[HRECMAX];
	char	ffname[HRECMAX];

	if(strncmp(fontname, "ÇdÇrâpêî", 8) == 0){
		num = atoi(&(fontname[8]));
		*retftype = 10;
		sprintf(fontfullname, "Çdâpêî%02d", num);
		return;
	}
	if(strncmp(fontname, "Ç`âpêî", 6) == 0){
		num = atoi(&(fontname[6]));
		*retftype = 10;
		sprintf(fontfullname, "Ç`âp%04d", num);
		return;
	}
	if(strncmp(fontname, "Ç`Çqâp", 6) == 0){
		num = atoi(&(fontname[6]));
		*retftype = 32;
		sprintf(fontfullname, "âpêî%03d", num);
		return;
	}
	if(strncmp(fontname, "ÉXÉ|Å[Éc", 8) == 0){
		num = atoi(&(fontname[8]));
		*retftype = 32;
		sprintf(fontfullname, "SPOT%03d", num);
		return;
	}
	
	if(m_fpFList == NULL){
		*retftype = 21;
		strcpy(fontfullname, "ÇlÇr ñæí©");
		return;
	}
	fseek(m_fpFList, 0L, 0);
	while(1){
		len = fread(buf, 1, HRECMAX, m_fpFList);
		if(len != HRECMAX){
			fontfullname[0] = '\0';
			*retftype = 0;
			break;
		}
		sscanf(buf, "%d", &type);
		strcpy(sfname, &(buf[3]));
		strcpy(ffname, &(buf[64]));
		if(strcmp(fontname, sfname) == 0){
			*retftype = type;
			strcpy(fontfullname, ffname);
			break;
		}
	}
}

DllExport int mojienvgetfontno(LPSTR fontname)
{
	char	buf[HRECMAX+16];
	int		len;
	int		type;
	char	sfname[HRECMAX];
	char	ffname[HRECMAX];
	int		i;

	if(m_fpFList == NULL){
		return(0);
	}
	fseek(m_fpFList, 0L, 0);
	i = 0;
	while(1){
		len = fread(buf, 1, HRECMAX, m_fpFList);
		if(len != HRECMAX){
			break;
		}
		sscanf(buf, "%d", &type);
		strcpy(sfname, &(buf[3]));
		strcpy(ffname, &(buf[64]));
		if(strcmp(fontname, sfname) == 0){
			break;
		}
		i++;
	}
	return(i);
}


DllExport int mojienvgetfonttype(LPSTR fullname)
{
	char	buf[HRECMAX+16];
	int		len;
	int		type;
	char	sfname[HRECMAX];
	char	ffname[HRECMAX];

	if(m_fpFList == NULL){
		return(0);
	}
	fseek(m_fpFList, 0L, 0);
	while(1){
		len = fread(buf, 1, HRECMAX, m_fpFList);
		if(len != HRECMAX){
			break;
		}
		sscanf(buf, "%d", &type);
		strcpy(sfname, &(buf[3]));
		strcpy(ffname, &(buf[64]));
		if(strcmp(fullname, sfname) == 0){
			return(type);
		}
		if(strcmp(fullname, ffname) == 0){
			return(type);
		}
	}
	return(TT2FONT);
}

DllExport void mojienvgetfilename(LPSTR fontname, LPSTR filename)
{
	char	buf[HRECMAX+16];
	int		len;
	int		type;
	char	sfname[HRECMAX];
	char	ffname[HRECMAX];

	filename[0] = '\0';
	if(m_fpFList == NULL){
		return;
	}
	fseek(m_fpFList, 0L, 0);
	while(1){
		len = fread(buf, 1, HRECMAX, m_fpFList);
		if(len != HRECMAX){
			break;
		}
		sscanf(buf, "%d", &type);
		strcpy(sfname, &(buf[3]));
		strcpy(ffname, &(buf[64]));
		if(strcmp(fontname, sfname) == 0){
			strcpy(filename, ffname);
			break;
		}
	}
}

DllExport void mojienvgetsfilename(LPSTR fontname, LPSTR filename)
{
	char	buf[HRECMAX+16];
	int		len;
	int		type;
	char	sfname[HRECMAX];
	char	ffname[HRECMAX];

	filename[0] = '\0';
	if(m_fpFList == NULL){
		return;
	}
	fseek(m_fpFList, 0L, 0);
	while(1){
		len = fread(buf, 1, HRECMAX, m_fpFList);
		if(len != HRECMAX){
			break;
		}
		sscanf(buf, "%d", &type);
		strcpy(sfname, &(buf[3]));
		strcpy(ffname, &(buf[64]));
		if(strcmp(fontname, ffname) == 0){
			strcpy(filename, sfname);
			break;
		}
	}
}

