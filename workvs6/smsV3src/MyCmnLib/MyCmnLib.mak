# Microsoft Developer Studio Generated NMAKE File, Based on MyCmnLib.dsp
!IF "$(CFG)" == ""
CFG=MyCmnLib - Win32 Debug
!MESSAGE 構成が指定されていません。ﾃﾞﾌｫﾙﾄの MyCmnLib - Win32 Debug を設定します。
!ENDIF 

!IF "$(CFG)" != "MyCmnLib - Win32 Release" && "$(CFG)" != "MyCmnLib - Win32 Debug"
!MESSAGE 指定された ﾋﾞﾙﾄﾞ ﾓｰﾄﾞ "$(CFG)" は正しくありません。
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "MyCmnLib.mak" CFG="MyCmnLib - Win32 Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "MyCmnLib - Win32 Release" ("Win32 (x86) Dynamic-Link Library" 用)
!MESSAGE "MyCmnLib - Win32 Debug" ("Win32 (x86) Dynamic-Link Library" 用)
!MESSAGE 
!ERROR 無効な構成が指定されています。
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "MyCmnLib - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\MyCmnLib.dll"


CLEAN :
	-@erase "$(INTDIR)\Funclib.obj"
	-@erase "$(INTDIR)\Imehndl.obj"
	-@erase "$(INTDIR)\MyCmbBox.obj"
	-@erase "$(INTDIR)\MyCmnLib.obj"
	-@erase "$(INTDIR)\MyCmnLib.pch"
	-@erase "$(INTDIR)\MyCmnLib.res"
	-@erase "$(INTDIR)\Mydialog.obj"
	-@erase "$(INTDIR)\Myedit.obj"
	-@erase "$(INTDIR)\MyFileDl.obj"
	-@erase "$(INTDIR)\MyLstBox.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\Sysenv.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\MyCmnLib.dll"
	-@erase "$(OUTDIR)\MyCmnLib.exp"
	-@erase "$(OUTDIR)\MyCmnLib.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "SKJI" /Fp"$(INTDIR)\MyCmnLib.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x411 /fo"$(INTDIR)\MyCmnLib.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\MyCmnLib.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\MyCmnLib.pdb" /machine:I386 /def:".\MyCmnLib.def" /out:"$(OUTDIR)\MyCmnLib.dll" /implib:"$(OUTDIR)\MyCmnLib.lib" 
DEF_FILE= \
	".\MyCmnLib.def"
LINK32_OBJS= \
	"$(INTDIR)\Funclib.obj" \
	"$(INTDIR)\Imehndl.obj" \
	"$(INTDIR)\MyCmbBox.obj" \
	"$(INTDIR)\MyCmnLib.obj" \
	"$(INTDIR)\Mydialog.obj" \
	"$(INTDIR)\Myedit.obj" \
	"$(INTDIR)\MyFileDl.obj" \
	"$(INTDIR)\MyLstBox.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\Sysenv.obj" \
	"$(INTDIR)\MyCmnLib.res"

"$(OUTDIR)\MyCmnLib.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "MyCmnLib - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\MyCmnLib.dll"


CLEAN :
	-@erase "$(INTDIR)\Funclib.obj"
	-@erase "$(INTDIR)\Imehndl.obj"
	-@erase "$(INTDIR)\MyCmbBox.obj"
	-@erase "$(INTDIR)\MyCmnLib.obj"
	-@erase "$(INTDIR)\MyCmnLib.pch"
	-@erase "$(INTDIR)\MyCmnLib.res"
	-@erase "$(INTDIR)\Mydialog.obj"
	-@erase "$(INTDIR)\Myedit.obj"
	-@erase "$(INTDIR)\MyFileDl.obj"
	-@erase "$(INTDIR)\MyLstBox.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\Sysenv.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\MyCmnLib.dll"
	-@erase "$(OUTDIR)\MyCmnLib.exp"
	-@erase "$(OUTDIR)\MyCmnLib.ilk"
	-@erase "$(OUTDIR)\MyCmnLib.lib"
	-@erase "$(OUTDIR)\MyCmnLib.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "_SKJI" /Fp"$(INTDIR)\MyCmnLib.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /F0x2 /GZ /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x411 /fo"$(INTDIR)\MyCmnLib.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\MyCmnLib.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\MyCmnLib.pdb" /debug /machine:I386 /def:".\MyCmnLib.def" /out:"$(OUTDIR)\MyCmnLib.dll" /implib:"$(OUTDIR)\MyCmnLib.lib" /pdbtype:sept 
DEF_FILE= \
	".\MyCmnLib.def"
LINK32_OBJS= \
	"$(INTDIR)\Funclib.obj" \
	"$(INTDIR)\Imehndl.obj" \
	"$(INTDIR)\MyCmbBox.obj" \
	"$(INTDIR)\MyCmnLib.obj" \
	"$(INTDIR)\Mydialog.obj" \
	"$(INTDIR)\Myedit.obj" \
	"$(INTDIR)\MyFileDl.obj" \
	"$(INTDIR)\MyLstBox.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\Sysenv.obj" \
	"$(INTDIR)\MyCmnLib.res"

"$(OUTDIR)\MyCmnLib.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("MyCmnLib.dep")
!INCLUDE "MyCmnLib.dep"
!ELSE 
!MESSAGE Warning: cannot find "MyCmnLib.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "MyCmnLib - Win32 Release" || "$(CFG)" == "MyCmnLib - Win32 Debug"
SOURCE=.\Funclib.cpp

"$(INTDIR)\Funclib.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MyCmnLib.pch"


SOURCE=.\Imehndl.cpp

"$(INTDIR)\Imehndl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MyCmnLib.pch"


SOURCE=.\MyCmbBox.cpp

"$(INTDIR)\MyCmbBox.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MyCmnLib.pch"


SOURCE=.\MyCmnLib.cpp

"$(INTDIR)\MyCmnLib.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MyCmnLib.pch"


SOURCE=.\MyCmnLib.rc

"$(INTDIR)\MyCmnLib.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\Mydialog.cpp

"$(INTDIR)\Mydialog.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MyCmnLib.pch"


SOURCE=.\Myedit.cpp

"$(INTDIR)\Myedit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MyCmnLib.pch"


SOURCE=.\MyFileDl.cpp

"$(INTDIR)\MyFileDl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MyCmnLib.pch"


SOURCE=.\MyLstBox.cpp

"$(INTDIR)\MyLstBox.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MyCmnLib.pch"


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "MyCmnLib - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "SKJI" /Fp"$(INTDIR)\MyCmnLib.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\MyCmnLib.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "MyCmnLib - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "_SKJI" /Fp"$(INTDIR)\MyCmnLib.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /F0x2 /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\MyCmnLib.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Sysenv.cpp

"$(INTDIR)\Sysenv.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\MyCmnLib.pch"



!ENDIF 

