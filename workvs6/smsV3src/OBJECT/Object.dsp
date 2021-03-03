# Microsoft Developer Studio Project File - Name="Object" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Object - Win32 Release
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "Object.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "Object.mak" CFG="Object - Win32 Release"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "Object - Win32 Release" ("Win32 (x86) Dynamic-Link Library" 用)
!MESSAGE "Object - Win32 Debug" ("Win32 (x86) Dynamic-Link Library" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Object - Win32 Release"

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
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "CPLUS" /D "_WINDLL" /D "_AFXDLL" /D "SOA" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x411 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x411 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 /nologo /subsystem:windows /dll /map /machine:I386

!ELSEIF  "$(CFG)" == "Object - Win32 Debug"

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
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "_USRDLL" /D "CPLUS" /D "_WINDLL" /D "_AFXDLL" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "SOA" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x411 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x411 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /dll /map /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "Object - Win32 Release"
# Name "Object - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ARCfont.cpp
# End Source File
# Begin Source File

SOURCE=.\ARKfont.cpp
# End Source File
# Begin Source File

SOURCE=.\Boldlib.cpp
# End Source File
# Begin Source File

SOURCE=.\clipdata.cpp
# End Source File
# Begin Source File

SOURCE=.\data.cpp
# End Source File
# Begin Source File

SOURCE=.\datafdib.CPP
# End Source File
# Begin Source File

SOURCE=.\datagrup.cpp
# End Source File
# Begin Source File

SOURCE=.\datalist.cpp
# End Source File
# Begin Source File

SOURCE=.\datalogo.CPP
# End Source File
# Begin Source File

SOURCE=.\dibcdata.cpp
# End Source File
# Begin Source File

SOURCE=.\Esf0font.cpp
# End Source File
# Begin Source File

SOURCE=.\Esf1font.cpp
# End Source File
# Begin Source File

SOURCE=.\Fsf1font.cpp
# End Source File
# Begin Source File

SOURCE=.\GradData.cpp
# End Source File
# Begin Source File

SOURCE=.\metadata.cpp
# End Source File
# Begin Source File

SOURCE=.\MojiText.cpp
# End Source File
# Begin Source File

SOURCE=.\mojivect.CPP
# End Source File
# Begin Source File

SOURCE=.\msurvect.cpp
# End Source File
# Begin Source File

SOURCE=.\Mx2font.cpp
# End Source File
# Begin Source File

SOURCE=.\NameGrup.cpp
# End Source File
# Begin Source File

SOURCE=.\Object.cpp
# End Source File
# Begin Source File

SOURCE=.\Object.def
# End Source File
# Begin Source File

SOURCE=.\Object.rc
# End Source File
# Begin Source File

SOURCE=.\oldrvect.cpp
# End Source File
# Begin Source File

SOURCE=.\Pblock.cpp
# End Source File
# Begin Source File

SOURCE=.\Plist.cpp
# End Source File
# Begin Source File

SOURCE=.\rulevect.CPP
# End Source File
# Begin Source File

SOURCE=.\scrfont.cpp
# End Source File
# Begin Source File

SOURCE=.\sptntext.cpp
# End Source File
# Begin Source File

SOURCE=.\sptnvect.CPP
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\strgtext.cpp
# End Source File
# Begin Source File

SOURCE=.\Strgvect.cpp
# End Source File
# Begin Source File

SOURCE=.\Swaktext.cpp
# End Source File
# Begin Source File

SOURCE=.\Swakvect.cpp
# End Source File
# Begin Source File

SOURCE=.\Vect.cpp
# End Source File
# Begin Source File

SOURCE=.\Wakuvect.cpp
# End Source File
# Begin Source File

SOURCE=.\Wnttfont.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Arcfont.h
# End Source File
# Begin Source File

SOURCE=.\ARKfont.h
# End Source File
# Begin Source File

SOURCE=.\Boldlib.h
# End Source File
# Begin Source File

SOURCE=.\clipdata.h
# End Source File
# Begin Source File

SOURCE=.\data.h
# End Source File
# Begin Source File

SOURCE=.\datafdib.H
# End Source File
# Begin Source File

SOURCE=.\datagrup.h
# End Source File
# Begin Source File

SOURCE=.\datalist.h
# End Source File
# Begin Source File

SOURCE=.\datalogo.H
# End Source File
# Begin Source File

SOURCE=.\Dibcdata.h
# End Source File
# Begin Source File

SOURCE=.\Esf0font.h
# End Source File
# Begin Source File

SOURCE=.\Esf1font.h
# End Source File
# Begin Source File

SOURCE=.\Fsf1font.h
# End Source File
# Begin Source File

SOURCE=.\GradData.h
# End Source File
# Begin Source File

SOURCE=.\metadata.h
# End Source File
# Begin Source File

SOURCE=.\MojiText.h
# End Source File
# Begin Source File

SOURCE=.\mojivect.h
# End Source File
# Begin Source File

SOURCE=.\msurvect.H
# End Source File
# Begin Source File

SOURCE=.\Mx2font.h
# End Source File
# Begin Source File

SOURCE=.\NameGrup.h
# End Source File
# Begin Source File

SOURCE=.\Object.h
# End Source File
# Begin Source File

SOURCE=.\oldrvect.h
# End Source File
# Begin Source File

SOURCE=.\Pblock.h
# End Source File
# Begin Source File

SOURCE=.\Plist.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\rulevect.H
# End Source File
# Begin Source File

SOURCE=.\scrfont.h
# End Source File
# Begin Source File

SOURCE=.\sptntext.h
# End Source File
# Begin Source File

SOURCE=.\sptnvect.H
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\strgtext.h
# End Source File
# Begin Source File

SOURCE=.\Strgvect.h
# End Source File
# Begin Source File

SOURCE=.\Swaktext.h
# End Source File
# Begin Source File

SOURCE=.\Swakvect.h
# End Source File
# Begin Source File

SOURCE=.\Vect.h
# End Source File
# Begin Source File

SOURCE=.\Wakuvect.h
# End Source File
# Begin Source File

SOURCE=.\Wnttfont.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\Object.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
