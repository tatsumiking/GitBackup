# Microsoft Developer Studio Project File - Name="sgs" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=sgs - Win32 Debug
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "sgs.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "sgs.mak" CFG="sgs - Win32 Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "sgs - Win32 Release" ("Win32 (x86) Application" 用)
!MESSAGE "sgs - Win32 Debug" ("Win32 (x86) Application" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "sgs - Win32 Release"

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
InputPath=.\Release\sgs.exe
SOURCE="$(InputPath)"

BuildCmds= \
	copy ..\crosslib\Release\crosslib.dll Release \
	copy ..\object\Release\object.dll Release \
	copy ..\objlib\Release\objlib.dll Release \
	

"Release\crosslib.dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"Release\object.dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"Release\objlib.dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "sgs - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "SOA" /Yu"stdafx.h" /FD /c
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
InputPath=.\Debug\sgs.exe
SOURCE="$(InputPath)"

BuildCmds= \
	copy ..\crosslib\debug\crosslib.dll debug \
	copy ..\object\debug\object.dll debug \
	copy ..\objlib\debug\objlib.dll debug \
	

"debug\crosslib.dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"debug\object.dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"debug\objlib.dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# Begin Target

# Name "sgs - Win32 Release"
# Name "sgs - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\cmnsrc\bmpfile.cpp
# End Source File
# Begin Source File

SOURCE=.\CFigure.cpp
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

SOURCE=..\ses\Cmd_edit.cpp
# End Source File
# Begin Source File

SOURCE=..\ses\Cmd_figu.cpp
# End Source File
# Begin Source File

SOURCE=..\ses\Cmd_file.cpp
# End Source File
# Begin Source File

SOURCE=.\Cmd_line.cpp
# End Source File
# Begin Source File

SOURCE=.\Cmd_moji.cpp
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Cmd_msur.cpp
# End Source File
# Begin Source File

SOURCE=.\Cmd_part.cpp
# End Source File
# Begin Source File

SOURCE=.\Cmd_pint.cpp
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Cmd_rule.cpp
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Cmd_slct.cpp
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

SOURCE=..\ses\Cmd_trns.cpp
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\cmd_waku.cpp
# End Source File
# Begin Source File

SOURCE=..\ses\Cmd_zoom.cpp
# End Source File
# Begin Source File

SOURCE=..\CMNSRC\CMDBSBMP.CPP
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Cmdcrdnt.cpp
# End Source File
# Begin Source File

SOURCE=..\Ses\Cmdkamon.cpp
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\cmdkcrct.cpp
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Cmdplout.cpp
# End Source File
# Begin Source File

SOURCE=.\CmdSLine.cpp
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

SOURCE=..\SES\DaoDBLib.cpp
# End Source File
# Begin Source File

SOURCE=..\SES\DDBEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\DGjLoad.cpp
# End Source File
# Begin Source File

SOURCE=.\DGjSave.cpp
# End Source File
# Begin Source File

SOURCE=..\CMNSRC\DGjSlct.cpp
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Diblib.cpp
# End Source File
# Begin Source File

SOURCE=..\SES\DImgSave.cpp
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\DlCrctLd.cpp
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Dldvpout.cpp
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\dlekmnld.cpp
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\dlekmnsv.cpp
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

SOURCE=.\Dlgmjld.cpp
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Dlgmjmov.cpp
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Dlgmjsiz.cpp
# End Source File
# Begin Source File

SOURCE=.\Dlgmjsv.cpp
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

SOURCE=.\DPrtLoad.cpp
# End Source File
# Begin Source File

SOURCE=.\DPrtSave.cpp
# End Source File
# Begin Source File

SOURCE=..\CMNSRC\dslctstrg.cpp
# End Source File
# Begin Source File

SOURCE=..\CMNSRC\DWKBARG1.CPP
# End Source File
# Begin Source File

SOURCE=..\CMNSRC\DWKBARG2.CPP
# End Source File
# Begin Source File

SOURCE=..\CMNSRC\dwkinskj.cpp
# End Source File
# Begin Source File

SOURCE=..\CMNSRC\DWkPpDel.cpp
# End Source File
# Begin Source File

SOURCE=..\SES\Excel.cpp
# End Source File
# Begin Source File

SOURCE=..\SES\ExcelLib.cpp
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\filearc.cpp
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\filedxf.cpp
# End Source File
# Begin Source File

SOURCE=..\Ses\Fileeps.cpp
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

SOURCE=.\sgs.cpp
# End Source File
# Begin Source File

SOURCE=.\sgs.rc
# End Source File
# Begin Source File

SOURCE=.\sgsDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\sgsView.cpp
# End Source File
# Begin Source File

SOURCE=.\SrvrItem.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Strgtool.cpp
# End Source File
# Begin Source File

SOURCE=.\Sysenv.cpp
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\viewfdlg.cpp
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Wakutool.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\cmnsrc\bmpfile.h
# End Source File
# Begin Source File

SOURCE=.\CFigure.h
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

SOURCE=..\ses\Cmd_edit.h
# End Source File
# Begin Source File

SOURCE=..\ses\Cmd_figu.h
# End Source File
# Begin Source File

SOURCE=..\ses\Cmd_file.h
# End Source File
# Begin Source File

SOURCE=.\Cmd_line.h
# End Source File
# Begin Source File

SOURCE=.\Cmd_moji.h
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Cmd_msur.h
# End Source File
# Begin Source File

SOURCE=.\Cmd_part.h
# End Source File
# Begin Source File

SOURCE=.\Cmd_pint.h
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Cmd_rule.h
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Cmd_slct.h
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

SOURCE=..\ses\Cmd_trns.h
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\cmd_waku.h
# End Source File
# Begin Source File

SOURCE=..\ses\Cmd_zoom.h
# End Source File
# Begin Source File

SOURCE=..\CMNSRC\CMDBSBMP.H
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Cmdcrdnt.h
# End Source File
# Begin Source File

SOURCE=..\Ses\Cmdkamon.h
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\cmdkcrct.h
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Cmdplout.h
# End Source File
# Begin Source File

SOURCE=.\CmdSLine.h
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

SOURCE=..\SES\DaoDBLib.h
# End Source File
# Begin Source File

SOURCE=..\SES\DDBEdit.h
# End Source File
# Begin Source File

SOURCE=.\DGjLoad.h
# End Source File
# Begin Source File

SOURCE=.\DGjSave.h
# End Source File
# Begin Source File

SOURCE=..\CMNSRC\DGjSlct.h
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Diblib.h
# End Source File
# Begin Source File

SOURCE=..\SES\DImgSave.h
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\DlCrctLd.h
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Dldvpout.h
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\dlekmnld.h
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\dlekmnsv.h
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

SOURCE=..\cmnsrc\dlggaiji.h
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

SOURCE=.\Dlgmjld.h
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Dlgmjmov.h
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Dlgmjsiz.h
# End Source File
# Begin Source File

SOURCE=.\Dlgmjsv.h
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

SOURCE=..\cmnsrc\dlgwkedt.h
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

SOURCE=.\DPrtLoad.h
# End Source File
# Begin Source File

SOURCE=.\DPrtSave.h
# End Source File
# Begin Source File

SOURCE=..\CMNSRC\dslctstrg.h
# End Source File
# Begin Source File

SOURCE=..\CMNSRC\DWKBARG1.H
# End Source File
# Begin Source File

SOURCE=..\CMNSRC\DWKBARG2.H
# End Source File
# Begin Source File

SOURCE=..\CMNSRC\dwkinskj.h
# End Source File
# Begin Source File

SOURCE=..\CMNSRC\DWkPpDel.h
# End Source File
# Begin Source File

SOURCE=..\SES\Excel.h
# End Source File
# Begin Source File

SOURCE=..\SES\ExcelLib.h
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\filearc.h
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\filedxf.h
# End Source File
# Begin Source File

SOURCE=..\Ses\Fileeps.h
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

SOURCE=.\sgs.h
# End Source File
# Begin Source File

SOURCE=.\sgsDoc.h
# End Source File
# Begin Source File

SOURCE=.\sgsView.h
# End Source File
# Begin Source File

SOURCE=.\SrvrItem.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Strgtool.h
# End Source File
# Begin Source File

SOURCE=.\Sysenv.h
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\viewfdlg.h
# End Source File
# Begin Source File

SOURCE=..\cmnsrc\Wakutool.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\bitmap10.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\bitmap11.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\bitmap12.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\bitmap13.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\bitmap14.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap4.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap5.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap6.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap7.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\bitmap8.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\bitmap9.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\BMP00000.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00002.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00003.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00004.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00005.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00006.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00007.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00008.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00009.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00010.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00011.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00012.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00013.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00014.bmp
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

SOURCE=.\res\figutool.bmp
# End Source File
# Begin Source File

SOURCE=.\res\grabtn.bmp
# End Source File
# Begin Source File

SOURCE=.\res\graptn1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\graptn2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\graptn3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\graptn4.bmp
# End Source File
# Begin Source File

SOURCE=.\res\graptn5.bmp
# End Source File
# Begin Source File

SOURCE=.\res\IToolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\leftside.bmp
# End Source File
# Begin Source File

SOURCE=.\res\movedown.bmp
# End Source File
# Begin Source File

SOURCE=.\res\moveleft.bmp
# End Source File
# Begin Source File

SOURCE=.\res\moverigh.bmp
# End Source File
# Begin Source File

SOURCE=.\res\moveup2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\moveup3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\nextbtn.bmp
# End Source File
# Begin Source File

SOURCE=.\res\nextbtn1.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\oleftdow.bmp
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

SOURCE=.\res\plgntool.bmp
# End Source File
# Begin Source File

SOURCE=.\res\prebtn.bmp
# End Source File
# Begin Source File

SOURCE=.\res\prebtn1.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\ses.ico
# End Source File
# Begin Source File

SOURCE=.\res\sgs.ico
# End Source File
# Begin Source File

SOURCE=.\res\sgs.rc2
# End Source File
# Begin Source File

SOURCE=.\res\sgsDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\sizebig1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\sizebig3.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\sizerlbi.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\sizerlsm.bmp
# End Source File
# Begin Source File

SOURCE=.\res\sizesmal.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\sizeudbi.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\sizeudsm.bmp
# End Source File
# Begin Source File

SOURCE=.\res\strgtool.bmp
# End Source File
# Begin Source File

SOURCE=.\res\strgvstr.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\wakutool.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\sgs.reg
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
