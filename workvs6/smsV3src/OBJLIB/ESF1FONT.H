
#define		VARSIZE		16
#define		FREJIONMASK		0x10
#define		FEORMASK		0x01

typedef struct {
	char	version[VARSIZE];
	long	idxsseekp;
	long	idxeseekp;
	long	datsseekp;
	long	dateseekp;
} e1fhtype;

typedef struct {
	long	dseekp;
	long	dlen;
} e1idxtype;

typedef struct {
	short	headlen;
	short	datalen;
	char	sx, sy;
	char	ex, ey;
} e1dhtype;

#define ESF1ISEEKP	0x00000020L
#define ESF1DSEEKP	0x000138A0L
#define ESF1PRTCT	0x94E0L	/* (47*100 + 60) * 8 + 32 */

#define	MINIMAXTIME		8
#define	ESF1MESH		1024
#define	ESF1ATRADD		0
#define	ESF1XCDADD		1
#define	ESF1YCDADD		3
#define	ESF1SIZADD		5

#define		ESF1REJIONMASK		0x10
#define		ESF1EORMASK		0x01

