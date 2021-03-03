# Microsoft Developer Studio Project File - Name="ses" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=ses - Win32 Debug
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "ses.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "ses.mak" CFG="ses - Win32 Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "ses - Win32 Release" ("Win32 (x86) Application" 用)
!MESSAGE "ses - Win32 Debug" ("Win32 (x86) Application" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ses - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "CPLUS" /D "SOA" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x411 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x411 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /map /machine:I386
# Begin Custom Build
InputPath=.\Release\ses.exe
SOURCE="$(InputPath)"

BuildCmds= \
	copy ..\Object\Release\Object.dll Release\Object.dll \
	copy ..\Object\Release\Object.map Release\Object.map \
	copy ..\Objlib\Release\Objlib.dll Release\Objlib.dll \
	copy ..\Objlib\Release\Objlib.map Release\Objlib.map \
	

"Release\Object.dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"Release\Object.map" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"Release\Objlib.dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"Release\Objlib.map" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "ses - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "CPLUS" /D "SOA" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x411 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x411 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# Begin Custom Build
InputPath=.\Debug\ses.exe
SOURCE="$(InputPath)"

BuildCmds= \
	copy ..\object\Debug\object.dll Debug \
	copy ..\object\Debug\object.map Debug \
	copy ..\objlib\Debug\objlib.dll Debug \
	copy ..\objlib\Debug\objlib.map Debug \
	

"Debug\object.dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"Debug\object.map" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"Debug\objlib.dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"Debug\objlib.map" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# Begin Target

# Name "ses - Win32 Release"
# Name "ses - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\cmnsrc\bmpfile.cpp
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.cpp
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Cmd_atr.cpp
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Cmd_base.cpp
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Cmd_clr.cpp
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Cmd_dibc.cpp
# End Source File
# Begin Source File

SOURCE=.\Cmd_edit.cpp
# End Source File
# Begin Source File

SOURCE=.\Cmd_figu.cpp
# End Source File
# Begin Source File

SOURCE=.\Cmd_file.cpp
# End Source File
# Begin Source File

SOURCE=..\CMNSRC\Cmd_ftp.cpp
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Cmd_line.cpp
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Cmd_moji.cpp
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Cmd_msur.cpp
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Cmd_pint.cpp
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Cmd_rule.cpp
# End Source File
# Begin Source File

SOURCE=..\CMNSRC\Cmd_send.cpp
# End Source File
# Begin Source File

SOURCE=..\Cmnsrc\Cmd_slct.cpp
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Cmd_spcl.cpp
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Cmd_strg.cpp
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Cmd_sub.cpp
# End Source File
# Begin Source File

SOURCE=..\CMNSRC\Cmd_swak.cpp
# End Source File
# Begin Source File

SOURCE=.\Cmd_trns.cpp
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\cmd_waku.cpp
# End Source File
# Begin Source File

SOURCE=.\Cmd_zoom.cpp
# End Source File
# Begin Source File

SOURCE=..\CMNSRC\CMDBSBMP.CPP
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Cmdcrdnt.cpp
# End Source File
# Begin Source File

SOURCE=.\Cmddbedt.cpp
# End Source File
# Begin Source File

SOURCE=..\CMNSRC\CmdEftp.cpp
# End Source File
# Begin Source File

SOURCE=.\cmdkamon.cpp
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\cmdkcrct.cpp
# End Source File
# Begin Source File

SOURCE=..\CMNSRC\CmdMente.cpp
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Cmdplout.cpp
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Cmdsprop.cpp
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Cmdsstrg.cpp
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Cmdsystm.cpp
# End Source File
# Begin Source File

SOURCE=.\CntrItem.cpp
# End Source File
# Begin Source File

SOURCE=.\Command.cpp
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\CrossLib.cpp
# End Source File
# Begin Source File

SOURCE=.\DaoDBLib.cpp
# End Source File
# Begin Source File

SOURCE=.\Dclrbar.cpp
# End Source File
# Begin Source File

SOURCE=..\CMNSRC\DCptrFolder.cpp
# End Source File
# Begin Source File

SOURCE=.\DDBEdit.cpp
# End Source File
# Begin Source File

SOURCE=..\CMNSRC\DGjSlct.cpp
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Diblib.cpp
# End Source File
# Begin Source File

SOURCE=.\DImgSave.cpp
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\DlCrctLd.cpp
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Dldvpout.cpp
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\DlEKmnLd.cpp
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\DlEKmnSv.cpp
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\dlepskld.cpp
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Dlgbmprd.cpp
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Dlgcnrkd.cpp
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Dlgdirst.cpp
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Dlgedbar.cpp
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\DlgESKmn.cpp
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Dlgfchng.cpp
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\DlgGaiji.cpp
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Dlggrdtn.cpp
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Dlginnum.cpp
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Dlgkmnld.cpp
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\DlgMjFit.cpp
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Dlgmjld.cpp
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Dlgmjmov.cpp
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Dlgmjsiz.cpp
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Dlgmjsv.cpp
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Dlgmsatr.cpp
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Dlgoutln.cpp
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Dlgplout.cpp
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Dlgsboxs.cpp
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Dlgscrcl.cpp
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Dlgscrnc.cpp
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Dlgscrne.cpp
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Dlgshdw.cpp
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Dlgslist.cpp
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Dlgstlup.cpp
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Dlgstrin.cpp
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Dlgstrvs.cpp
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Dlgtrimg.cpp
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Dlgtwpnt.cpp
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Dlgtxtin.cpp
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Dlgwkatr.cpp
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Dlgwkdel.cpp
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\dlgwkedt.cpp
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Dlgwkgrp.cpp
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Dlgwkinp.cpp
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Dlgwkld.cpp
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Dlgwkppr.cpp
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Dlgwksrt.cpp
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Dlgwksv.cpp
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Dlinpair.cpp
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\dlrlstyl.cpp
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\dlstrins.cpp
# End Source File
# Begin Source File

SOURCE=..\CMNSRC\DlWkAChg.cpp
# End Source File
# Begin Source File

SOURCE=.\DMDBSlct.cpp
# End Source File
# Begin Source File

SOURCE=.\DMmryEnvSet.cpp
# End Source File
# Begin Source File

SOURCE=..\CMNSRC\DSetSp.cpp
# End Source File
# Begin Source File

SOURCE=..\CMNSRC\DSlctFtpAdrs.cpp
# End Source File
# Begin Source File

SOURCE=..\CMNSRC\DSlctMAdrs.cpp
# End Source File
# Begin Source File

SOURCE=..\CMNSRC\dslctstrg.cpp
# End Source File
# Begin Source File

SOURCE=..\CMNSRC\DWKBAR1.CPP
# End Source File
# Begin Source File

SOURCE=..\CMNSRC\DWKBAR2.CPP
# End Source File
# Begin Source File

SOURCE=..\CMNSRC\DWKBAR3.CPP
# End Source File
# Begin Source File

SOURCE=..\CMNSRC\DWKBARSD.CPP
# End Source File
# Begin Source File

SOURCE=..\CMNSRC\Dwkcptr.cpp
# End Source File
# Begin Source File

SOURCE=..\CMNSRC\dwkinskj.cpp
# ADD CPP /I "..\SES"
# End Source File
# Begin Source File

SOURCE=..\CMNSRC\DWKPPAD.CPP
# End Source File
# Begin Source File

SOURCE=..\CMNSRC\DWkPpDel.cpp
# End Source File
# Begin Source File

SOURCE=.\DXlsSlct.cpp
# End Source File
# Begin Source File

SOURCE=.\Excel.cpp
# End Source File
# Begin Source File

SOURCE=.\ExcelLib.cpp
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\filearc.cpp
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\filedxf.cpp
# End Source File
# Begin Source File

SOURCE=.\Fileeps.cpp
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Filevect.cpp
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Fontcbox.cpp
# End Source File
# Begin Source File

SOURCE=.\IpFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\Mente.cpp
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\minmax.cpp
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\MyButton.cpp
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Mydialog.cpp
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Mydlgbar.cpp
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Myedit.cpp
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\MyFileDl.cpp
# End Source File
# Begin Source File

SOURCE=.\ses.cpp
# End Source File
# Begin Source File

SOURCE=.\ses.rc
# End Source File
# Begin Source File

SOURCE=.\sesDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\sesView.cpp
# End Source File
# Begin Source File

SOURCE=.\SrvrItem.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\Sysenv.cpp
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\viewfdlg.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\cmnsrc\bmpfile.h
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.h
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Cmd_atr.h
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Cmd_base.h
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Cmd_clr.h
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Cmd_dibc.h
# End Source File
# Begin Source File

SOURCE=.\Cmd_edit.h
# End Source File
# Begin Source File

SOURCE=.\Cmd_figu.h
# End Source File
# Begin Source File

SOURCE=.\Cmd_file.h
# End Source File
# Begin Source File

SOURCE=..\CMNSRC\Cmd_ftp.h
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Cmd_line.h
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Cmd_moji.h
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Cmd_msur.h
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Cmd_pint.h
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Cmd_rule.h
# End Source File
# Begin Source File

SOURCE=..\CMNSRC\Cmd_send.h
# End Source File
# Begin Source File

SOURCE=..\Cmnsrc\Cmd_slct.h
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Cmd_spcl.h
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Cmd_strg.h
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Cmd_sub.h
# End Source File
# Begin Source File

SOURCE=..\CMNSRC\Cmd_swak.h
# End Source File
# Begin Source File

SOURCE=.\Cmd_trns.h
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\cmd_waku.h
# End Source File
# Begin Source File

SOURCE=.\Cmd_zoom.h
# End Source File
# Begin Source File

SOURCE=..\CMNSRC\CMDBSBMP.H
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Cmdcrdnt.h
# End Source File
# Begin Source File

SOURCE=.\Cmddbedt.h
# End Source File
# Begin Source File

SOURCE=..\CMNSRC\CmdEftp.h
# End Source File
# Begin Source File

SOURCE=.\cmdkamon.h
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\cmdkcrct.h
# End Source File
# Begin Source File

SOURCE=..\CMNSRC\CmdMente.h
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Cmdplout.h
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Cmdsprop.h
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Cmdsstrg.h
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Cmdsystm.h
# End Source File
# Begin Source File

SOURCE=.\CntrItem.h
# End Source File
# Begin Source File

SOURCE=.\Command.h
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\CrossLib.h
# End Source File
# Begin Source File

SOURCE=.\DaoDBLib.h
# End Source File
# Begin Source File

SOURCE=.\Dclrbar.h
# End Source File
# Begin Source File

SOURCE=..\CMNSRC\DCptrFolder.h
# End Source File
# Begin Source File

SOURCE=.\DDBEdit.h
# End Source File
# Begin Source File

SOURCE=..\CMNSRC\DGjSlct.h
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Diblib.h
# End Source File
# Begin Source File

SOURCE=.\DImgSave.h
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Dldvpout.h
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\DlEKmnLd.h
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\DlEKmnSv.h
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\dlepskld.h
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Dlgbmprd.h
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Dlgcnrkd.h
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Dlgdirsl.h
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Dlgedbar.h
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\DlgESKmn.h
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\dlgfchng.h
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\DlgGaiji.h
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Dlggrdtn.h
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Dlginnum.h
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Dlgkmnld.h
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\DlgMjFit.h
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Dlgmjld.h
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Dlgmjmov.h
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Dlgmjsiz.h
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Dlgmjsv.h
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Dlgmsatr.h
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Dlgoutln.h
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Dlgplout.h
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Dlgsboxs.h
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Dlgscrcl.h
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Dlgscrnc.h
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Dlgscrne.h
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Dlgshdw.h
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Dlgslist.h
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Dlgstlup.h
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Dlgstrin.h
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Dlgstrvs.h
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Dlgtrimg.h
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Dlgtwpnt.h
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Dlgtxtin.h
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Dlgwkatr.h
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Dlgwkdel.h
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\DlgWkEdt.h
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Dlgwkgrp.h
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Dlgwkinp.h
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Dlgwkld.h
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Dlgwkppr.h
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Dlgwksrt.h
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Dlgwksv.h
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Dlinpair.h
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\dlrlstyl.h
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\dlstrins.h
# End Source File
# Begin Source File

SOURCE=..\CMNSRC\DlWkAChg.h
# End Source File
# Begin Source File

SOURCE=.\DMDBSlct.h
# End Source File
# Begin Source File

SOURCE=.\DMmryEnvSet.h
# End Source File
# Begin Source File

SOURCE=..\CMNSRC\DSetSp.h
# End Source File
# Begin Source File

SOURCE=..\CMNSRC\DSlctFtpAdrs.h
# End Source File
# Begin Source File

SOURCE=..\CMNSRC\DSlctMAdrs.h
# End Source File
# Begin Source File

SOURCE=..\CMNSRC\dslctstrg.h
# End Source File
# Begin Source File

SOURCE=..\CMNSRC\DWKBAR1.H
# End Source File
# Begin Source File

SOURCE=..\CMNSRC\DWKBAR2.H
# End Source File
# Begin Source File

SOURCE=..\CMNSRC\DWKBAR3.H
# End Source File
# Begin Source File

SOURCE=..\CMNSRC\DWKBARSD.H
# End Source File
# Begin Source File

SOURCE=..\CMNSRC\Dwkcptr.h
# End Source File
# Begin Source File

SOURCE=..\CMNSRC\dwkinskj.h
# End Source File
# Begin Source File

SOURCE=..\CMNSRC\DWKPPAD.H
# End Source File
# Begin Source File

SOURCE=..\CMNSRC\DWkPpDel.h
# End Source File
# Begin Source File

SOURCE=.\DXlsSlct.h
# End Source File
# Begin Source File

SOURCE=.\Excel.h
# End Source File
# Begin Source File

SOURCE=.\ExcelLib.h
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\filearc.h
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\filedxf.h
# End Source File
# Begin Source File

SOURCE=.\Fileeps.h
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Filevect.h
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Fontcbox.h
# End Source File
# Begin Source File

SOURCE=.\IpFrame.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\Mente.h
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\minmax.h
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\MyButton.h
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Mydialog.h
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Mydlgbar.h
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Myedit.h
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\MyFileDl.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\ses.h
# End Source File
# Begin Source File

SOURCE=.\sesDoc.h
# End Source File
# Begin Source File

SOURCE=.\sesView.h
# End Source File
# Begin Source File

SOURCE=.\SrvrItem.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\Sysenv.h
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\viewfdlg.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\Res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\bitmap10.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\bitmap11.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\bitmap2.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\bitmap3.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\bitmap4.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\bitmap5.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\bitmap6.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Bitmap7.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\bitmap8.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\bitmap9.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Bmp00002.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Bmp00003.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Bmp00004.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Bmp00005.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Bmp00006.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Bmp00007.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Bmp00008.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Bmp00009.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Bmp00010.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\bmp00011.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\bmp00012.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\bmp00013.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\bmp00014.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\bmp00015.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\bmp00016.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\bmp00017.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\bmp00018.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\bmp00019.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\bmp00020.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\bmp00021.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\bmp00022.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\bmp00023.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\ccntrcnt.bmp
# End Source File
# Begin Source File

SOURCE=.\res\cdrntool.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\cleftdow.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\cleftup.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\conerct.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\conerct1.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\conerlb.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\conerlb1.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\conerlt.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\conerlt1.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\conerrb.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\conerrb1.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\conerrt.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\conerrt1.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\crightdo.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\crightup.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\cur_zoom.cur
# End Source File
# Begin Source File

SOURCE=.\Res\cursor2.cur
# End Source File
# Begin Source File

SOURCE=.\Res\cursor3.cur
# End Source File
# Begin Source File

SOURCE=.\Res\cus_hand.cur
# End Source File
# Begin Source File

SOURCE=.\Res\Figutool.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Grabtn.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Graptn1.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Graptn2.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Graptn3.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Graptn4.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Graptn5.bmp
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\IToolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\leftside.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Linetool.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Movedown.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\movedwn1.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\movedwn2.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\movedwn3.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Moveleft.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\movelft1.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\movelft2.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\movelft3.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Moverigh.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\movetgh1.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\movetgh2.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\movetgh3.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\moveup1.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Moveup2.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Moveup3.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Nextbtn.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Nextbtn1.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\nonclrbtn.bmp
# End Source File
# Begin Source File

SOURCE=.\res\objtoolb.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\oleftdow.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\oleftup.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\oleftup1.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\orightdo.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\orightup.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Plgntool.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Pointtoo.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Prebtn.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\prebtn1.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\scntrcnt.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ses.ico
# End Source File
# Begin Source File

SOURCE=.\res\ses.rc2
# End Source File
# Begin Source File

SOURCE=.\res\sesDoc.ico
# End Source File
# Begin Source File

SOURCE=.\Res\Sizebig1.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\sizebig2.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Sizebig3.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\sizerlb1.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\sizerlb2.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\sizerlb3.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\sizerlbi.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\sizerls1.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\sizerls2.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\sizerls3.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\sizerlsm.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Sizesmal.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\sizesml1.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\sizesml2.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\sizesml3.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\sizeudb1.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\sizeudb2.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\sizeudb3.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\sizeudbi.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\sizeuds1.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\sizeuds2.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\sizeuds3.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\sizeudsm.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\sleftdow.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\sleftup.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\sleftup1.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\srightdo.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\srightup.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\startct.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\startlb.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\startlt.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\startrb.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\startrt.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\strgtate.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Strgtool.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\strgtyno.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\strgvs1.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\strgvs2.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\strgvs3.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\strgyoko.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar1.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Wakutool.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\ses.reg
# End Source File
# Begin Source File

SOURCE=..\imgkit\IMGCOM3.LIB
# End Source File
# Begin Source File

SOURCE=..\imgkit\IMGCUT3.LIB
# End Source File
# Begin Source File

SOURCE=..\imgkit\IMGDSP3.LIB
# End Source File
# Begin Source File

SOURCE=..\imgkit\IMGETC3.LIB
# End Source File
# Begin Source File

SOURCE=..\imgkit\IMGFILE3.LIB
# End Source File
# Begin Source File

SOURCE=..\imgkit\IMGSCAN3.LIB
# End Source File
# Begin Source File

SOURCE=..\imgkit\IMGTIFF3.LIB
# End Source File
# End Target
# End Project
