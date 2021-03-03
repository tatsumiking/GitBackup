# Microsoft Developer Studio Project File - Name="objlib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=objlib - Win32 Release
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "Objlib.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "Objlib.mak" CFG="objlib - Win32 Release"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "objlib - Win32 Release" ("Win32 (x86) Dynamic-Link Library" 用)
!MESSAGE "objlib - Win32 Debug" ("Win32 (x86) Dynamic-Link Library" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "objlib - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "SOA" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL"
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL"
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /map /machine:I386

!ELSEIF  "$(CFG)" == "objlib - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "SOA" /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o /win32 "NUL"
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o /win32 "NUL"
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /map /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "objlib - Win32 Release"
# Name "objlib - Win32 Debug"
# Begin Source File

SOURCE=.\Baselib.c
# End Source File
# Begin Source File

SOURCE=.\cmprlib.C
# End Source File
# Begin Source File

SOURCE=.\Crosslib.c
# End Source File
# Begin Source File

SOURCE=.\Curvelib.c
# End Source File
# Begin Source File

SOURCE=.\Curvelib.h
# End Source File
# Begin Source File

SOURCE=.\diblib.c
# End Source File
# Begin Source File

SOURCE=.\disp.c
# End Source File
# Begin Source File

SOURCE=.\Diza.c
# End Source File
# Begin Source File

SOURCE=.\dlldef.h
# End Source File
# Begin Source File

SOURCE=.\Dltlib.c
# End Source File
# Begin Source File

SOURCE=.\Epsflib.c
# End Source File
# Begin Source File

SOURCE=.\Fdxflib.c
# End Source File
# Begin Source File

SOURCE=.\figuenv.c
# End Source File
# Begin Source File

SOURCE=.\Kamonlib.c
# End Source File
# Begin Source File

SOURCE=.\Mathlib.c
# End Source File
# Begin Source File

SOURCE=.\Mdfylib.c
# End Source File
# Begin Source File

SOURCE=.\Mdfylib.h
# End Source File
# Begin Source File

SOURCE=.\Mojienv.c
# End Source File
# Begin Source File

SOURCE=.\outlib.c
# End Source File
# Begin Source File

SOURCE=.\Outlnlib.c
# End Source File
# Begin Source File

SOURCE=.\Outlnlib.h
# End Source File
# Begin Source File

SOURCE=.\Pdrwlib.c
# End Source File
# Begin Source File

SOURCE=.\Plgnfill.c
# End Source File
# Begin Source File

SOURCE=.\Plot.c
# End Source File
# Begin Source File

SOURCE=.\Ploter.h
# End Source File
# Begin Source File

SOURCE=.\Pointatr.h
# End Source File
# Begin Source File

SOURCE=.\Pout.c
# End Source File
# Begin Source File

SOURCE=.\Protect.c
# End Source File
# Begin Source File

SOURCE=.\Ptnenv.c
# End Source File
# Begin Source File

SOURCE=.\rulelib.c
# End Source File
# Begin Source File

SOURCE=.\Scrnlib.c
# End Source File
# Begin Source File

SOURCE=.\Senvlib.c
# End Source File
# Begin Source File

SOURCE=.\Strcttrn.c
# End Source File
# Begin Source File

SOURCE=.\Wakuenv.c
# End Source File
# Begin Source File

SOURCE=.\Wincall.c
# End Source File
# Begin Source File

SOURCE=.\Wkftbl.c
# End Source File
# End Target
# End Project
