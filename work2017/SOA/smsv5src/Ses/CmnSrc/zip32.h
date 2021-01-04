typedef int (WINAPI DLLPRNT) (LPSTR, unsigned long);
typedef int (WINAPI DLLCOMMENT)(LPSTR);
typedef int (WINAPI DLLPASSWORD) (LPSTR, int, LPCSTR, LPCSTR);
typedef int (WINAPI DLLSERVICE) (LPCSTR, unsigned long);

typedef struct {        /* zip options */
	LPSTR Date;             /* Date to include after */
	LPSTR szRootDir;        /* Directory to use as base for zipping */
	LPSTR szTempDir;        /* Temporary directory used during zipping */
	BOOL fTemp;             /* Use temporary directory '-b' during zipping */
	BOOL fSuffix;           /* include suffixes (not implemented) */
	BOOL fEncrypt;          /* encrypt files */
	BOOL fSystem;           /* include system and hidden files */
	BOOL fVolume;           /* Include volume label */
	BOOL fExtra;            /* Exclude extra attributes */
	BOOL fNoDirEntries;     /* Do not add directory entries */
	BOOL fExcludeDate;      /* Exclude files newer than specified date */
	BOOL fIncludeDate;      /* Include only files newer than specified date */
	BOOL fVerbose;          /* Mention oddities in zip file structure */
	BOOL fQuiet;            /* Quiet operation */
	BOOL fCRLF_LF;          /* Translate CR/LF to LF */
	BOOL fLF_CRLF;          /* Translate LF to CR/LF */
	BOOL fJunkDir;          /* Junk directory names */
	BOOL fGrow;             /* Allow appending to a zip file */
	BOOL fForce;            /* Make entries using DOS names (k for Katz) */
	BOOL fMove;             /* Delete files added or updated in zip file */
	BOOL fDeleteEntries;    /* Delete files from zip file */
	BOOL fUpdate;           /* Update zip file--overwrite only if newer */
	BOOL fFreshen;          /* Freshen zip file--overwrite only */
	BOOL fJunkSFX;          /* Junk SFX prefix */
	BOOL fLatestTime;       /* Set zip file time to time of latest file in it */
	BOOL fComment;          /* Put comment in zip file */
	BOOL fOffsets;          /* Update archive offsets for SFX files */
	BOOL fPrivilege;        /* Use privileges (WIN32 only) */
	BOOL fEncryption;       /* TRUE if encryption supported, else FALSE.
							   this is a read only flag */
	int  fRecurse;          /* Recurse into subdirectories. 1 => -r, 2 => -R */
	int  fRepair;           /* Repair archive. 1 => -F, 2 => -FF */
	char fLevel;            /* Compression level (0 - 9) */
} ZPOPT, far *LPZPOPT;

typedef struct {
	int  argc;              /* Count of files to zip */
	LPSTR lpszZipFN;        /* name of archive to create/update */
	char **FNV;             /* array of file names to zip up */
} ZCL, far *LPZCL;

typedef struct {
	DLLPRNT *print;
	DLLCOMMENT *comment;
	DLLPASSWORD *password;
	DLLSERVICE *ServiceApplication;
} ZIPUSERFUNCTIONS, far * LPZIPUSERFUNCTIONS;

