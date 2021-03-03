# Microsoft Developer Studio Project File - Name="sms" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=sms - Win32 Release
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "sms.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "sms.mak" CFG="sms - Win32 Release"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "sms - Win32 Release" ("Win32 (x86) Application" 用)
!MESSAGE "sms - Win32 Debug" ("Win32 (x86) Application" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "sms - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "CPLUS" /D "SOA" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x411 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /map /machine:I386
# Begin Custom Build
InputPath=.\Release\sms.exe
SOURCE="$(InputPath)"

BuildCmds= \
	copy ..\object\Release\object.dll Release \
	copy ..\objlib\Release\objlib.dll Release \
	

"Release\Object.dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"Release\Objlib.dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "sms - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W4 /Gm /GX /ZI /Od /D "_AFXDLL" /D "_MBCS" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "CPLUS" /D "SOA" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x411 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# Begin Custom Build
InputPath=.\Debug\sms.exe
SOURCE="$(InputPath)"

BuildCmds= \
	copy ..\object\debug\object.dll debug \
	copy ..\objlib\debug\objlib.dll debug \
	

"debug\object.dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"debug\objlib.dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# Begin Target

# Name "sms - Win32 Release"
# Name "sms - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ChildFrm.cpp
# End Source File
# Begin Source File

SOURCE=..\CMNSRC\Cmd_base.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgChkOut.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgGSlct.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgImgDsp.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgImgSize.cpp
# End Source File
# Begin Source File

SOURCE=..\CMNSRC\Dlginnum.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgInput.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgLstDsp.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgMain.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgNumArea.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgOutput.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgWkSlct.cpp
# End Source File
# Begin Source File

SOURCE=..\SES\Fileeps.cpp
# End Source File
# Begin Source File

SOURCE=..\CMNSRC\Imehndl.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\RdsOutCrct.cpp
# End Source File
# Begin Source File

SOURCE=.\sms.cpp
# End Source File
# Begin Source File

SOURCE=.\sms.rc
# End Source File
# Begin Source File

SOURCE=.\smsDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\smsView.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\Sysenv.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ChildFrm.h
# End Source File
# Begin Source File

SOURCE=..\CMNSRC\Cmd_base.h
# End Source File
# Begin Source File

SOURCE=.\DlgChkOut.h
# End Source File
# Begin Source File

SOURCE=.\DlgGSlct.h
# End Source File
# Begin Source File

SOURCE=.\DlgImgDsp.h
# End Source File
# Begin Source File

SOURCE=.\DlgImgSize.h
# End Source File
# Begin Source File

SOURCE=..\CMNSRC\Dlginnum.h
# End Source File
# Begin Source File

SOURCE=.\DlgInput.h
# End Source File
# Begin Source File

SOURCE=.\DlgLstDsp.h
# End Source File
# Begin Source File

SOURCE=.\DlgMain.h
# End Source File
# Begin Source File

SOURCE=.\DlgNumArea.h
# End Source File
# Begin Source File

SOURCE=.\DlgOutput.h
# End Source File
# Begin Source File

SOURCE=.\DlgWkSlct.h
# End Source File
# Begin Source File

SOURCE=..\SES\Fileeps.h
# End Source File
# Begin Source File

SOURCE=..\CMNSRC\Imehndl.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\RdsOutCrct.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\sms.h
# End Source File
# Begin Source File

SOURCE=.\smsDoc.h
# End Source File
# Begin Source File

SOURCE=.\smsView.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\Sysenv.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\ses.ico
# End Source File
# Begin Source File

SOURCE=.\res\sms.ico
# End Source File
# Begin Source File

SOURCE=.\res\sms.rc2
# End Source File
# Begin Source File

SOURCE=.\res\smsDoc.ico
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
