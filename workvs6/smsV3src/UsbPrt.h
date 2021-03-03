#ifndef __USBPRT_H__
#define __USBPRT_H__

#ifdef __cplusplus
    extern "C" {
#endif
long WINAPI Check(long nId);
#ifdef __cplusplus
    }
#endif

typedef long (__stdcall * LPUSBPRTCHK)(long);

#endif	//__USBPRT_H__	
