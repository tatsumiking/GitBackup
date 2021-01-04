DllExport int outlibOpen();
DllExport void outlibPutString(char *buf);
DllExport void outlibPutData(char *buf, int len);
BOOL RsSetDefaultTimeOut(HANDLE hPortID);
DllExport void outlibClose();
void outlibHndleClose();
BOOL MyBuildCommDCB2( DCB *dcb, DCB *yourdcb );
