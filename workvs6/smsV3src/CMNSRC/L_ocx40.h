#ifndef _L_OCX40_H
#define _L_OCX40_H
// LEAD コントロール定数定義ファイル
#define L_KEY_EXPRESS        ""
#define L_KEY_GIFLZW         ""
#define L_KEY_TIFLZW         ""
#define L_KEY_FPXEXTENSIONS  ""
#define L_KEY_OCR            ""
#define L_KEY_VIDEO          ""
#define L_KEY_MEDICAL        ""

#define UNLOCKSUPPORT(leadocx) \
{ \
   (leadocx).UnlockSupport(L_SUPPORT_EXPRESS, L_KEY_EXPRESS); \
   (leadocx).UnlockSupport(L_SUPPORT_GIFLZW, L_KEY_GIFLZW); \
   (leadocx).UnlockSupport(L_SUPPORT_TIFLZW, L_KEY_TIFLZW); \
   (leadocx).UnlockSupport(L_SUPPORT_FPXEXTENSIONS, L_KEY_FPXEXTENSIONS); \
   (leadocx).UnlockSupport(L_SUPPORT_OCR, L_KEY_OCR); \
   (leadocx).UnlockSupport(L_SUPPORT_VIDEO, L_KEY_VIDEO); \
   (leadocx).UnlockSupport(L_SUPPORT_MEDICAL, L_KEY_MEDICAL); \
}

        enum ColorResPaletteConstants {
                CRP_FIXEDPALETTE = 0x0001,
                CRP_OPTIMIZEDPALETTE = 0x0002,
                CRP_BYTEORDERBGR = 0x0004,
                CRP_BYTEORDERRGB = 0x0000,
                CRP_IDENTITYPALETTE = 0x0008,
                CRP_USERPALETTE = 0x0010,
                CRP_NETSCAPEPALETTE = 0x0040,
                CRP_BYTEORDERGRAY = 0x0080,
        };

        enum ColorResDitherConstants {
                CRD_NODITHERING = 0x0000,
                CRD_FLOYDSTEINDITHERING = 0x0001,
                CRD_STUCKIDITHERING = 0x0002,
                CRD_BURKESDITHERING = 0x0003,
                CRD_SIERRADITHERING = 0x0004,
                CRD_STEVENSONARCEDITHERING = 0x0005,
                CRD_JARVISDITHERING = 0x0006,
                CRD_ORDEREDDITHERING = 0x0007,
        };

        enum SpatialFilterConstants {
                FLT_EMBOSS_N = 0,
                FLT_EMBOSS_NE = 1,
                FLT_EMBOSS_E = 2,
                FLT_EMBOSS_SE = 3,
                FLT_EMBOSS_S = 4,
                FLT_EMBOSS_SW = 5,
                FLT_EMBOSS_W = 6,
                FLT_EMBOSS_NW = 7,

                FLT_GRADIENT_N = 8,
                FLT_GRADIENT_NE = 9,
                FLT_GRADIENT_E = 10,
                FLT_GRADIENT_SE = 11,
                FLT_GRADIENT_S = 12,
                FLT_GRADIENT_SW = 13,
                FLT_GRADIENT_W = 14,
                FLT_GRADIENT_NW = 15,

                FLT_LAPLACE_1 = 16,
                FLT_LAPLACE_2 = 17,
                FLT_LAPLACE_3 = 18,
                FLT_LAPLACE_DIAG = 19,
                FLT_LAPLACE_HORZ = 20,
                FLT_LAPLACE_VERT = 21,

                FLT_SOBEL_HORZ = 22,
                FLT_SOBEL_VERT = 23,

                FLT_PREWITT_HORZ = 24,
                FLT_PREWITT_VERT = 25,

                FLT_SHIFTDIFF_DIAG = 26,
                FLT_SHIFTDIFF_HORZ = 27,
                FLT_SHIFTDIFF_VERT = 28,

                FLT_LINESEG_HORZ = 29,
                FLT_LINESEG_VERT = 30,
                FLT_LINESEG_LTOR = 31,
                FLT_LINESEG_RTOL = 32
        };


        enum HalftoneConstants {
                HALFTONE_PRINT = 0,
                HALFTONE_VIEW = 1,
        };

        enum QFactorConstants {
                QFACTOR_LEAD_0 = 0,
                QFACTOR_LEAD_1 = 1,
                QFACTOR_PQ1 = -1,
                QFACTOR_PQ2 = -2,
                QFACTOR_QFS = -3,
                QFACTOR_QMS = -4,
                QFACTOR_QS = -5,
                QFACTOR_SQS = -6,
                QFACTOR_SQT = -7,
                QFACTOR_MCQ = -8,
                QFACTOR_MC = -9
        };

        enum BinaryFilterConstants {
                BFLT_EROSION_OMNI = 0,
                BFLT_EROSION_HORZ = 1,
                BFLT_EROSION_VERT = 2,
                BFLT_EROSION_DIAG = 3,
                BFLT_DILATION_OMNI = 4,
                BFLT_DILATION_HORZ = 5,
                BFLT_DILATION_VERT = 6,
                BFLT_DILATION_DIAG = 7
        };

        enum ColorSeparationConstants {
                COLORSEP_RGB = 0x00,
                COLORSEP_CMYK = 0x01,
                COLORSEP_HSV = 0x02,
                COLORSEP_HLS = 0x03,
                COLORSEP_CMY = 0x04
        };

        enum ChannelConstants {
                CHANNEL_MASTER = 0,
                CHANNEL_RED = 1,
                CHANNEL_GREEN = 2,
                CHANNEL_BLUE = 3
        };

/* this macro that can be used in Combine method to copy data from the
   source to the destination */
#define L_SRC_COPY (CB_DST_0 | CB_OP_OR)

        enum CombineConstants {
                CB_SRC_NOP = 0x0000,
                CB_SRC_NOT = 0x0001,
                CB_SRC_0 = 0x0002,
                CB_SRC_1 = 0x0003,

                CB_DST_NOP = 0x0000,
                CB_DST_NOT = 0x0010,
                CB_DST_0 = 0x0020,
                CB_DST_1 = 0x0030,

                CB_OP_AND = 0x0000,
                CB_OP_OR = 0x0100,
                CB_OP_XOR = 0x0200,
                CB_OP_ADD = 0x0300,
                CB_OP_SUBSRC = 0x0400,
                CB_OP_SUBDST = 0x0500,
                CB_OP_MUL = 0x0600,
                CB_OP_DIVSRC = 0x0700,
                CB_OP_DIVDST = 0x0800,
                CB_OP_AVG = 0x0900,
                CB_OP_MIN = 0x0A00,
                CB_OP_MAX = 0x0B00,

                CB_RES_NOP = 0x0000,
                CB_RES_NOT = 0x1000,
                CB_RES_0 = 0x2000,
                CB_RES_1 = 0x3000
        };

        enum EmbossConstants {
                EMBOSS_N = 0,
                EMBOSS_NE = 1,
                EMBOSS_E = 2,
                EMBOSS_SE = 3,
                EMBOSS_S = 4,
                EMBOSS_SW = 5,
                EMBOSS_W = 6,
                EMBOSS_NW = 7
        };

        enum CopyConstants {
                COPY_EMPTY = 0x0001,
                COPY_DIB = 0x0002,
                COPY_DDB = 0x0004,
                COPY_PALETTE = 0x0008,
                COPY_RGN = 0x0010
        };

        enum UnderlayConstants {
                UNDERLAY_TILE = 0,
                UNDERLAY_STRETCH = 1
        };

        enum VersionLevelConstants {
                VERSIONLEVEL_EXP = 0,
                VERSIONLEVEL_PRO = 1,
                VERSIONLEVEL_MEDICAL = 2,
        };

        enum FileConstants {
                FILE_PCX = 1,
                FILE_GIF = 2,
                FILE_TIF = 3,
                FILE_TGA = 4,
                FILE_CMP = 5,
                FILE_BMP = 6,
                FROM_BUFFER = 7,
                FILE_BITMAP = 9,
                FILE_JFIF = 10,
                FILE_JTIF = 11,
                FILE_BIN = 12,
                FILE_HANDLE = 13,
                FILE_OS2 = 14,
                FILE_WMF = 15,
                FILE_EPS = 16,
                FILE_TIFLZW = 17,
                FILE_LEAD = 20,
                FILE_LEAD1JFIF = 21,
                FILE_LEAD1JTIF = 22,
                FILE_LEAD2JFIF = 23,
                FILE_LEAD2JTIF = 24,
                FILE_CCITT = 25,
                FILE_LEAD1BIT = 26,
                FILE_CCITT_GROUP3_1DIM = 27,
                FILE_CCITT_GROUP3_2DIM = 28,
                FILE_CCITT_GROUP4 = 29,
                FILE_LEAD_NOLOSS = 30,
                FILE_CALS = 50,
                FILE_MAC = 51,
                FILE_IMG = 52,
                FILE_MSP = 53,
                FILE_WPG = 54,
                FILE_RAS = 55,
                FILE_PCT = 56,
                FILE_PCD = 57,
                FILE_DXF = 58,
                FILE_AVI = 59,
                FILE_WAV = 60,
                FILE_FLI = 61,
                FILE_CGM = 62,
                FILE_EPSTIFF = 63,
                FILE_EPSWMF = 64,
                FILE_CMPNOLOSS = 65,
                FILE_FAX_G3_1D = 66,
                FILE_FAX_G3_2D = 67,
                FILE_FAX_G4 = 68,
                FILE_WFX_G3_1D = 69,
                FILE_WFX_G4 = 70,
                FILE_ICA_G3_1D = 71,
                FILE_ICA_G3_2D = 72,
                FILE_ICA_G4 = 73,
                FILE_OS2_2 = 74,
                FILE_PNG = 75,
                FILE_PSD = 76,
                FILE_FPX = 80,
                FILE_RAWICA_G3_1D = 77,
                FILE_RAWICA_G3_2D = 78,
                FILE_RAWICA_G4 = 79,
                FILE_FPX_SINGLE_COLOR = 81,
                FILE_FPX_JPEG = 82,
                FILE_FPX_JPEG_QFACTOR = 83,
                FILE_BMP_RLE = 84,
                FILE_TIF_CMYK = 85,
                FILE_TIFLZW_CMYK = 86,
                FILE_TIF_PACKBITS = 87,
                FILE_TIF_PACKBITS_CMYK = 88,
                FILE_DICOM_GRAY = 89,
                FILE_DICOM_COLOR = 90,
                FILE_WIN_ICO = 91,
                FILE_WIN_CUR = 92,
                FILE_TIF_YCC = 93,
                FILE_TIFLZW_YCC = 94,
                FILE_TIF_PACKBITS_YCC = 95,
                FILE_EXIF = 96,
                FILE_EXIF_YCC = 97,
                FILE_EXIF_JPEG = 98,
                FILE_AWD = 99,
                FILE_FASTEST = 100,
                FILE_EXIF_JPEG_411 = 101,
        };

        enum BitonalScalingConstants {
                BITONALSCALING_NORMAL,
                BITONALSCALING_FAVORBLACK,
                BITONALSCALING_SCALETOGRAY
        };

        enum PaintDitherConstants {
                PAINTDITHER_ORDERED,
                PAINTDITHER_DIFFUSION
        };

        enum PaintPaletteConstants {
                PAINTPALETTE_AUTO,
                PAINTPALETTE_FIXED,
                PAINTPALETTE_AUTONETSCAPE,
                PAINTPALETTE_FIXEDNETSCAPE,
        };

        enum ResizeConstants {
                RESIZE_NORMAL,
                RESIZE_RESAMPLE,
                RESIZE_FAVORBLACK,
                RESIZE_FAVORBLACK_OR_RESAMPLE,
        };

        enum PasteConstants {
                 PASTE_ISREADY = 0x4000
        };

// ActiveX error codes
#include "..\mncmn\L_OcxErr.h"

        enum PCDResConstants {
                PCDRES_BASE_OVER_64,
                PCDRES_BASE_OVER_16,
                PCDRES_BASE_OVER_4,
                PCDRES_BASE,
                PCDRES_4BASE,
                PCDRES_16BASE
        };

        enum dbEditModeConstants {
                DB_EDITMODE_NONE,
                DB_EDITMODE_EDIT,
                DB_EDITMODE_ADDNEW
        };

        enum dbOpenOptionsConstants {
                DB_OPENOPTIONS_NONE = 0,
                DB_OPENOPTIONS_READONLY = 4,
                DB_OPENOPTIONS_APPENDONLY = 8,
        };

        enum dbLockingModeConstants {
                DB_LOCKINGMODE_OPTIMISTIC,
                DB_LOCKINGMODE_PESSIMISTIC
        };


        enum TwainPixelTypeConstants {
                TWAIN_PIX_DEFAULT = -1,
                TWAIN_PIX_HALF,
                TWAIN_PIX_GRAY,
                TWAIN_PIX_RGB,
                TWAIN_PIX_PALETTE
        };

        enum TwainFlagsConstants {
                TWAIN_SHOWUI = 1,
                TWAIN_SHOWSTATUS = 2,
                TWAIN_NATIVEXFER = 4,
                TWAIN_KEEPOPEN = 8,
        };
        #define TWAIN_DEFAULT_BRIGHTNESS    2000
        #define TWAIN_DEFAULT_INTENSITY     2000
        #define TWAIN_DEFAULT_CONTRAST      2000

        #define TWAIN_UPDATE_DEFAULTS_ONLY  0x10
        #define TWAIN_UPDATE_NON_DEFAULTS   0x20
        #define TWAIN_CACHE_SOURCE          0x80 /* Close the data source after each scan - slower scanning */

        #define TWAIN_FEEDER_DEFAULT        2
        #define TWAIN_AUTOFEED_DEFAULT      2

        enum IsGrayscaleConstants {
                GRAY_NO = 0,
                GRAY_ORDEREDNORMAL,
                GRAY_ORDEREDREVERSE = 2,
                GRAY_ORDEREDINVERSE = 2,
                GRAY_NOTORDERED
        };

        enum SupportLockConstants {
                L_SUPPORT_EXPRESS = 0,
                L_SUPPORT_GIFLZW,
                L_SUPPORT_TIFLZW,
                L_SUPPORT_FPXEXTENSIONS,
                L_SUPPORT_OCR,
                L_SUPPORT_VIDEO,
                L_SUPPORT_MEDICAL,
        };

        enum RgnMarkingModeConstants {
               RGNMARK_NONE,
               RGNMARK_RECT,
               RGNMARK_ROUNDRECT,
               RGNMARK_ELLIPSE,
               RGNMARK_FREEHAND,
         };

        enum RgnFrameTypeConstants {
               RGNFRAME_NONE,
               RGNFRAME_STATIC,
         };

        enum RgnCombineModeConstants {
                L_RGN_AND,
                L_RGN_SET,
                L_RGN_ANDNOTBITMAP,
                L_RGN_ANDNOTRGN,
                L_RGN_OR,
                L_RGN_XOR,
                L_RGN_SETNOT,
         };

         enum PolygonFillModeConstants {  /* FillMode in SetRgnPolygon */
               L_POLY_WINDING,
               L_POLY_ALTERNATE
         };

        enum AnnUserModeConstants {
               ANNUSERMODE_NONE,
               ANNUSERMODE_DESIGN,
               ANNUSERMODE_RUN,
         } ;

        enum  AnnToolConstants {
               ANNTOOL_SELECT,
               ANNTOOL_LINE,
               ANNTOOL_RECT,
               ANNTOOL_ELLIPSE,
               ANNTOOL_POLYLINE,
               ANNTOOL_POLYGON,
               ANNTOOL_POINTER,
               ANNTOOL_FREEHAND,
               ANNTOOL_HILITE,
               ANNTOOL_REDACT,
               ANNTOOL_TEXT,
               ANNTOOL_NOTE,
               ANNTOOL_STAMP,
               ANNTOOL_BUTTON,
               ANNTOOL_HOTSPOT,
               ANNTOOL_AUDIO,
               ANNTOOL_RULER,
         };

        enum AnnObjectConstants {
               ANNOBJECT_CONTAINER,
               ANNOBJECT_POINTER,
               ANNOBJECT_AUDIO,
               ANNOBJECT_BUTTON,
               ANNOBJECT_ELLIPSE,
               ANNOBJECT_FREEHAND,
               ANNOBJECT_HILITE,
               ANNOBJECT_HOTSPOT,
               ANNOBJECT_LINE,
               ANNOBJECT_NOTE,
               ANNOBJECT_POLYGON,
               ANNOBJECT_POLYLINE,
               ANNOBJECT_RECT,
               ANNOBJECT_REDACT,
               ANNOBJECT_STAMP,
               ANNOBJECT_TEXT,
               ANNOBJECT_AUTOMATION,
               ANNOBJECT_RULER,
         };

         enum AnnFormatConstants {
               ANNFMT_NATIVE,
               ANNFMT_WMF,
               ANNFMT_TIFFTAG,
               ANNFMT_WANGTAG,
         };

        enum PaintSizeModeConstants {
           PAINTSIZEMODE_NORMAL,
           PAINTSIZEMODE_STRETCH,
           PAINTSIZEMODE_FITSIDES,
           PAINTSIZEMODE_FIT,
           PAINTSIZEMODE_ZOOM,
         };

         enum LoadInfoFlagConstants {
               LOADINFO_TOPLEFT = 1,
               LOADINFO_ORDERRGB = 2,
               LOADINFO_WHITEONBLACK = 4,
               LOADINFO_LSB = 8
         };

        enum  AutoAnimationLoopConstants {
           AUTOANIMATIONLOOP_DEFAULT,
           AUTOANIMATIONLOOP_INFINITE,
        };

       enum ProgressivePassesConstants {
          PROGRESSIVEPASSES_WHEN_MEANINGFUL = -1,
          PROGRESSIVEPASSES_ALWAYS = -2,
          PROGRESSIVEPASSES_ONCE = 0,
        };

       enum AnimationDisposalConstants {
          ANIMATIONDISPOSAL_NONE,
          ANIMATIONDISPOSAL_LEAVE,
          ANIMATIONDISPOSAL_RESTOREBACKGROUND,
          ANIMATIONDISPOSAL_RESTOREPREVIOUS,
        };

       enum AnnAutoTextConstants {
          ANNAUTOTEXT_MENU_UNDO,                   
          ANNAUTOTEXT_MENU_CUT,                    
          ANNAUTOTEXT_MENU_COPY,                   
          ANNAUTOTEXT_MENU_PASTE,                  
          ANNAUTOTEXT_MENU_DELETE,                 
          ANNAUTOTEXT_MENU_SELECTALL,              
          ANNAUTOTEXT_MENU_BRINGTOFRONT,           
          ANNAUTOTEXT_MENU_SENDTOBACK,             
          ANNAUTOTEXT_MENU_DEFAULTPROPERTIES,      
          ANNAUTOTEXT_MENU_SELECTEDPROPERTIES,     
          ANNAUTOTEXT_MENU_CONTAINERPROPERTIES,    
          ANNAUTOTEXT_MENU_POINTERPROPERTIES,      
          ANNAUTOTEXT_MENU_AUDIOPROPERTIES,        
          ANNAUTOTEXT_MENU_BUTTONPROPERTIES,       
          ANNAUTOTEXT_MENU_ELLIPSEPROPERTIES,      
          ANNAUTOTEXT_MENU_FREEHANDPROPERTIES,     
          ANNAUTOTEXT_MENU_HILITEPROPERTIES,       
          ANNAUTOTEXT_MENU_HOTSPOTPROPERTIES,      
          ANNAUTOTEXT_MENU_LINEPROPERTIES,         
          ANNAUTOTEXT_MENU_NOTEPROPERTIES,         
          ANNAUTOTEXT_MENU_POLYGONPROPERTIES,      
          ANNAUTOTEXT_MENU_POLYLINEPROPERTIES,     
          ANNAUTOTEXT_MENU_RECTANGLEPROPERTIES,    
          ANNAUTOTEXT_MENU_REDACTPROPERTIES,       
          ANNAUTOTEXT_MENU_STAMPPROPERTIES,        
          ANNAUTOTEXT_MENU_TEXTPROPERTIES,         
          ANNAUTOTEXT_MENU_AUTOMATIONPROPERTIES,   
          ANNAUTOTEXT_MENU_RULERPROPERTIES,        
          ANNAUTOTEXT_MENU_LINE,                   
          ANNAUTOTEXT_MENU_FILL,                   
          ANNAUTOTEXT_MENU_FOREGROUNDCOLOR,        
          ANNAUTOTEXT_MENU_BACKGROUNDCOLOR,        
          ANNAUTOTEXT_MENU_FONT,                   
          ANNAUTOTEXT_MENU_BITMAP,                 
          ANNAUTOTEXT_MENU_TEXT,                   
          ANNAUTOTEXT_MENU_FILE,                   
          ANNAUTOTEXT_MENU_ROP2,                   
          ANNAUTOTEXT_MENU_TRANSPARENT,            
          ANNAUTOTEXT_MENU_LOCK,                   
          ANNAUTOTEXT_MENU_UNLOCK,                 
          ANNAUTOTEXT_MENU_LOCKALL,                
          ANNAUTOTEXT_MENU_UNLOCKALL,              
          ANNAUTOTEXT_MENU_HYPERLINK,              
          ANNAUTOTEXT_MENU_NOTEBACKGROUNDCOLOR,    
          ANNAUTOTEXT_MENU_REDACTBACKGROUNDCOLOR,  
          ANNAUTOTEXT_MENU_HILITEBACKGROUNDCOLOR,  
          ANNAUTOTEXT_MENU_RULER,                  
          ANNAUTOTEXT_LINE_CAPTION,                
          ANNAUTOTEXT_LINE_WIDTH,                  
          ANNAUTOTEXT_LINE_STYLE,                  
          ANNAUTOTEXT_LINE_OK,                     
          ANNAUTOTEXT_LINE_CANCEL,                 
          ANNAUTOTEXT_LINE_SOLID,                  
          ANNAUTOTEXT_LINE_DASH,                   
          ANNAUTOTEXT_LINE_DOT,                    
          ANNAUTOTEXT_LINE_DASHDOT,                
          ANNAUTOTEXT_LINE_DASHDOTDOT,             
          ANNAUTOTEXT_LINE_NULL,                   
          ANNAUTOTEXT_FILL_CAPTION,                
          ANNAUTOTEXT_FILL_MODE,                   
          ANNAUTOTEXT_FILL_PATTERNS,               
          ANNAUTOTEXT_FILL_POLYGON,                
          ANNAUTOTEXT_FILL_OK,                     
          ANNAUTOTEXT_FILL_CANCEL,                 
          ANNAUTOTEXT_FILL_TRANSPARENT,            
          ANNAUTOTEXT_FILL_TRANSLUCENT,            
          ANNAUTOTEXT_FILL_OPAQUE,                 
          ANNAUTOTEXT_FILL_SOLID,                  
          ANNAUTOTEXT_FILL_BACKWARDDIAGONAL,       
          ANNAUTOTEXT_FILL_CROSS,                  
          ANNAUTOTEXT_FILL_DIAGONALCROSS,          
          ANNAUTOTEXT_FILL_FORWARDDIAGONAL,        
          ANNAUTOTEXT_FILL_HORIZONTAL,             
          ANNAUTOTEXT_FILL_VERTICAL,               
          ANNAUTOTEXT_FILL_WINDING,                
          ANNAUTOTEXT_FILL_ALTERNATE,              
          ANNAUTOTEXT_TEXT_CAPTION,                
          ANNAUTOTEXT_TEXT_OK,                     
          ANNAUTOTEXT_TEXT_CANCEL,                 
          ANNAUTOTEXT_AUDIOFILE_CAPTION,           
          ANNAUTOTEXT_AUDIOFILE_FILTER,            
          ANNAUTOTEXT_STAMPFILE_CAPTION,           
          ANNAUTOTEXT_STAMPFILE_FILTER,            
          ANNAUTOTEXT_ITEM_DEFAULTTEXT,            
          ANNAUTOTEXT_ROP2_CAPTION,                
          ANNAUTOTEXT_ROP2_TEXT,                   
          ANNAUTOTEXT_ROP2_OK,                     
          ANNAUTOTEXT_ROP2_CANCEL,                 
          ANNAUTOTEXT_ROP2_COPY,                   
          ANNAUTOTEXT_ROP2_AND,                    
          ANNAUTOTEXT_ROP2_XOR,                    
          ANNAUTOTEXT_LOCK_CAPTION,                
          ANNAUTOTEXT_LOCK_TEXT,                   
          ANNAUTOTEXT_LOCK_OK,                     
          ANNAUTOTEXT_LOCK_CANCEL ,                
          ANNAUTOTEXT_UNLOCK_CAPTION,              
          ANNAUTOTEXT_UNLOCK_TEXT,                 
          ANNAUTOTEXT_UNLOCK_OK,                   
          ANNAUTOTEXT_UNLOCK_CANCEL,               
          ANNAUTOTEXT_RULER_CAPTION,               
          ANNAUTOTEXT_RULER_UNITS,                 
          ANNAUTOTEXT_RULER_ABBREV,                
          ANNAUTOTEXT_RULER_PRECISION,             
          ANNAUTOTEXT_RULER_LENGTH,                
          ANNAUTOTEXT_RULER_OK,                    
          ANNAUTOTEXT_RULER_CANCEL,                
          ANNAUTOTEXT_RULER_INCHES,                
          ANNAUTOTEXT_RULER_FEET,                  
          ANNAUTOTEXT_RULER_YARDS,                 
          ANNAUTOTEXT_RULER_MICROMETERS,           
          ANNAUTOTEXT_RULER_MILIMETERS,            
          ANNAUTOTEXT_RULER_CENTIMETERS,           
          ANNAUTOTEXT_RULER_METERS,                
          ANNAUTOTEXT_RULER_TWIPS,                 
          ANNAUTOTEXT_RULER_POINTS,                
          ANNAUTOTEXT_RULER_PIXELS,                
          ANNAUTOTEXT_MAX,                         
        };

        enum AppearanceConstants {
           APPEARANCE_FLAT,
           APPEARANCE_THREED,
        };


        enum CommentConstants {
            CMNT_SZARTIST = 0, 
            CMNT_SZCOPYRIGHT = 1, 
            CMNT_SZDATETIME = 2, 
            CMNT_SZDESC = 3, 
            CMNT_SZHOSTCOMP = 4, 
            CMNT_SZMAKE = 5, 
            CMNT_SZMODEL = 6, 
            CMNT_SZNAMEOFDOC = 7, 
            CMNT_SZNAMEOFPAGE = 8, 
            CMNT_SZSOFTWARE = 9, 
            CMNT_SZPATIENTNAME = 10, 
            CMNT_SZPATIENTID = 11, 
            CMNT_SZPATIENTBIRTHDATE = 12, 
            CMNT_SZPATIENTSEX = 13, 
            CMNT_SZSTUDYINSTANCE = 14, 
            CMNT_SZSTUDYDATE = 15, 
            CMNT_SZSTUDYTIME = 16, 
            CMNT_SZSTUDYREFERRINGPHYSICIAN = 17, 
            CMNT_SZSERIESMODALITY = 18, 
            CMNT_SZSERIESID = 19, 
            CMNT_SZSERIESNUMBER = 20, 
            CMNT_EXIFVERSION = 21, 
            CMNT_SZDATETIMEORIGINAL = 22, 
            CMNT_SZDATETIMEDIGITIZED = 23, 
            CMNT_SHUTTERSPEEDVALUE = 24, 
            CMNT_APERTURE = 25, 
            CMNT_BRIGHTNESS = 26, 
            CMNT_EXPOSUREBIAS = 27, 
            CMNT_MAXAPERTURE = 28, 
            CMNT_SUBJECTDISTANCE = 29, 
            CMNT_METERINGMODE = 30, 
            CMNT_LIGHTSOURCE = 31, 
            CMNT_FLASH = 32, 
            CMNT_FOCALLENGTH = 33, 
            CMNT_EXPOSURETIME = 34, 
            CMNT_FNUMBER = 35, 
            CMNT_MAKERNOTE = 36, 
            CMNT_USERCOMMENT = 37, 
            CMNT_SZSUBSECTIME = 38, 
            CMNT_SZSUBSECTIMEORIGINAL = 39, 
            CMNT_SZSUBSECTIMEDIGITIZED = 40, 
            CMNT_GPSVERSIONID = 41, 
            CMNT_GPSLATITUDEREF = 42, 
            CMNT_GPSLATITUDE = 43, 
            CMNT_GPSLONGITUDEREF = 44, 
            CMNT_GPSLONGITUDE = 45, 
            CMNT_GPSALTITUDEREF = 46, 
            CMNT_GPSALTITUDE = 47, 
            CMNT_GPSTIMESTAMP = 48, 
            CMNT_GPSSATELLITES = 49, 
            CMNT_GPSSTATUS = 50, 
            CMNT_GPSMEASUREMODE = 51, 
            CMNT_GPSDOP = 52, 
            CMNT_GPSSPEEDREF = 53, 
            CMNT_GPSSPEED = 54, 
            CMNT_GPSTRACKREF = 55, 
            CMNT_GPSTRACK = 56, 
            CMNT_GPSIMGDIRECTIONREF = 57, 
            CMNT_GPSIMGDIRECTION = 58, 
            CMNT_GPSMAPDATUM = 59, 
            CMNT_GPSDESTLATITUDEREF = 60, 
            CMNT_GPSDESTLATITUDE = 61, 
            CMNT_GPSDESTLONGITUDERE = 62, 
            CMNT_GPSDESTLONGITUDE = 63, 
            CMNT_GPSDESTBEARINGREF = 64, 
            CMNT_GPSDESTBEARING = 65, 
            CMNT_GPSDESTDISTANCEREF = 66, 
            CMNT_GPSDESTDISTANCE = 67, 
            CMNT_LAST = 67,
        };

        enum DrawingFillStyleConstants {
              DRAWFILLSTYLE_SOLID = 0, 
              DRAWFILLSTYLE_TRANSPARENT = 1, 
              DRAWFILLSTYLE_HORIZONTAL_LINE = 2, 
              DRAWFILLSTYLE_VERTICAL_LINE = 3, 
              DRAWFILLSTYLE_UPWARD_DIAGONAL = 4, 
              DRAWFILLSTYLE_DOWNWARD_DIAGONAL = 5, 
              DRAWFILLSTYLE_CROSS = 6, 
              DRAWFILLSTYLE_DIAGONAL_CROSS = 7, 
              DRAWFILLSTYLE_DIAGONAL_LAST = 7,
        };

        enum DrawingPenStyleConstants {
              DRAWPENSTYLE_SOLID = 0, 
              DRAWPENSTYLE_DASH = 1, 
              DRAWPENSTYLE_DOT  = 2, 
              DRAWPENSTYLE_DASH_DOT = 3,  
              DRAWPENSTYLE_DASH_DOT_DOT = 4, 
              DRAWPENSTYLE_TRANSPARENT = 5, 
              DRAWPENSTYLE_INSIDE_SOLID = 6, 
              DRAWPENSTYLE_LAST = 6,
        };

        enum DrawingModeConstants {
              DRAWMODE_BLACKNESS = 1, 
              DRAWMODE_NOT_MERGE_PEN = 2, 
              DRAWMODE_MASK_NOT_PEN = 3, 
              DRAWMODE_NOT_COPY_PEN = 4, 
              DRAWMODE_MASK_PEN_NOT = 5, 
              DRAWMODE_INVERT = 6, 
              DRAWMODE_XOR_PEN = 7, 
              DRAWMODE_NOT_MASK_PEN = 8, 
              DRAWMODE_MASK_PEN = 9, 
              DRAWMODE_NOT_XOR_PEN = 10, 
              DRAWMODE_NOP = 11, 
              DRAWMODE_MERGE_NOT_PEN = 12, 
              DRAWMODE_COPY_PEN = 13, 
              DRAWMODE_MERGE_PEN_NOT = 14, 
              DRAWMODE_MERGE_PEN = 15, 
              DRAWMODE_WHITENESS = 16, 
              DRAWMODE_LAST = 16,
        };


        enum TransitionStyleConstants{
              EFX_TRANSITION_NONE = 0,
              EFX_TRANSITION_GRADIENT = 1,
              EFX_TRANSITION_PATTERN = 2,
              EFX_TRANSITION_MAX = 2
        };


// Pattern
#define EFX_PATTERN_CLASS                        0
#define EFX_PATTERN_SOLID                        (EFX_PATTERN_CLASS+0)
#define EFX_PATTERN_TRANSPARENT                  (EFX_PATTERN_CLASS+1)
#define EFX_PATTERN_HORZ_LINE                    (EFX_PATTERN_CLASS+2)
#define EFX_PATTERN_VERT_LINE                    (EFX_PATTERN_CLASS+3)
#define EFX_PATTERN_DOWNWARD_DIAG                (EFX_PATTERN_CLASS+4)
#define EFX_PATTERN_UPWARD_DIAG                  (EFX_PATTERN_CLASS+5)
#define EFX_PATTERN_CROSS                        (EFX_PATTERN_CLASS+6)
#define EFX_PATTERN_DIAG_CROSS                   (EFX_PATTERN_CLASS+7)
#define EFX_PATTERN_MAX                          (EFX_PATTERN_CLASS+7)


// Linear
#define EFX_GRADIENT_LINE_CLASS                  1000
#define EFX_GRADIENT_LINE_L_TO_R                 (EFX_GRADIENT_LINE_CLASS+0)
#define EFX_GRADIENT_LINE_R_TO_L                 (EFX_GRADIENT_LINE_CLASS+1)
#define EFX_GRADIENT_LINE_T_TO_B                 (EFX_GRADIENT_LINE_CLASS+2)
#define EFX_GRADIENT_LINE_B_TO_T                 (EFX_GRADIENT_LINE_CLASS+3)
#define EFX_GRADIENT_LINE_LT_TO_RB               (EFX_GRADIENT_LINE_CLASS+4)
#define EFX_GRADIENT_LINE_LB_TO_RT               (EFX_GRADIENT_LINE_CLASS+5)
#define EFX_GRADIENT_LINE_RT_TO_LB               (EFX_GRADIENT_LINE_CLASS+6)
#define EFX_GRADIENT_LINE_RB_TO_LT               (EFX_GRADIENT_LINE_CLASS+7)
#define EFX_GRADIENT_LINE2_L_AND_R_TO_C          (EFX_GRADIENT_LINE_CLASS+8)
#define EFX_GRADIENT_LINE2_C_TO_L_AND_R          (EFX_GRADIENT_LINE_CLASS+9)
#define EFX_GRADIENT_LINE2_T_AND_B_TO_C          (EFX_GRADIENT_LINE_CLASS+10)
#define EFX_GRADIENT_LINE2_C_TO_T_AND_B          (EFX_GRADIENT_LINE_CLASS+11)
#define EFX_GRADIENT_LINE2_LT_AND_RB_TO_C        (EFX_GRADIENT_LINE_CLASS+12)
#define EFX_GRADIENT_LINE2_C_TO_LT_AND_RB        (EFX_GRADIENT_LINE_CLASS+13)
#define EFX_GRADIENT_LINE2_RT_AND_LB_TO_C        (EFX_GRADIENT_LINE_CLASS+14)
#define EFX_GRADIENT_LINE2_C_TO_RT_AND_LB        (EFX_GRADIENT_LINE_CLASS+15)
#define EFX_GRADIENT_LINE_MAX                    (EFX_GRADIENT_LINE_CLASS+15)

// Angular
#define EFX_GRADIENT_ANGLE_CLASS                 2000
#define EFX_GRADIENT_ANGLE_TO_LT                 (EFX_GRADIENT_ANGLE_CLASS+0)
#define EFX_GRADIENT_ANGLE_TO_LB                 (EFX_GRADIENT_ANGLE_CLASS+1)
#define EFX_GRADIENT_ANGLE_TO_RT                 (EFX_GRADIENT_ANGLE_CLASS+2)
#define EFX_GRADIENT_ANGLE_TO_RB                 (EFX_GRADIENT_ANGLE_CLASS+3)
#define EFX_GRADIENT_ANGLE_FROM_LT               (EFX_GRADIENT_ANGLE_CLASS+4)
#define EFX_GRADIENT_ANGLE_FROM_LB               (EFX_GRADIENT_ANGLE_CLASS+5)
#define EFX_GRADIENT_ANGLE_FROM_RT               (EFX_GRADIENT_ANGLE_CLASS+6)
#define EFX_GRADIENT_ANGLE_FROM_RB               (EFX_GRADIENT_ANGLE_CLASS+7)
#define EFX_GRADIENT_ANGLE_MAX                   (EFX_GRADIENT_ANGLE_CLASS+7)

// Rectangular
#define EFX_GRADIENT_RECTANGLE_CLASS             3000
#define EFX_GRADIENT_RECTANGLE_TO_C              (EFX_GRADIENT_RECTANGLE_CLASS+0)
#define EFX_GRADIENT_RECTANGLE_FROM_C            (EFX_GRADIENT_RECTANGLE_CLASS+1)
#define EFX_GRADIENT_RECTANGLE_MAX               (EFX_GRADIENT_RECTANGLE_CLASS+1)

// Elliptical
#define EFX_GRADIENT_ELLIPSE_CLASS               4000
#define EFX_GRADIENT_ELLIPSE_TO_C                (EFX_GRADIENT_ELLIPSE_CLASS+0)
#define EFX_GRADIENT_ELLIPSE_FROM_C              (EFX_GRADIENT_ELLIPSE_CLASS+1)
#define EFX_GRADIENT_ELLIPSE_MAX                 (EFX_GRADIENT_ELLIPSE_CLASS+1)

// Conical
#define EFX_GRADIENT_CONE_CLASS                  5000
#define EFX_GRADIENT_CONE_FROM_L                 (EFX_GRADIENT_CONE_CLASS+0)
#define EFX_GRADIENT_CONE_FROM_R                 (EFX_GRADIENT_CONE_CLASS+1)
#define EFX_GRADIENT_CONE_FROM_T                 (EFX_GRADIENT_CONE_CLASS+2)
#define EFX_GRADIENT_CONE_FROM_B                 (EFX_GRADIENT_CONE_CLASS+3)
#define EFX_GRADIENT_CONE_FROM_LT                (EFX_GRADIENT_CONE_CLASS+4)
#define EFX_GRADIENT_CONE_FROM_LB                (EFX_GRADIENT_CONE_CLASS+5)
#define EFX_GRADIENT_CONE_FROM_RT                (EFX_GRADIENT_CONE_CLASS+6)
#define EFX_GRADIENT_CONE_FROM_RB                (EFX_GRADIENT_CONE_CLASS+7)
#define EFX_GRADIENT_CONE_MAX                    (EFX_GRADIENT_CONE_CLASS+7)


// Square
#define EFX_SHAPE_SQUARE_CLASS                   1000
#define EFX_SHAPE_SQUARE                         (EFX_SHAPE_SQUARE_CLASS+0)
#define EFX_SHAPE_SQUARE_ROUNDED                 (EFX_SHAPE_SQUARE_CLASS+1)
#define EFX_SHAPE_SQUARE_MAX                     (EFX_SHAPE_SQUARE_CLASS+1)

// Rectangle
#define EFX_SHAPE_RECTANGLE_CLASS                2000
#define EFX_SHAPE_RECTANGLE                      (EFX_SHAPE_RECTANGLE_CLASS+0)
#define EFX_SHAPE_RECTANGLE_ROUNDED              (EFX_SHAPE_RECTANGLE_CLASS+1)
#define EFX_SHAPE_RECTANGLE_MAX                  (EFX_SHAPE_RECTANGLE_CLASS+1)

// Parallelogram
#define EFX_SHAPE_PARALLELOGRAM_CLASS            3000
#define EFX_SHAPE_PARALLELOGRAM_L                (EFX_SHAPE_PARALLELOGRAM_CLASS+0)
#define EFX_SHAPE_PARALLELOGRAM_R                (EFX_SHAPE_PARALLELOGRAM_CLASS+1)
#define EFX_SHAPE_PARALLELOGRAM_T                (EFX_SHAPE_PARALLELOGRAM_CLASS+2)
#define EFX_SHAPE_PARALLELOGRAM_B                (EFX_SHAPE_PARALLELOGRAM_CLASS+3)
#define EFX_SHAPE_PARALLELOGRAM_MAX              (EFX_SHAPE_PARALLELOGRAM_CLASS+3)

// Trapezoid
#define EFX_SHAPE_TRAPEZOID_CLASS                4000
#define EFX_SHAPE_TRAPEZOID_L                    (EFX_SHAPE_TRAPEZOID_CLASS+0)
#define EFX_SHAPE_TRAPEZOID_R                    (EFX_SHAPE_TRAPEZOID_CLASS+1)
#define EFX_SHAPE_TRAPEZOID_T                    (EFX_SHAPE_TRAPEZOID_CLASS+2)
#define EFX_SHAPE_TRAPEZOID_B                    (EFX_SHAPE_TRAPEZOID_CLASS+3)
#define EFX_SHAPE_TRAPEZOID_MAX                  (EFX_SHAPE_TRAPEZOID_CLASS+3)

// Triangle
#define EFX_SHAPE_TRIANGLE_CLASS                 5000
#define EFX_SHAPE_TRIANGLE_EQUILATERAL_L         (EFX_SHAPE_TRIANGLE_CLASS+0)
#define EFX_SHAPE_TRIANGLE_EQUILATERAL_R         (EFX_SHAPE_TRIANGLE_CLASS+1)
#define EFX_SHAPE_TRIANGLE_EQUILATERAL_T         (EFX_SHAPE_TRIANGLE_CLASS+2)
#define EFX_SHAPE_TRIANGLE_EQUILATERAL_B         (EFX_SHAPE_TRIANGLE_CLASS+3)
#define EFX_SHAPE_TRIANGLE_ISOSCELES_L           (EFX_SHAPE_TRIANGLE_CLASS+4)
#define EFX_SHAPE_TRIANGLE_ISOSCELES_R           (EFX_SHAPE_TRIANGLE_CLASS+5)
#define EFX_SHAPE_TRIANGLE_ISOSCELES_T           (EFX_SHAPE_TRIANGLE_CLASS+6)
#define EFX_SHAPE_TRIANGLE_ISOSCELES_B           (EFX_SHAPE_TRIANGLE_CLASS+7)
#define EFX_SHAPE_TRIANGLE_RIGHT_LT              (EFX_SHAPE_TRIANGLE_CLASS+8)
#define EFX_SHAPE_TRIANGLE_RIGHT_LB              (EFX_SHAPE_TRIANGLE_CLASS+9)
#define EFX_SHAPE_TRIANGLE_RIGHT_RT              (EFX_SHAPE_TRIANGLE_CLASS+10)
#define EFX_SHAPE_TRIANGLE_RIGHT_RB              (EFX_SHAPE_TRIANGLE_CLASS+11)
#define EFX_SHAPE_TRIANGLE_MAX                   (EFX_SHAPE_TRIANGLE_CLASS+11)

// Other polygons
#define EFX_SHAPE_OTHER_CLASS                    6000
#define EFX_SHAPE_OCTAGON                        (EFX_SHAPE_OTHER_CLASS+0)
#define EFX_SHAPE_DIAMOND                        (EFX_SHAPE_OTHER_CLASS+1)
#define EFX_SHAPE_PENTAGON                       (EFX_SHAPE_OTHER_CLASS+2)
#define EFX_SHAPE_HEXAGON                        (EFX_SHAPE_OTHER_CLASS+3)
#define EFX_SHAPE_LINE                           (EFX_SHAPE_OTHER_CLASS+4)
#define EFX_SHAPE_REGION                         (EFX_SHAPE_OTHER_CLASS+5)
#define EFX_SHAPE_OTHER_MAX                      (EFX_SHAPE_OTHER_CLASS+5)

// Circle
#define EFX_SHAPE_CIRCLE_CLASS                   7000
#define EFX_SHAPE_CIRCLE                         (EFX_SHAPE_CIRCLE_CLASS+0)
#define EFX_SHAPE_CIRCLE_90_LT                   (EFX_SHAPE_CIRCLE_CLASS+1)
#define EFX_SHAPE_CIRCLE_90_LB                   (EFX_SHAPE_CIRCLE_CLASS+2)
#define EFX_SHAPE_CIRCLE_90_RT                   (EFX_SHAPE_CIRCLE_CLASS+3)
#define EFX_SHAPE_CIRCLE_90_RB                   (EFX_SHAPE_CIRCLE_CLASS+4)
#define EFX_SHAPE_CIRCLE_180_L                   (EFX_SHAPE_CIRCLE_CLASS+5)
#define EFX_SHAPE_CIRCLE_180_R                   (EFX_SHAPE_CIRCLE_CLASS+6)
#define EFX_SHAPE_CIRCLE_180_T                   (EFX_SHAPE_CIRCLE_CLASS+7)
#define EFX_SHAPE_CIRCLE_180_B                   (EFX_SHAPE_CIRCLE_CLASS+8)
#define EFX_SHAPE_CIRCLE_270_LT                  (EFX_SHAPE_CIRCLE_CLASS+9)
#define EFX_SHAPE_CIRCLE_270_LB                  (EFX_SHAPE_CIRCLE_CLASS+10)
#define EFX_SHAPE_CIRCLE_270_RT                  (EFX_SHAPE_CIRCLE_CLASS+11)
#define EFX_SHAPE_CIRCLE_270_RB                  (EFX_SHAPE_CIRCLE_CLASS+12)
#define EFX_SHAPE_CIRCLE_MAX                     (EFX_SHAPE_CIRCLE_CLASS+12)

// Ellipse
#define EFX_SHAPE_ELLIPSE_CLASS                  8000
#define EFX_SHAPE_ELLIPSE                        (EFX_SHAPE_ELLIPSE_CLASS+0)
#define EFX_SHAPE_ELLIPSE_90_LT                  (EFX_SHAPE_ELLIPSE_CLASS+1)
#define EFX_SHAPE_ELLIPSE_90_LB                  (EFX_SHAPE_ELLIPSE_CLASS+2)
#define EFX_SHAPE_ELLIPSE_90_RT                  (EFX_SHAPE_ELLIPSE_CLASS+3)
#define EFX_SHAPE_ELLIPSE_90_RB                  (EFX_SHAPE_ELLIPSE_CLASS+4)
#define EFX_SHAPE_ELLIPSE_180_L                  (EFX_SHAPE_ELLIPSE_CLASS+5)
#define EFX_SHAPE_ELLIPSE_180_R                  (EFX_SHAPE_ELLIPSE_CLASS+6)
#define EFX_SHAPE_ELLIPSE_180_T                  (EFX_SHAPE_ELLIPSE_CLASS+7)
#define EFX_SHAPE_ELLIPSE_180_B                  (EFX_SHAPE_ELLIPSE_CLASS+8)
#define EFX_SHAPE_ELLIPSE_270_LT                 (EFX_SHAPE_ELLIPSE_CLASS+9)
#define EFX_SHAPE_ELLIPSE_270_LB                 (EFX_SHAPE_ELLIPSE_CLASS+10)
#define EFX_SHAPE_ELLIPSE_270_RT                 (EFX_SHAPE_ELLIPSE_CLASS+11)
#define EFX_SHAPE_ELLIPSE_270_RB                 (EFX_SHAPE_ELLIPSE_CLASS+12)
#define EFX_SHAPE_ELLIPSE_MAX                    (EFX_SHAPE_ELLIPSE_CLASS+12)

// Star
#define EFX_SHAPE_STAR_CLASS                     9000
#define EFX_SHAPE_STAR4                          (EFX_SHAPE_STAR_CLASS+0)
#define EFX_SHAPE_STAR5                          (EFX_SHAPE_STAR_CLASS+1)
#define EFX_SHAPE_STAR8                          (EFX_SHAPE_STAR_CLASS+2)
#define EFX_SHAPE_STAR16                         (EFX_SHAPE_STAR_CLASS+3)
#define EFX_SHAPE_STAR24                         (EFX_SHAPE_STAR_CLASS+4)
#define EFX_SHAPE_STAR32                         (EFX_SHAPE_STAR_CLASS+5)
#define EFX_SHAPE_STAR_MAX                       (EFX_SHAPE_STAR_CLASS+5)

// Cross
#define EFX_SHAPE_CROSS_CLASS                    10000
#define EFX_SHAPE_CROSS_SMALL                    (EFX_SHAPE_CROSS_CLASS+0)
#define EFX_SHAPE_CROSS_MEDIUM                   (EFX_SHAPE_CROSS_CLASS+1)
#define EFX_SHAPE_CROSS_LARGE                    (EFX_SHAPE_CROSS_CLASS+2)
#define EFX_SHAPE_CROSS_MAX                      (EFX_SHAPE_CROSS_CLASS+2)

// Arrow
#define EFX_SHAPE_ARROW_CLASS                    11000
#define EFX_SHAPE_ARROW_L                        (EFX_SHAPE_ARROW_CLASS+0)
#define EFX_SHAPE_ARROW_R                        (EFX_SHAPE_ARROW_CLASS+1)
#define EFX_SHAPE_ARROW_T                        (EFX_SHAPE_ARROW_CLASS+2)
#define EFX_SHAPE_ARROW_B                        (EFX_SHAPE_ARROW_CLASS+3)
#define EFX_SHAPE_ARROW2_L_AND_R                 (EFX_SHAPE_ARROW_CLASS+4)
#define EFX_SHAPE_ARROW2_T_AND_B                 (EFX_SHAPE_ARROW_CLASS+5)
#define EFX_SHAPE_ARROW2_L_AND_T                 (EFX_SHAPE_ARROW_CLASS+6)
#define EFX_SHAPE_ARROW2_L_AND_B                 (EFX_SHAPE_ARROW_CLASS+7)
#define EFX_SHAPE_ARROW2_R_AND_T                 (EFX_SHAPE_ARROW_CLASS+8)
#define EFX_SHAPE_ARROW2_R_AND_B                 (EFX_SHAPE_ARROW_CLASS+9)
#define EFX_SHAPE_ARROW3_L_AND_T_AND_R           (EFX_SHAPE_ARROW_CLASS+10)
#define EFX_SHAPE_ARROW3_T_AND_R_AND_B           (EFX_SHAPE_ARROW_CLASS+11)
#define EFX_SHAPE_ARROW3_L_AND_B_AND_R           (EFX_SHAPE_ARROW_CLASS+12)
#define EFX_SHAPE_ARROW3_L_AND_T_AND_B           (EFX_SHAPE_ARROW_CLASS+13)
#define EFX_SHAPE_ARROW4_L_AND_T_AND_R_AND_B     (EFX_SHAPE_ARROW_CLASS+14)
#define EFX_SHAPE_ARROW_MAX                      (EFX_SHAPE_ARROW_CLASS+14)


//Effect types

// No effect
#define EFX_EFFECT_NONE_CLASS                                 1000
#define EFX_EFFECT_NONE                                       (EFX_EFFECT_NONE_CLASS+0)
#define EFX_EFFECT_NO_PICTURE                                 (EFX_EFFECT_NONE_CLASS+1)
#define EFX_EFFECT_NONE_MAX                                   (EFX_EFFECT_NONE_CLASS+1)

// Linear wipe
#define EFX_EFFECT_WIPE_CLASS                                 2000
#define EFX_EFFECT_WIPE_L_TO_R                                (EFX_EFFECT_WIPE_CLASS+0)
#define EFX_EFFECT_WIPE_R_TO_L                                (EFX_EFFECT_WIPE_CLASS+1)
#define EFX_EFFECT_WIPE_T_TO_B                                (EFX_EFFECT_WIPE_CLASS+2)
#define EFX_EFFECT_WIPE_B_TO_T                                (EFX_EFFECT_WIPE_CLASS+3)
#define EFX_EFFECT_WIPE_LT_TO_RB                              (EFX_EFFECT_WIPE_CLASS+4)
#define EFX_EFFECT_WIPE_LB_TO_RT                              (EFX_EFFECT_WIPE_CLASS+5)
#define EFX_EFFECT_WIPE_RT_TO_LB                              (EFX_EFFECT_WIPE_CLASS+6)
#define EFX_EFFECT_WIPE_RB_TO_LT                              (EFX_EFFECT_WIPE_CLASS+7)
#define EFX_EFFECT_WIPE_INTERH_L_TO_R_THEN_L_TO_R             (EFX_EFFECT_WIPE_CLASS+8)
#define EFX_EFFECT_WIPE_INTERV_T_TO_B_THEN_T_TO_B             (EFX_EFFECT_WIPE_CLASS+9)
#define EFX_EFFECT_WIPE_INTERH_L_TO_R_THEN_R_TO_L             (EFX_EFFECT_WIPE_CLASS+10)
#define EFX_EFFECT_WIPE_INTERV_T_TO_B_THEN_B_TO_T             (EFX_EFFECT_WIPE_CLASS+11)
#define EFX_EFFECT_WIPE_INTERV_L_TO_R_THEN_L_TO_R             (EFX_EFFECT_WIPE_CLASS+12)
#define EFX_EFFECT_WIPE_INTERV_R_TO_L_THEN_R_TO_L             (EFX_EFFECT_WIPE_CLASS+13)
#define EFX_EFFECT_WIPE_INTERH_T_TO_B_THEN_T_TO_B             (EFX_EFFECT_WIPE_CLASS+14)
#define EFX_EFFECT_WIPE_INTERH_B_TO_T_THEN_B_TO_T             (EFX_EFFECT_WIPE_CLASS+15)
#define EFX_EFFECT_WIPE_INTERV_L_TO_R_THEN_R_TO_L             (EFX_EFFECT_WIPE_CLASS+16)
#define EFX_EFFECT_WIPE_INTERV_R_TO_L_THEN_L_TO_R             (EFX_EFFECT_WIPE_CLASS+17)
#define EFX_EFFECT_WIPE_INTERH_T_TO_B_THEN_B_TO_T             (EFX_EFFECT_WIPE_CLASS+18)
#define EFX_EFFECT_WIPE_INTERH_B_TO_T_THEN_T_TO_B             (EFX_EFFECT_WIPE_CLASS+19)
#define EFX_EFFECT_WIPE_INTERH_L_TO_R_AND_R_TO_L              (EFX_EFFECT_WIPE_CLASS+20)
#define EFX_EFFECT_WIPE_INTERV_T_TO_B_AND_B_TO_T              (EFX_EFFECT_WIPE_CLASS+21)
#define EFX_EFFECT_WIPE_INTERV_L_AND_R_TO_C                   (EFX_EFFECT_WIPE_CLASS+22)
#define EFX_EFFECT_WIPE_INTERV_C_TO_L_AND_R                   (EFX_EFFECT_WIPE_CLASS+23)
#define EFX_EFFECT_WIPE_INTERH_T_AND_B_TO_C                   (EFX_EFFECT_WIPE_CLASS+24)
#define EFX_EFFECT_WIPE_INTERH_C_TO_T_AND_B                   (EFX_EFFECT_WIPE_CLASS+25)
#define EFX_EFFECT_WIPE_INTERV_L_AND_R_TO_C_THEN_C_TO_L_AND_R (EFX_EFFECT_WIPE_CLASS+26)
#define EFX_EFFECT_WIPE_INTERV_C_TO_L_AND_R_THEN_L_AND_R_TO_C (EFX_EFFECT_WIPE_CLASS+27)
#define EFX_EFFECT_WIPE_INTERH_T_AND_B_TO_C_THEN_C_TO_T_AND_B (EFX_EFFECT_WIPE_CLASS+28)
#define EFX_EFFECT_WIPE_INTERH_C_TO_T_AND_B_THEN_T_AND_B_TO_C (EFX_EFFECT_WIPE_CLASS+29)
#define EFX_EFFECT_WIPE2_LT_AND_RB_TO_C                       (EFX_EFFECT_WIPE_CLASS+30)
#define EFX_EFFECT_WIPE2_LB_AND_RT_TO_C                       (EFX_EFFECT_WIPE_CLASS+31)
#define EFX_EFFECT_WIPE2_C_TO_LT_AND_RB                       (EFX_EFFECT_WIPE_CLASS+32)
#define EFX_EFFECT_WIPE2_C_TO_LB_AND_RT                       (EFX_EFFECT_WIPE_CLASS+33)
#define EFX_EFFECT_WIPE2_LB_AND_RB_TO_T                       (EFX_EFFECT_WIPE_CLASS+34)
#define EFX_EFFECT_WIPE2_LT_AND_RT_TO_B                       (EFX_EFFECT_WIPE_CLASS+35)
#define EFX_EFFECT_WIPE2_RT_AND_RB_TO_L                       (EFX_EFFECT_WIPE_CLASS+36)
#define EFX_EFFECT_WIPE2_LT_AND_LB_TO_R                       (EFX_EFFECT_WIPE_CLASS+37)
#define EFX_EFFECT_WIPE2_L_T_TO_B_AND_R_B_TO_T                (EFX_EFFECT_WIPE_CLASS+38)
#define EFX_EFFECT_WIPE2_T_L_TO_R_AND_B_R_TO_L                (EFX_EFFECT_WIPE_CLASS+39)
#define EFX_EFFECT_WIPE4_CORNERS_OUT                          (EFX_EFFECT_WIPE_CLASS+40)
#define EFX_EFFECT_WIPE4_C_TO_L_AND_R_THEN_T_AND_B            (EFX_EFFECT_WIPE_CLASS+41)
#define EFX_EFFECT_WIPE4_T_AND_B_THEN_L_AND_R_TO_C            (EFX_EFFECT_WIPE_CLASS+42)
#define EFX_EFFECT_WIPE4_L_L_L_L                              (EFX_EFFECT_WIPE_CLASS+43)
#define EFX_EFFECT_WIPE4_L_L_L_R                              (EFX_EFFECT_WIPE_CLASS+44)
#define EFX_EFFECT_WIPE4_L_L_L_T                              (EFX_EFFECT_WIPE_CLASS+45)
#define EFX_EFFECT_WIPE4_L_L_L_B                              (EFX_EFFECT_WIPE_CLASS+46)
#define EFX_EFFECT_WIPE4_L_L_R_L                              (EFX_EFFECT_WIPE_CLASS+47)
#define EFX_EFFECT_WIPE4_L_L_R_R                              (EFX_EFFECT_WIPE_CLASS+48)
#define EFX_EFFECT_WIPE4_L_L_R_T                              (EFX_EFFECT_WIPE_CLASS+49)
#define EFX_EFFECT_WIPE4_L_L_R_B                              (EFX_EFFECT_WIPE_CLASS+50)
#define EFX_EFFECT_WIPE4_L_L_T_L                              (EFX_EFFECT_WIPE_CLASS+51)
#define EFX_EFFECT_WIPE4_L_L_T_R                              (EFX_EFFECT_WIPE_CLASS+52)
#define EFX_EFFECT_WIPE4_L_L_T_T                              (EFX_EFFECT_WIPE_CLASS+53)
#define EFX_EFFECT_WIPE4_L_L_T_B                              (EFX_EFFECT_WIPE_CLASS+54)
#define EFX_EFFECT_WIPE4_L_L_B_L                              (EFX_EFFECT_WIPE_CLASS+55)
#define EFX_EFFECT_WIPE4_L_L_B_R                              (EFX_EFFECT_WIPE_CLASS+56)
#define EFX_EFFECT_WIPE4_L_L_B_T                              (EFX_EFFECT_WIPE_CLASS+57)
#define EFX_EFFECT_WIPE4_L_L_B_B                              (EFX_EFFECT_WIPE_CLASS+58)
#define EFX_EFFECT_WIPE4_L_R_L_L                              (EFX_EFFECT_WIPE_CLASS+59)
#define EFX_EFFECT_WIPE4_L_R_L_R                              (EFX_EFFECT_WIPE_CLASS+60)
#define EFX_EFFECT_WIPE4_L_R_L_T                              (EFX_EFFECT_WIPE_CLASS+61)
#define EFX_EFFECT_WIPE4_L_R_L_B                              (EFX_EFFECT_WIPE_CLASS+62)
#define EFX_EFFECT_WIPE4_L_R_R_L                              (EFX_EFFECT_WIPE_CLASS+63)
#define EFX_EFFECT_WIPE4_L_R_R_R                              (EFX_EFFECT_WIPE_CLASS+64)
#define EFX_EFFECT_WIPE4_L_R_R_T                              (EFX_EFFECT_WIPE_CLASS+65)
#define EFX_EFFECT_WIPE4_L_R_R_B                              (EFX_EFFECT_WIPE_CLASS+66)
#define EFX_EFFECT_WIPE4_L_R_T_L                              (EFX_EFFECT_WIPE_CLASS+67)
#define EFX_EFFECT_WIPE4_L_R_T_R                              (EFX_EFFECT_WIPE_CLASS+68)
#define EFX_EFFECT_WIPE4_L_R_T_T                              (EFX_EFFECT_WIPE_CLASS+69)
#define EFX_EFFECT_WIPE4_L_R_T_B                              (EFX_EFFECT_WIPE_CLASS+70)
#define EFX_EFFECT_WIPE4_L_R_B_L                              (EFX_EFFECT_WIPE_CLASS+71)
#define EFX_EFFECT_WIPE4_L_R_B_R                              (EFX_EFFECT_WIPE_CLASS+72)
#define EFX_EFFECT_WIPE4_L_R_B_T                              (EFX_EFFECT_WIPE_CLASS+73)
#define EFX_EFFECT_WIPE4_L_R_B_B                              (EFX_EFFECT_WIPE_CLASS+74)
#define EFX_EFFECT_WIPE4_L_T_L_L                              (EFX_EFFECT_WIPE_CLASS+75)
#define EFX_EFFECT_WIPE4_L_T_L_R                              (EFX_EFFECT_WIPE_CLASS+76)
#define EFX_EFFECT_WIPE4_L_T_L_T                              (EFX_EFFECT_WIPE_CLASS+77)
#define EFX_EFFECT_WIPE4_L_T_L_B                              (EFX_EFFECT_WIPE_CLASS+78)
#define EFX_EFFECT_WIPE4_L_T_R_L                              (EFX_EFFECT_WIPE_CLASS+79)
#define EFX_EFFECT_WIPE4_L_T_R_R                              (EFX_EFFECT_WIPE_CLASS+80)
#define EFX_EFFECT_WIPE4_L_T_R_T                              (EFX_EFFECT_WIPE_CLASS+81)
#define EFX_EFFECT_WIPE4_L_T_R_B                              (EFX_EFFECT_WIPE_CLASS+82)
#define EFX_EFFECT_WIPE4_L_T_T_L                              (EFX_EFFECT_WIPE_CLASS+83)
#define EFX_EFFECT_WIPE4_L_T_T_R                              (EFX_EFFECT_WIPE_CLASS+84)
#define EFX_EFFECT_WIPE4_L_T_T_T                              (EFX_EFFECT_WIPE_CLASS+85)
#define EFX_EFFECT_WIPE4_L_T_T_B                              (EFX_EFFECT_WIPE_CLASS+86)
#define EFX_EFFECT_WIPE4_L_T_B_L                              (EFX_EFFECT_WIPE_CLASS+87)
#define EFX_EFFECT_WIPE4_L_T_B_R                              (EFX_EFFECT_WIPE_CLASS+88)
#define EFX_EFFECT_WIPE4_L_T_B_T                              (EFX_EFFECT_WIPE_CLASS+89)
#define EFX_EFFECT_WIPE4_L_T_B_B                              (EFX_EFFECT_WIPE_CLASS+90)
#define EFX_EFFECT_WIPE4_L_B_L_L                              (EFX_EFFECT_WIPE_CLASS+91)
#define EFX_EFFECT_WIPE4_L_B_L_R                              (EFX_EFFECT_WIPE_CLASS+92)
#define EFX_EFFECT_WIPE4_L_B_L_T                              (EFX_EFFECT_WIPE_CLASS+93)
#define EFX_EFFECT_WIPE4_L_B_L_B                              (EFX_EFFECT_WIPE_CLASS+94)
#define EFX_EFFECT_WIPE4_L_B_R_L                              (EFX_EFFECT_WIPE_CLASS+95)
#define EFX_EFFECT_WIPE4_L_B_R_R                              (EFX_EFFECT_WIPE_CLASS+96)
#define EFX_EFFECT_WIPE4_L_B_R_T                              (EFX_EFFECT_WIPE_CLASS+97)
#define EFX_EFFECT_WIPE4_L_B_R_B                              (EFX_EFFECT_WIPE_CLASS+98)
#define EFX_EFFECT_WIPE4_L_B_T_L                              (EFX_EFFECT_WIPE_CLASS+99)
#define EFX_EFFECT_WIPE4_L_B_T_R                              (EFX_EFFECT_WIPE_CLASS+100)
#define EFX_EFFECT_WIPE4_L_B_T_T                              (EFX_EFFECT_WIPE_CLASS+101)
#define EFX_EFFECT_WIPE4_L_B_T_B                              (EFX_EFFECT_WIPE_CLASS+102)
#define EFX_EFFECT_WIPE4_L_B_B_L                              (EFX_EFFECT_WIPE_CLASS+103)
#define EFX_EFFECT_WIPE4_L_B_B_R                              (EFX_EFFECT_WIPE_CLASS+104)
#define EFX_EFFECT_WIPE4_L_B_B_T                              (EFX_EFFECT_WIPE_CLASS+105)
#define EFX_EFFECT_WIPE4_L_B_B_B                              (EFX_EFFECT_WIPE_CLASS+106)
#define EFX_EFFECT_WIPE4_R_L_L_L                              (EFX_EFFECT_WIPE_CLASS+107)
#define EFX_EFFECT_WIPE4_R_L_L_R                              (EFX_EFFECT_WIPE_CLASS+108)
#define EFX_EFFECT_WIPE4_R_L_L_T                              (EFX_EFFECT_WIPE_CLASS+109)
#define EFX_EFFECT_WIPE4_R_L_L_B                              (EFX_EFFECT_WIPE_CLASS+110)
#define EFX_EFFECT_WIPE4_R_L_R_L                              (EFX_EFFECT_WIPE_CLASS+111)
#define EFX_EFFECT_WIPE4_R_L_R_R                              (EFX_EFFECT_WIPE_CLASS+112)
#define EFX_EFFECT_WIPE4_R_L_R_T                              (EFX_EFFECT_WIPE_CLASS+113)
#define EFX_EFFECT_WIPE4_R_L_R_B                              (EFX_EFFECT_WIPE_CLASS+114)
#define EFX_EFFECT_WIPE4_R_L_T_L                              (EFX_EFFECT_WIPE_CLASS+115)
#define EFX_EFFECT_WIPE4_R_L_T_R                              (EFX_EFFECT_WIPE_CLASS+116)
#define EFX_EFFECT_WIPE4_R_L_T_T                              (EFX_EFFECT_WIPE_CLASS+117)
#define EFX_EFFECT_WIPE4_R_L_T_B                              (EFX_EFFECT_WIPE_CLASS+118)
#define EFX_EFFECT_WIPE4_R_L_B_L                              (EFX_EFFECT_WIPE_CLASS+119)
#define EFX_EFFECT_WIPE4_R_L_B_R                              (EFX_EFFECT_WIPE_CLASS+120)
#define EFX_EFFECT_WIPE4_R_L_B_T                              (EFX_EFFECT_WIPE_CLASS+121)
#define EFX_EFFECT_WIPE4_R_L_B_B                              (EFX_EFFECT_WIPE_CLASS+122)
#define EFX_EFFECT_WIPE4_R_R_L_L                              (EFX_EFFECT_WIPE_CLASS+123)
#define EFX_EFFECT_WIPE4_R_R_L_R                              (EFX_EFFECT_WIPE_CLASS+124)
#define EFX_EFFECT_WIPE4_R_R_L_T                              (EFX_EFFECT_WIPE_CLASS+125)
#define EFX_EFFECT_WIPE4_R_R_L_B                              (EFX_EFFECT_WIPE_CLASS+126)
#define EFX_EFFECT_WIPE4_R_R_R_L                              (EFX_EFFECT_WIPE_CLASS+127)
#define EFX_EFFECT_WIPE4_R_R_R_R                              (EFX_EFFECT_WIPE_CLASS+128)
#define EFX_EFFECT_WIPE4_R_R_R_T                              (EFX_EFFECT_WIPE_CLASS+129)
#define EFX_EFFECT_WIPE4_R_R_R_B                              (EFX_EFFECT_WIPE_CLASS+130)
#define EFX_EFFECT_WIPE4_R_R_T_L                              (EFX_EFFECT_WIPE_CLASS+131)
#define EFX_EFFECT_WIPE4_R_R_T_R                              (EFX_EFFECT_WIPE_CLASS+132)
#define EFX_EFFECT_WIPE4_R_R_T_T                              (EFX_EFFECT_WIPE_CLASS+133)
#define EFX_EFFECT_WIPE4_R_R_T_B                              (EFX_EFFECT_WIPE_CLASS+134)
#define EFX_EFFECT_WIPE4_R_R_B_L                              (EFX_EFFECT_WIPE_CLASS+135)
#define EFX_EFFECT_WIPE4_R_R_B_R                              (EFX_EFFECT_WIPE_CLASS+136)
#define EFX_EFFECT_WIPE4_R_R_B_T                              (EFX_EFFECT_WIPE_CLASS+137)
#define EFX_EFFECT_WIPE4_R_R_B_B                              (EFX_EFFECT_WIPE_CLASS+138)
#define EFX_EFFECT_WIPE4_R_T_L_L                              (EFX_EFFECT_WIPE_CLASS+139)
#define EFX_EFFECT_WIPE4_R_T_L_R                              (EFX_EFFECT_WIPE_CLASS+140)
#define EFX_EFFECT_WIPE4_R_T_L_T                              (EFX_EFFECT_WIPE_CLASS+141)
#define EFX_EFFECT_WIPE4_R_T_L_B                              (EFX_EFFECT_WIPE_CLASS+142)
#define EFX_EFFECT_WIPE4_R_T_R_L                              (EFX_EFFECT_WIPE_CLASS+143)
#define EFX_EFFECT_WIPE4_R_T_R_R                              (EFX_EFFECT_WIPE_CLASS+144)
#define EFX_EFFECT_WIPE4_R_T_R_T                              (EFX_EFFECT_WIPE_CLASS+145)
#define EFX_EFFECT_WIPE4_R_T_R_B                              (EFX_EFFECT_WIPE_CLASS+146)
#define EFX_EFFECT_WIPE4_R_T_T_L                              (EFX_EFFECT_WIPE_CLASS+147)
#define EFX_EFFECT_WIPE4_R_T_T_R                              (EFX_EFFECT_WIPE_CLASS+148)
#define EFX_EFFECT_WIPE4_R_T_T_T                              (EFX_EFFECT_WIPE_CLASS+149)
#define EFX_EFFECT_WIPE4_R_T_T_B                              (EFX_EFFECT_WIPE_CLASS+150)
#define EFX_EFFECT_WIPE4_R_T_B_L                              (EFX_EFFECT_WIPE_CLASS+151)
#define EFX_EFFECT_WIPE4_R_T_B_R                              (EFX_EFFECT_WIPE_CLASS+152)
#define EFX_EFFECT_WIPE4_R_T_B_T                              (EFX_EFFECT_WIPE_CLASS+153)
#define EFX_EFFECT_WIPE4_R_T_B_B                              (EFX_EFFECT_WIPE_CLASS+154)
#define EFX_EFFECT_WIPE4_R_B_L_L                              (EFX_EFFECT_WIPE_CLASS+155)
#define EFX_EFFECT_WIPE4_R_B_L_R                              (EFX_EFFECT_WIPE_CLASS+156)
#define EFX_EFFECT_WIPE4_R_B_L_T                              (EFX_EFFECT_WIPE_CLASS+157)
#define EFX_EFFECT_WIPE4_R_B_L_B                              (EFX_EFFECT_WIPE_CLASS+158)
#define EFX_EFFECT_WIPE4_R_B_R_L                              (EFX_EFFECT_WIPE_CLASS+159)
#define EFX_EFFECT_WIPE4_R_B_R_R                              (EFX_EFFECT_WIPE_CLASS+160)
#define EFX_EFFECT_WIPE4_R_B_R_T                              (EFX_EFFECT_WIPE_CLASS+161)
#define EFX_EFFECT_WIPE4_R_B_R_B                              (EFX_EFFECT_WIPE_CLASS+162)
#define EFX_EFFECT_WIPE4_R_B_T_L                              (EFX_EFFECT_WIPE_CLASS+163)
#define EFX_EFFECT_WIPE4_R_B_T_R                              (EFX_EFFECT_WIPE_CLASS+164)
#define EFX_EFFECT_WIPE4_R_B_T_T                              (EFX_EFFECT_WIPE_CLASS+165)
#define EFX_EFFECT_WIPE4_R_B_T_B                              (EFX_EFFECT_WIPE_CLASS+166)
#define EFX_EFFECT_WIPE4_R_B_B_L                              (EFX_EFFECT_WIPE_CLASS+167)
#define EFX_EFFECT_WIPE4_R_B_B_R                              (EFX_EFFECT_WIPE_CLASS+168)
#define EFX_EFFECT_WIPE4_R_B_B_T                              (EFX_EFFECT_WIPE_CLASS+169)
#define EFX_EFFECT_WIPE4_R_B_B_B                              (EFX_EFFECT_WIPE_CLASS+170)
#define EFX_EFFECT_WIPE4_T_L_L_L                              (EFX_EFFECT_WIPE_CLASS+171)
#define EFX_EFFECT_WIPE4_T_L_L_R                              (EFX_EFFECT_WIPE_CLASS+172)
#define EFX_EFFECT_WIPE4_T_L_L_T                              (EFX_EFFECT_WIPE_CLASS+173)
#define EFX_EFFECT_WIPE4_T_L_L_B                              (EFX_EFFECT_WIPE_CLASS+174)
#define EFX_EFFECT_WIPE4_T_L_R_L                              (EFX_EFFECT_WIPE_CLASS+175)
#define EFX_EFFECT_WIPE4_T_L_R_R                              (EFX_EFFECT_WIPE_CLASS+176)
#define EFX_EFFECT_WIPE4_T_L_R_T                              (EFX_EFFECT_WIPE_CLASS+177)
#define EFX_EFFECT_WIPE4_T_L_R_B                              (EFX_EFFECT_WIPE_CLASS+178)
#define EFX_EFFECT_WIPE4_T_L_T_L                              (EFX_EFFECT_WIPE_CLASS+179)
#define EFX_EFFECT_WIPE4_T_L_T_R                              (EFX_EFFECT_WIPE_CLASS+180)
#define EFX_EFFECT_WIPE4_T_L_T_T                              (EFX_EFFECT_WIPE_CLASS+181)
#define EFX_EFFECT_WIPE4_T_L_T_B                              (EFX_EFFECT_WIPE_CLASS+182)
#define EFX_EFFECT_WIPE4_T_L_B_L                              (EFX_EFFECT_WIPE_CLASS+183)
#define EFX_EFFECT_WIPE4_T_L_B_R                              (EFX_EFFECT_WIPE_CLASS+184)
#define EFX_EFFECT_WIPE4_T_L_B_T                              (EFX_EFFECT_WIPE_CLASS+185)
#define EFX_EFFECT_WIPE4_T_L_B_B                              (EFX_EFFECT_WIPE_CLASS+186)
#define EFX_EFFECT_WIPE4_T_R_L_L                              (EFX_EFFECT_WIPE_CLASS+187)
#define EFX_EFFECT_WIPE4_T_R_L_R                              (EFX_EFFECT_WIPE_CLASS+188)
#define EFX_EFFECT_WIPE4_T_R_L_T                              (EFX_EFFECT_WIPE_CLASS+189)
#define EFX_EFFECT_WIPE4_T_R_L_B                              (EFX_EFFECT_WIPE_CLASS+190)
#define EFX_EFFECT_WIPE4_T_R_R_L                              (EFX_EFFECT_WIPE_CLASS+191)
#define EFX_EFFECT_WIPE4_T_R_R_R                              (EFX_EFFECT_WIPE_CLASS+192)
#define EFX_EFFECT_WIPE4_T_R_R_T                              (EFX_EFFECT_WIPE_CLASS+193)
#define EFX_EFFECT_WIPE4_T_R_R_B                              (EFX_EFFECT_WIPE_CLASS+194)
#define EFX_EFFECT_WIPE4_T_R_T_L                              (EFX_EFFECT_WIPE_CLASS+195)
#define EFX_EFFECT_WIPE4_T_R_T_R                              (EFX_EFFECT_WIPE_CLASS+196)
#define EFX_EFFECT_WIPE4_T_R_T_T                              (EFX_EFFECT_WIPE_CLASS+197)
#define EFX_EFFECT_WIPE4_T_R_T_B                              (EFX_EFFECT_WIPE_CLASS+198)
#define EFX_EFFECT_WIPE4_T_R_B_L                              (EFX_EFFECT_WIPE_CLASS+199)
#define EFX_EFFECT_WIPE4_T_R_B_R                              (EFX_EFFECT_WIPE_CLASS+200)
#define EFX_EFFECT_WIPE4_T_R_B_T                              (EFX_EFFECT_WIPE_CLASS+201)
#define EFX_EFFECT_WIPE4_T_R_B_B                              (EFX_EFFECT_WIPE_CLASS+202)
#define EFX_EFFECT_WIPE4_T_T_L_L                              (EFX_EFFECT_WIPE_CLASS+203)
#define EFX_EFFECT_WIPE4_T_T_L_R                              (EFX_EFFECT_WIPE_CLASS+204)
#define EFX_EFFECT_WIPE4_T_T_L_T                              (EFX_EFFECT_WIPE_CLASS+205)
#define EFX_EFFECT_WIPE4_T_T_L_B                              (EFX_EFFECT_WIPE_CLASS+206)
#define EFX_EFFECT_WIPE4_T_T_R_L                              (EFX_EFFECT_WIPE_CLASS+207)
#define EFX_EFFECT_WIPE4_T_T_R_R                              (EFX_EFFECT_WIPE_CLASS+208)
#define EFX_EFFECT_WIPE4_T_T_R_T                              (EFX_EFFECT_WIPE_CLASS+209)
#define EFX_EFFECT_WIPE4_T_T_R_B                              (EFX_EFFECT_WIPE_CLASS+210)
#define EFX_EFFECT_WIPE4_T_T_T_L                              (EFX_EFFECT_WIPE_CLASS+211)
#define EFX_EFFECT_WIPE4_T_T_T_R                              (EFX_EFFECT_WIPE_CLASS+212)
#define EFX_EFFECT_WIPE4_T_T_T_T                              (EFX_EFFECT_WIPE_CLASS+213)
#define EFX_EFFECT_WIPE4_T_T_T_B                              (EFX_EFFECT_WIPE_CLASS+214)
#define EFX_EFFECT_WIPE4_T_T_B_L                              (EFX_EFFECT_WIPE_CLASS+215)
#define EFX_EFFECT_WIPE4_T_T_B_R                              (EFX_EFFECT_WIPE_CLASS+216)
#define EFX_EFFECT_WIPE4_T_T_B_T                              (EFX_EFFECT_WIPE_CLASS+217)
#define EFX_EFFECT_WIPE4_T_T_B_B                              (EFX_EFFECT_WIPE_CLASS+218)
#define EFX_EFFECT_WIPE4_T_B_L_L                              (EFX_EFFECT_WIPE_CLASS+219)
#define EFX_EFFECT_WIPE4_T_B_L_R                              (EFX_EFFECT_WIPE_CLASS+220)
#define EFX_EFFECT_WIPE4_T_B_L_T                              (EFX_EFFECT_WIPE_CLASS+221)
#define EFX_EFFECT_WIPE4_T_B_L_B                              (EFX_EFFECT_WIPE_CLASS+222)
#define EFX_EFFECT_WIPE4_T_B_R_L                              (EFX_EFFECT_WIPE_CLASS+223)
#define EFX_EFFECT_WIPE4_T_B_R_R                              (EFX_EFFECT_WIPE_CLASS+224)
#define EFX_EFFECT_WIPE4_T_B_R_T                              (EFX_EFFECT_WIPE_CLASS+225)
#define EFX_EFFECT_WIPE4_T_B_R_B                              (EFX_EFFECT_WIPE_CLASS+226)
#define EFX_EFFECT_WIPE4_T_B_T_L                              (EFX_EFFECT_WIPE_CLASS+227)
#define EFX_EFFECT_WIPE4_T_B_T_R                              (EFX_EFFECT_WIPE_CLASS+228)
#define EFX_EFFECT_WIPE4_T_B_T_T                              (EFX_EFFECT_WIPE_CLASS+229)
#define EFX_EFFECT_WIPE4_T_B_T_B                              (EFX_EFFECT_WIPE_CLASS+230)
#define EFX_EFFECT_WIPE4_T_B_B_L                              (EFX_EFFECT_WIPE_CLASS+231)
#define EFX_EFFECT_WIPE4_T_B_B_R                              (EFX_EFFECT_WIPE_CLASS+232)
#define EFX_EFFECT_WIPE4_T_B_B_T                              (EFX_EFFECT_WIPE_CLASS+233)
#define EFX_EFFECT_WIPE4_T_B_B_B                              (EFX_EFFECT_WIPE_CLASS+234)
#define EFX_EFFECT_WIPE4_B_L_L_L                              (EFX_EFFECT_WIPE_CLASS+235)
#define EFX_EFFECT_WIPE4_B_L_L_R                              (EFX_EFFECT_WIPE_CLASS+236)
#define EFX_EFFECT_WIPE4_B_L_L_T                              (EFX_EFFECT_WIPE_CLASS+237)
#define EFX_EFFECT_WIPE4_B_L_L_B                              (EFX_EFFECT_WIPE_CLASS+238)
#define EFX_EFFECT_WIPE4_B_L_R_L                              (EFX_EFFECT_WIPE_CLASS+239)
#define EFX_EFFECT_WIPE4_B_L_R_R                              (EFX_EFFECT_WIPE_CLASS+240)
#define EFX_EFFECT_WIPE4_B_L_R_T                              (EFX_EFFECT_WIPE_CLASS+241)
#define EFX_EFFECT_WIPE4_B_L_R_B                              (EFX_EFFECT_WIPE_CLASS+242)
#define EFX_EFFECT_WIPE4_B_L_T_L                              (EFX_EFFECT_WIPE_CLASS+243)
#define EFX_EFFECT_WIPE4_B_L_T_R                              (EFX_EFFECT_WIPE_CLASS+244)
#define EFX_EFFECT_WIPE4_B_L_T_T                              (EFX_EFFECT_WIPE_CLASS+245)
#define EFX_EFFECT_WIPE4_B_L_T_B                              (EFX_EFFECT_WIPE_CLASS+246)
#define EFX_EFFECT_WIPE4_B_L_B_L                              (EFX_EFFECT_WIPE_CLASS+247)
#define EFX_EFFECT_WIPE4_B_L_B_R                              (EFX_EFFECT_WIPE_CLASS+248)
#define EFX_EFFECT_WIPE4_B_L_B_T                              (EFX_EFFECT_WIPE_CLASS+249)
#define EFX_EFFECT_WIPE4_B_L_B_B                              (EFX_EFFECT_WIPE_CLASS+250)
#define EFX_EFFECT_WIPE4_B_R_L_L                              (EFX_EFFECT_WIPE_CLASS+251)
#define EFX_EFFECT_WIPE4_B_R_L_R                              (EFX_EFFECT_WIPE_CLASS+252)
#define EFX_EFFECT_WIPE4_B_R_L_T                              (EFX_EFFECT_WIPE_CLASS+253)
#define EFX_EFFECT_WIPE4_B_R_L_B                              (EFX_EFFECT_WIPE_CLASS+254)
#define EFX_EFFECT_WIPE4_B_R_R_L                              (EFX_EFFECT_WIPE_CLASS+255)
#define EFX_EFFECT_WIPE4_B_R_R_R                              (EFX_EFFECT_WIPE_CLASS+256)
#define EFX_EFFECT_WIPE4_B_R_R_T                              (EFX_EFFECT_WIPE_CLASS+257)
#define EFX_EFFECT_WIPE4_B_R_R_B                              (EFX_EFFECT_WIPE_CLASS+258)
#define EFX_EFFECT_WIPE4_B_R_T_L                              (EFX_EFFECT_WIPE_CLASS+259)
#define EFX_EFFECT_WIPE4_B_R_T_R                              (EFX_EFFECT_WIPE_CLASS+260)
#define EFX_EFFECT_WIPE4_B_R_T_T                              (EFX_EFFECT_WIPE_CLASS+261)
#define EFX_EFFECT_WIPE4_B_R_T_B                              (EFX_EFFECT_WIPE_CLASS+262)
#define EFX_EFFECT_WIPE4_B_R_B_L                              (EFX_EFFECT_WIPE_CLASS+263)
#define EFX_EFFECT_WIPE4_B_R_B_R                              (EFX_EFFECT_WIPE_CLASS+264)
#define EFX_EFFECT_WIPE4_B_R_B_T                              (EFX_EFFECT_WIPE_CLASS+265)
#define EFX_EFFECT_WIPE4_B_R_B_B                              (EFX_EFFECT_WIPE_CLASS+266)
#define EFX_EFFECT_WIPE4_B_T_L_L                              (EFX_EFFECT_WIPE_CLASS+267)
#define EFX_EFFECT_WIPE4_B_T_L_R                              (EFX_EFFECT_WIPE_CLASS+268)
#define EFX_EFFECT_WIPE4_B_T_L_T                              (EFX_EFFECT_WIPE_CLASS+269)
#define EFX_EFFECT_WIPE4_B_T_L_B                              (EFX_EFFECT_WIPE_CLASS+270)
#define EFX_EFFECT_WIPE4_B_T_R_L                              (EFX_EFFECT_WIPE_CLASS+271)
#define EFX_EFFECT_WIPE4_B_T_R_R                              (EFX_EFFECT_WIPE_CLASS+272)
#define EFX_EFFECT_WIPE4_B_T_R_T                              (EFX_EFFECT_WIPE_CLASS+273)
#define EFX_EFFECT_WIPE4_B_T_R_B                              (EFX_EFFECT_WIPE_CLASS+274)
#define EFX_EFFECT_WIPE4_B_T_T_L                              (EFX_EFFECT_WIPE_CLASS+275)
#define EFX_EFFECT_WIPE4_B_T_T_R                              (EFX_EFFECT_WIPE_CLASS+276)
#define EFX_EFFECT_WIPE4_B_T_T_T                              (EFX_EFFECT_WIPE_CLASS+277)
#define EFX_EFFECT_WIPE4_B_T_T_B                              (EFX_EFFECT_WIPE_CLASS+278)
#define EFX_EFFECT_WIPE4_B_T_B_L                              (EFX_EFFECT_WIPE_CLASS+279)
#define EFX_EFFECT_WIPE4_B_T_B_R                              (EFX_EFFECT_WIPE_CLASS+280)
#define EFX_EFFECT_WIPE4_B_T_B_T                              (EFX_EFFECT_WIPE_CLASS+281)
#define EFX_EFFECT_WIPE4_B_T_B_B                              (EFX_EFFECT_WIPE_CLASS+282)
#define EFX_EFFECT_WIPE4_B_B_L_L                              (EFX_EFFECT_WIPE_CLASS+283)
#define EFX_EFFECT_WIPE4_B_B_L_R                              (EFX_EFFECT_WIPE_CLASS+284)
#define EFX_EFFECT_WIPE4_B_B_L_T                              (EFX_EFFECT_WIPE_CLASS+285)
#define EFX_EFFECT_WIPE4_B_B_L_B                              (EFX_EFFECT_WIPE_CLASS+286)
#define EFX_EFFECT_WIPE4_B_B_R_L                              (EFX_EFFECT_WIPE_CLASS+287)
#define EFX_EFFECT_WIPE4_B_B_R_R                              (EFX_EFFECT_WIPE_CLASS+288)
#define EFX_EFFECT_WIPE4_B_B_R_T                              (EFX_EFFECT_WIPE_CLASS+289)
#define EFX_EFFECT_WIPE4_B_B_R_B                              (EFX_EFFECT_WIPE_CLASS+290)
#define EFX_EFFECT_WIPE4_B_B_T_L                              (EFX_EFFECT_WIPE_CLASS+291)
#define EFX_EFFECT_WIPE4_B_B_T_R                              (EFX_EFFECT_WIPE_CLASS+292)
#define EFX_EFFECT_WIPE4_B_B_T_T                              (EFX_EFFECT_WIPE_CLASS+293)
#define EFX_EFFECT_WIPE4_B_B_T_B                              (EFX_EFFECT_WIPE_CLASS+294)
#define EFX_EFFECT_WIPE4_B_B_B_L                              (EFX_EFFECT_WIPE_CLASS+295)
#define EFX_EFFECT_WIPE4_B_B_B_R                              (EFX_EFFECT_WIPE_CLASS+296)
#define EFX_EFFECT_WIPE4_B_B_B_T                              (EFX_EFFECT_WIPE_CLASS+297)
#define EFX_EFFECT_WIPE4_B_B_B_B                              (EFX_EFFECT_WIPE_CLASS+298)
#define EFX_EFFECT_WIPE4_LT_LT_LT_LT                          (EFX_EFFECT_WIPE_CLASS+299)
#define EFX_EFFECT_WIPE4_LT_LT_LT_LB                          (EFX_EFFECT_WIPE_CLASS+300)
#define EFX_EFFECT_WIPE4_LT_LT_LT_RT                          (EFX_EFFECT_WIPE_CLASS+301)
#define EFX_EFFECT_WIPE4_LT_LT_LT_RB                          (EFX_EFFECT_WIPE_CLASS+302)
#define EFX_EFFECT_WIPE4_LT_LT_LB_LT                          (EFX_EFFECT_WIPE_CLASS+303)
#define EFX_EFFECT_WIPE4_LT_LT_LB_LB                          (EFX_EFFECT_WIPE_CLASS+304)
#define EFX_EFFECT_WIPE4_LT_LT_LB_RT                          (EFX_EFFECT_WIPE_CLASS+305)
#define EFX_EFFECT_WIPE4_LT_LT_LB_RB                          (EFX_EFFECT_WIPE_CLASS+306)
#define EFX_EFFECT_WIPE4_LT_LT_RT_LT                          (EFX_EFFECT_WIPE_CLASS+307)
#define EFX_EFFECT_WIPE4_LT_LT_RT_LB                          (EFX_EFFECT_WIPE_CLASS+308)
#define EFX_EFFECT_WIPE4_LT_LT_RT_RT                          (EFX_EFFECT_WIPE_CLASS+309)
#define EFX_EFFECT_WIPE4_LT_LT_RT_RB                          (EFX_EFFECT_WIPE_CLASS+310)
#define EFX_EFFECT_WIPE4_LT_LT_RB_LT                          (EFX_EFFECT_WIPE_CLASS+311)
#define EFX_EFFECT_WIPE4_LT_LT_RB_LB                          (EFX_EFFECT_WIPE_CLASS+312)
#define EFX_EFFECT_WIPE4_LT_LT_RB_RT                          (EFX_EFFECT_WIPE_CLASS+313)
#define EFX_EFFECT_WIPE4_LT_LT_RB_RB                          (EFX_EFFECT_WIPE_CLASS+314)
#define EFX_EFFECT_WIPE4_LT_LB_LT_LT                          (EFX_EFFECT_WIPE_CLASS+315)
#define EFX_EFFECT_WIPE4_LT_LB_LT_LB                          (EFX_EFFECT_WIPE_CLASS+316)
#define EFX_EFFECT_WIPE4_LT_LB_LT_RT                          (EFX_EFFECT_WIPE_CLASS+317)
#define EFX_EFFECT_WIPE4_LT_LB_LT_RB                          (EFX_EFFECT_WIPE_CLASS+318)
#define EFX_EFFECT_WIPE4_LT_LB_LB_LT                          (EFX_EFFECT_WIPE_CLASS+319)
#define EFX_EFFECT_WIPE4_LT_LB_LB_LB                          (EFX_EFFECT_WIPE_CLASS+320)
#define EFX_EFFECT_WIPE4_LT_LB_LB_RT                          (EFX_EFFECT_WIPE_CLASS+321)
#define EFX_EFFECT_WIPE4_LT_LB_LB_RB                          (EFX_EFFECT_WIPE_CLASS+322)
#define EFX_EFFECT_WIPE4_LT_LB_RT_LT                          (EFX_EFFECT_WIPE_CLASS+323)
#define EFX_EFFECT_WIPE4_LT_LB_RT_LB                          (EFX_EFFECT_WIPE_CLASS+324)
#define EFX_EFFECT_WIPE4_LT_LB_RT_RT                          (EFX_EFFECT_WIPE_CLASS+325)
#define EFX_EFFECT_WIPE4_LT_LB_RT_RB                          (EFX_EFFECT_WIPE_CLASS+326)
#define EFX_EFFECT_WIPE4_LT_LB_RB_LT                          (EFX_EFFECT_WIPE_CLASS+327)
#define EFX_EFFECT_WIPE4_LT_LB_RB_LB                          (EFX_EFFECT_WIPE_CLASS+328)
#define EFX_EFFECT_WIPE4_LT_LB_RB_RT                          (EFX_EFFECT_WIPE_CLASS+329)
#define EFX_EFFECT_WIPE4_LT_LB_RB_RB                          (EFX_EFFECT_WIPE_CLASS+330)
#define EFX_EFFECT_WIPE4_LT_RT_LT_LT                          (EFX_EFFECT_WIPE_CLASS+331)
#define EFX_EFFECT_WIPE4_LT_RT_LT_LB                          (EFX_EFFECT_WIPE_CLASS+332)
#define EFX_EFFECT_WIPE4_LT_RT_LT_RT                          (EFX_EFFECT_WIPE_CLASS+333)
#define EFX_EFFECT_WIPE4_LT_RT_LT_RB                          (EFX_EFFECT_WIPE_CLASS+334)
#define EFX_EFFECT_WIPE4_LT_RT_LB_LT                          (EFX_EFFECT_WIPE_CLASS+335)
#define EFX_EFFECT_WIPE4_LT_RT_LB_LB                          (EFX_EFFECT_WIPE_CLASS+336)
#define EFX_EFFECT_WIPE4_LT_RT_LB_RT                          (EFX_EFFECT_WIPE_CLASS+337)
#define EFX_EFFECT_WIPE4_LT_RT_LB_RB                          (EFX_EFFECT_WIPE_CLASS+338)
#define EFX_EFFECT_WIPE4_LT_RT_RT_LT                          (EFX_EFFECT_WIPE_CLASS+339)
#define EFX_EFFECT_WIPE4_LT_RT_RT_LB                          (EFX_EFFECT_WIPE_CLASS+340)
#define EFX_EFFECT_WIPE4_LT_RT_RT_RT                          (EFX_EFFECT_WIPE_CLASS+341)
#define EFX_EFFECT_WIPE4_LT_RT_RT_RB                          (EFX_EFFECT_WIPE_CLASS+342)
#define EFX_EFFECT_WIPE4_LT_RT_RB_LT                          (EFX_EFFECT_WIPE_CLASS+343)
#define EFX_EFFECT_WIPE4_LT_RT_RB_LB                          (EFX_EFFECT_WIPE_CLASS+344)
#define EFX_EFFECT_WIPE4_LT_RT_RB_RT                          (EFX_EFFECT_WIPE_CLASS+345)
#define EFX_EFFECT_WIPE4_LT_RT_RB_RB                          (EFX_EFFECT_WIPE_CLASS+346)
#define EFX_EFFECT_WIPE4_LT_RB_LT_LT                          (EFX_EFFECT_WIPE_CLASS+347)
#define EFX_EFFECT_WIPE4_LT_RB_LT_LB                          (EFX_EFFECT_WIPE_CLASS+348)
#define EFX_EFFECT_WIPE4_LT_RB_LT_RT                          (EFX_EFFECT_WIPE_CLASS+349)
#define EFX_EFFECT_WIPE4_LT_RB_LT_RB                          (EFX_EFFECT_WIPE_CLASS+350)
#define EFX_EFFECT_WIPE4_LT_RB_LB_LT                          (EFX_EFFECT_WIPE_CLASS+351)
#define EFX_EFFECT_WIPE4_LT_RB_LB_LB                          (EFX_EFFECT_WIPE_CLASS+352)
#define EFX_EFFECT_WIPE4_LT_RB_LB_RT                          (EFX_EFFECT_WIPE_CLASS+353)
#define EFX_EFFECT_WIPE4_LT_RB_LB_RB                          (EFX_EFFECT_WIPE_CLASS+354)
#define EFX_EFFECT_WIPE4_LT_RB_RT_LT                          (EFX_EFFECT_WIPE_CLASS+355)
#define EFX_EFFECT_WIPE4_LT_RB_RT_LB                          (EFX_EFFECT_WIPE_CLASS+356)
#define EFX_EFFECT_WIPE4_LT_RB_RT_RT                          (EFX_EFFECT_WIPE_CLASS+357)
#define EFX_EFFECT_WIPE4_LT_RB_RT_RB                          (EFX_EFFECT_WIPE_CLASS+358)
#define EFX_EFFECT_WIPE4_LT_RB_RB_LT                          (EFX_EFFECT_WIPE_CLASS+359)
#define EFX_EFFECT_WIPE4_LT_RB_RB_LB                          (EFX_EFFECT_WIPE_CLASS+360)
#define EFX_EFFECT_WIPE4_LT_RB_RB_RT                          (EFX_EFFECT_WIPE_CLASS+361)
#define EFX_EFFECT_WIPE4_LT_RB_RB_RB                          (EFX_EFFECT_WIPE_CLASS+362)
#define EFX_EFFECT_WIPE4_LB_LT_LT_LT                          (EFX_EFFECT_WIPE_CLASS+363)
#define EFX_EFFECT_WIPE4_LB_LT_LT_LB                          (EFX_EFFECT_WIPE_CLASS+364)
#define EFX_EFFECT_WIPE4_LB_LT_LT_RT                          (EFX_EFFECT_WIPE_CLASS+365)
#define EFX_EFFECT_WIPE4_LB_LT_LT_RB                          (EFX_EFFECT_WIPE_CLASS+366)
#define EFX_EFFECT_WIPE4_LB_LT_LB_LT                          (EFX_EFFECT_WIPE_CLASS+367)
#define EFX_EFFECT_WIPE4_LB_LT_LB_LB                          (EFX_EFFECT_WIPE_CLASS+368)
#define EFX_EFFECT_WIPE4_LB_LT_LB_RT                          (EFX_EFFECT_WIPE_CLASS+369)
#define EFX_EFFECT_WIPE4_LB_LT_LB_RB                          (EFX_EFFECT_WIPE_CLASS+370)
#define EFX_EFFECT_WIPE4_LB_LT_RT_LT                          (EFX_EFFECT_WIPE_CLASS+371)
#define EFX_EFFECT_WIPE4_LB_LT_RT_LB                          (EFX_EFFECT_WIPE_CLASS+372)
#define EFX_EFFECT_WIPE4_LB_LT_RT_RT                          (EFX_EFFECT_WIPE_CLASS+373)
#define EFX_EFFECT_WIPE4_LB_LT_RT_RB                          (EFX_EFFECT_WIPE_CLASS+374)
#define EFX_EFFECT_WIPE4_LB_LT_RB_LT                          (EFX_EFFECT_WIPE_CLASS+375)
#define EFX_EFFECT_WIPE4_LB_LT_RB_LB                          (EFX_EFFECT_WIPE_CLASS+376)
#define EFX_EFFECT_WIPE4_LB_LT_RB_RT                          (EFX_EFFECT_WIPE_CLASS+377)
#define EFX_EFFECT_WIPE4_LB_LT_RB_RB                          (EFX_EFFECT_WIPE_CLASS+378)
#define EFX_EFFECT_WIPE4_LB_LB_LT_LT                          (EFX_EFFECT_WIPE_CLASS+379)
#define EFX_EFFECT_WIPE4_LB_LB_LT_LB                          (EFX_EFFECT_WIPE_CLASS+380)
#define EFX_EFFECT_WIPE4_LB_LB_LT_RT                          (EFX_EFFECT_WIPE_CLASS+381)
#define EFX_EFFECT_WIPE4_LB_LB_LT_RB                          (EFX_EFFECT_WIPE_CLASS+382)
#define EFX_EFFECT_WIPE4_LB_LB_LB_LT                          (EFX_EFFECT_WIPE_CLASS+383)
#define EFX_EFFECT_WIPE4_LB_LB_LB_LB                          (EFX_EFFECT_WIPE_CLASS+384)
#define EFX_EFFECT_WIPE4_LB_LB_LB_RT                          (EFX_EFFECT_WIPE_CLASS+385)
#define EFX_EFFECT_WIPE4_LB_LB_LB_RB                          (EFX_EFFECT_WIPE_CLASS+386)
#define EFX_EFFECT_WIPE4_LB_LB_RT_LT                          (EFX_EFFECT_WIPE_CLASS+387)
#define EFX_EFFECT_WIPE4_LB_LB_RT_LB                          (EFX_EFFECT_WIPE_CLASS+388)
#define EFX_EFFECT_WIPE4_LB_LB_RT_RT                          (EFX_EFFECT_WIPE_CLASS+389)
#define EFX_EFFECT_WIPE4_LB_LB_RT_RB                          (EFX_EFFECT_WIPE_CLASS+390)
#define EFX_EFFECT_WIPE4_LB_LB_RB_LT                          (EFX_EFFECT_WIPE_CLASS+391)
#define EFX_EFFECT_WIPE4_LB_LB_RB_LB                          (EFX_EFFECT_WIPE_CLASS+392)
#define EFX_EFFECT_WIPE4_LB_LB_RB_RT                          (EFX_EFFECT_WIPE_CLASS+393)
#define EFX_EFFECT_WIPE4_LB_LB_RB_RB                          (EFX_EFFECT_WIPE_CLASS+394)
#define EFX_EFFECT_WIPE4_LB_RT_LT_LT                          (EFX_EFFECT_WIPE_CLASS+395)
#define EFX_EFFECT_WIPE4_LB_RT_LT_LB                          (EFX_EFFECT_WIPE_CLASS+396)
#define EFX_EFFECT_WIPE4_LB_RT_LT_RT                          (EFX_EFFECT_WIPE_CLASS+397)
#define EFX_EFFECT_WIPE4_LB_RT_LT_RB                          (EFX_EFFECT_WIPE_CLASS+398)
#define EFX_EFFECT_WIPE4_LB_RT_LB_LT                          (EFX_EFFECT_WIPE_CLASS+399)
#define EFX_EFFECT_WIPE4_LB_RT_LB_LB                          (EFX_EFFECT_WIPE_CLASS+400)
#define EFX_EFFECT_WIPE4_LB_RT_LB_RT                          (EFX_EFFECT_WIPE_CLASS+401)
#define EFX_EFFECT_WIPE4_LB_RT_LB_RB                          (EFX_EFFECT_WIPE_CLASS+402)
#define EFX_EFFECT_WIPE4_LB_RT_RT_LT                          (EFX_EFFECT_WIPE_CLASS+403)
#define EFX_EFFECT_WIPE4_LB_RT_RT_LB                          (EFX_EFFECT_WIPE_CLASS+404)
#define EFX_EFFECT_WIPE4_LB_RT_RT_RT                          (EFX_EFFECT_WIPE_CLASS+405)
#define EFX_EFFECT_WIPE4_LB_RT_RT_RB                          (EFX_EFFECT_WIPE_CLASS+406)
#define EFX_EFFECT_WIPE4_LB_RT_RB_LT                          (EFX_EFFECT_WIPE_CLASS+407)
#define EFX_EFFECT_WIPE4_LB_RT_RB_LB                          (EFX_EFFECT_WIPE_CLASS+408)
#define EFX_EFFECT_WIPE4_LB_RT_RB_RT                          (EFX_EFFECT_WIPE_CLASS+409)
#define EFX_EFFECT_WIPE4_LB_RT_RB_RB                          (EFX_EFFECT_WIPE_CLASS+410)
#define EFX_EFFECT_WIPE4_LB_RB_LT_LT                          (EFX_EFFECT_WIPE_CLASS+411)
#define EFX_EFFECT_WIPE4_LB_RB_LT_LB                          (EFX_EFFECT_WIPE_CLASS+412)
#define EFX_EFFECT_WIPE4_LB_RB_LT_RT                          (EFX_EFFECT_WIPE_CLASS+413)
#define EFX_EFFECT_WIPE4_LB_RB_LT_RB                          (EFX_EFFECT_WIPE_CLASS+414)
#define EFX_EFFECT_WIPE4_LB_RB_LB_LT                          (EFX_EFFECT_WIPE_CLASS+415)
#define EFX_EFFECT_WIPE4_LB_RB_LB_LB                          (EFX_EFFECT_WIPE_CLASS+416)
#define EFX_EFFECT_WIPE4_LB_RB_LB_RT                          (EFX_EFFECT_WIPE_CLASS+417)
#define EFX_EFFECT_WIPE4_LB_RB_LB_RB                          (EFX_EFFECT_WIPE_CLASS+418)
#define EFX_EFFECT_WIPE4_LB_RB_RT_LT                          (EFX_EFFECT_WIPE_CLASS+419)
#define EFX_EFFECT_WIPE4_LB_RB_RT_LB                          (EFX_EFFECT_WIPE_CLASS+420)
#define EFX_EFFECT_WIPE4_LB_RB_RT_RT                          (EFX_EFFECT_WIPE_CLASS+421)
#define EFX_EFFECT_WIPE4_LB_RB_RT_RB                          (EFX_EFFECT_WIPE_CLASS+422)
#define EFX_EFFECT_WIPE4_LB_RB_RB_LT                          (EFX_EFFECT_WIPE_CLASS+423)
#define EFX_EFFECT_WIPE4_LB_RB_RB_LB                          (EFX_EFFECT_WIPE_CLASS+424)
#define EFX_EFFECT_WIPE4_LB_RB_RB_RT                          (EFX_EFFECT_WIPE_CLASS+425)
#define EFX_EFFECT_WIPE4_LB_RB_RB_RB                          (EFX_EFFECT_WIPE_CLASS+426)
#define EFX_EFFECT_WIPE4_RT_LT_LT_LT                          (EFX_EFFECT_WIPE_CLASS+427)
#define EFX_EFFECT_WIPE4_RT_LT_LT_LB                          (EFX_EFFECT_WIPE_CLASS+428)
#define EFX_EFFECT_WIPE4_RT_LT_LT_RT                          (EFX_EFFECT_WIPE_CLASS+429)
#define EFX_EFFECT_WIPE4_RT_LT_LT_RB                          (EFX_EFFECT_WIPE_CLASS+430)
#define EFX_EFFECT_WIPE4_RT_LT_LB_LT                          (EFX_EFFECT_WIPE_CLASS+431)
#define EFX_EFFECT_WIPE4_RT_LT_LB_LB                          (EFX_EFFECT_WIPE_CLASS+432)
#define EFX_EFFECT_WIPE4_RT_LT_LB_RT                          (EFX_EFFECT_WIPE_CLASS+433)
#define EFX_EFFECT_WIPE4_RT_LT_LB_RB                          (EFX_EFFECT_WIPE_CLASS+434)
#define EFX_EFFECT_WIPE4_RT_LT_RT_LT                          (EFX_EFFECT_WIPE_CLASS+435)
#define EFX_EFFECT_WIPE4_RT_LT_RT_LB                          (EFX_EFFECT_WIPE_CLASS+436)
#define EFX_EFFECT_WIPE4_RT_LT_RT_RT                          (EFX_EFFECT_WIPE_CLASS+437)
#define EFX_EFFECT_WIPE4_RT_LT_RT_RB                          (EFX_EFFECT_WIPE_CLASS+438)
#define EFX_EFFECT_WIPE4_RT_LT_RB_LT                          (EFX_EFFECT_WIPE_CLASS+439)
#define EFX_EFFECT_WIPE4_RT_LT_RB_LB                          (EFX_EFFECT_WIPE_CLASS+440)
#define EFX_EFFECT_WIPE4_RT_LT_RB_RT                          (EFX_EFFECT_WIPE_CLASS+441)
#define EFX_EFFECT_WIPE4_RT_LT_RB_RB                          (EFX_EFFECT_WIPE_CLASS+442)
#define EFX_EFFECT_WIPE4_RT_LB_LT_LT                          (EFX_EFFECT_WIPE_CLASS+443)
#define EFX_EFFECT_WIPE4_RT_LB_LT_LB                          (EFX_EFFECT_WIPE_CLASS+444)
#define EFX_EFFECT_WIPE4_RT_LB_LT_RT                          (EFX_EFFECT_WIPE_CLASS+445)
#define EFX_EFFECT_WIPE4_RT_LB_LT_RB                          (EFX_EFFECT_WIPE_CLASS+446)
#define EFX_EFFECT_WIPE4_RT_LB_LB_LT                          (EFX_EFFECT_WIPE_CLASS+447)
#define EFX_EFFECT_WIPE4_RT_LB_LB_LB                          (EFX_EFFECT_WIPE_CLASS+448)
#define EFX_EFFECT_WIPE4_RT_LB_LB_RT                          (EFX_EFFECT_WIPE_CLASS+449)
#define EFX_EFFECT_WIPE4_RT_LB_LB_RB                          (EFX_EFFECT_WIPE_CLASS+450)
#define EFX_EFFECT_WIPE4_RT_LB_RT_LT                          (EFX_EFFECT_WIPE_CLASS+451)
#define EFX_EFFECT_WIPE4_RT_LB_RT_LB                          (EFX_EFFECT_WIPE_CLASS+452)
#define EFX_EFFECT_WIPE4_RT_LB_RT_RT                          (EFX_EFFECT_WIPE_CLASS+453)
#define EFX_EFFECT_WIPE4_RT_LB_RT_RB                          (EFX_EFFECT_WIPE_CLASS+454)
#define EFX_EFFECT_WIPE4_RT_LB_RB_LT                          (EFX_EFFECT_WIPE_CLASS+455)
#define EFX_EFFECT_WIPE4_RT_LB_RB_LB                          (EFX_EFFECT_WIPE_CLASS+456)
#define EFX_EFFECT_WIPE4_RT_LB_RB_RT                          (EFX_EFFECT_WIPE_CLASS+457)
#define EFX_EFFECT_WIPE4_RT_LB_RB_RB                          (EFX_EFFECT_WIPE_CLASS+458)
#define EFX_EFFECT_WIPE4_RT_RT_LT_LT                          (EFX_EFFECT_WIPE_CLASS+459)
#define EFX_EFFECT_WIPE4_RT_RT_LT_LB                          (EFX_EFFECT_WIPE_CLASS+460)
#define EFX_EFFECT_WIPE4_RT_RT_LT_RT                          (EFX_EFFECT_WIPE_CLASS+461)
#define EFX_EFFECT_WIPE4_RT_RT_LT_RB                          (EFX_EFFECT_WIPE_CLASS+462)
#define EFX_EFFECT_WIPE4_RT_RT_LB_LT                          (EFX_EFFECT_WIPE_CLASS+463)
#define EFX_EFFECT_WIPE4_RT_RT_LB_LB                          (EFX_EFFECT_WIPE_CLASS+464)
#define EFX_EFFECT_WIPE4_RT_RT_LB_RT                          (EFX_EFFECT_WIPE_CLASS+465)
#define EFX_EFFECT_WIPE4_RT_RT_LB_RB                          (EFX_EFFECT_WIPE_CLASS+466)
#define EFX_EFFECT_WIPE4_RT_RT_RT_LT                          (EFX_EFFECT_WIPE_CLASS+467)
#define EFX_EFFECT_WIPE4_RT_RT_RT_LB                          (EFX_EFFECT_WIPE_CLASS+468)
#define EFX_EFFECT_WIPE4_RT_RT_RT_RT                          (EFX_EFFECT_WIPE_CLASS+469)
#define EFX_EFFECT_WIPE4_RT_RT_RT_RB                          (EFX_EFFECT_WIPE_CLASS+470)
#define EFX_EFFECT_WIPE4_RT_RT_RB_LT                          (EFX_EFFECT_WIPE_CLASS+471)
#define EFX_EFFECT_WIPE4_RT_RT_RB_LB                          (EFX_EFFECT_WIPE_CLASS+472)
#define EFX_EFFECT_WIPE4_RT_RT_RB_RT                          (EFX_EFFECT_WIPE_CLASS+473)
#define EFX_EFFECT_WIPE4_RT_RT_RB_RB                          (EFX_EFFECT_WIPE_CLASS+474)
#define EFX_EFFECT_WIPE4_RT_RB_LT_LT                          (EFX_EFFECT_WIPE_CLASS+475)
#define EFX_EFFECT_WIPE4_RT_RB_LT_LB                          (EFX_EFFECT_WIPE_CLASS+476)
#define EFX_EFFECT_WIPE4_RT_RB_LT_RT                          (EFX_EFFECT_WIPE_CLASS+477)
#define EFX_EFFECT_WIPE4_RT_RB_LT_RB                          (EFX_EFFECT_WIPE_CLASS+478)
#define EFX_EFFECT_WIPE4_RT_RB_LB_LT                          (EFX_EFFECT_WIPE_CLASS+479)
#define EFX_EFFECT_WIPE4_RT_RB_LB_LB                          (EFX_EFFECT_WIPE_CLASS+480)
#define EFX_EFFECT_WIPE4_RT_RB_LB_RT                          (EFX_EFFECT_WIPE_CLASS+481)
#define EFX_EFFECT_WIPE4_RT_RB_LB_RB                          (EFX_EFFECT_WIPE_CLASS+482)
#define EFX_EFFECT_WIPE4_RT_RB_RT_LT                          (EFX_EFFECT_WIPE_CLASS+483)
#define EFX_EFFECT_WIPE4_RT_RB_RT_LB                          (EFX_EFFECT_WIPE_CLASS+484)
#define EFX_EFFECT_WIPE4_RT_RB_RT_RT                          (EFX_EFFECT_WIPE_CLASS+485)
#define EFX_EFFECT_WIPE4_RT_RB_RT_RB                          (EFX_EFFECT_WIPE_CLASS+486)
#define EFX_EFFECT_WIPE4_RT_RB_RB_LT                          (EFX_EFFECT_WIPE_CLASS+487)
#define EFX_EFFECT_WIPE4_RT_RB_RB_LB                          (EFX_EFFECT_WIPE_CLASS+488)
#define EFX_EFFECT_WIPE4_RT_RB_RB_RT                          (EFX_EFFECT_WIPE_CLASS+489)
#define EFX_EFFECT_WIPE4_RT_RB_RB_RB                          (EFX_EFFECT_WIPE_CLASS+490)
#define EFX_EFFECT_WIPE4_RB_LT_LT_LT                          (EFX_EFFECT_WIPE_CLASS+491)
#define EFX_EFFECT_WIPE4_RB_LT_LT_LB                          (EFX_EFFECT_WIPE_CLASS+492)
#define EFX_EFFECT_WIPE4_RB_LT_LT_RT                          (EFX_EFFECT_WIPE_CLASS+493)
#define EFX_EFFECT_WIPE4_RB_LT_LT_RB                          (EFX_EFFECT_WIPE_CLASS+494)
#define EFX_EFFECT_WIPE4_RB_LT_LB_LT                          (EFX_EFFECT_WIPE_CLASS+495)
#define EFX_EFFECT_WIPE4_RB_LT_LB_LB                          (EFX_EFFECT_WIPE_CLASS+496)
#define EFX_EFFECT_WIPE4_RB_LT_LB_RT                          (EFX_EFFECT_WIPE_CLASS+497)
#define EFX_EFFECT_WIPE4_RB_LT_LB_RB                          (EFX_EFFECT_WIPE_CLASS+498)
#define EFX_EFFECT_WIPE4_RB_LT_RT_LT                          (EFX_EFFECT_WIPE_CLASS+499)
#define EFX_EFFECT_WIPE4_RB_LT_RT_LB                          (EFX_EFFECT_WIPE_CLASS+500)
#define EFX_EFFECT_WIPE4_RB_LT_RT_RT                          (EFX_EFFECT_WIPE_CLASS+501)
#define EFX_EFFECT_WIPE4_RB_LT_RT_RB                          (EFX_EFFECT_WIPE_CLASS+502)
#define EFX_EFFECT_WIPE4_RB_LT_RB_LT                          (EFX_EFFECT_WIPE_CLASS+503)
#define EFX_EFFECT_WIPE4_RB_LT_RB_LB                          (EFX_EFFECT_WIPE_CLASS+504)
#define EFX_EFFECT_WIPE4_RB_LT_RB_RT                          (EFX_EFFECT_WIPE_CLASS+505)
#define EFX_EFFECT_WIPE4_RB_LT_RB_RB                          (EFX_EFFECT_WIPE_CLASS+506)
#define EFX_EFFECT_WIPE4_RB_LB_LT_LT                          (EFX_EFFECT_WIPE_CLASS+507)
#define EFX_EFFECT_WIPE4_RB_LB_LT_LB                          (EFX_EFFECT_WIPE_CLASS+508)
#define EFX_EFFECT_WIPE4_RB_LB_LT_RT                          (EFX_EFFECT_WIPE_CLASS+509)
#define EFX_EFFECT_WIPE4_RB_LB_LT_RB                          (EFX_EFFECT_WIPE_CLASS+510)
#define EFX_EFFECT_WIPE4_RB_LB_LB_LT                          (EFX_EFFECT_WIPE_CLASS+511)
#define EFX_EFFECT_WIPE4_RB_LB_LB_LB                          (EFX_EFFECT_WIPE_CLASS+512)
#define EFX_EFFECT_WIPE4_RB_LB_LB_RT                          (EFX_EFFECT_WIPE_CLASS+513)
#define EFX_EFFECT_WIPE4_RB_LB_LB_RB                          (EFX_EFFECT_WIPE_CLASS+514)
#define EFX_EFFECT_WIPE4_RB_LB_RT_LT                          (EFX_EFFECT_WIPE_CLASS+515)
#define EFX_EFFECT_WIPE4_RB_LB_RT_LB                          (EFX_EFFECT_WIPE_CLASS+516)
#define EFX_EFFECT_WIPE4_RB_LB_RT_RT                          (EFX_EFFECT_WIPE_CLASS+517)
#define EFX_EFFECT_WIPE4_RB_LB_RT_RB                          (EFX_EFFECT_WIPE_CLASS+518)
#define EFX_EFFECT_WIPE4_RB_LB_RB_LT                          (EFX_EFFECT_WIPE_CLASS+519)
#define EFX_EFFECT_WIPE4_RB_LB_RB_LB                          (EFX_EFFECT_WIPE_CLASS+520)
#define EFX_EFFECT_WIPE4_RB_LB_RB_RT                          (EFX_EFFECT_WIPE_CLASS+521)
#define EFX_EFFECT_WIPE4_RB_LB_RB_RB                          (EFX_EFFECT_WIPE_CLASS+522)
#define EFX_EFFECT_WIPE4_RB_RT_LT_LT                          (EFX_EFFECT_WIPE_CLASS+523)
#define EFX_EFFECT_WIPE4_RB_RT_LT_LB                          (EFX_EFFECT_WIPE_CLASS+524)
#define EFX_EFFECT_WIPE4_RB_RT_LT_RT                          (EFX_EFFECT_WIPE_CLASS+525)
#define EFX_EFFECT_WIPE4_RB_RT_LT_RB                          (EFX_EFFECT_WIPE_CLASS+526)
#define EFX_EFFECT_WIPE4_RB_RT_LB_LT                          (EFX_EFFECT_WIPE_CLASS+527)
#define EFX_EFFECT_WIPE4_RB_RT_LB_LB                          (EFX_EFFECT_WIPE_CLASS+528)
#define EFX_EFFECT_WIPE4_RB_RT_LB_RT                          (EFX_EFFECT_WIPE_CLASS+529)
#define EFX_EFFECT_WIPE4_RB_RT_LB_RB                          (EFX_EFFECT_WIPE_CLASS+530)
#define EFX_EFFECT_WIPE4_RB_RT_RT_LT                          (EFX_EFFECT_WIPE_CLASS+531)
#define EFX_EFFECT_WIPE4_RB_RT_RT_LB                          (EFX_EFFECT_WIPE_CLASS+532)
#define EFX_EFFECT_WIPE4_RB_RT_RT_RT                          (EFX_EFFECT_WIPE_CLASS+533)
#define EFX_EFFECT_WIPE4_RB_RT_RT_RB                          (EFX_EFFECT_WIPE_CLASS+534)
#define EFX_EFFECT_WIPE4_RB_RT_RB_LT                          (EFX_EFFECT_WIPE_CLASS+535)
#define EFX_EFFECT_WIPE4_RB_RT_RB_LB                          (EFX_EFFECT_WIPE_CLASS+536)
#define EFX_EFFECT_WIPE4_RB_RT_RB_RT                          (EFX_EFFECT_WIPE_CLASS+537)
#define EFX_EFFECT_WIPE4_RB_RT_RB_RB                          (EFX_EFFECT_WIPE_CLASS+538)
#define EFX_EFFECT_WIPE4_RB_RB_LT_LT                          (EFX_EFFECT_WIPE_CLASS+539)
#define EFX_EFFECT_WIPE4_RB_RB_LT_LB                          (EFX_EFFECT_WIPE_CLASS+540)
#define EFX_EFFECT_WIPE4_RB_RB_LT_RT                          (EFX_EFFECT_WIPE_CLASS+541)
#define EFX_EFFECT_WIPE4_RB_RB_LT_RB                          (EFX_EFFECT_WIPE_CLASS+542)
#define EFX_EFFECT_WIPE4_RB_RB_LB_LT                          (EFX_EFFECT_WIPE_CLASS+543)
#define EFX_EFFECT_WIPE4_RB_RB_LB_LB                          (EFX_EFFECT_WIPE_CLASS+544)
#define EFX_EFFECT_WIPE4_RB_RB_LB_RT                          (EFX_EFFECT_WIPE_CLASS+545)
#define EFX_EFFECT_WIPE4_RB_RB_LB_RB                          (EFX_EFFECT_WIPE_CLASS+546)
#define EFX_EFFECT_WIPE4_RB_RB_RT_LT                          (EFX_EFFECT_WIPE_CLASS+547)
#define EFX_EFFECT_WIPE4_RB_RB_RT_LB                          (EFX_EFFECT_WIPE_CLASS+548)
#define EFX_EFFECT_WIPE4_RB_RB_RT_RT                          (EFX_EFFECT_WIPE_CLASS+549)
#define EFX_EFFECT_WIPE4_RB_RB_RT_RB                          (EFX_EFFECT_WIPE_CLASS+550)
#define EFX_EFFECT_WIPE4_RB_RB_RB_LT                          (EFX_EFFECT_WIPE_CLASS+551)
#define EFX_EFFECT_WIPE4_RB_RB_RB_LB                          (EFX_EFFECT_WIPE_CLASS+552)
#define EFX_EFFECT_WIPE4_RB_RB_RB_RT                          (EFX_EFFECT_WIPE_CLASS+553)
#define EFX_EFFECT_WIPE4_RB_RB_RB_RB                          (EFX_EFFECT_WIPE_CLASS+554)
#define EFX_EFFECT_WIPE_MAX                                   (EFX_EFFECT_WIPE_CLASS+554)

// Rectangular wipe
#define EFX_EFFECT_WIPE_RECTANGLE_CLASS                       3000
#define EFX_EFFECT_WIPE_RECTANGLE_IN                          (EFX_EFFECT_WIPE_RECTANGLE_CLASS+0)
#define EFX_EFFECT_WIPE_RECTANGLE_OUT                         (EFX_EFFECT_WIPE_RECTANGLE_CLASS+1)
#define EFX_EFFECT_WIPE_RECTANGLE_INTER_IN_THEN_IN            (EFX_EFFECT_WIPE_RECTANGLE_CLASS+2)
#define EFX_EFFECT_WIPE_RECTANGLE_INTER_OUT_THEN_OUT          (EFX_EFFECT_WIPE_RECTANGLE_CLASS+3)
#define EFX_EFFECT_WIPE_RECTANGLE_INTER_IN_THEN_OUT           (EFX_EFFECT_WIPE_RECTANGLE_CLASS+4)
#define EFX_EFFECT_WIPE_RECTANGLE_INTER_OUT_THEN_IN           (EFX_EFFECT_WIPE_RECTANGLE_CLASS+5)
#define EFX_EFFECT_WIPE_RECTANGLE_MAX                         (EFX_EFFECT_WIPE_RECTANGLE_CLASS+5)

// Circular wipe
#define EFX_EFFECT_WIPE_CIRCLE_CLASS                          4000
#define EFX_EFFECT_WIPE_CIRCLE_C_CW_FROM_L                    (EFX_EFFECT_WIPE_CIRCLE_CLASS+0)
#define EFX_EFFECT_WIPE_CIRCLE_C_CCW_FROM_L                   (EFX_EFFECT_WIPE_CIRCLE_CLASS+1)
#define EFX_EFFECT_WIPE_CIRCLE_C_CW_FROM_R                    (EFX_EFFECT_WIPE_CIRCLE_CLASS+2)
#define EFX_EFFECT_WIPE_CIRCLE_C_CCW_FROM_R                   (EFX_EFFECT_WIPE_CIRCLE_CLASS+3)
#define EFX_EFFECT_WIPE_CIRCLE_C_CW_FROM_T                    (EFX_EFFECT_WIPE_CIRCLE_CLASS+4)
#define EFX_EFFECT_WIPE_CIRCLE_C_CCW_FROM_T                   (EFX_EFFECT_WIPE_CIRCLE_CLASS+5)
#define EFX_EFFECT_WIPE_CIRCLE_C_CW_FROM_B                    (EFX_EFFECT_WIPE_CIRCLE_CLASS+6)
#define EFX_EFFECT_WIPE_CIRCLE_C_CCW_FROM_B                   (EFX_EFFECT_WIPE_CIRCLE_CLASS+7)
#define EFX_EFFECT_WIPE_CIRCLE_L_CCW_FROM_B                   (EFX_EFFECT_WIPE_CIRCLE_CLASS+8)
#define EFX_EFFECT_WIPE_CIRCLE_L_CW_FROM_T                    (EFX_EFFECT_WIPE_CIRCLE_CLASS+9)
#define EFX_EFFECT_WIPE_CIRCLE_R_CW_FROM_B                    (EFX_EFFECT_WIPE_CIRCLE_CLASS+10)
#define EFX_EFFECT_WIPE_CIRCLE_R_CCW_FROM_T                   (EFX_EFFECT_WIPE_CIRCLE_CLASS+11)
#define EFX_EFFECT_WIPE_CIRCLE_T_CCW_FROM_L                   (EFX_EFFECT_WIPE_CIRCLE_CLASS+12)
#define EFX_EFFECT_WIPE_CIRCLE_T_CW_FROM_R                    (EFX_EFFECT_WIPE_CIRCLE_CLASS+13)
#define EFX_EFFECT_WIPE_CIRCLE_B_CW_FROM_L                    (EFX_EFFECT_WIPE_CIRCLE_CLASS+14)
#define EFX_EFFECT_WIPE_CIRCLE_B_CCW_FROM_R                   (EFX_EFFECT_WIPE_CIRCLE_CLASS+15)
#define EFX_EFFECT_WIPE_CIRCLE2_C_CW_FROM_L_AND_FROM_R        (EFX_EFFECT_WIPE_CIRCLE_CLASS+16)
#define EFX_EFFECT_WIPE_CIRCLE2_C_CCW_FROM_L_AND_FROM_R       (EFX_EFFECT_WIPE_CIRCLE_CLASS+17)
#define EFX_EFFECT_WIPE_CIRCLE2_C_CW_FROM_T_AND_FROM_B        (EFX_EFFECT_WIPE_CIRCLE_CLASS+18)
#define EFX_EFFECT_WIPE_CIRCLE2_C_CCW_FROM_T_AND_FROM_B       (EFX_EFFECT_WIPE_CIRCLE_CLASS+19)
#define EFX_EFFECT_WIPE_CIRCLE2_L_CW_FROM_T_AND_CCW_FROM_B    (EFX_EFFECT_WIPE_CIRCLE_CLASS+20)
#define EFX_EFFECT_WIPE_CIRCLE2_L_CCW_TO_T_AND_CW_TO_B        (EFX_EFFECT_WIPE_CIRCLE_CLASS+21)
#define EFX_EFFECT_WIPE_CIRCLE2_R_CCW_FROM_T_AND_CW_FROM_B    (EFX_EFFECT_WIPE_CIRCLE_CLASS+22)
#define EFX_EFFECT_WIPE_CIRCLE2_R_CW_TO_T_AND_CCW_TO_B        (EFX_EFFECT_WIPE_CIRCLE_CLASS+23)
#define EFX_EFFECT_WIPE_CIRCLE2_T_CCW_FROM_L_AND_CW_FROM_R    (EFX_EFFECT_WIPE_CIRCLE_CLASS+24)
#define EFX_EFFECT_WIPE_CIRCLE2_T_CW_TO_L_AND_CCW_TO_R        (EFX_EFFECT_WIPE_CIRCLE_CLASS+25)
#define EFX_EFFECT_WIPE_CIRCLE2_B_CW_FROM_L_AND_CCW_FROM_R    (EFX_EFFECT_WIPE_CIRCLE_CLASS+26)
#define EFX_EFFECT_WIPE_CIRCLE2_B_CCW_TO_L_AND_CW_TO_R        (EFX_EFFECT_WIPE_CIRCLE_CLASS+27)
#define EFX_EFFECT_WIPE_CIRCLE4_C_CW_CCW_CCW_CW               (EFX_EFFECT_WIPE_CIRCLE_CLASS+28)
#define EFX_EFFECT_WIPE_CIRCLE4_C_CCW_CW_CW_CCW               (EFX_EFFECT_WIPE_CIRCLE_CLASS+29)
#define EFX_EFFECT_WIPE_CIRCLE_MAX                            (EFX_EFFECT_WIPE_CIRCLE_CLASS+29)

// Push
#define EFX_EFFECT_PUSH_CLASS                                 5000
#define EFX_EFFECT_PUSH_R_TO_L                                (EFX_EFFECT_PUSH_CLASS+0)
#define EFX_EFFECT_PUSH_L_TO_R                                (EFX_EFFECT_PUSH_CLASS+1)
#define EFX_EFFECT_PUSH_B_TO_T                                (EFX_EFFECT_PUSH_CLASS+2)
#define EFX_EFFECT_PUSH_T_TO_B                                (EFX_EFFECT_PUSH_CLASS+3)
#define EFX_EFFECT_PUSH2_T_R_TO_L_AND_B_L_TO_R                (EFX_EFFECT_PUSH_CLASS+4)
#define EFX_EFFECT_PUSH2_T_L_TO_R_AND_B_R_TO_L                (EFX_EFFECT_PUSH_CLASS+5)
#define EFX_EFFECT_PUSH2_L_B_TO_T_AND_R_T_TO_B                (EFX_EFFECT_PUSH_CLASS+6)
#define EFX_EFFECT_PUSH2_L_T_TO_B_AND_R_B_TO_T                (EFX_EFFECT_PUSH_CLASS+7)
#define EFX_EFFECT_PUSH4_L_L_L_L                              (EFX_EFFECT_PUSH_CLASS+8)
#define EFX_EFFECT_PUSH4_L_L_L_R                              (EFX_EFFECT_PUSH_CLASS+9)
#define EFX_EFFECT_PUSH4_L_L_L_T                              (EFX_EFFECT_PUSH_CLASS+10)
#define EFX_EFFECT_PUSH4_L_L_L_B                              (EFX_EFFECT_PUSH_CLASS+11)
#define EFX_EFFECT_PUSH4_L_L_R_L                              (EFX_EFFECT_PUSH_CLASS+12)
#define EFX_EFFECT_PUSH4_L_L_R_R                              (EFX_EFFECT_PUSH_CLASS+13)
#define EFX_EFFECT_PUSH4_L_L_R_T                              (EFX_EFFECT_PUSH_CLASS+14)
#define EFX_EFFECT_PUSH4_L_L_R_B                              (EFX_EFFECT_PUSH_CLASS+15)
#define EFX_EFFECT_PUSH4_L_L_T_L                              (EFX_EFFECT_PUSH_CLASS+16)
#define EFX_EFFECT_PUSH4_L_L_T_R                              (EFX_EFFECT_PUSH_CLASS+17)
#define EFX_EFFECT_PUSH4_L_L_T_T                              (EFX_EFFECT_PUSH_CLASS+18)
#define EFX_EFFECT_PUSH4_L_L_T_B                              (EFX_EFFECT_PUSH_CLASS+19)
#define EFX_EFFECT_PUSH4_L_L_B_L                              (EFX_EFFECT_PUSH_CLASS+20)
#define EFX_EFFECT_PUSH4_L_L_B_R                              (EFX_EFFECT_PUSH_CLASS+21)
#define EFX_EFFECT_PUSH4_L_L_B_T                              (EFX_EFFECT_PUSH_CLASS+22)
#define EFX_EFFECT_PUSH4_L_L_B_B                              (EFX_EFFECT_PUSH_CLASS+23)
#define EFX_EFFECT_PUSH4_L_R_L_L                              (EFX_EFFECT_PUSH_CLASS+24)
#define EFX_EFFECT_PUSH4_L_R_L_R                              (EFX_EFFECT_PUSH_CLASS+25)
#define EFX_EFFECT_PUSH4_L_R_L_T                              (EFX_EFFECT_PUSH_CLASS+26)
#define EFX_EFFECT_PUSH4_L_R_L_B                              (EFX_EFFECT_PUSH_CLASS+27)
#define EFX_EFFECT_PUSH4_L_R_R_L                              (EFX_EFFECT_PUSH_CLASS+28)
#define EFX_EFFECT_PUSH4_L_R_R_R                              (EFX_EFFECT_PUSH_CLASS+29)
#define EFX_EFFECT_PUSH4_L_R_R_T                              (EFX_EFFECT_PUSH_CLASS+30)
#define EFX_EFFECT_PUSH4_L_R_R_B                              (EFX_EFFECT_PUSH_CLASS+31)
#define EFX_EFFECT_PUSH4_L_R_T_L                              (EFX_EFFECT_PUSH_CLASS+32)
#define EFX_EFFECT_PUSH4_L_R_T_R                              (EFX_EFFECT_PUSH_CLASS+33)
#define EFX_EFFECT_PUSH4_L_R_T_T                              (EFX_EFFECT_PUSH_CLASS+34)
#define EFX_EFFECT_PUSH4_L_R_T_B                              (EFX_EFFECT_PUSH_CLASS+35)
#define EFX_EFFECT_PUSH4_L_R_B_L                              (EFX_EFFECT_PUSH_CLASS+36)
#define EFX_EFFECT_PUSH4_L_R_B_R                              (EFX_EFFECT_PUSH_CLASS+37)
#define EFX_EFFECT_PUSH4_L_R_B_T                              (EFX_EFFECT_PUSH_CLASS+38)
#define EFX_EFFECT_PUSH4_L_R_B_B                              (EFX_EFFECT_PUSH_CLASS+39)
#define EFX_EFFECT_PUSH4_L_T_L_L                              (EFX_EFFECT_PUSH_CLASS+40)
#define EFX_EFFECT_PUSH4_L_T_L_R                              (EFX_EFFECT_PUSH_CLASS+41)
#define EFX_EFFECT_PUSH4_L_T_L_T                              (EFX_EFFECT_PUSH_CLASS+42)
#define EFX_EFFECT_PUSH4_L_T_L_B                              (EFX_EFFECT_PUSH_CLASS+43)
#define EFX_EFFECT_PUSH4_L_T_R_L                              (EFX_EFFECT_PUSH_CLASS+44)
#define EFX_EFFECT_PUSH4_L_T_R_R                              (EFX_EFFECT_PUSH_CLASS+45)
#define EFX_EFFECT_PUSH4_L_T_R_T                              (EFX_EFFECT_PUSH_CLASS+46)
#define EFX_EFFECT_PUSH4_L_T_R_B                              (EFX_EFFECT_PUSH_CLASS+47)
#define EFX_EFFECT_PUSH4_L_T_T_L                              (EFX_EFFECT_PUSH_CLASS+48)
#define EFX_EFFECT_PUSH4_L_T_T_R                              (EFX_EFFECT_PUSH_CLASS+49)
#define EFX_EFFECT_PUSH4_L_T_T_T                              (EFX_EFFECT_PUSH_CLASS+50)
#define EFX_EFFECT_PUSH4_L_T_T_B                              (EFX_EFFECT_PUSH_CLASS+51)
#define EFX_EFFECT_PUSH4_L_T_B_L                              (EFX_EFFECT_PUSH_CLASS+52)
#define EFX_EFFECT_PUSH4_L_T_B_R                              (EFX_EFFECT_PUSH_CLASS+53)
#define EFX_EFFECT_PUSH4_L_T_B_T                              (EFX_EFFECT_PUSH_CLASS+54)
#define EFX_EFFECT_PUSH4_L_T_B_B                              (EFX_EFFECT_PUSH_CLASS+55)
#define EFX_EFFECT_PUSH4_L_B_L_L                              (EFX_EFFECT_PUSH_CLASS+56)
#define EFX_EFFECT_PUSH4_L_B_L_R                              (EFX_EFFECT_PUSH_CLASS+57)
#define EFX_EFFECT_PUSH4_L_B_L_T                              (EFX_EFFECT_PUSH_CLASS+58)
#define EFX_EFFECT_PUSH4_L_B_L_B                              (EFX_EFFECT_PUSH_CLASS+59)
#define EFX_EFFECT_PUSH4_L_B_R_L                              (EFX_EFFECT_PUSH_CLASS+60)
#define EFX_EFFECT_PUSH4_L_B_R_R                              (EFX_EFFECT_PUSH_CLASS+61)
#define EFX_EFFECT_PUSH4_L_B_R_T                              (EFX_EFFECT_PUSH_CLASS+62)
#define EFX_EFFECT_PUSH4_L_B_R_B                              (EFX_EFFECT_PUSH_CLASS+63)
#define EFX_EFFECT_PUSH4_L_B_T_L                              (EFX_EFFECT_PUSH_CLASS+64)
#define EFX_EFFECT_PUSH4_L_B_T_R                              (EFX_EFFECT_PUSH_CLASS+65)
#define EFX_EFFECT_PUSH4_L_B_T_T                              (EFX_EFFECT_PUSH_CLASS+66)
#define EFX_EFFECT_PUSH4_L_B_T_B                              (EFX_EFFECT_PUSH_CLASS+67)
#define EFX_EFFECT_PUSH4_L_B_B_L                              (EFX_EFFECT_PUSH_CLASS+68)
#define EFX_EFFECT_PUSH4_L_B_B_R                              (EFX_EFFECT_PUSH_CLASS+69)
#define EFX_EFFECT_PUSH4_L_B_B_T                              (EFX_EFFECT_PUSH_CLASS+70)
#define EFX_EFFECT_PUSH4_L_B_B_B                              (EFX_EFFECT_PUSH_CLASS+71)
#define EFX_EFFECT_PUSH4_R_L_L_L                              (EFX_EFFECT_PUSH_CLASS+72)
#define EFX_EFFECT_PUSH4_R_L_L_R                              (EFX_EFFECT_PUSH_CLASS+73)
#define EFX_EFFECT_PUSH4_R_L_L_T                              (EFX_EFFECT_PUSH_CLASS+74)
#define EFX_EFFECT_PUSH4_R_L_L_B                              (EFX_EFFECT_PUSH_CLASS+75)
#define EFX_EFFECT_PUSH4_R_L_R_L                              (EFX_EFFECT_PUSH_CLASS+76)
#define EFX_EFFECT_PUSH4_R_L_R_R                              (EFX_EFFECT_PUSH_CLASS+77)
#define EFX_EFFECT_PUSH4_R_L_R_T                              (EFX_EFFECT_PUSH_CLASS+78)
#define EFX_EFFECT_PUSH4_R_L_R_B                              (EFX_EFFECT_PUSH_CLASS+79)
#define EFX_EFFECT_PUSH4_R_L_T_L                              (EFX_EFFECT_PUSH_CLASS+80)
#define EFX_EFFECT_PUSH4_R_L_T_R                              (EFX_EFFECT_PUSH_CLASS+81)
#define EFX_EFFECT_PUSH4_R_L_T_T                              (EFX_EFFECT_PUSH_CLASS+82)
#define EFX_EFFECT_PUSH4_R_L_T_B                              (EFX_EFFECT_PUSH_CLASS+83)
#define EFX_EFFECT_PUSH4_R_L_B_L                              (EFX_EFFECT_PUSH_CLASS+84)
#define EFX_EFFECT_PUSH4_R_L_B_R                              (EFX_EFFECT_PUSH_CLASS+85)
#define EFX_EFFECT_PUSH4_R_L_B_T                              (EFX_EFFECT_PUSH_CLASS+86)
#define EFX_EFFECT_PUSH4_R_L_B_B                              (EFX_EFFECT_PUSH_CLASS+87)
#define EFX_EFFECT_PUSH4_R_R_L_L                              (EFX_EFFECT_PUSH_CLASS+88)
#define EFX_EFFECT_PUSH4_R_R_L_R                              (EFX_EFFECT_PUSH_CLASS+89)
#define EFX_EFFECT_PUSH4_R_R_L_T                              (EFX_EFFECT_PUSH_CLASS+90)
#define EFX_EFFECT_PUSH4_R_R_L_B                              (EFX_EFFECT_PUSH_CLASS+91)
#define EFX_EFFECT_PUSH4_R_R_R_L                              (EFX_EFFECT_PUSH_CLASS+92)
#define EFX_EFFECT_PUSH4_R_R_R_R                              (EFX_EFFECT_PUSH_CLASS+93)
#define EFX_EFFECT_PUSH4_R_R_R_T                              (EFX_EFFECT_PUSH_CLASS+94)
#define EFX_EFFECT_PUSH4_R_R_R_B                              (EFX_EFFECT_PUSH_CLASS+95)
#define EFX_EFFECT_PUSH4_R_R_T_L                              (EFX_EFFECT_PUSH_CLASS+96)
#define EFX_EFFECT_PUSH4_R_R_T_R                              (EFX_EFFECT_PUSH_CLASS+97)
#define EFX_EFFECT_PUSH4_R_R_T_T                              (EFX_EFFECT_PUSH_CLASS+98)
#define EFX_EFFECT_PUSH4_R_R_T_B                              (EFX_EFFECT_PUSH_CLASS+99)
#define EFX_EFFECT_PUSH4_R_R_B_L                              (EFX_EFFECT_PUSH_CLASS+100)
#define EFX_EFFECT_PUSH4_R_R_B_R                              (EFX_EFFECT_PUSH_CLASS+101)
#define EFX_EFFECT_PUSH4_R_R_B_T                              (EFX_EFFECT_PUSH_CLASS+102)
#define EFX_EFFECT_PUSH4_R_R_B_B                              (EFX_EFFECT_PUSH_CLASS+103)
#define EFX_EFFECT_PUSH4_R_T_L_L                              (EFX_EFFECT_PUSH_CLASS+104)
#define EFX_EFFECT_PUSH4_R_T_L_R                              (EFX_EFFECT_PUSH_CLASS+105)
#define EFX_EFFECT_PUSH4_R_T_L_T                              (EFX_EFFECT_PUSH_CLASS+106)
#define EFX_EFFECT_PUSH4_R_T_L_B                              (EFX_EFFECT_PUSH_CLASS+107)
#define EFX_EFFECT_PUSH4_R_T_R_L                              (EFX_EFFECT_PUSH_CLASS+108)
#define EFX_EFFECT_PUSH4_R_T_R_R                              (EFX_EFFECT_PUSH_CLASS+109)
#define EFX_EFFECT_PUSH4_R_T_R_T                              (EFX_EFFECT_PUSH_CLASS+110)
#define EFX_EFFECT_PUSH4_R_T_R_B                              (EFX_EFFECT_PUSH_CLASS+111)
#define EFX_EFFECT_PUSH4_R_T_T_L                              (EFX_EFFECT_PUSH_CLASS+112)
#define EFX_EFFECT_PUSH4_R_T_T_R                              (EFX_EFFECT_PUSH_CLASS+113)
#define EFX_EFFECT_PUSH4_R_T_T_T                              (EFX_EFFECT_PUSH_CLASS+114)
#define EFX_EFFECT_PUSH4_R_T_T_B                              (EFX_EFFECT_PUSH_CLASS+115)
#define EFX_EFFECT_PUSH4_R_T_B_L                              (EFX_EFFECT_PUSH_CLASS+116)
#define EFX_EFFECT_PUSH4_R_T_B_R                              (EFX_EFFECT_PUSH_CLASS+117)
#define EFX_EFFECT_PUSH4_R_T_B_T                              (EFX_EFFECT_PUSH_CLASS+118)
#define EFX_EFFECT_PUSH4_R_T_B_B                              (EFX_EFFECT_PUSH_CLASS+119)
#define EFX_EFFECT_PUSH4_R_B_L_L                              (EFX_EFFECT_PUSH_CLASS+120)
#define EFX_EFFECT_PUSH4_R_B_L_R                              (EFX_EFFECT_PUSH_CLASS+121)
#define EFX_EFFECT_PUSH4_R_B_L_T                              (EFX_EFFECT_PUSH_CLASS+122)
#define EFX_EFFECT_PUSH4_R_B_L_B                              (EFX_EFFECT_PUSH_CLASS+123)
#define EFX_EFFECT_PUSH4_R_B_R_L                              (EFX_EFFECT_PUSH_CLASS+124)
#define EFX_EFFECT_PUSH4_R_B_R_R                              (EFX_EFFECT_PUSH_CLASS+125)
#define EFX_EFFECT_PUSH4_R_B_R_T                              (EFX_EFFECT_PUSH_CLASS+126)
#define EFX_EFFECT_PUSH4_R_B_R_B                              (EFX_EFFECT_PUSH_CLASS+127)
#define EFX_EFFECT_PUSH4_R_B_T_L                              (EFX_EFFECT_PUSH_CLASS+128)
#define EFX_EFFECT_PUSH4_R_B_T_R                              (EFX_EFFECT_PUSH_CLASS+129)
#define EFX_EFFECT_PUSH4_R_B_T_T                              (EFX_EFFECT_PUSH_CLASS+130)
#define EFX_EFFECT_PUSH4_R_B_T_B                              (EFX_EFFECT_PUSH_CLASS+131)
#define EFX_EFFECT_PUSH4_R_B_B_L                              (EFX_EFFECT_PUSH_CLASS+132)
#define EFX_EFFECT_PUSH4_R_B_B_R                              (EFX_EFFECT_PUSH_CLASS+133)
#define EFX_EFFECT_PUSH4_R_B_B_T                              (EFX_EFFECT_PUSH_CLASS+134)
#define EFX_EFFECT_PUSH4_R_B_B_B                              (EFX_EFFECT_PUSH_CLASS+135)
#define EFX_EFFECT_PUSH4_T_L_L_L                              (EFX_EFFECT_PUSH_CLASS+136)
#define EFX_EFFECT_PUSH4_T_L_L_R                              (EFX_EFFECT_PUSH_CLASS+137)
#define EFX_EFFECT_PUSH4_T_L_L_T                              (EFX_EFFECT_PUSH_CLASS+138)
#define EFX_EFFECT_PUSH4_T_L_L_B                              (EFX_EFFECT_PUSH_CLASS+139)
#define EFX_EFFECT_PUSH4_T_L_R_L                              (EFX_EFFECT_PUSH_CLASS+140)
#define EFX_EFFECT_PUSH4_T_L_R_R                              (EFX_EFFECT_PUSH_CLASS+141)
#define EFX_EFFECT_PUSH4_T_L_R_T                              (EFX_EFFECT_PUSH_CLASS+142)
#define EFX_EFFECT_PUSH4_T_L_R_B                              (EFX_EFFECT_PUSH_CLASS+143)
#define EFX_EFFECT_PUSH4_T_L_T_L                              (EFX_EFFECT_PUSH_CLASS+144)
#define EFX_EFFECT_PUSH4_T_L_T_R                              (EFX_EFFECT_PUSH_CLASS+145)
#define EFX_EFFECT_PUSH4_T_L_T_T                              (EFX_EFFECT_PUSH_CLASS+146)
#define EFX_EFFECT_PUSH4_T_L_T_B                              (EFX_EFFECT_PUSH_CLASS+147)
#define EFX_EFFECT_PUSH4_T_L_B_L                              (EFX_EFFECT_PUSH_CLASS+148)
#define EFX_EFFECT_PUSH4_T_L_B_R                              (EFX_EFFECT_PUSH_CLASS+149)
#define EFX_EFFECT_PUSH4_T_L_B_T                              (EFX_EFFECT_PUSH_CLASS+150)
#define EFX_EFFECT_PUSH4_T_L_B_B                              (EFX_EFFECT_PUSH_CLASS+151)
#define EFX_EFFECT_PUSH4_T_R_L_L                              (EFX_EFFECT_PUSH_CLASS+152)
#define EFX_EFFECT_PUSH4_T_R_L_R                              (EFX_EFFECT_PUSH_CLASS+153)
#define EFX_EFFECT_PUSH4_T_R_L_T                              (EFX_EFFECT_PUSH_CLASS+154)
#define EFX_EFFECT_PUSH4_T_R_L_B                              (EFX_EFFECT_PUSH_CLASS+155)
#define EFX_EFFECT_PUSH4_T_R_R_L                              (EFX_EFFECT_PUSH_CLASS+156)
#define EFX_EFFECT_PUSH4_T_R_R_R                              (EFX_EFFECT_PUSH_CLASS+157)
#define EFX_EFFECT_PUSH4_T_R_R_T                              (EFX_EFFECT_PUSH_CLASS+158)
#define EFX_EFFECT_PUSH4_T_R_R_B                              (EFX_EFFECT_PUSH_CLASS+159)
#define EFX_EFFECT_PUSH4_T_R_T_L                              (EFX_EFFECT_PUSH_CLASS+160)
#define EFX_EFFECT_PUSH4_T_R_T_R                              (EFX_EFFECT_PUSH_CLASS+161)
#define EFX_EFFECT_PUSH4_T_R_T_T                              (EFX_EFFECT_PUSH_CLASS+162)
#define EFX_EFFECT_PUSH4_T_R_T_B                              (EFX_EFFECT_PUSH_CLASS+163)
#define EFX_EFFECT_PUSH4_T_R_B_L                              (EFX_EFFECT_PUSH_CLASS+164)
#define EFX_EFFECT_PUSH4_T_R_B_R                              (EFX_EFFECT_PUSH_CLASS+165)
#define EFX_EFFECT_PUSH4_T_R_B_T                              (EFX_EFFECT_PUSH_CLASS+166)
#define EFX_EFFECT_PUSH4_T_R_B_B                              (EFX_EFFECT_PUSH_CLASS+167)
#define EFX_EFFECT_PUSH4_T_T_L_L                              (EFX_EFFECT_PUSH_CLASS+168)
#define EFX_EFFECT_PUSH4_T_T_L_R                              (EFX_EFFECT_PUSH_CLASS+169)
#define EFX_EFFECT_PUSH4_T_T_L_T                              (EFX_EFFECT_PUSH_CLASS+170)
#define EFX_EFFECT_PUSH4_T_T_L_B                              (EFX_EFFECT_PUSH_CLASS+171)
#define EFX_EFFECT_PUSH4_T_T_R_L                              (EFX_EFFECT_PUSH_CLASS+172)
#define EFX_EFFECT_PUSH4_T_T_R_R                              (EFX_EFFECT_PUSH_CLASS+173)
#define EFX_EFFECT_PUSH4_T_T_R_T                              (EFX_EFFECT_PUSH_CLASS+174)
#define EFX_EFFECT_PUSH4_T_T_R_B                              (EFX_EFFECT_PUSH_CLASS+175)
#define EFX_EFFECT_PUSH4_T_T_T_L                              (EFX_EFFECT_PUSH_CLASS+176)
#define EFX_EFFECT_PUSH4_T_T_T_R                              (EFX_EFFECT_PUSH_CLASS+177)
#define EFX_EFFECT_PUSH4_T_T_T_T                              (EFX_EFFECT_PUSH_CLASS+178)
#define EFX_EFFECT_PUSH4_T_T_T_B                              (EFX_EFFECT_PUSH_CLASS+179)
#define EFX_EFFECT_PUSH4_T_T_B_L                              (EFX_EFFECT_PUSH_CLASS+180)
#define EFX_EFFECT_PUSH4_T_T_B_R                              (EFX_EFFECT_PUSH_CLASS+181)
#define EFX_EFFECT_PUSH4_T_T_B_T                              (EFX_EFFECT_PUSH_CLASS+182)
#define EFX_EFFECT_PUSH4_T_T_B_B                              (EFX_EFFECT_PUSH_CLASS+183)
#define EFX_EFFECT_PUSH4_T_B_L_L                              (EFX_EFFECT_PUSH_CLASS+184)
#define EFX_EFFECT_PUSH4_T_B_L_R                              (EFX_EFFECT_PUSH_CLASS+185)
#define EFX_EFFECT_PUSH4_T_B_L_T                              (EFX_EFFECT_PUSH_CLASS+186)
#define EFX_EFFECT_PUSH4_T_B_L_B                              (EFX_EFFECT_PUSH_CLASS+187)
#define EFX_EFFECT_PUSH4_T_B_R_L                              (EFX_EFFECT_PUSH_CLASS+188)
#define EFX_EFFECT_PUSH4_T_B_R_R                              (EFX_EFFECT_PUSH_CLASS+189)
#define EFX_EFFECT_PUSH4_T_B_R_T                              (EFX_EFFECT_PUSH_CLASS+190)
#define EFX_EFFECT_PUSH4_T_B_R_B                              (EFX_EFFECT_PUSH_CLASS+191)
#define EFX_EFFECT_PUSH4_T_B_T_L                              (EFX_EFFECT_PUSH_CLASS+192)
#define EFX_EFFECT_PUSH4_T_B_T_R                              (EFX_EFFECT_PUSH_CLASS+193)
#define EFX_EFFECT_PUSH4_T_B_T_T                              (EFX_EFFECT_PUSH_CLASS+194)
#define EFX_EFFECT_PUSH4_T_B_T_B                              (EFX_EFFECT_PUSH_CLASS+195)
#define EFX_EFFECT_PUSH4_T_B_B_L                              (EFX_EFFECT_PUSH_CLASS+196)
#define EFX_EFFECT_PUSH4_T_B_B_R                              (EFX_EFFECT_PUSH_CLASS+197)
#define EFX_EFFECT_PUSH4_T_B_B_T                              (EFX_EFFECT_PUSH_CLASS+198)
#define EFX_EFFECT_PUSH4_T_B_B_B                              (EFX_EFFECT_PUSH_CLASS+199)
#define EFX_EFFECT_PUSH4_B_L_L_L                              (EFX_EFFECT_PUSH_CLASS+200)
#define EFX_EFFECT_PUSH4_B_L_L_R                              (EFX_EFFECT_PUSH_CLASS+201)
#define EFX_EFFECT_PUSH4_B_L_L_T                              (EFX_EFFECT_PUSH_CLASS+202)
#define EFX_EFFECT_PUSH4_B_L_L_B                              (EFX_EFFECT_PUSH_CLASS+203)
#define EFX_EFFECT_PUSH4_B_L_R_L                              (EFX_EFFECT_PUSH_CLASS+204)
#define EFX_EFFECT_PUSH4_B_L_R_R                              (EFX_EFFECT_PUSH_CLASS+205)
#define EFX_EFFECT_PUSH4_B_L_R_T                              (EFX_EFFECT_PUSH_CLASS+206)
#define EFX_EFFECT_PUSH4_B_L_R_B                              (EFX_EFFECT_PUSH_CLASS+207)
#define EFX_EFFECT_PUSH4_B_L_T_L                              (EFX_EFFECT_PUSH_CLASS+208)
#define EFX_EFFECT_PUSH4_B_L_T_R                              (EFX_EFFECT_PUSH_CLASS+209)
#define EFX_EFFECT_PUSH4_B_L_T_T                              (EFX_EFFECT_PUSH_CLASS+210)
#define EFX_EFFECT_PUSH4_B_L_T_B                              (EFX_EFFECT_PUSH_CLASS+211)
#define EFX_EFFECT_PUSH4_B_L_B_L                              (EFX_EFFECT_PUSH_CLASS+212)
#define EFX_EFFECT_PUSH4_B_L_B_R                              (EFX_EFFECT_PUSH_CLASS+213)
#define EFX_EFFECT_PUSH4_B_L_B_T                              (EFX_EFFECT_PUSH_CLASS+214)
#define EFX_EFFECT_PUSH4_B_L_B_B                              (EFX_EFFECT_PUSH_CLASS+215)
#define EFX_EFFECT_PUSH4_B_R_L_L                              (EFX_EFFECT_PUSH_CLASS+216)
#define EFX_EFFECT_PUSH4_B_R_L_R                              (EFX_EFFECT_PUSH_CLASS+217)
#define EFX_EFFECT_PUSH4_B_R_L_T                              (EFX_EFFECT_PUSH_CLASS+218)
#define EFX_EFFECT_PUSH4_B_R_L_B                              (EFX_EFFECT_PUSH_CLASS+219)
#define EFX_EFFECT_PUSH4_B_R_R_L                              (EFX_EFFECT_PUSH_CLASS+220)
#define EFX_EFFECT_PUSH4_B_R_R_R                              (EFX_EFFECT_PUSH_CLASS+221)
#define EFX_EFFECT_PUSH4_B_R_R_T                              (EFX_EFFECT_PUSH_CLASS+222)
#define EFX_EFFECT_PUSH4_B_R_R_B                              (EFX_EFFECT_PUSH_CLASS+223)
#define EFX_EFFECT_PUSH4_B_R_T_L                              (EFX_EFFECT_PUSH_CLASS+224)
#define EFX_EFFECT_PUSH4_B_R_T_R                              (EFX_EFFECT_PUSH_CLASS+225)
#define EFX_EFFECT_PUSH4_B_R_T_T                              (EFX_EFFECT_PUSH_CLASS+226)
#define EFX_EFFECT_PUSH4_B_R_T_B                              (EFX_EFFECT_PUSH_CLASS+227)
#define EFX_EFFECT_PUSH4_B_R_B_L                              (EFX_EFFECT_PUSH_CLASS+228)
#define EFX_EFFECT_PUSH4_B_R_B_R                              (EFX_EFFECT_PUSH_CLASS+229)
#define EFX_EFFECT_PUSH4_B_R_B_T                              (EFX_EFFECT_PUSH_CLASS+230)
#define EFX_EFFECT_PUSH4_B_R_B_B                              (EFX_EFFECT_PUSH_CLASS+231)
#define EFX_EFFECT_PUSH4_B_T_L_L                              (EFX_EFFECT_PUSH_CLASS+232)
#define EFX_EFFECT_PUSH4_B_T_L_R                              (EFX_EFFECT_PUSH_CLASS+233)
#define EFX_EFFECT_PUSH4_B_T_L_T                              (EFX_EFFECT_PUSH_CLASS+234)
#define EFX_EFFECT_PUSH4_B_T_L_B                              (EFX_EFFECT_PUSH_CLASS+235)
#define EFX_EFFECT_PUSH4_B_T_R_L                              (EFX_EFFECT_PUSH_CLASS+236)
#define EFX_EFFECT_PUSH4_B_T_R_R                              (EFX_EFFECT_PUSH_CLASS+237)
#define EFX_EFFECT_PUSH4_B_T_R_T                              (EFX_EFFECT_PUSH_CLASS+238)
#define EFX_EFFECT_PUSH4_B_T_R_B                              (EFX_EFFECT_PUSH_CLASS+239)
#define EFX_EFFECT_PUSH4_B_T_T_L                              (EFX_EFFECT_PUSH_CLASS+240)
#define EFX_EFFECT_PUSH4_B_T_T_R                              (EFX_EFFECT_PUSH_CLASS+241)
#define EFX_EFFECT_PUSH4_B_T_T_T                              (EFX_EFFECT_PUSH_CLASS+242)
#define EFX_EFFECT_PUSH4_B_T_T_B                              (EFX_EFFECT_PUSH_CLASS+243)
#define EFX_EFFECT_PUSH4_B_T_B_L                              (EFX_EFFECT_PUSH_CLASS+244)
#define EFX_EFFECT_PUSH4_B_T_B_R                              (EFX_EFFECT_PUSH_CLASS+245)
#define EFX_EFFECT_PUSH4_B_T_B_T                              (EFX_EFFECT_PUSH_CLASS+246)
#define EFX_EFFECT_PUSH4_B_T_B_B                              (EFX_EFFECT_PUSH_CLASS+247)
#define EFX_EFFECT_PUSH4_B_B_L_L                              (EFX_EFFECT_PUSH_CLASS+248)
#define EFX_EFFECT_PUSH4_B_B_L_R                              (EFX_EFFECT_PUSH_CLASS+249)
#define EFX_EFFECT_PUSH4_B_B_L_T                              (EFX_EFFECT_PUSH_CLASS+250)
#define EFX_EFFECT_PUSH4_B_B_L_B                              (EFX_EFFECT_PUSH_CLASS+251)
#define EFX_EFFECT_PUSH4_B_B_R_L                              (EFX_EFFECT_PUSH_CLASS+252)
#define EFX_EFFECT_PUSH4_B_B_R_R                              (EFX_EFFECT_PUSH_CLASS+253)
#define EFX_EFFECT_PUSH4_B_B_R_T                              (EFX_EFFECT_PUSH_CLASS+254)
#define EFX_EFFECT_PUSH4_B_B_R_B                              (EFX_EFFECT_PUSH_CLASS+255)
#define EFX_EFFECT_PUSH4_B_B_T_L                              (EFX_EFFECT_PUSH_CLASS+256)
#define EFX_EFFECT_PUSH4_B_B_T_R                              (EFX_EFFECT_PUSH_CLASS+257)
#define EFX_EFFECT_PUSH4_B_B_T_T                              (EFX_EFFECT_PUSH_CLASS+258)
#define EFX_EFFECT_PUSH4_B_B_T_B                              (EFX_EFFECT_PUSH_CLASS+259)
#define EFX_EFFECT_PUSH4_B_B_B_L                              (EFX_EFFECT_PUSH_CLASS+260)
#define EFX_EFFECT_PUSH4_B_B_B_R                              (EFX_EFFECT_PUSH_CLASS+261)
#define EFX_EFFECT_PUSH4_B_B_B_T                              (EFX_EFFECT_PUSH_CLASS+262)
#define EFX_EFFECT_PUSH4_B_B_B_B                              (EFX_EFFECT_PUSH_CLASS+263)
#define EFX_EFFECT_PUSH_MAX                                   (EFX_EFFECT_PUSH_CLASS+263)

// Slide
#define EFX_EFFECT_SLIDE_CLASS                                6000
#define EFX_EFFECT_SLIDE_R_TO_L                               (EFX_EFFECT_SLIDE_CLASS+0)
#define EFX_EFFECT_SLIDE_L_TO_R                               (EFX_EFFECT_SLIDE_CLASS+1)
#define EFX_EFFECT_SLIDE_B_TO_T                               (EFX_EFFECT_SLIDE_CLASS+2)
#define EFX_EFFECT_SLIDE_T_TO_B                               (EFX_EFFECT_SLIDE_CLASS+3)
#define EFX_EFFECT_SLIDE_LT_TO_RB                             (EFX_EFFECT_SLIDE_CLASS+4)
#define EFX_EFFECT_SLIDE_RT_TO_LB                             (EFX_EFFECT_SLIDE_CLASS+5)
#define EFX_EFFECT_SLIDE_LB_TO_RT                             (EFX_EFFECT_SLIDE_CLASS+6)
#define EFX_EFFECT_SLIDE_RB_TO_LT                             (EFX_EFFECT_SLIDE_CLASS+7)
#define EFX_EFFECT_SLIDE_INTERH_L_TO_R_AND_R_TO_L             (EFX_EFFECT_SLIDE_CLASS+8)
#define EFX_EFFECT_SLIDE_INTERV_T_TO_B_AND_B_TO_T             (EFX_EFFECT_SLIDE_CLASS+9)
#define EFX_EFFECT_SLIDE2_T_R_TO_L_AND_B_L_TO_R               (EFX_EFFECT_SLIDE_CLASS+10)
#define EFX_EFFECT_SLIDE2_T_L_TO_R_AND_B_R_TO_L               (EFX_EFFECT_SLIDE_CLASS+11)
#define EFX_EFFECT_SLIDE2_L_B_TO_T_AND_R_T_TO_B               (EFX_EFFECT_SLIDE_CLASS+12)
#define EFX_EFFECT_SLIDE2_L_T_TO_B_AND_R_B_TO_T               (EFX_EFFECT_SLIDE_CLASS+13)
#define EFX_EFFECT_SLIDE4_L_L_L_L                             (EFX_EFFECT_SLIDE_CLASS+14)
#define EFX_EFFECT_SLIDE4_L_L_L_R                             (EFX_EFFECT_SLIDE_CLASS+15)
#define EFX_EFFECT_SLIDE4_L_L_L_T                             (EFX_EFFECT_SLIDE_CLASS+16)
#define EFX_EFFECT_SLIDE4_L_L_L_B                             (EFX_EFFECT_SLIDE_CLASS+17)
#define EFX_EFFECT_SLIDE4_L_L_R_L                             (EFX_EFFECT_SLIDE_CLASS+18)
#define EFX_EFFECT_SLIDE4_L_L_R_R                             (EFX_EFFECT_SLIDE_CLASS+19)
#define EFX_EFFECT_SLIDE4_L_L_R_T                             (EFX_EFFECT_SLIDE_CLASS+20)
#define EFX_EFFECT_SLIDE4_L_L_R_B                             (EFX_EFFECT_SLIDE_CLASS+21)
#define EFX_EFFECT_SLIDE4_L_L_T_L                             (EFX_EFFECT_SLIDE_CLASS+22)
#define EFX_EFFECT_SLIDE4_L_L_T_R                             (EFX_EFFECT_SLIDE_CLASS+23)
#define EFX_EFFECT_SLIDE4_L_L_T_T                             (EFX_EFFECT_SLIDE_CLASS+24)
#define EFX_EFFECT_SLIDE4_L_L_T_B                             (EFX_EFFECT_SLIDE_CLASS+25)
#define EFX_EFFECT_SLIDE4_L_L_B_L                             (EFX_EFFECT_SLIDE_CLASS+26)
#define EFX_EFFECT_SLIDE4_L_L_B_R                             (EFX_EFFECT_SLIDE_CLASS+27)
#define EFX_EFFECT_SLIDE4_L_L_B_T                             (EFX_EFFECT_SLIDE_CLASS+28)
#define EFX_EFFECT_SLIDE4_L_L_B_B                             (EFX_EFFECT_SLIDE_CLASS+29)
#define EFX_EFFECT_SLIDE4_L_R_L_L                             (EFX_EFFECT_SLIDE_CLASS+30)
#define EFX_EFFECT_SLIDE4_L_R_L_R                             (EFX_EFFECT_SLIDE_CLASS+31)
#define EFX_EFFECT_SLIDE4_L_R_L_T                             (EFX_EFFECT_SLIDE_CLASS+32)
#define EFX_EFFECT_SLIDE4_L_R_L_B                             (EFX_EFFECT_SLIDE_CLASS+33)
#define EFX_EFFECT_SLIDE4_L_R_R_L                             (EFX_EFFECT_SLIDE_CLASS+34)
#define EFX_EFFECT_SLIDE4_L_R_R_R                             (EFX_EFFECT_SLIDE_CLASS+35)
#define EFX_EFFECT_SLIDE4_L_R_R_T                             (EFX_EFFECT_SLIDE_CLASS+36)
#define EFX_EFFECT_SLIDE4_L_R_R_B                             (EFX_EFFECT_SLIDE_CLASS+37)
#define EFX_EFFECT_SLIDE4_L_R_T_L                             (EFX_EFFECT_SLIDE_CLASS+38)
#define EFX_EFFECT_SLIDE4_L_R_T_R                             (EFX_EFFECT_SLIDE_CLASS+39)
#define EFX_EFFECT_SLIDE4_L_R_T_T                             (EFX_EFFECT_SLIDE_CLASS+40)
#define EFX_EFFECT_SLIDE4_L_R_T_B                             (EFX_EFFECT_SLIDE_CLASS+41)
#define EFX_EFFECT_SLIDE4_L_R_B_L                             (EFX_EFFECT_SLIDE_CLASS+42)
#define EFX_EFFECT_SLIDE4_L_R_B_R                             (EFX_EFFECT_SLIDE_CLASS+43)
#define EFX_EFFECT_SLIDE4_L_R_B_T                             (EFX_EFFECT_SLIDE_CLASS+44)
#define EFX_EFFECT_SLIDE4_L_R_B_B                             (EFX_EFFECT_SLIDE_CLASS+45)
#define EFX_EFFECT_SLIDE4_L_T_L_L                             (EFX_EFFECT_SLIDE_CLASS+46)
#define EFX_EFFECT_SLIDE4_L_T_L_R                             (EFX_EFFECT_SLIDE_CLASS+47)
#define EFX_EFFECT_SLIDE4_L_T_L_T                             (EFX_EFFECT_SLIDE_CLASS+48)
#define EFX_EFFECT_SLIDE4_L_T_L_B                             (EFX_EFFECT_SLIDE_CLASS+49)
#define EFX_EFFECT_SLIDE4_L_T_R_L                             (EFX_EFFECT_SLIDE_CLASS+50)
#define EFX_EFFECT_SLIDE4_L_T_R_R                             (EFX_EFFECT_SLIDE_CLASS+51)
#define EFX_EFFECT_SLIDE4_L_T_R_T                             (EFX_EFFECT_SLIDE_CLASS+52)
#define EFX_EFFECT_SLIDE4_L_T_R_B                             (EFX_EFFECT_SLIDE_CLASS+53)
#define EFX_EFFECT_SLIDE4_L_T_T_L                             (EFX_EFFECT_SLIDE_CLASS+54)
#define EFX_EFFECT_SLIDE4_L_T_T_R                             (EFX_EFFECT_SLIDE_CLASS+55)
#define EFX_EFFECT_SLIDE4_L_T_T_T                             (EFX_EFFECT_SLIDE_CLASS+56)
#define EFX_EFFECT_SLIDE4_L_T_T_B                             (EFX_EFFECT_SLIDE_CLASS+57)
#define EFX_EFFECT_SLIDE4_L_T_B_L                             (EFX_EFFECT_SLIDE_CLASS+58)
#define EFX_EFFECT_SLIDE4_L_T_B_R                             (EFX_EFFECT_SLIDE_CLASS+59)
#define EFX_EFFECT_SLIDE4_L_T_B_T                             (EFX_EFFECT_SLIDE_CLASS+60)
#define EFX_EFFECT_SLIDE4_L_T_B_B                             (EFX_EFFECT_SLIDE_CLASS+61)
#define EFX_EFFECT_SLIDE4_L_B_L_L                             (EFX_EFFECT_SLIDE_CLASS+62)
#define EFX_EFFECT_SLIDE4_L_B_L_R                             (EFX_EFFECT_SLIDE_CLASS+63)
#define EFX_EFFECT_SLIDE4_L_B_L_T                             (EFX_EFFECT_SLIDE_CLASS+64)
#define EFX_EFFECT_SLIDE4_L_B_L_B                             (EFX_EFFECT_SLIDE_CLASS+65)
#define EFX_EFFECT_SLIDE4_L_B_R_L                             (EFX_EFFECT_SLIDE_CLASS+66)
#define EFX_EFFECT_SLIDE4_L_B_R_R                             (EFX_EFFECT_SLIDE_CLASS+67)
#define EFX_EFFECT_SLIDE4_L_B_R_T                             (EFX_EFFECT_SLIDE_CLASS+68)
#define EFX_EFFECT_SLIDE4_L_B_R_B                             (EFX_EFFECT_SLIDE_CLASS+69)
#define EFX_EFFECT_SLIDE4_L_B_T_L                             (EFX_EFFECT_SLIDE_CLASS+70)
#define EFX_EFFECT_SLIDE4_L_B_T_R                             (EFX_EFFECT_SLIDE_CLASS+71)
#define EFX_EFFECT_SLIDE4_L_B_T_T                             (EFX_EFFECT_SLIDE_CLASS+72)
#define EFX_EFFECT_SLIDE4_L_B_T_B                             (EFX_EFFECT_SLIDE_CLASS+73)
#define EFX_EFFECT_SLIDE4_L_B_B_L                             (EFX_EFFECT_SLIDE_CLASS+74)
#define EFX_EFFECT_SLIDE4_L_B_B_R                             (EFX_EFFECT_SLIDE_CLASS+75)
#define EFX_EFFECT_SLIDE4_L_B_B_T                             (EFX_EFFECT_SLIDE_CLASS+76)
#define EFX_EFFECT_SLIDE4_L_B_B_B                             (EFX_EFFECT_SLIDE_CLASS+77)
#define EFX_EFFECT_SLIDE4_R_L_L_L                             (EFX_EFFECT_SLIDE_CLASS+78)
#define EFX_EFFECT_SLIDE4_R_L_L_R                             (EFX_EFFECT_SLIDE_CLASS+79)
#define EFX_EFFECT_SLIDE4_R_L_L_T                             (EFX_EFFECT_SLIDE_CLASS+80)
#define EFX_EFFECT_SLIDE4_R_L_L_B                             (EFX_EFFECT_SLIDE_CLASS+81)
#define EFX_EFFECT_SLIDE4_R_L_R_L                             (EFX_EFFECT_SLIDE_CLASS+82)
#define EFX_EFFECT_SLIDE4_R_L_R_R                             (EFX_EFFECT_SLIDE_CLASS+83)
#define EFX_EFFECT_SLIDE4_R_L_R_T                             (EFX_EFFECT_SLIDE_CLASS+84)
#define EFX_EFFECT_SLIDE4_R_L_R_B                             (EFX_EFFECT_SLIDE_CLASS+85)
#define EFX_EFFECT_SLIDE4_R_L_T_L                             (EFX_EFFECT_SLIDE_CLASS+86)
#define EFX_EFFECT_SLIDE4_R_L_T_R                             (EFX_EFFECT_SLIDE_CLASS+87)
#define EFX_EFFECT_SLIDE4_R_L_T_T                             (EFX_EFFECT_SLIDE_CLASS+88)
#define EFX_EFFECT_SLIDE4_R_L_T_B                             (EFX_EFFECT_SLIDE_CLASS+89)
#define EFX_EFFECT_SLIDE4_R_L_B_L                             (EFX_EFFECT_SLIDE_CLASS+90)
#define EFX_EFFECT_SLIDE4_R_L_B_R                             (EFX_EFFECT_SLIDE_CLASS+91)
#define EFX_EFFECT_SLIDE4_R_L_B_T                             (EFX_EFFECT_SLIDE_CLASS+92)
#define EFX_EFFECT_SLIDE4_R_L_B_B                             (EFX_EFFECT_SLIDE_CLASS+93)
#define EFX_EFFECT_SLIDE4_R_R_L_L                             (EFX_EFFECT_SLIDE_CLASS+94)
#define EFX_EFFECT_SLIDE4_R_R_L_R                             (EFX_EFFECT_SLIDE_CLASS+95)
#define EFX_EFFECT_SLIDE4_R_R_L_T                             (EFX_EFFECT_SLIDE_CLASS+96)
#define EFX_EFFECT_SLIDE4_R_R_L_B                             (EFX_EFFECT_SLIDE_CLASS+97)
#define EFX_EFFECT_SLIDE4_R_R_R_L                             (EFX_EFFECT_SLIDE_CLASS+98)
#define EFX_EFFECT_SLIDE4_R_R_R_R                             (EFX_EFFECT_SLIDE_CLASS+99)
#define EFX_EFFECT_SLIDE4_R_R_R_T                             (EFX_EFFECT_SLIDE_CLASS+100)
#define EFX_EFFECT_SLIDE4_R_R_R_B                             (EFX_EFFECT_SLIDE_CLASS+101)
#define EFX_EFFECT_SLIDE4_R_R_T_L                             (EFX_EFFECT_SLIDE_CLASS+102)
#define EFX_EFFECT_SLIDE4_R_R_T_R                             (EFX_EFFECT_SLIDE_CLASS+103)
#define EFX_EFFECT_SLIDE4_R_R_T_T                             (EFX_EFFECT_SLIDE_CLASS+104)
#define EFX_EFFECT_SLIDE4_R_R_T_B                             (EFX_EFFECT_SLIDE_CLASS+105)
#define EFX_EFFECT_SLIDE4_R_R_B_L                             (EFX_EFFECT_SLIDE_CLASS+106)
#define EFX_EFFECT_SLIDE4_R_R_B_R                             (EFX_EFFECT_SLIDE_CLASS+107)
#define EFX_EFFECT_SLIDE4_R_R_B_T                             (EFX_EFFECT_SLIDE_CLASS+108)
#define EFX_EFFECT_SLIDE4_R_R_B_B                             (EFX_EFFECT_SLIDE_CLASS+109)
#define EFX_EFFECT_SLIDE4_R_T_L_L                             (EFX_EFFECT_SLIDE_CLASS+110)
#define EFX_EFFECT_SLIDE4_R_T_L_R                             (EFX_EFFECT_SLIDE_CLASS+111)
#define EFX_EFFECT_SLIDE4_R_T_L_T                             (EFX_EFFECT_SLIDE_CLASS+112)
#define EFX_EFFECT_SLIDE4_R_T_L_B                             (EFX_EFFECT_SLIDE_CLASS+113)
#define EFX_EFFECT_SLIDE4_R_T_R_L                             (EFX_EFFECT_SLIDE_CLASS+114)
#define EFX_EFFECT_SLIDE4_R_T_R_R                             (EFX_EFFECT_SLIDE_CLASS+115)
#define EFX_EFFECT_SLIDE4_R_T_R_T                             (EFX_EFFECT_SLIDE_CLASS+116)
#define EFX_EFFECT_SLIDE4_R_T_R_B                             (EFX_EFFECT_SLIDE_CLASS+117)
#define EFX_EFFECT_SLIDE4_R_T_T_L                             (EFX_EFFECT_SLIDE_CLASS+118)
#define EFX_EFFECT_SLIDE4_R_T_T_R                             (EFX_EFFECT_SLIDE_CLASS+119)
#define EFX_EFFECT_SLIDE4_R_T_T_T                             (EFX_EFFECT_SLIDE_CLASS+120)
#define EFX_EFFECT_SLIDE4_R_T_T_B                             (EFX_EFFECT_SLIDE_CLASS+121)
#define EFX_EFFECT_SLIDE4_R_T_B_L                             (EFX_EFFECT_SLIDE_CLASS+122)
#define EFX_EFFECT_SLIDE4_R_T_B_R                             (EFX_EFFECT_SLIDE_CLASS+123)
#define EFX_EFFECT_SLIDE4_R_T_B_T                             (EFX_EFFECT_SLIDE_CLASS+124)
#define EFX_EFFECT_SLIDE4_R_T_B_B                             (EFX_EFFECT_SLIDE_CLASS+125)
#define EFX_EFFECT_SLIDE4_R_B_L_L                             (EFX_EFFECT_SLIDE_CLASS+126)
#define EFX_EFFECT_SLIDE4_R_B_L_R                             (EFX_EFFECT_SLIDE_CLASS+127)
#define EFX_EFFECT_SLIDE4_R_B_L_T                             (EFX_EFFECT_SLIDE_CLASS+128)
#define EFX_EFFECT_SLIDE4_R_B_L_B                             (EFX_EFFECT_SLIDE_CLASS+129)
#define EFX_EFFECT_SLIDE4_R_B_R_L                             (EFX_EFFECT_SLIDE_CLASS+130)
#define EFX_EFFECT_SLIDE4_R_B_R_R                             (EFX_EFFECT_SLIDE_CLASS+131)
#define EFX_EFFECT_SLIDE4_R_B_R_T                             (EFX_EFFECT_SLIDE_CLASS+132)
#define EFX_EFFECT_SLIDE4_R_B_R_B                             (EFX_EFFECT_SLIDE_CLASS+133)
#define EFX_EFFECT_SLIDE4_R_B_T_L                             (EFX_EFFECT_SLIDE_CLASS+134)
#define EFX_EFFECT_SLIDE4_R_B_T_R                             (EFX_EFFECT_SLIDE_CLASS+135)
#define EFX_EFFECT_SLIDE4_R_B_T_T                             (EFX_EFFECT_SLIDE_CLASS+136)
#define EFX_EFFECT_SLIDE4_R_B_T_B                             (EFX_EFFECT_SLIDE_CLASS+137)
#define EFX_EFFECT_SLIDE4_R_B_B_L                             (EFX_EFFECT_SLIDE_CLASS+138)
#define EFX_EFFECT_SLIDE4_R_B_B_R                             (EFX_EFFECT_SLIDE_CLASS+139)
#define EFX_EFFECT_SLIDE4_R_B_B_T                             (EFX_EFFECT_SLIDE_CLASS+140)
#define EFX_EFFECT_SLIDE4_R_B_B_B                             (EFX_EFFECT_SLIDE_CLASS+141)
#define EFX_EFFECT_SLIDE4_T_L_L_L                             (EFX_EFFECT_SLIDE_CLASS+142)
#define EFX_EFFECT_SLIDE4_T_L_L_R                             (EFX_EFFECT_SLIDE_CLASS+143)
#define EFX_EFFECT_SLIDE4_T_L_L_T                             (EFX_EFFECT_SLIDE_CLASS+144)
#define EFX_EFFECT_SLIDE4_T_L_L_B                             (EFX_EFFECT_SLIDE_CLASS+145)
#define EFX_EFFECT_SLIDE4_T_L_R_L                             (EFX_EFFECT_SLIDE_CLASS+146)
#define EFX_EFFECT_SLIDE4_T_L_R_R                             (EFX_EFFECT_SLIDE_CLASS+147)
#define EFX_EFFECT_SLIDE4_T_L_R_T                             (EFX_EFFECT_SLIDE_CLASS+148)
#define EFX_EFFECT_SLIDE4_T_L_R_B                             (EFX_EFFECT_SLIDE_CLASS+149)
#define EFX_EFFECT_SLIDE4_T_L_T_L                             (EFX_EFFECT_SLIDE_CLASS+150)
#define EFX_EFFECT_SLIDE4_T_L_T_R                             (EFX_EFFECT_SLIDE_CLASS+151)
#define EFX_EFFECT_SLIDE4_T_L_T_T                             (EFX_EFFECT_SLIDE_CLASS+152)
#define EFX_EFFECT_SLIDE4_T_L_T_B                             (EFX_EFFECT_SLIDE_CLASS+153)
#define EFX_EFFECT_SLIDE4_T_L_B_L                             (EFX_EFFECT_SLIDE_CLASS+154)
#define EFX_EFFECT_SLIDE4_T_L_B_R                             (EFX_EFFECT_SLIDE_CLASS+155)
#define EFX_EFFECT_SLIDE4_T_L_B_T                             (EFX_EFFECT_SLIDE_CLASS+156)
#define EFX_EFFECT_SLIDE4_T_L_B_B                             (EFX_EFFECT_SLIDE_CLASS+157)
#define EFX_EFFECT_SLIDE4_T_R_L_L                             (EFX_EFFECT_SLIDE_CLASS+158)
#define EFX_EFFECT_SLIDE4_T_R_L_R                             (EFX_EFFECT_SLIDE_CLASS+159)
#define EFX_EFFECT_SLIDE4_T_R_L_T                             (EFX_EFFECT_SLIDE_CLASS+160)
#define EFX_EFFECT_SLIDE4_T_R_L_B                             (EFX_EFFECT_SLIDE_CLASS+161)
#define EFX_EFFECT_SLIDE4_T_R_R_L                             (EFX_EFFECT_SLIDE_CLASS+162)
#define EFX_EFFECT_SLIDE4_T_R_R_R                             (EFX_EFFECT_SLIDE_CLASS+163)
#define EFX_EFFECT_SLIDE4_T_R_R_T                             (EFX_EFFECT_SLIDE_CLASS+164)
#define EFX_EFFECT_SLIDE4_T_R_R_B                             (EFX_EFFECT_SLIDE_CLASS+165)
#define EFX_EFFECT_SLIDE4_T_R_T_L                             (EFX_EFFECT_SLIDE_CLASS+166)
#define EFX_EFFECT_SLIDE4_T_R_T_R                             (EFX_EFFECT_SLIDE_CLASS+167)
#define EFX_EFFECT_SLIDE4_T_R_T_T                             (EFX_EFFECT_SLIDE_CLASS+168)
#define EFX_EFFECT_SLIDE4_T_R_T_B                             (EFX_EFFECT_SLIDE_CLASS+169)
#define EFX_EFFECT_SLIDE4_T_R_B_L                             (EFX_EFFECT_SLIDE_CLASS+170)
#define EFX_EFFECT_SLIDE4_T_R_B_R                             (EFX_EFFECT_SLIDE_CLASS+171)
#define EFX_EFFECT_SLIDE4_T_R_B_T                             (EFX_EFFECT_SLIDE_CLASS+172)
#define EFX_EFFECT_SLIDE4_T_R_B_B                             (EFX_EFFECT_SLIDE_CLASS+173)
#define EFX_EFFECT_SLIDE4_T_T_L_L                             (EFX_EFFECT_SLIDE_CLASS+174)
#define EFX_EFFECT_SLIDE4_T_T_L_R                             (EFX_EFFECT_SLIDE_CLASS+175)
#define EFX_EFFECT_SLIDE4_T_T_L_T                             (EFX_EFFECT_SLIDE_CLASS+176)
#define EFX_EFFECT_SLIDE4_T_T_L_B                             (EFX_EFFECT_SLIDE_CLASS+177)
#define EFX_EFFECT_SLIDE4_T_T_R_L                             (EFX_EFFECT_SLIDE_CLASS+178)
#define EFX_EFFECT_SLIDE4_T_T_R_R                             (EFX_EFFECT_SLIDE_CLASS+179)
#define EFX_EFFECT_SLIDE4_T_T_R_T                             (EFX_EFFECT_SLIDE_CLASS+180)
#define EFX_EFFECT_SLIDE4_T_T_R_B                             (EFX_EFFECT_SLIDE_CLASS+181)
#define EFX_EFFECT_SLIDE4_T_T_T_L                             (EFX_EFFECT_SLIDE_CLASS+182)
#define EFX_EFFECT_SLIDE4_T_T_T_R                             (EFX_EFFECT_SLIDE_CLASS+183)
#define EFX_EFFECT_SLIDE4_T_T_T_T                             (EFX_EFFECT_SLIDE_CLASS+184)
#define EFX_EFFECT_SLIDE4_T_T_T_B                             (EFX_EFFECT_SLIDE_CLASS+185)
#define EFX_EFFECT_SLIDE4_T_T_B_L                             (EFX_EFFECT_SLIDE_CLASS+186)
#define EFX_EFFECT_SLIDE4_T_T_B_R                             (EFX_EFFECT_SLIDE_CLASS+187)
#define EFX_EFFECT_SLIDE4_T_T_B_T                             (EFX_EFFECT_SLIDE_CLASS+188)
#define EFX_EFFECT_SLIDE4_T_T_B_B                             (EFX_EFFECT_SLIDE_CLASS+189)
#define EFX_EFFECT_SLIDE4_T_B_L_L                             (EFX_EFFECT_SLIDE_CLASS+190)
#define EFX_EFFECT_SLIDE4_T_B_L_R                             (EFX_EFFECT_SLIDE_CLASS+191)
#define EFX_EFFECT_SLIDE4_T_B_L_T                             (EFX_EFFECT_SLIDE_CLASS+192)
#define EFX_EFFECT_SLIDE4_T_B_L_B                             (EFX_EFFECT_SLIDE_CLASS+193)
#define EFX_EFFECT_SLIDE4_T_B_R_L                             (EFX_EFFECT_SLIDE_CLASS+194)
#define EFX_EFFECT_SLIDE4_T_B_R_R                             (EFX_EFFECT_SLIDE_CLASS+195)
#define EFX_EFFECT_SLIDE4_T_B_R_T                             (EFX_EFFECT_SLIDE_CLASS+196)
#define EFX_EFFECT_SLIDE4_T_B_R_B                             (EFX_EFFECT_SLIDE_CLASS+197)
#define EFX_EFFECT_SLIDE4_T_B_T_L                             (EFX_EFFECT_SLIDE_CLASS+198)
#define EFX_EFFECT_SLIDE4_T_B_T_R                             (EFX_EFFECT_SLIDE_CLASS+199)
#define EFX_EFFECT_SLIDE4_T_B_T_T                             (EFX_EFFECT_SLIDE_CLASS+200)
#define EFX_EFFECT_SLIDE4_T_B_T_B                             (EFX_EFFECT_SLIDE_CLASS+201)
#define EFX_EFFECT_SLIDE4_T_B_B_L                             (EFX_EFFECT_SLIDE_CLASS+202)
#define EFX_EFFECT_SLIDE4_T_B_B_R                             (EFX_EFFECT_SLIDE_CLASS+203)
#define EFX_EFFECT_SLIDE4_T_B_B_T                             (EFX_EFFECT_SLIDE_CLASS+204)
#define EFX_EFFECT_SLIDE4_T_B_B_B                             (EFX_EFFECT_SLIDE_CLASS+205)
#define EFX_EFFECT_SLIDE4_B_L_L_L                             (EFX_EFFECT_SLIDE_CLASS+206)
#define EFX_EFFECT_SLIDE4_B_L_L_R                             (EFX_EFFECT_SLIDE_CLASS+207)
#define EFX_EFFECT_SLIDE4_B_L_L_T                             (EFX_EFFECT_SLIDE_CLASS+208)
#define EFX_EFFECT_SLIDE4_B_L_L_B                             (EFX_EFFECT_SLIDE_CLASS+209)
#define EFX_EFFECT_SLIDE4_B_L_R_L                             (EFX_EFFECT_SLIDE_CLASS+210)
#define EFX_EFFECT_SLIDE4_B_L_R_R                             (EFX_EFFECT_SLIDE_CLASS+211)
#define EFX_EFFECT_SLIDE4_B_L_R_T                             (EFX_EFFECT_SLIDE_CLASS+212)
#define EFX_EFFECT_SLIDE4_B_L_R_B                             (EFX_EFFECT_SLIDE_CLASS+213)
#define EFX_EFFECT_SLIDE4_B_L_T_L                             (EFX_EFFECT_SLIDE_CLASS+214)
#define EFX_EFFECT_SLIDE4_B_L_T_R                             (EFX_EFFECT_SLIDE_CLASS+215)
#define EFX_EFFECT_SLIDE4_B_L_T_T                             (EFX_EFFECT_SLIDE_CLASS+216)
#define EFX_EFFECT_SLIDE4_B_L_T_B                             (EFX_EFFECT_SLIDE_CLASS+217)
#define EFX_EFFECT_SLIDE4_B_L_B_L                             (EFX_EFFECT_SLIDE_CLASS+218)
#define EFX_EFFECT_SLIDE4_B_L_B_R                             (EFX_EFFECT_SLIDE_CLASS+219)
#define EFX_EFFECT_SLIDE4_B_L_B_T                             (EFX_EFFECT_SLIDE_CLASS+220)
#define EFX_EFFECT_SLIDE4_B_L_B_B                             (EFX_EFFECT_SLIDE_CLASS+221)
#define EFX_EFFECT_SLIDE4_B_R_L_L                             (EFX_EFFECT_SLIDE_CLASS+222)
#define EFX_EFFECT_SLIDE4_B_R_L_R                             (EFX_EFFECT_SLIDE_CLASS+223)
#define EFX_EFFECT_SLIDE4_B_R_L_T                             (EFX_EFFECT_SLIDE_CLASS+224)
#define EFX_EFFECT_SLIDE4_B_R_L_B                             (EFX_EFFECT_SLIDE_CLASS+225)
#define EFX_EFFECT_SLIDE4_B_R_R_L                             (EFX_EFFECT_SLIDE_CLASS+226)
#define EFX_EFFECT_SLIDE4_B_R_R_R                             (EFX_EFFECT_SLIDE_CLASS+227)
#define EFX_EFFECT_SLIDE4_B_R_R_T                             (EFX_EFFECT_SLIDE_CLASS+228)
#define EFX_EFFECT_SLIDE4_B_R_R_B                             (EFX_EFFECT_SLIDE_CLASS+229)
#define EFX_EFFECT_SLIDE4_B_R_T_L                             (EFX_EFFECT_SLIDE_CLASS+230)
#define EFX_EFFECT_SLIDE4_B_R_T_R                             (EFX_EFFECT_SLIDE_CLASS+231)
#define EFX_EFFECT_SLIDE4_B_R_T_T                             (EFX_EFFECT_SLIDE_CLASS+232)
#define EFX_EFFECT_SLIDE4_B_R_T_B                             (EFX_EFFECT_SLIDE_CLASS+233)
#define EFX_EFFECT_SLIDE4_B_R_B_L                             (EFX_EFFECT_SLIDE_CLASS+234)
#define EFX_EFFECT_SLIDE4_B_R_B_R                             (EFX_EFFECT_SLIDE_CLASS+235)
#define EFX_EFFECT_SLIDE4_B_R_B_T                             (EFX_EFFECT_SLIDE_CLASS+236)
#define EFX_EFFECT_SLIDE4_B_R_B_B                             (EFX_EFFECT_SLIDE_CLASS+237)
#define EFX_EFFECT_SLIDE4_B_T_L_L                             (EFX_EFFECT_SLIDE_CLASS+238)
#define EFX_EFFECT_SLIDE4_B_T_L_R                             (EFX_EFFECT_SLIDE_CLASS+239)
#define EFX_EFFECT_SLIDE4_B_T_L_T                             (EFX_EFFECT_SLIDE_CLASS+240)
#define EFX_EFFECT_SLIDE4_B_T_L_B                             (EFX_EFFECT_SLIDE_CLASS+241)
#define EFX_EFFECT_SLIDE4_B_T_R_L                             (EFX_EFFECT_SLIDE_CLASS+242)
#define EFX_EFFECT_SLIDE4_B_T_R_R                             (EFX_EFFECT_SLIDE_CLASS+243)
#define EFX_EFFECT_SLIDE4_B_T_R_T                             (EFX_EFFECT_SLIDE_CLASS+244)
#define EFX_EFFECT_SLIDE4_B_T_R_B                             (EFX_EFFECT_SLIDE_CLASS+245)
#define EFX_EFFECT_SLIDE4_B_T_T_L                             (EFX_EFFECT_SLIDE_CLASS+246)
#define EFX_EFFECT_SLIDE4_B_T_T_R                             (EFX_EFFECT_SLIDE_CLASS+247)
#define EFX_EFFECT_SLIDE4_B_T_T_T                             (EFX_EFFECT_SLIDE_CLASS+248)
#define EFX_EFFECT_SLIDE4_B_T_T_B                             (EFX_EFFECT_SLIDE_CLASS+249)
#define EFX_EFFECT_SLIDE4_B_T_B_L                             (EFX_EFFECT_SLIDE_CLASS+250)
#define EFX_EFFECT_SLIDE4_B_T_B_R                             (EFX_EFFECT_SLIDE_CLASS+251)
#define EFX_EFFECT_SLIDE4_B_T_B_T                             (EFX_EFFECT_SLIDE_CLASS+252)
#define EFX_EFFECT_SLIDE4_B_T_B_B                             (EFX_EFFECT_SLIDE_CLASS+253)
#define EFX_EFFECT_SLIDE4_B_B_L_L                             (EFX_EFFECT_SLIDE_CLASS+254)
#define EFX_EFFECT_SLIDE4_B_B_L_R                             (EFX_EFFECT_SLIDE_CLASS+255)
#define EFX_EFFECT_SLIDE4_B_B_L_T                             (EFX_EFFECT_SLIDE_CLASS+256)
#define EFX_EFFECT_SLIDE4_B_B_L_B                             (EFX_EFFECT_SLIDE_CLASS+257)
#define EFX_EFFECT_SLIDE4_B_B_R_L                             (EFX_EFFECT_SLIDE_CLASS+258)
#define EFX_EFFECT_SLIDE4_B_B_R_R                             (EFX_EFFECT_SLIDE_CLASS+259)
#define EFX_EFFECT_SLIDE4_B_B_R_T                             (EFX_EFFECT_SLIDE_CLASS+260)
#define EFX_EFFECT_SLIDE4_B_B_R_B                             (EFX_EFFECT_SLIDE_CLASS+261)
#define EFX_EFFECT_SLIDE4_B_B_T_L                             (EFX_EFFECT_SLIDE_CLASS+262)
#define EFX_EFFECT_SLIDE4_B_B_T_R                             (EFX_EFFECT_SLIDE_CLASS+263)
#define EFX_EFFECT_SLIDE4_B_B_T_T                             (EFX_EFFECT_SLIDE_CLASS+264)
#define EFX_EFFECT_SLIDE4_B_B_T_B                             (EFX_EFFECT_SLIDE_CLASS+265)
#define EFX_EFFECT_SLIDE4_B_B_B_L                             (EFX_EFFECT_SLIDE_CLASS+266)
#define EFX_EFFECT_SLIDE4_B_B_B_R                             (EFX_EFFECT_SLIDE_CLASS+267)
#define EFX_EFFECT_SLIDE4_B_B_B_T                             (EFX_EFFECT_SLIDE_CLASS+268)
#define EFX_EFFECT_SLIDE4_B_B_B_B                             (EFX_EFFECT_SLIDE_CLASS+269)
#define EFX_EFFECT_SLIDE4_LT_LT_LT_LT                         (EFX_EFFECT_SLIDE_CLASS+270)
#define EFX_EFFECT_SLIDE4_LT_LT_LT_LB                         (EFX_EFFECT_SLIDE_CLASS+271)
#define EFX_EFFECT_SLIDE4_LT_LT_LT_RT                         (EFX_EFFECT_SLIDE_CLASS+272)
#define EFX_EFFECT_SLIDE4_LT_LT_LT_RB                         (EFX_EFFECT_SLIDE_CLASS+273)
#define EFX_EFFECT_SLIDE4_LT_LT_LB_LT                         (EFX_EFFECT_SLIDE_CLASS+274)
#define EFX_EFFECT_SLIDE4_LT_LT_LB_LB                         (EFX_EFFECT_SLIDE_CLASS+275)
#define EFX_EFFECT_SLIDE4_LT_LT_LB_RT                         (EFX_EFFECT_SLIDE_CLASS+276)
#define EFX_EFFECT_SLIDE4_LT_LT_LB_RB                         (EFX_EFFECT_SLIDE_CLASS+277)
#define EFX_EFFECT_SLIDE4_LT_LT_RT_LT                         (EFX_EFFECT_SLIDE_CLASS+278)
#define EFX_EFFECT_SLIDE4_LT_LT_RT_LB                         (EFX_EFFECT_SLIDE_CLASS+279)
#define EFX_EFFECT_SLIDE4_LT_LT_RT_RT                         (EFX_EFFECT_SLIDE_CLASS+280)
#define EFX_EFFECT_SLIDE4_LT_LT_RT_RB                         (EFX_EFFECT_SLIDE_CLASS+281)
#define EFX_EFFECT_SLIDE4_LT_LT_RB_LT                         (EFX_EFFECT_SLIDE_CLASS+282)
#define EFX_EFFECT_SLIDE4_LT_LT_RB_LB                         (EFX_EFFECT_SLIDE_CLASS+283)
#define EFX_EFFECT_SLIDE4_LT_LT_RB_RT                         (EFX_EFFECT_SLIDE_CLASS+284)
#define EFX_EFFECT_SLIDE4_LT_LT_RB_RB                         (EFX_EFFECT_SLIDE_CLASS+285)
#define EFX_EFFECT_SLIDE4_LT_LB_LT_LT                         (EFX_EFFECT_SLIDE_CLASS+286)
#define EFX_EFFECT_SLIDE4_LT_LB_LT_LB                         (EFX_EFFECT_SLIDE_CLASS+287)
#define EFX_EFFECT_SLIDE4_LT_LB_LT_RT                         (EFX_EFFECT_SLIDE_CLASS+288)
#define EFX_EFFECT_SLIDE4_LT_LB_LT_RB                         (EFX_EFFECT_SLIDE_CLASS+289)
#define EFX_EFFECT_SLIDE4_LT_LB_LB_LT                         (EFX_EFFECT_SLIDE_CLASS+290)
#define EFX_EFFECT_SLIDE4_LT_LB_LB_LB                         (EFX_EFFECT_SLIDE_CLASS+291)
#define EFX_EFFECT_SLIDE4_LT_LB_LB_RT                         (EFX_EFFECT_SLIDE_CLASS+292)
#define EFX_EFFECT_SLIDE4_LT_LB_LB_RB                         (EFX_EFFECT_SLIDE_CLASS+293)
#define EFX_EFFECT_SLIDE4_LT_LB_RT_LT                         (EFX_EFFECT_SLIDE_CLASS+294)
#define EFX_EFFECT_SLIDE4_LT_LB_RT_LB                         (EFX_EFFECT_SLIDE_CLASS+295)
#define EFX_EFFECT_SLIDE4_LT_LB_RT_RT                         (EFX_EFFECT_SLIDE_CLASS+296)
#define EFX_EFFECT_SLIDE4_LT_LB_RT_RB                         (EFX_EFFECT_SLIDE_CLASS+297)
#define EFX_EFFECT_SLIDE4_LT_LB_RB_LT                         (EFX_EFFECT_SLIDE_CLASS+298)
#define EFX_EFFECT_SLIDE4_LT_LB_RB_LB                         (EFX_EFFECT_SLIDE_CLASS+299)
#define EFX_EFFECT_SLIDE4_LT_LB_RB_RT                         (EFX_EFFECT_SLIDE_CLASS+300)
#define EFX_EFFECT_SLIDE4_LT_LB_RB_RB                         (EFX_EFFECT_SLIDE_CLASS+301)
#define EFX_EFFECT_SLIDE4_LT_RT_LT_LT                         (EFX_EFFECT_SLIDE_CLASS+302)
#define EFX_EFFECT_SLIDE4_LT_RT_LT_LB                         (EFX_EFFECT_SLIDE_CLASS+303)
#define EFX_EFFECT_SLIDE4_LT_RT_LT_RT                         (EFX_EFFECT_SLIDE_CLASS+304)
#define EFX_EFFECT_SLIDE4_LT_RT_LT_RB                         (EFX_EFFECT_SLIDE_CLASS+305)
#define EFX_EFFECT_SLIDE4_LT_RT_LB_LT                         (EFX_EFFECT_SLIDE_CLASS+306)
#define EFX_EFFECT_SLIDE4_LT_RT_LB_LB                         (EFX_EFFECT_SLIDE_CLASS+307)
#define EFX_EFFECT_SLIDE4_LT_RT_LB_RT                         (EFX_EFFECT_SLIDE_CLASS+308)
#define EFX_EFFECT_SLIDE4_LT_RT_LB_RB                         (EFX_EFFECT_SLIDE_CLASS+309)
#define EFX_EFFECT_SLIDE4_LT_RT_RT_LT                         (EFX_EFFECT_SLIDE_CLASS+310)
#define EFX_EFFECT_SLIDE4_LT_RT_RT_LB                         (EFX_EFFECT_SLIDE_CLASS+311)
#define EFX_EFFECT_SLIDE4_LT_RT_RT_RT                         (EFX_EFFECT_SLIDE_CLASS+312)
#define EFX_EFFECT_SLIDE4_LT_RT_RT_RB                         (EFX_EFFECT_SLIDE_CLASS+313)
#define EFX_EFFECT_SLIDE4_LT_RT_RB_LT                         (EFX_EFFECT_SLIDE_CLASS+314)
#define EFX_EFFECT_SLIDE4_LT_RT_RB_LB                         (EFX_EFFECT_SLIDE_CLASS+315)
#define EFX_EFFECT_SLIDE4_LT_RT_RB_RT                         (EFX_EFFECT_SLIDE_CLASS+316)
#define EFX_EFFECT_SLIDE4_LT_RT_RB_RB                         (EFX_EFFECT_SLIDE_CLASS+317)
#define EFX_EFFECT_SLIDE4_LT_RB_LT_LT                         (EFX_EFFECT_SLIDE_CLASS+318)
#define EFX_EFFECT_SLIDE4_LT_RB_LT_LB                         (EFX_EFFECT_SLIDE_CLASS+319)
#define EFX_EFFECT_SLIDE4_LT_RB_LT_RT                         (EFX_EFFECT_SLIDE_CLASS+320)
#define EFX_EFFECT_SLIDE4_LT_RB_LT_RB                         (EFX_EFFECT_SLIDE_CLASS+321)
#define EFX_EFFECT_SLIDE4_LT_RB_LB_LT                         (EFX_EFFECT_SLIDE_CLASS+322)
#define EFX_EFFECT_SLIDE4_LT_RB_LB_LB                         (EFX_EFFECT_SLIDE_CLASS+323)
#define EFX_EFFECT_SLIDE4_LT_RB_LB_RT                         (EFX_EFFECT_SLIDE_CLASS+324)
#define EFX_EFFECT_SLIDE4_LT_RB_LB_RB                         (EFX_EFFECT_SLIDE_CLASS+325)
#define EFX_EFFECT_SLIDE4_LT_RB_RT_LT                         (EFX_EFFECT_SLIDE_CLASS+326)
#define EFX_EFFECT_SLIDE4_LT_RB_RT_LB                         (EFX_EFFECT_SLIDE_CLASS+327)
#define EFX_EFFECT_SLIDE4_LT_RB_RT_RT                         (EFX_EFFECT_SLIDE_CLASS+328)
#define EFX_EFFECT_SLIDE4_LT_RB_RT_RB                         (EFX_EFFECT_SLIDE_CLASS+329)
#define EFX_EFFECT_SLIDE4_LT_RB_RB_LT                         (EFX_EFFECT_SLIDE_CLASS+330)
#define EFX_EFFECT_SLIDE4_LT_RB_RB_LB                         (EFX_EFFECT_SLIDE_CLASS+331)
#define EFX_EFFECT_SLIDE4_LT_RB_RB_RT                         (EFX_EFFECT_SLIDE_CLASS+332)
#define EFX_EFFECT_SLIDE4_LT_RB_RB_RB                         (EFX_EFFECT_SLIDE_CLASS+333)
#define EFX_EFFECT_SLIDE4_LB_LT_LT_LT                         (EFX_EFFECT_SLIDE_CLASS+334)
#define EFX_EFFECT_SLIDE4_LB_LT_LT_LB                         (EFX_EFFECT_SLIDE_CLASS+335)
#define EFX_EFFECT_SLIDE4_LB_LT_LT_RT                         (EFX_EFFECT_SLIDE_CLASS+336)
#define EFX_EFFECT_SLIDE4_LB_LT_LT_RB                         (EFX_EFFECT_SLIDE_CLASS+337)
#define EFX_EFFECT_SLIDE4_LB_LT_LB_LT                         (EFX_EFFECT_SLIDE_CLASS+338)
#define EFX_EFFECT_SLIDE4_LB_LT_LB_LB                         (EFX_EFFECT_SLIDE_CLASS+339)
#define EFX_EFFECT_SLIDE4_LB_LT_LB_RT                         (EFX_EFFECT_SLIDE_CLASS+340)
#define EFX_EFFECT_SLIDE4_LB_LT_LB_RB                         (EFX_EFFECT_SLIDE_CLASS+341)
#define EFX_EFFECT_SLIDE4_LB_LT_RT_LT                         (EFX_EFFECT_SLIDE_CLASS+342)
#define EFX_EFFECT_SLIDE4_LB_LT_RT_LB                         (EFX_EFFECT_SLIDE_CLASS+343)
#define EFX_EFFECT_SLIDE4_LB_LT_RT_RT                         (EFX_EFFECT_SLIDE_CLASS+344)
#define EFX_EFFECT_SLIDE4_LB_LT_RT_RB                         (EFX_EFFECT_SLIDE_CLASS+345)
#define EFX_EFFECT_SLIDE4_LB_LT_RB_LT                         (EFX_EFFECT_SLIDE_CLASS+346)
#define EFX_EFFECT_SLIDE4_LB_LT_RB_LB                         (EFX_EFFECT_SLIDE_CLASS+347)
#define EFX_EFFECT_SLIDE4_LB_LT_RB_RT                         (EFX_EFFECT_SLIDE_CLASS+348)
#define EFX_EFFECT_SLIDE4_LB_LT_RB_RB                         (EFX_EFFECT_SLIDE_CLASS+349)
#define EFX_EFFECT_SLIDE4_LB_LB_LT_LT                         (EFX_EFFECT_SLIDE_CLASS+350)
#define EFX_EFFECT_SLIDE4_LB_LB_LT_LB                         (EFX_EFFECT_SLIDE_CLASS+351)
#define EFX_EFFECT_SLIDE4_LB_LB_LT_RT                         (EFX_EFFECT_SLIDE_CLASS+352)
#define EFX_EFFECT_SLIDE4_LB_LB_LT_RB                         (EFX_EFFECT_SLIDE_CLASS+353)
#define EFX_EFFECT_SLIDE4_LB_LB_LB_LT                         (EFX_EFFECT_SLIDE_CLASS+354)
#define EFX_EFFECT_SLIDE4_LB_LB_LB_LB                         (EFX_EFFECT_SLIDE_CLASS+355)
#define EFX_EFFECT_SLIDE4_LB_LB_LB_RT                         (EFX_EFFECT_SLIDE_CLASS+356)
#define EFX_EFFECT_SLIDE4_LB_LB_LB_RB                         (EFX_EFFECT_SLIDE_CLASS+357)
#define EFX_EFFECT_SLIDE4_LB_LB_RT_LT                         (EFX_EFFECT_SLIDE_CLASS+358)
#define EFX_EFFECT_SLIDE4_LB_LB_RT_LB                         (EFX_EFFECT_SLIDE_CLASS+359)
#define EFX_EFFECT_SLIDE4_LB_LB_RT_RT                         (EFX_EFFECT_SLIDE_CLASS+360)
#define EFX_EFFECT_SLIDE4_LB_LB_RT_RB                         (EFX_EFFECT_SLIDE_CLASS+361)
#define EFX_EFFECT_SLIDE4_LB_LB_RB_LT                         (EFX_EFFECT_SLIDE_CLASS+362)
#define EFX_EFFECT_SLIDE4_LB_LB_RB_LB                         (EFX_EFFECT_SLIDE_CLASS+363)
#define EFX_EFFECT_SLIDE4_LB_LB_RB_RT                         (EFX_EFFECT_SLIDE_CLASS+364)
#define EFX_EFFECT_SLIDE4_LB_LB_RB_RB                         (EFX_EFFECT_SLIDE_CLASS+365)
#define EFX_EFFECT_SLIDE4_LB_RT_LT_LT                         (EFX_EFFECT_SLIDE_CLASS+366)
#define EFX_EFFECT_SLIDE4_LB_RT_LT_LB                         (EFX_EFFECT_SLIDE_CLASS+367)
#define EFX_EFFECT_SLIDE4_LB_RT_LT_RT                         (EFX_EFFECT_SLIDE_CLASS+368)
#define EFX_EFFECT_SLIDE4_LB_RT_LT_RB                         (EFX_EFFECT_SLIDE_CLASS+369)
#define EFX_EFFECT_SLIDE4_LB_RT_LB_LT                         (EFX_EFFECT_SLIDE_CLASS+370)
#define EFX_EFFECT_SLIDE4_LB_RT_LB_LB                         (EFX_EFFECT_SLIDE_CLASS+371)
#define EFX_EFFECT_SLIDE4_LB_RT_LB_RT                         (EFX_EFFECT_SLIDE_CLASS+372)
#define EFX_EFFECT_SLIDE4_LB_RT_LB_RB                         (EFX_EFFECT_SLIDE_CLASS+373)
#define EFX_EFFECT_SLIDE4_LB_RT_RT_LT                         (EFX_EFFECT_SLIDE_CLASS+374)
#define EFX_EFFECT_SLIDE4_LB_RT_RT_LB                         (EFX_EFFECT_SLIDE_CLASS+375)
#define EFX_EFFECT_SLIDE4_LB_RT_RT_RT                         (EFX_EFFECT_SLIDE_CLASS+376)
#define EFX_EFFECT_SLIDE4_LB_RT_RT_RB                         (EFX_EFFECT_SLIDE_CLASS+377)
#define EFX_EFFECT_SLIDE4_LB_RT_RB_LT                         (EFX_EFFECT_SLIDE_CLASS+378)
#define EFX_EFFECT_SLIDE4_LB_RT_RB_LB                         (EFX_EFFECT_SLIDE_CLASS+379)
#define EFX_EFFECT_SLIDE4_LB_RT_RB_RT                         (EFX_EFFECT_SLIDE_CLASS+380)
#define EFX_EFFECT_SLIDE4_LB_RT_RB_RB                         (EFX_EFFECT_SLIDE_CLASS+381)
#define EFX_EFFECT_SLIDE4_LB_RB_LT_LT                         (EFX_EFFECT_SLIDE_CLASS+382)
#define EFX_EFFECT_SLIDE4_LB_RB_LT_LB                         (EFX_EFFECT_SLIDE_CLASS+383)
#define EFX_EFFECT_SLIDE4_LB_RB_LT_RT                         (EFX_EFFECT_SLIDE_CLASS+384)
#define EFX_EFFECT_SLIDE4_LB_RB_LT_RB                         (EFX_EFFECT_SLIDE_CLASS+385)
#define EFX_EFFECT_SLIDE4_LB_RB_LB_LT                         (EFX_EFFECT_SLIDE_CLASS+386)
#define EFX_EFFECT_SLIDE4_LB_RB_LB_LB                         (EFX_EFFECT_SLIDE_CLASS+387)
#define EFX_EFFECT_SLIDE4_LB_RB_LB_RT                         (EFX_EFFECT_SLIDE_CLASS+388)
#define EFX_EFFECT_SLIDE4_LB_RB_LB_RB                         (EFX_EFFECT_SLIDE_CLASS+389)
#define EFX_EFFECT_SLIDE4_LB_RB_RT_LT                         (EFX_EFFECT_SLIDE_CLASS+390)
#define EFX_EFFECT_SLIDE4_LB_RB_RT_LB                         (EFX_EFFECT_SLIDE_CLASS+391)
#define EFX_EFFECT_SLIDE4_LB_RB_RT_RT                         (EFX_EFFECT_SLIDE_CLASS+392)
#define EFX_EFFECT_SLIDE4_LB_RB_RT_RB                         (EFX_EFFECT_SLIDE_CLASS+393)
#define EFX_EFFECT_SLIDE4_LB_RB_RB_LT                         (EFX_EFFECT_SLIDE_CLASS+394)
#define EFX_EFFECT_SLIDE4_LB_RB_RB_LB                         (EFX_EFFECT_SLIDE_CLASS+395)
#define EFX_EFFECT_SLIDE4_LB_RB_RB_RT                         (EFX_EFFECT_SLIDE_CLASS+396)
#define EFX_EFFECT_SLIDE4_LB_RB_RB_RB                         (EFX_EFFECT_SLIDE_CLASS+397)
#define EFX_EFFECT_SLIDE4_RT_LT_LT_LT                         (EFX_EFFECT_SLIDE_CLASS+398)
#define EFX_EFFECT_SLIDE4_RT_LT_LT_LB                         (EFX_EFFECT_SLIDE_CLASS+399)
#define EFX_EFFECT_SLIDE4_RT_LT_LT_RT                         (EFX_EFFECT_SLIDE_CLASS+400)
#define EFX_EFFECT_SLIDE4_RT_LT_LT_RB                         (EFX_EFFECT_SLIDE_CLASS+401)
#define EFX_EFFECT_SLIDE4_RT_LT_LB_LT                         (EFX_EFFECT_SLIDE_CLASS+402)
#define EFX_EFFECT_SLIDE4_RT_LT_LB_LB                         (EFX_EFFECT_SLIDE_CLASS+403)
#define EFX_EFFECT_SLIDE4_RT_LT_LB_RT                         (EFX_EFFECT_SLIDE_CLASS+404)
#define EFX_EFFECT_SLIDE4_RT_LT_LB_RB                         (EFX_EFFECT_SLIDE_CLASS+405)
#define EFX_EFFECT_SLIDE4_RT_LT_RT_LT                         (EFX_EFFECT_SLIDE_CLASS+406)
#define EFX_EFFECT_SLIDE4_RT_LT_RT_LB                         (EFX_EFFECT_SLIDE_CLASS+407)
#define EFX_EFFECT_SLIDE4_RT_LT_RT_RT                         (EFX_EFFECT_SLIDE_CLASS+408)
#define EFX_EFFECT_SLIDE4_RT_LT_RT_RB                         (EFX_EFFECT_SLIDE_CLASS+409)
#define EFX_EFFECT_SLIDE4_RT_LT_RB_LT                         (EFX_EFFECT_SLIDE_CLASS+410)
#define EFX_EFFECT_SLIDE4_RT_LT_RB_LB                         (EFX_EFFECT_SLIDE_CLASS+411)
#define EFX_EFFECT_SLIDE4_RT_LT_RB_RT                         (EFX_EFFECT_SLIDE_CLASS+412)
#define EFX_EFFECT_SLIDE4_RT_LT_RB_RB                         (EFX_EFFECT_SLIDE_CLASS+413)
#define EFX_EFFECT_SLIDE4_RT_LB_LT_LT                         (EFX_EFFECT_SLIDE_CLASS+414)
#define EFX_EFFECT_SLIDE4_RT_LB_LT_LB                         (EFX_EFFECT_SLIDE_CLASS+415)
#define EFX_EFFECT_SLIDE4_RT_LB_LT_RT                         (EFX_EFFECT_SLIDE_CLASS+416)
#define EFX_EFFECT_SLIDE4_RT_LB_LT_RB                         (EFX_EFFECT_SLIDE_CLASS+417)
#define EFX_EFFECT_SLIDE4_RT_LB_LB_LT                         (EFX_EFFECT_SLIDE_CLASS+418)
#define EFX_EFFECT_SLIDE4_RT_LB_LB_LB                         (EFX_EFFECT_SLIDE_CLASS+419)
#define EFX_EFFECT_SLIDE4_RT_LB_LB_RT                         (EFX_EFFECT_SLIDE_CLASS+420)
#define EFX_EFFECT_SLIDE4_RT_LB_LB_RB                         (EFX_EFFECT_SLIDE_CLASS+421)
#define EFX_EFFECT_SLIDE4_RT_LB_RT_LT                         (EFX_EFFECT_SLIDE_CLASS+422)
#define EFX_EFFECT_SLIDE4_RT_LB_RT_LB                         (EFX_EFFECT_SLIDE_CLASS+423)
#define EFX_EFFECT_SLIDE4_RT_LB_RT_RT                         (EFX_EFFECT_SLIDE_CLASS+424)
#define EFX_EFFECT_SLIDE4_RT_LB_RT_RB                         (EFX_EFFECT_SLIDE_CLASS+425)
#define EFX_EFFECT_SLIDE4_RT_LB_RB_LT                         (EFX_EFFECT_SLIDE_CLASS+426)
#define EFX_EFFECT_SLIDE4_RT_LB_RB_LB                         (EFX_EFFECT_SLIDE_CLASS+427)
#define EFX_EFFECT_SLIDE4_RT_LB_RB_RT                         (EFX_EFFECT_SLIDE_CLASS+428)
#define EFX_EFFECT_SLIDE4_RT_LB_RB_RB                         (EFX_EFFECT_SLIDE_CLASS+429)
#define EFX_EFFECT_SLIDE4_RT_RT_LT_LT                         (EFX_EFFECT_SLIDE_CLASS+430)
#define EFX_EFFECT_SLIDE4_RT_RT_LT_LB                         (EFX_EFFECT_SLIDE_CLASS+431)
#define EFX_EFFECT_SLIDE4_RT_RT_LT_RT                         (EFX_EFFECT_SLIDE_CLASS+432)
#define EFX_EFFECT_SLIDE4_RT_RT_LT_RB                         (EFX_EFFECT_SLIDE_CLASS+433)
#define EFX_EFFECT_SLIDE4_RT_RT_LB_LT                         (EFX_EFFECT_SLIDE_CLASS+434)
#define EFX_EFFECT_SLIDE4_RT_RT_LB_LB                         (EFX_EFFECT_SLIDE_CLASS+435)
#define EFX_EFFECT_SLIDE4_RT_RT_LB_RT                         (EFX_EFFECT_SLIDE_CLASS+436)
#define EFX_EFFECT_SLIDE4_RT_RT_LB_RB                         (EFX_EFFECT_SLIDE_CLASS+437)
#define EFX_EFFECT_SLIDE4_RT_RT_RT_LT                         (EFX_EFFECT_SLIDE_CLASS+438)
#define EFX_EFFECT_SLIDE4_RT_RT_RT_LB                         (EFX_EFFECT_SLIDE_CLASS+439)
#define EFX_EFFECT_SLIDE4_RT_RT_RT_RT                         (EFX_EFFECT_SLIDE_CLASS+440)
#define EFX_EFFECT_SLIDE4_RT_RT_RT_RB                         (EFX_EFFECT_SLIDE_CLASS+441)
#define EFX_EFFECT_SLIDE4_RT_RT_RB_LT                         (EFX_EFFECT_SLIDE_CLASS+442)
#define EFX_EFFECT_SLIDE4_RT_RT_RB_LB                         (EFX_EFFECT_SLIDE_CLASS+443)
#define EFX_EFFECT_SLIDE4_RT_RT_RB_RT                         (EFX_EFFECT_SLIDE_CLASS+444)
#define EFX_EFFECT_SLIDE4_RT_RT_RB_RB                         (EFX_EFFECT_SLIDE_CLASS+445)
#define EFX_EFFECT_SLIDE4_RT_RB_LT_LT                         (EFX_EFFECT_SLIDE_CLASS+446)
#define EFX_EFFECT_SLIDE4_RT_RB_LT_LB                         (EFX_EFFECT_SLIDE_CLASS+447)
#define EFX_EFFECT_SLIDE4_RT_RB_LT_RT                         (EFX_EFFECT_SLIDE_CLASS+448)
#define EFX_EFFECT_SLIDE4_RT_RB_LT_RB                         (EFX_EFFECT_SLIDE_CLASS+449)
#define EFX_EFFECT_SLIDE4_RT_RB_LB_LT                         (EFX_EFFECT_SLIDE_CLASS+450)
#define EFX_EFFECT_SLIDE4_RT_RB_LB_LB                         (EFX_EFFECT_SLIDE_CLASS+451)
#define EFX_EFFECT_SLIDE4_RT_RB_LB_RT                         (EFX_EFFECT_SLIDE_CLASS+452)
#define EFX_EFFECT_SLIDE4_RT_RB_LB_RB                         (EFX_EFFECT_SLIDE_CLASS+453)
#define EFX_EFFECT_SLIDE4_RT_RB_RT_LT                         (EFX_EFFECT_SLIDE_CLASS+454)
#define EFX_EFFECT_SLIDE4_RT_RB_RT_LB                         (EFX_EFFECT_SLIDE_CLASS+455)
#define EFX_EFFECT_SLIDE4_RT_RB_RT_RT                         (EFX_EFFECT_SLIDE_CLASS+456)
#define EFX_EFFECT_SLIDE4_RT_RB_RT_RB                         (EFX_EFFECT_SLIDE_CLASS+457)
#define EFX_EFFECT_SLIDE4_RT_RB_RB_LT                         (EFX_EFFECT_SLIDE_CLASS+458)
#define EFX_EFFECT_SLIDE4_RT_RB_RB_LB                         (EFX_EFFECT_SLIDE_CLASS+459)
#define EFX_EFFECT_SLIDE4_RT_RB_RB_RT                         (EFX_EFFECT_SLIDE_CLASS+460)
#define EFX_EFFECT_SLIDE4_RT_RB_RB_RB                         (EFX_EFFECT_SLIDE_CLASS+461)
#define EFX_EFFECT_SLIDE4_RB_LT_LT_LT                         (EFX_EFFECT_SLIDE_CLASS+462)
#define EFX_EFFECT_SLIDE4_RB_LT_LT_LB                         (EFX_EFFECT_SLIDE_CLASS+463)
#define EFX_EFFECT_SLIDE4_RB_LT_LT_RT                         (EFX_EFFECT_SLIDE_CLASS+464)
#define EFX_EFFECT_SLIDE4_RB_LT_LT_RB                         (EFX_EFFECT_SLIDE_CLASS+465)
#define EFX_EFFECT_SLIDE4_RB_LT_LB_LT                         (EFX_EFFECT_SLIDE_CLASS+466)
#define EFX_EFFECT_SLIDE4_RB_LT_LB_LB                         (EFX_EFFECT_SLIDE_CLASS+467)
#define EFX_EFFECT_SLIDE4_RB_LT_LB_RT                         (EFX_EFFECT_SLIDE_CLASS+468)
#define EFX_EFFECT_SLIDE4_RB_LT_LB_RB                         (EFX_EFFECT_SLIDE_CLASS+469)
#define EFX_EFFECT_SLIDE4_RB_LT_RT_LT                         (EFX_EFFECT_SLIDE_CLASS+470)
#define EFX_EFFECT_SLIDE4_RB_LT_RT_LB                         (EFX_EFFECT_SLIDE_CLASS+471)
#define EFX_EFFECT_SLIDE4_RB_LT_RT_RT                         (EFX_EFFECT_SLIDE_CLASS+472)
#define EFX_EFFECT_SLIDE4_RB_LT_RT_RB                         (EFX_EFFECT_SLIDE_CLASS+473)
#define EFX_EFFECT_SLIDE4_RB_LT_RB_LT                         (EFX_EFFECT_SLIDE_CLASS+474)
#define EFX_EFFECT_SLIDE4_RB_LT_RB_LB                         (EFX_EFFECT_SLIDE_CLASS+475)
#define EFX_EFFECT_SLIDE4_RB_LT_RB_RT                         (EFX_EFFECT_SLIDE_CLASS+476)
#define EFX_EFFECT_SLIDE4_RB_LT_RB_RB                         (EFX_EFFECT_SLIDE_CLASS+477)
#define EFX_EFFECT_SLIDE4_RB_LB_LT_LT                         (EFX_EFFECT_SLIDE_CLASS+478)
#define EFX_EFFECT_SLIDE4_RB_LB_LT_LB                         (EFX_EFFECT_SLIDE_CLASS+479)
#define EFX_EFFECT_SLIDE4_RB_LB_LT_RT                         (EFX_EFFECT_SLIDE_CLASS+480)
#define EFX_EFFECT_SLIDE4_RB_LB_LT_RB                         (EFX_EFFECT_SLIDE_CLASS+481)
#define EFX_EFFECT_SLIDE4_RB_LB_LB_LT                         (EFX_EFFECT_SLIDE_CLASS+482)
#define EFX_EFFECT_SLIDE4_RB_LB_LB_LB                         (EFX_EFFECT_SLIDE_CLASS+483)
#define EFX_EFFECT_SLIDE4_RB_LB_LB_RT                         (EFX_EFFECT_SLIDE_CLASS+484)
#define EFX_EFFECT_SLIDE4_RB_LB_LB_RB                         (EFX_EFFECT_SLIDE_CLASS+485)
#define EFX_EFFECT_SLIDE4_RB_LB_RT_LT                         (EFX_EFFECT_SLIDE_CLASS+486)
#define EFX_EFFECT_SLIDE4_RB_LB_RT_LB                         (EFX_EFFECT_SLIDE_CLASS+487)
#define EFX_EFFECT_SLIDE4_RB_LB_RT_RT                         (EFX_EFFECT_SLIDE_CLASS+488)
#define EFX_EFFECT_SLIDE4_RB_LB_RT_RB                         (EFX_EFFECT_SLIDE_CLASS+489)
#define EFX_EFFECT_SLIDE4_RB_LB_RB_LT                         (EFX_EFFECT_SLIDE_CLASS+490)
#define EFX_EFFECT_SLIDE4_RB_LB_RB_LB                         (EFX_EFFECT_SLIDE_CLASS+491)
#define EFX_EFFECT_SLIDE4_RB_LB_RB_RT                         (EFX_EFFECT_SLIDE_CLASS+492)
#define EFX_EFFECT_SLIDE4_RB_LB_RB_RB                         (EFX_EFFECT_SLIDE_CLASS+493)
#define EFX_EFFECT_SLIDE4_RB_RT_LT_LT                         (EFX_EFFECT_SLIDE_CLASS+494)
#define EFX_EFFECT_SLIDE4_RB_RT_LT_LB                         (EFX_EFFECT_SLIDE_CLASS+495)
#define EFX_EFFECT_SLIDE4_RB_RT_LT_RT                         (EFX_EFFECT_SLIDE_CLASS+496)
#define EFX_EFFECT_SLIDE4_RB_RT_LT_RB                         (EFX_EFFECT_SLIDE_CLASS+497)
#define EFX_EFFECT_SLIDE4_RB_RT_LB_LT                         (EFX_EFFECT_SLIDE_CLASS+498)
#define EFX_EFFECT_SLIDE4_RB_RT_LB_LB                         (EFX_EFFECT_SLIDE_CLASS+499)
#define EFX_EFFECT_SLIDE4_RB_RT_LB_RT                         (EFX_EFFECT_SLIDE_CLASS+500)
#define EFX_EFFECT_SLIDE4_RB_RT_LB_RB                         (EFX_EFFECT_SLIDE_CLASS+501)
#define EFX_EFFECT_SLIDE4_RB_RT_RT_LT                         (EFX_EFFECT_SLIDE_CLASS+502)
#define EFX_EFFECT_SLIDE4_RB_RT_RT_LB                         (EFX_EFFECT_SLIDE_CLASS+503)
#define EFX_EFFECT_SLIDE4_RB_RT_RT_RT                         (EFX_EFFECT_SLIDE_CLASS+504)
#define EFX_EFFECT_SLIDE4_RB_RT_RT_RB                         (EFX_EFFECT_SLIDE_CLASS+505)
#define EFX_EFFECT_SLIDE4_RB_RT_RB_LT                         (EFX_EFFECT_SLIDE_CLASS+506)
#define EFX_EFFECT_SLIDE4_RB_RT_RB_LB                         (EFX_EFFECT_SLIDE_CLASS+507)
#define EFX_EFFECT_SLIDE4_RB_RT_RB_RT                         (EFX_EFFECT_SLIDE_CLASS+508)
#define EFX_EFFECT_SLIDE4_RB_RT_RB_RB                         (EFX_EFFECT_SLIDE_CLASS+509)
#define EFX_EFFECT_SLIDE4_RB_RB_LT_LT                         (EFX_EFFECT_SLIDE_CLASS+510)
#define EFX_EFFECT_SLIDE4_RB_RB_LT_LB                         (EFX_EFFECT_SLIDE_CLASS+511)
#define EFX_EFFECT_SLIDE4_RB_RB_LT_RT                         (EFX_EFFECT_SLIDE_CLASS+512)
#define EFX_EFFECT_SLIDE4_RB_RB_LT_RB                         (EFX_EFFECT_SLIDE_CLASS+513)
#define EFX_EFFECT_SLIDE4_RB_RB_LB_LT                         (EFX_EFFECT_SLIDE_CLASS+514)
#define EFX_EFFECT_SLIDE4_RB_RB_LB_LB                         (EFX_EFFECT_SLIDE_CLASS+515)
#define EFX_EFFECT_SLIDE4_RB_RB_LB_RT                         (EFX_EFFECT_SLIDE_CLASS+516)
#define EFX_EFFECT_SLIDE4_RB_RB_LB_RB                         (EFX_EFFECT_SLIDE_CLASS+517)
#define EFX_EFFECT_SLIDE4_RB_RB_RT_LT                         (EFX_EFFECT_SLIDE_CLASS+518)
#define EFX_EFFECT_SLIDE4_RB_RB_RT_LB                         (EFX_EFFECT_SLIDE_CLASS+519)
#define EFX_EFFECT_SLIDE4_RB_RB_RT_RT                         (EFX_EFFECT_SLIDE_CLASS+520)
#define EFX_EFFECT_SLIDE4_RB_RB_RT_RB                         (EFX_EFFECT_SLIDE_CLASS+521)
#define EFX_EFFECT_SLIDE4_RB_RB_RB_LT                         (EFX_EFFECT_SLIDE_CLASS+522)
#define EFX_EFFECT_SLIDE4_RB_RB_RB_LB                         (EFX_EFFECT_SLIDE_CLASS+523)
#define EFX_EFFECT_SLIDE4_RB_RB_RB_RT                         (EFX_EFFECT_SLIDE_CLASS+524)
#define EFX_EFFECT_SLIDE4_RB_RB_RB_RB                         (EFX_EFFECT_SLIDE_CLASS+525)
#define EFX_EFFECT_SLIDE_MAX                                  (EFX_EFFECT_SLIDE_CLASS+525)

// Roll
#define EFX_EFFECT_ROLL_CLASS                                 7000
#define EFX_EFFECT_ROLL_L_TO_R                                (EFX_EFFECT_ROLL_CLASS+0)
#define EFX_EFFECT_ROLL_R_TO_L                                (EFX_EFFECT_ROLL_CLASS+1)
#define EFX_EFFECT_ROLL_T_TO_B                                (EFX_EFFECT_ROLL_CLASS+2)
#define EFX_EFFECT_ROLL_B_TO_T                                (EFX_EFFECT_ROLL_CLASS+3)
#define EFX_EFFECT_ROLL2_T_R_TO_L_AND_B_L_TO_R                (EFX_EFFECT_ROLL_CLASS+4)
#define EFX_EFFECT_ROLL2_T_L_TO_R_AND_B_R_TO_L                (EFX_EFFECT_ROLL_CLASS+5)
#define EFX_EFFECT_ROLL2_L_B_TO_T_AND_R_T_TO_B                (EFX_EFFECT_ROLL_CLASS+6)
#define EFX_EFFECT_ROLL2_L_T_TO_B_AND_R_B_TO_T                (EFX_EFFECT_ROLL_CLASS+7)
#define EFX_EFFECT_ROLL4_L_L_L_L                              (EFX_EFFECT_ROLL_CLASS+8)
#define EFX_EFFECT_ROLL4_L_L_L_R                              (EFX_EFFECT_ROLL_CLASS+9)
#define EFX_EFFECT_ROLL4_L_L_L_T                              (EFX_EFFECT_ROLL_CLASS+10)
#define EFX_EFFECT_ROLL4_L_L_L_B                              (EFX_EFFECT_ROLL_CLASS+11)
#define EFX_EFFECT_ROLL4_L_L_R_L                              (EFX_EFFECT_ROLL_CLASS+12)
#define EFX_EFFECT_ROLL4_L_L_R_R                              (EFX_EFFECT_ROLL_CLASS+13)
#define EFX_EFFECT_ROLL4_L_L_R_T                              (EFX_EFFECT_ROLL_CLASS+14)
#define EFX_EFFECT_ROLL4_L_L_R_B                              (EFX_EFFECT_ROLL_CLASS+15)
#define EFX_EFFECT_ROLL4_L_L_T_L                              (EFX_EFFECT_ROLL_CLASS+16)
#define EFX_EFFECT_ROLL4_L_L_T_R                              (EFX_EFFECT_ROLL_CLASS+17)
#define EFX_EFFECT_ROLL4_L_L_T_T                              (EFX_EFFECT_ROLL_CLASS+18)
#define EFX_EFFECT_ROLL4_L_L_T_B                              (EFX_EFFECT_ROLL_CLASS+19)
#define EFX_EFFECT_ROLL4_L_L_B_L                              (EFX_EFFECT_ROLL_CLASS+20)
#define EFX_EFFECT_ROLL4_L_L_B_R                              (EFX_EFFECT_ROLL_CLASS+21)
#define EFX_EFFECT_ROLL4_L_L_B_T                              (EFX_EFFECT_ROLL_CLASS+22)
#define EFX_EFFECT_ROLL4_L_L_B_B                              (EFX_EFFECT_ROLL_CLASS+23)
#define EFX_EFFECT_ROLL4_L_R_L_L                              (EFX_EFFECT_ROLL_CLASS+24)
#define EFX_EFFECT_ROLL4_L_R_L_R                              (EFX_EFFECT_ROLL_CLASS+25)
#define EFX_EFFECT_ROLL4_L_R_L_T                              (EFX_EFFECT_ROLL_CLASS+26)
#define EFX_EFFECT_ROLL4_L_R_L_B                              (EFX_EFFECT_ROLL_CLASS+27)
#define EFX_EFFECT_ROLL4_L_R_R_L                              (EFX_EFFECT_ROLL_CLASS+28)
#define EFX_EFFECT_ROLL4_L_R_R_R                              (EFX_EFFECT_ROLL_CLASS+29)
#define EFX_EFFECT_ROLL4_L_R_R_T                              (EFX_EFFECT_ROLL_CLASS+30)
#define EFX_EFFECT_ROLL4_L_R_R_B                              (EFX_EFFECT_ROLL_CLASS+31)
#define EFX_EFFECT_ROLL4_L_R_T_L                              (EFX_EFFECT_ROLL_CLASS+32)
#define EFX_EFFECT_ROLL4_L_R_T_R                              (EFX_EFFECT_ROLL_CLASS+33)
#define EFX_EFFECT_ROLL4_L_R_T_T                              (EFX_EFFECT_ROLL_CLASS+34)
#define EFX_EFFECT_ROLL4_L_R_T_B                              (EFX_EFFECT_ROLL_CLASS+35)
#define EFX_EFFECT_ROLL4_L_R_B_L                              (EFX_EFFECT_ROLL_CLASS+36)
#define EFX_EFFECT_ROLL4_L_R_B_R                              (EFX_EFFECT_ROLL_CLASS+37)
#define EFX_EFFECT_ROLL4_L_R_B_T                              (EFX_EFFECT_ROLL_CLASS+38)
#define EFX_EFFECT_ROLL4_L_R_B_B                              (EFX_EFFECT_ROLL_CLASS+39)
#define EFX_EFFECT_ROLL4_L_T_L_L                              (EFX_EFFECT_ROLL_CLASS+40)
#define EFX_EFFECT_ROLL4_L_T_L_R                              (EFX_EFFECT_ROLL_CLASS+41)
#define EFX_EFFECT_ROLL4_L_T_L_T                              (EFX_EFFECT_ROLL_CLASS+42)
#define EFX_EFFECT_ROLL4_L_T_L_B                              (EFX_EFFECT_ROLL_CLASS+43)
#define EFX_EFFECT_ROLL4_L_T_R_L                              (EFX_EFFECT_ROLL_CLASS+44)
#define EFX_EFFECT_ROLL4_L_T_R_R                              (EFX_EFFECT_ROLL_CLASS+45)
#define EFX_EFFECT_ROLL4_L_T_R_T                              (EFX_EFFECT_ROLL_CLASS+46)
#define EFX_EFFECT_ROLL4_L_T_R_B                              (EFX_EFFECT_ROLL_CLASS+47)
#define EFX_EFFECT_ROLL4_L_T_T_L                              (EFX_EFFECT_ROLL_CLASS+48)
#define EFX_EFFECT_ROLL4_L_T_T_R                              (EFX_EFFECT_ROLL_CLASS+49)
#define EFX_EFFECT_ROLL4_L_T_T_T                              (EFX_EFFECT_ROLL_CLASS+50)
#define EFX_EFFECT_ROLL4_L_T_T_B                              (EFX_EFFECT_ROLL_CLASS+51)
#define EFX_EFFECT_ROLL4_L_T_B_L                              (EFX_EFFECT_ROLL_CLASS+52)
#define EFX_EFFECT_ROLL4_L_T_B_R                              (EFX_EFFECT_ROLL_CLASS+53)
#define EFX_EFFECT_ROLL4_L_T_B_T                              (EFX_EFFECT_ROLL_CLASS+54)
#define EFX_EFFECT_ROLL4_L_T_B_B                              (EFX_EFFECT_ROLL_CLASS+55)
#define EFX_EFFECT_ROLL4_L_B_L_L                              (EFX_EFFECT_ROLL_CLASS+56)
#define EFX_EFFECT_ROLL4_L_B_L_R                              (EFX_EFFECT_ROLL_CLASS+57)
#define EFX_EFFECT_ROLL4_L_B_L_T                              (EFX_EFFECT_ROLL_CLASS+58)
#define EFX_EFFECT_ROLL4_L_B_L_B                              (EFX_EFFECT_ROLL_CLASS+59)
#define EFX_EFFECT_ROLL4_L_B_R_L                              (EFX_EFFECT_ROLL_CLASS+60)
#define EFX_EFFECT_ROLL4_L_B_R_R                              (EFX_EFFECT_ROLL_CLASS+61)
#define EFX_EFFECT_ROLL4_L_B_R_T                              (EFX_EFFECT_ROLL_CLASS+62)
#define EFX_EFFECT_ROLL4_L_B_R_B                              (EFX_EFFECT_ROLL_CLASS+63)
#define EFX_EFFECT_ROLL4_L_B_T_L                              (EFX_EFFECT_ROLL_CLASS+64)
#define EFX_EFFECT_ROLL4_L_B_T_R                              (EFX_EFFECT_ROLL_CLASS+65)
#define EFX_EFFECT_ROLL4_L_B_T_T                              (EFX_EFFECT_ROLL_CLASS+66)
#define EFX_EFFECT_ROLL4_L_B_T_B                              (EFX_EFFECT_ROLL_CLASS+67)
#define EFX_EFFECT_ROLL4_L_B_B_L                              (EFX_EFFECT_ROLL_CLASS+68)
#define EFX_EFFECT_ROLL4_L_B_B_R                              (EFX_EFFECT_ROLL_CLASS+69)
#define EFX_EFFECT_ROLL4_L_B_B_T                              (EFX_EFFECT_ROLL_CLASS+70)
#define EFX_EFFECT_ROLL4_L_B_B_B                              (EFX_EFFECT_ROLL_CLASS+71)
#define EFX_EFFECT_ROLL4_R_L_L_L                              (EFX_EFFECT_ROLL_CLASS+72)
#define EFX_EFFECT_ROLL4_R_L_L_R                              (EFX_EFFECT_ROLL_CLASS+73)
#define EFX_EFFECT_ROLL4_R_L_L_T                              (EFX_EFFECT_ROLL_CLASS+74)
#define EFX_EFFECT_ROLL4_R_L_L_B                              (EFX_EFFECT_ROLL_CLASS+75)
#define EFX_EFFECT_ROLL4_R_L_R_L                              (EFX_EFFECT_ROLL_CLASS+76)
#define EFX_EFFECT_ROLL4_R_L_R_R                              (EFX_EFFECT_ROLL_CLASS+77)
#define EFX_EFFECT_ROLL4_R_L_R_T                              (EFX_EFFECT_ROLL_CLASS+78)
#define EFX_EFFECT_ROLL4_R_L_R_B                              (EFX_EFFECT_ROLL_CLASS+79)
#define EFX_EFFECT_ROLL4_R_L_T_L                              (EFX_EFFECT_ROLL_CLASS+80)
#define EFX_EFFECT_ROLL4_R_L_T_R                              (EFX_EFFECT_ROLL_CLASS+81)
#define EFX_EFFECT_ROLL4_R_L_T_T                              (EFX_EFFECT_ROLL_CLASS+82)
#define EFX_EFFECT_ROLL4_R_L_T_B                              (EFX_EFFECT_ROLL_CLASS+83)
#define EFX_EFFECT_ROLL4_R_L_B_L                              (EFX_EFFECT_ROLL_CLASS+84)
#define EFX_EFFECT_ROLL4_R_L_B_R                              (EFX_EFFECT_ROLL_CLASS+85)
#define EFX_EFFECT_ROLL4_R_L_B_T                              (EFX_EFFECT_ROLL_CLASS+86)
#define EFX_EFFECT_ROLL4_R_L_B_B                              (EFX_EFFECT_ROLL_CLASS+87)
#define EFX_EFFECT_ROLL4_R_R_L_L                              (EFX_EFFECT_ROLL_CLASS+88)
#define EFX_EFFECT_ROLL4_R_R_L_R                              (EFX_EFFECT_ROLL_CLASS+89)
#define EFX_EFFECT_ROLL4_R_R_L_T                              (EFX_EFFECT_ROLL_CLASS+90)
#define EFX_EFFECT_ROLL4_R_R_L_B                              (EFX_EFFECT_ROLL_CLASS+91)
#define EFX_EFFECT_ROLL4_R_R_R_L                              (EFX_EFFECT_ROLL_CLASS+92)
#define EFX_EFFECT_ROLL4_R_R_R_R                              (EFX_EFFECT_ROLL_CLASS+93)
#define EFX_EFFECT_ROLL4_R_R_R_T                              (EFX_EFFECT_ROLL_CLASS+94)
#define EFX_EFFECT_ROLL4_R_R_R_B                              (EFX_EFFECT_ROLL_CLASS+95)
#define EFX_EFFECT_ROLL4_R_R_T_L                              (EFX_EFFECT_ROLL_CLASS+96)
#define EFX_EFFECT_ROLL4_R_R_T_R                              (EFX_EFFECT_ROLL_CLASS+97)
#define EFX_EFFECT_ROLL4_R_R_T_T                              (EFX_EFFECT_ROLL_CLASS+98)
#define EFX_EFFECT_ROLL4_R_R_T_B                              (EFX_EFFECT_ROLL_CLASS+99)
#define EFX_EFFECT_ROLL4_R_R_B_L                              (EFX_EFFECT_ROLL_CLASS+100)
#define EFX_EFFECT_ROLL4_R_R_B_R                              (EFX_EFFECT_ROLL_CLASS+101)
#define EFX_EFFECT_ROLL4_R_R_B_T                              (EFX_EFFECT_ROLL_CLASS+102)
#define EFX_EFFECT_ROLL4_R_R_B_B                              (EFX_EFFECT_ROLL_CLASS+103)
#define EFX_EFFECT_ROLL4_R_T_L_L                              (EFX_EFFECT_ROLL_CLASS+104)
#define EFX_EFFECT_ROLL4_R_T_L_R                              (EFX_EFFECT_ROLL_CLASS+105)
#define EFX_EFFECT_ROLL4_R_T_L_T                              (EFX_EFFECT_ROLL_CLASS+106)
#define EFX_EFFECT_ROLL4_R_T_L_B                              (EFX_EFFECT_ROLL_CLASS+107)
#define EFX_EFFECT_ROLL4_R_T_R_L                              (EFX_EFFECT_ROLL_CLASS+108)
#define EFX_EFFECT_ROLL4_R_T_R_R                              (EFX_EFFECT_ROLL_CLASS+109)
#define EFX_EFFECT_ROLL4_R_T_R_T                              (EFX_EFFECT_ROLL_CLASS+110)
#define EFX_EFFECT_ROLL4_R_T_R_B                              (EFX_EFFECT_ROLL_CLASS+111)
#define EFX_EFFECT_ROLL4_R_T_T_L                              (EFX_EFFECT_ROLL_CLASS+112)
#define EFX_EFFECT_ROLL4_R_T_T_R                              (EFX_EFFECT_ROLL_CLASS+113)
#define EFX_EFFECT_ROLL4_R_T_T_T                              (EFX_EFFECT_ROLL_CLASS+114)
#define EFX_EFFECT_ROLL4_R_T_T_B                              (EFX_EFFECT_ROLL_CLASS+115)
#define EFX_EFFECT_ROLL4_R_T_B_L                              (EFX_EFFECT_ROLL_CLASS+116)
#define EFX_EFFECT_ROLL4_R_T_B_R                              (EFX_EFFECT_ROLL_CLASS+117)
#define EFX_EFFECT_ROLL4_R_T_B_T                              (EFX_EFFECT_ROLL_CLASS+118)
#define EFX_EFFECT_ROLL4_R_T_B_B                              (EFX_EFFECT_ROLL_CLASS+119)
#define EFX_EFFECT_ROLL4_R_B_L_L                              (EFX_EFFECT_ROLL_CLASS+120)
#define EFX_EFFECT_ROLL4_R_B_L_R                              (EFX_EFFECT_ROLL_CLASS+121)
#define EFX_EFFECT_ROLL4_R_B_L_T                              (EFX_EFFECT_ROLL_CLASS+122)
#define EFX_EFFECT_ROLL4_R_B_L_B                              (EFX_EFFECT_ROLL_CLASS+123)
#define EFX_EFFECT_ROLL4_R_B_R_L                              (EFX_EFFECT_ROLL_CLASS+124)
#define EFX_EFFECT_ROLL4_R_B_R_R                              (EFX_EFFECT_ROLL_CLASS+125)
#define EFX_EFFECT_ROLL4_R_B_R_T                              (EFX_EFFECT_ROLL_CLASS+126)
#define EFX_EFFECT_ROLL4_R_B_R_B                              (EFX_EFFECT_ROLL_CLASS+127)
#define EFX_EFFECT_ROLL4_R_B_T_L                              (EFX_EFFECT_ROLL_CLASS+128)
#define EFX_EFFECT_ROLL4_R_B_T_R                              (EFX_EFFECT_ROLL_CLASS+129)
#define EFX_EFFECT_ROLL4_R_B_T_T                              (EFX_EFFECT_ROLL_CLASS+130)
#define EFX_EFFECT_ROLL4_R_B_T_B                              (EFX_EFFECT_ROLL_CLASS+131)
#define EFX_EFFECT_ROLL4_R_B_B_L                              (EFX_EFFECT_ROLL_CLASS+132)
#define EFX_EFFECT_ROLL4_R_B_B_R                              (EFX_EFFECT_ROLL_CLASS+133)
#define EFX_EFFECT_ROLL4_R_B_B_T                              (EFX_EFFECT_ROLL_CLASS+134)
#define EFX_EFFECT_ROLL4_R_B_B_B                              (EFX_EFFECT_ROLL_CLASS+135)
#define EFX_EFFECT_ROLL4_T_L_L_L                              (EFX_EFFECT_ROLL_CLASS+136)
#define EFX_EFFECT_ROLL4_T_L_L_R                              (EFX_EFFECT_ROLL_CLASS+137)
#define EFX_EFFECT_ROLL4_T_L_L_T                              (EFX_EFFECT_ROLL_CLASS+138)
#define EFX_EFFECT_ROLL4_T_L_L_B                              (EFX_EFFECT_ROLL_CLASS+139)
#define EFX_EFFECT_ROLL4_T_L_R_L                              (EFX_EFFECT_ROLL_CLASS+140)
#define EFX_EFFECT_ROLL4_T_L_R_R                              (EFX_EFFECT_ROLL_CLASS+141)
#define EFX_EFFECT_ROLL4_T_L_R_T                              (EFX_EFFECT_ROLL_CLASS+142)
#define EFX_EFFECT_ROLL4_T_L_R_B                              (EFX_EFFECT_ROLL_CLASS+143)
#define EFX_EFFECT_ROLL4_T_L_T_L                              (EFX_EFFECT_ROLL_CLASS+144)
#define EFX_EFFECT_ROLL4_T_L_T_R                              (EFX_EFFECT_ROLL_CLASS+145)
#define EFX_EFFECT_ROLL4_T_L_T_T                              (EFX_EFFECT_ROLL_CLASS+146)
#define EFX_EFFECT_ROLL4_T_L_T_B                              (EFX_EFFECT_ROLL_CLASS+147)
#define EFX_EFFECT_ROLL4_T_L_B_L                              (EFX_EFFECT_ROLL_CLASS+148)
#define EFX_EFFECT_ROLL4_T_L_B_R                              (EFX_EFFECT_ROLL_CLASS+149)
#define EFX_EFFECT_ROLL4_T_L_B_T                              (EFX_EFFECT_ROLL_CLASS+150)
#define EFX_EFFECT_ROLL4_T_L_B_B                              (EFX_EFFECT_ROLL_CLASS+151)
#define EFX_EFFECT_ROLL4_T_R_L_L                              (EFX_EFFECT_ROLL_CLASS+152)
#define EFX_EFFECT_ROLL4_T_R_L_R                              (EFX_EFFECT_ROLL_CLASS+153)
#define EFX_EFFECT_ROLL4_T_R_L_T                              (EFX_EFFECT_ROLL_CLASS+154)
#define EFX_EFFECT_ROLL4_T_R_L_B                              (EFX_EFFECT_ROLL_CLASS+155)
#define EFX_EFFECT_ROLL4_T_R_R_L                              (EFX_EFFECT_ROLL_CLASS+156)
#define EFX_EFFECT_ROLL4_T_R_R_R                              (EFX_EFFECT_ROLL_CLASS+157)
#define EFX_EFFECT_ROLL4_T_R_R_T                              (EFX_EFFECT_ROLL_CLASS+158)
#define EFX_EFFECT_ROLL4_T_R_R_B                              (EFX_EFFECT_ROLL_CLASS+159)
#define EFX_EFFECT_ROLL4_T_R_T_L                              (EFX_EFFECT_ROLL_CLASS+160)
#define EFX_EFFECT_ROLL4_T_R_T_R                              (EFX_EFFECT_ROLL_CLASS+161)
#define EFX_EFFECT_ROLL4_T_R_T_T                              (EFX_EFFECT_ROLL_CLASS+162)
#define EFX_EFFECT_ROLL4_T_R_T_B                              (EFX_EFFECT_ROLL_CLASS+163)
#define EFX_EFFECT_ROLL4_T_R_B_L                              (EFX_EFFECT_ROLL_CLASS+164)
#define EFX_EFFECT_ROLL4_T_R_B_R                              (EFX_EFFECT_ROLL_CLASS+165)
#define EFX_EFFECT_ROLL4_T_R_B_T                              (EFX_EFFECT_ROLL_CLASS+166)
#define EFX_EFFECT_ROLL4_T_R_B_B                              (EFX_EFFECT_ROLL_CLASS+167)
#define EFX_EFFECT_ROLL4_T_T_L_L                              (EFX_EFFECT_ROLL_CLASS+168)
#define EFX_EFFECT_ROLL4_T_T_L_R                              (EFX_EFFECT_ROLL_CLASS+169)
#define EFX_EFFECT_ROLL4_T_T_L_T                              (EFX_EFFECT_ROLL_CLASS+170)
#define EFX_EFFECT_ROLL4_T_T_L_B                              (EFX_EFFECT_ROLL_CLASS+171)
#define EFX_EFFECT_ROLL4_T_T_R_L                              (EFX_EFFECT_ROLL_CLASS+172)
#define EFX_EFFECT_ROLL4_T_T_R_R                              (EFX_EFFECT_ROLL_CLASS+173)
#define EFX_EFFECT_ROLL4_T_T_R_T                              (EFX_EFFECT_ROLL_CLASS+174)
#define EFX_EFFECT_ROLL4_T_T_R_B                              (EFX_EFFECT_ROLL_CLASS+175)
#define EFX_EFFECT_ROLL4_T_T_T_L                              (EFX_EFFECT_ROLL_CLASS+176)
#define EFX_EFFECT_ROLL4_T_T_T_R                              (EFX_EFFECT_ROLL_CLASS+177)
#define EFX_EFFECT_ROLL4_T_T_T_T                              (EFX_EFFECT_ROLL_CLASS+178)
#define EFX_EFFECT_ROLL4_T_T_T_B                              (EFX_EFFECT_ROLL_CLASS+179)
#define EFX_EFFECT_ROLL4_T_T_B_L                              (EFX_EFFECT_ROLL_CLASS+180)
#define EFX_EFFECT_ROLL4_T_T_B_R                              (EFX_EFFECT_ROLL_CLASS+181)
#define EFX_EFFECT_ROLL4_T_T_B_T                              (EFX_EFFECT_ROLL_CLASS+182)
#define EFX_EFFECT_ROLL4_T_T_B_B                              (EFX_EFFECT_ROLL_CLASS+183)
#define EFX_EFFECT_ROLL4_T_B_L_L                              (EFX_EFFECT_ROLL_CLASS+184)
#define EFX_EFFECT_ROLL4_T_B_L_R                              (EFX_EFFECT_ROLL_CLASS+185)
#define EFX_EFFECT_ROLL4_T_B_L_T                              (EFX_EFFECT_ROLL_CLASS+186)
#define EFX_EFFECT_ROLL4_T_B_L_B                              (EFX_EFFECT_ROLL_CLASS+187)
#define EFX_EFFECT_ROLL4_T_B_R_L                              (EFX_EFFECT_ROLL_CLASS+188)
#define EFX_EFFECT_ROLL4_T_B_R_R                              (EFX_EFFECT_ROLL_CLASS+189)
#define EFX_EFFECT_ROLL4_T_B_R_T                              (EFX_EFFECT_ROLL_CLASS+190)
#define EFX_EFFECT_ROLL4_T_B_R_B                              (EFX_EFFECT_ROLL_CLASS+191)
#define EFX_EFFECT_ROLL4_T_B_T_L                              (EFX_EFFECT_ROLL_CLASS+192)
#define EFX_EFFECT_ROLL4_T_B_T_R                              (EFX_EFFECT_ROLL_CLASS+193)
#define EFX_EFFECT_ROLL4_T_B_T_T                              (EFX_EFFECT_ROLL_CLASS+194)
#define EFX_EFFECT_ROLL4_T_B_T_B                              (EFX_EFFECT_ROLL_CLASS+195)
#define EFX_EFFECT_ROLL4_T_B_B_L                              (EFX_EFFECT_ROLL_CLASS+196)
#define EFX_EFFECT_ROLL4_T_B_B_R                              (EFX_EFFECT_ROLL_CLASS+197)
#define EFX_EFFECT_ROLL4_T_B_B_T                              (EFX_EFFECT_ROLL_CLASS+198)
#define EFX_EFFECT_ROLL4_T_B_B_B                              (EFX_EFFECT_ROLL_CLASS+199)
#define EFX_EFFECT_ROLL4_B_L_L_L                              (EFX_EFFECT_ROLL_CLASS+200)
#define EFX_EFFECT_ROLL4_B_L_L_R                              (EFX_EFFECT_ROLL_CLASS+201)
#define EFX_EFFECT_ROLL4_B_L_L_T                              (EFX_EFFECT_ROLL_CLASS+202)
#define EFX_EFFECT_ROLL4_B_L_L_B                              (EFX_EFFECT_ROLL_CLASS+203)
#define EFX_EFFECT_ROLL4_B_L_R_L                              (EFX_EFFECT_ROLL_CLASS+204)
#define EFX_EFFECT_ROLL4_B_L_R_R                              (EFX_EFFECT_ROLL_CLASS+205)
#define EFX_EFFECT_ROLL4_B_L_R_T                              (EFX_EFFECT_ROLL_CLASS+206)
#define EFX_EFFECT_ROLL4_B_L_R_B                              (EFX_EFFECT_ROLL_CLASS+207)
#define EFX_EFFECT_ROLL4_B_L_T_L                              (EFX_EFFECT_ROLL_CLASS+208)
#define EFX_EFFECT_ROLL4_B_L_T_R                              (EFX_EFFECT_ROLL_CLASS+209)
#define EFX_EFFECT_ROLL4_B_L_T_T                              (EFX_EFFECT_ROLL_CLASS+210)
#define EFX_EFFECT_ROLL4_B_L_T_B                              (EFX_EFFECT_ROLL_CLASS+211)
#define EFX_EFFECT_ROLL4_B_L_B_L                              (EFX_EFFECT_ROLL_CLASS+212)
#define EFX_EFFECT_ROLL4_B_L_B_R                              (EFX_EFFECT_ROLL_CLASS+213)
#define EFX_EFFECT_ROLL4_B_L_B_T                              (EFX_EFFECT_ROLL_CLASS+214)
#define EFX_EFFECT_ROLL4_B_L_B_B                              (EFX_EFFECT_ROLL_CLASS+215)
#define EFX_EFFECT_ROLL4_B_R_L_L                              (EFX_EFFECT_ROLL_CLASS+216)
#define EFX_EFFECT_ROLL4_B_R_L_R                              (EFX_EFFECT_ROLL_CLASS+217)
#define EFX_EFFECT_ROLL4_B_R_L_T                              (EFX_EFFECT_ROLL_CLASS+218)
#define EFX_EFFECT_ROLL4_B_R_L_B                              (EFX_EFFECT_ROLL_CLASS+219)
#define EFX_EFFECT_ROLL4_B_R_R_L                              (EFX_EFFECT_ROLL_CLASS+220)
#define EFX_EFFECT_ROLL4_B_R_R_R                              (EFX_EFFECT_ROLL_CLASS+221)
#define EFX_EFFECT_ROLL4_B_R_R_T                              (EFX_EFFECT_ROLL_CLASS+222)
#define EFX_EFFECT_ROLL4_B_R_R_B                              (EFX_EFFECT_ROLL_CLASS+223)
#define EFX_EFFECT_ROLL4_B_R_T_L                              (EFX_EFFECT_ROLL_CLASS+224)
#define EFX_EFFECT_ROLL4_B_R_T_R                              (EFX_EFFECT_ROLL_CLASS+225)
#define EFX_EFFECT_ROLL4_B_R_T_T                              (EFX_EFFECT_ROLL_CLASS+226)
#define EFX_EFFECT_ROLL4_B_R_T_B                              (EFX_EFFECT_ROLL_CLASS+227)
#define EFX_EFFECT_ROLL4_B_R_B_L                              (EFX_EFFECT_ROLL_CLASS+228)
#define EFX_EFFECT_ROLL4_B_R_B_R                              (EFX_EFFECT_ROLL_CLASS+229)
#define EFX_EFFECT_ROLL4_B_R_B_T                              (EFX_EFFECT_ROLL_CLASS+230)
#define EFX_EFFECT_ROLL4_B_R_B_B                              (EFX_EFFECT_ROLL_CLASS+231)
#define EFX_EFFECT_ROLL4_B_T_L_L                              (EFX_EFFECT_ROLL_CLASS+232)
#define EFX_EFFECT_ROLL4_B_T_L_R                              (EFX_EFFECT_ROLL_CLASS+233)
#define EFX_EFFECT_ROLL4_B_T_L_T                              (EFX_EFFECT_ROLL_CLASS+234)
#define EFX_EFFECT_ROLL4_B_T_L_B                              (EFX_EFFECT_ROLL_CLASS+235)
#define EFX_EFFECT_ROLL4_B_T_R_L                              (EFX_EFFECT_ROLL_CLASS+236)
#define EFX_EFFECT_ROLL4_B_T_R_R                              (EFX_EFFECT_ROLL_CLASS+237)
#define EFX_EFFECT_ROLL4_B_T_R_T                              (EFX_EFFECT_ROLL_CLASS+238)
#define EFX_EFFECT_ROLL4_B_T_R_B                              (EFX_EFFECT_ROLL_CLASS+239)
#define EFX_EFFECT_ROLL4_B_T_T_L                              (EFX_EFFECT_ROLL_CLASS+240)
#define EFX_EFFECT_ROLL4_B_T_T_R                              (EFX_EFFECT_ROLL_CLASS+241)
#define EFX_EFFECT_ROLL4_B_T_T_T                              (EFX_EFFECT_ROLL_CLASS+242)
#define EFX_EFFECT_ROLL4_B_T_T_B                              (EFX_EFFECT_ROLL_CLASS+243)
#define EFX_EFFECT_ROLL4_B_T_B_L                              (EFX_EFFECT_ROLL_CLASS+244)
#define EFX_EFFECT_ROLL4_B_T_B_R                              (EFX_EFFECT_ROLL_CLASS+245)
#define EFX_EFFECT_ROLL4_B_T_B_T                              (EFX_EFFECT_ROLL_CLASS+246)
#define EFX_EFFECT_ROLL4_B_T_B_B                              (EFX_EFFECT_ROLL_CLASS+247)
#define EFX_EFFECT_ROLL4_B_B_L_L                              (EFX_EFFECT_ROLL_CLASS+248)
#define EFX_EFFECT_ROLL4_B_B_L_R                              (EFX_EFFECT_ROLL_CLASS+249)
#define EFX_EFFECT_ROLL4_B_B_L_T                              (EFX_EFFECT_ROLL_CLASS+250)
#define EFX_EFFECT_ROLL4_B_B_L_B                              (EFX_EFFECT_ROLL_CLASS+251)
#define EFX_EFFECT_ROLL4_B_B_R_L                              (EFX_EFFECT_ROLL_CLASS+252)
#define EFX_EFFECT_ROLL4_B_B_R_R                              (EFX_EFFECT_ROLL_CLASS+253)
#define EFX_EFFECT_ROLL4_B_B_R_T                              (EFX_EFFECT_ROLL_CLASS+254)
#define EFX_EFFECT_ROLL4_B_B_R_B                              (EFX_EFFECT_ROLL_CLASS+255)
#define EFX_EFFECT_ROLL4_B_B_T_L                              (EFX_EFFECT_ROLL_CLASS+256)
#define EFX_EFFECT_ROLL4_B_B_T_R                              (EFX_EFFECT_ROLL_CLASS+257)
#define EFX_EFFECT_ROLL4_B_B_T_T                              (EFX_EFFECT_ROLL_CLASS+258)
#define EFX_EFFECT_ROLL4_B_B_T_B                              (EFX_EFFECT_ROLL_CLASS+259)
#define EFX_EFFECT_ROLL4_B_B_B_L                              (EFX_EFFECT_ROLL_CLASS+260)
#define EFX_EFFECT_ROLL4_B_B_B_R                              (EFX_EFFECT_ROLL_CLASS+261)
#define EFX_EFFECT_ROLL4_B_B_B_T                              (EFX_EFFECT_ROLL_CLASS+262)
#define EFX_EFFECT_ROLL4_B_B_B_B                              (EFX_EFFECT_ROLL_CLASS+263)
#define EFX_EFFECT_ROLL_MAX                                   (EFX_EFFECT_ROLL_CLASS+263)

// Rotate
#define EFX_EFFECT_ROTATE_CLASS                               8000
#define EFX_EFFECT_ROTATE_L_TO_R                              (EFX_EFFECT_ROTATE_CLASS+0)
#define EFX_EFFECT_ROTATE_R_TO_L                              (EFX_EFFECT_ROTATE_CLASS+1)
#define EFX_EFFECT_ROTATE_T_TO_B                              (EFX_EFFECT_ROTATE_CLASS+2)
#define EFX_EFFECT_ROTATE_B_TO_T                              (EFX_EFFECT_ROTATE_CLASS+3)
#define EFX_EFFECT_ROTATE4_L_L_L_L                            (EFX_EFFECT_ROTATE_CLASS+4)
#define EFX_EFFECT_ROTATE4_L_L_L_R                            (EFX_EFFECT_ROTATE_CLASS+5)
#define EFX_EFFECT_ROTATE4_L_L_L_T                            (EFX_EFFECT_ROTATE_CLASS+6)
#define EFX_EFFECT_ROTATE4_L_L_L_B                            (EFX_EFFECT_ROTATE_CLASS+7)
#define EFX_EFFECT_ROTATE4_L_L_R_L                            (EFX_EFFECT_ROTATE_CLASS+8)
#define EFX_EFFECT_ROTATE4_L_L_R_R                            (EFX_EFFECT_ROTATE_CLASS+9)
#define EFX_EFFECT_ROTATE4_L_L_R_T                            (EFX_EFFECT_ROTATE_CLASS+10)
#define EFX_EFFECT_ROTATE4_L_L_R_B                            (EFX_EFFECT_ROTATE_CLASS+11)
#define EFX_EFFECT_ROTATE4_L_L_T_L                            (EFX_EFFECT_ROTATE_CLASS+12)
#define EFX_EFFECT_ROTATE4_L_L_T_R                            (EFX_EFFECT_ROTATE_CLASS+13)
#define EFX_EFFECT_ROTATE4_L_L_T_T                            (EFX_EFFECT_ROTATE_CLASS+14)
#define EFX_EFFECT_ROTATE4_L_L_T_B                            (EFX_EFFECT_ROTATE_CLASS+15)
#define EFX_EFFECT_ROTATE4_L_L_B_L                            (EFX_EFFECT_ROTATE_CLASS+16)
#define EFX_EFFECT_ROTATE4_L_L_B_R                            (EFX_EFFECT_ROTATE_CLASS+17)
#define EFX_EFFECT_ROTATE4_L_L_B_T                            (EFX_EFFECT_ROTATE_CLASS+18)
#define EFX_EFFECT_ROTATE4_L_L_B_B                            (EFX_EFFECT_ROTATE_CLASS+19)
#define EFX_EFFECT_ROTATE4_L_R_L_L                            (EFX_EFFECT_ROTATE_CLASS+20)
#define EFX_EFFECT_ROTATE4_L_R_L_R                            (EFX_EFFECT_ROTATE_CLASS+21)
#define EFX_EFFECT_ROTATE4_L_R_L_T                            (EFX_EFFECT_ROTATE_CLASS+22)
#define EFX_EFFECT_ROTATE4_L_R_L_B                            (EFX_EFFECT_ROTATE_CLASS+23)
#define EFX_EFFECT_ROTATE4_L_R_R_L                            (EFX_EFFECT_ROTATE_CLASS+24)
#define EFX_EFFECT_ROTATE4_L_R_R_R                            (EFX_EFFECT_ROTATE_CLASS+25)
#define EFX_EFFECT_ROTATE4_L_R_R_T                            (EFX_EFFECT_ROTATE_CLASS+26)
#define EFX_EFFECT_ROTATE4_L_R_R_B                            (EFX_EFFECT_ROTATE_CLASS+27)
#define EFX_EFFECT_ROTATE4_L_R_T_L                            (EFX_EFFECT_ROTATE_CLASS+28)
#define EFX_EFFECT_ROTATE4_L_R_T_R                            (EFX_EFFECT_ROTATE_CLASS+29)
#define EFX_EFFECT_ROTATE4_L_R_T_T                            (EFX_EFFECT_ROTATE_CLASS+30)
#define EFX_EFFECT_ROTATE4_L_R_T_B                            (EFX_EFFECT_ROTATE_CLASS+31)
#define EFX_EFFECT_ROTATE4_L_R_B_L                            (EFX_EFFECT_ROTATE_CLASS+32)
#define EFX_EFFECT_ROTATE4_L_R_B_R                            (EFX_EFFECT_ROTATE_CLASS+33)
#define EFX_EFFECT_ROTATE4_L_R_B_T                            (EFX_EFFECT_ROTATE_CLASS+34)
#define EFX_EFFECT_ROTATE4_L_R_B_B                            (EFX_EFFECT_ROTATE_CLASS+35)
#define EFX_EFFECT_ROTATE4_L_T_L_L                            (EFX_EFFECT_ROTATE_CLASS+36)
#define EFX_EFFECT_ROTATE4_L_T_L_R                            (EFX_EFFECT_ROTATE_CLASS+37)
#define EFX_EFFECT_ROTATE4_L_T_L_T                            (EFX_EFFECT_ROTATE_CLASS+38)
#define EFX_EFFECT_ROTATE4_L_T_L_B                            (EFX_EFFECT_ROTATE_CLASS+39)
#define EFX_EFFECT_ROTATE4_L_T_R_L                            (EFX_EFFECT_ROTATE_CLASS+40)
#define EFX_EFFECT_ROTATE4_L_T_R_R                            (EFX_EFFECT_ROTATE_CLASS+41)
#define EFX_EFFECT_ROTATE4_L_T_R_T                            (EFX_EFFECT_ROTATE_CLASS+42)
#define EFX_EFFECT_ROTATE4_L_T_R_B                            (EFX_EFFECT_ROTATE_CLASS+43)
#define EFX_EFFECT_ROTATE4_L_T_T_L                            (EFX_EFFECT_ROTATE_CLASS+44)
#define EFX_EFFECT_ROTATE4_L_T_T_R                            (EFX_EFFECT_ROTATE_CLASS+45)
#define EFX_EFFECT_ROTATE4_L_T_T_T                            (EFX_EFFECT_ROTATE_CLASS+46)
#define EFX_EFFECT_ROTATE4_L_T_T_B                            (EFX_EFFECT_ROTATE_CLASS+47)
#define EFX_EFFECT_ROTATE4_L_T_B_L                            (EFX_EFFECT_ROTATE_CLASS+48)
#define EFX_EFFECT_ROTATE4_L_T_B_R                            (EFX_EFFECT_ROTATE_CLASS+49)
#define EFX_EFFECT_ROTATE4_L_T_B_T                            (EFX_EFFECT_ROTATE_CLASS+50)
#define EFX_EFFECT_ROTATE4_L_T_B_B                            (EFX_EFFECT_ROTATE_CLASS+51)
#define EFX_EFFECT_ROTATE4_L_B_L_L                            (EFX_EFFECT_ROTATE_CLASS+52)
#define EFX_EFFECT_ROTATE4_L_B_L_R                            (EFX_EFFECT_ROTATE_CLASS+53)
#define EFX_EFFECT_ROTATE4_L_B_L_T                            (EFX_EFFECT_ROTATE_CLASS+54)
#define EFX_EFFECT_ROTATE4_L_B_L_B                            (EFX_EFFECT_ROTATE_CLASS+55)
#define EFX_EFFECT_ROTATE4_L_B_R_L                            (EFX_EFFECT_ROTATE_CLASS+56)
#define EFX_EFFECT_ROTATE4_L_B_R_R                            (EFX_EFFECT_ROTATE_CLASS+57)
#define EFX_EFFECT_ROTATE4_L_B_R_T                            (EFX_EFFECT_ROTATE_CLASS+58)
#define EFX_EFFECT_ROTATE4_L_B_R_B                            (EFX_EFFECT_ROTATE_CLASS+59)
#define EFX_EFFECT_ROTATE4_L_B_T_L                            (EFX_EFFECT_ROTATE_CLASS+60)
#define EFX_EFFECT_ROTATE4_L_B_T_R                            (EFX_EFFECT_ROTATE_CLASS+61)
#define EFX_EFFECT_ROTATE4_L_B_T_T                            (EFX_EFFECT_ROTATE_CLASS+62)
#define EFX_EFFECT_ROTATE4_L_B_T_B                            (EFX_EFFECT_ROTATE_CLASS+63)
#define EFX_EFFECT_ROTATE4_L_B_B_L                            (EFX_EFFECT_ROTATE_CLASS+64)
#define EFX_EFFECT_ROTATE4_L_B_B_R                            (EFX_EFFECT_ROTATE_CLASS+65)
#define EFX_EFFECT_ROTATE4_L_B_B_T                            (EFX_EFFECT_ROTATE_CLASS+66)
#define EFX_EFFECT_ROTATE4_L_B_B_B                            (EFX_EFFECT_ROTATE_CLASS+67)
#define EFX_EFFECT_ROTATE4_R_L_L_L                            (EFX_EFFECT_ROTATE_CLASS+68)
#define EFX_EFFECT_ROTATE4_R_L_L_R                            (EFX_EFFECT_ROTATE_CLASS+69)
#define EFX_EFFECT_ROTATE4_R_L_L_T                            (EFX_EFFECT_ROTATE_CLASS+70)
#define EFX_EFFECT_ROTATE4_R_L_L_B                            (EFX_EFFECT_ROTATE_CLASS+71)
#define EFX_EFFECT_ROTATE4_R_L_R_L                            (EFX_EFFECT_ROTATE_CLASS+72)
#define EFX_EFFECT_ROTATE4_R_L_R_R                            (EFX_EFFECT_ROTATE_CLASS+73)
#define EFX_EFFECT_ROTATE4_R_L_R_T                            (EFX_EFFECT_ROTATE_CLASS+74)
#define EFX_EFFECT_ROTATE4_R_L_R_B                            (EFX_EFFECT_ROTATE_CLASS+75)
#define EFX_EFFECT_ROTATE4_R_L_T_L                            (EFX_EFFECT_ROTATE_CLASS+76)
#define EFX_EFFECT_ROTATE4_R_L_T_R                            (EFX_EFFECT_ROTATE_CLASS+77)
#define EFX_EFFECT_ROTATE4_R_L_T_T                            (EFX_EFFECT_ROTATE_CLASS+78)
#define EFX_EFFECT_ROTATE4_R_L_T_B                            (EFX_EFFECT_ROTATE_CLASS+79)
#define EFX_EFFECT_ROTATE4_R_L_B_L                            (EFX_EFFECT_ROTATE_CLASS+80)
#define EFX_EFFECT_ROTATE4_R_L_B_R                            (EFX_EFFECT_ROTATE_CLASS+81)
#define EFX_EFFECT_ROTATE4_R_L_B_T                            (EFX_EFFECT_ROTATE_CLASS+82)
#define EFX_EFFECT_ROTATE4_R_L_B_B                            (EFX_EFFECT_ROTATE_CLASS+83)
#define EFX_EFFECT_ROTATE4_R_R_L_L                            (EFX_EFFECT_ROTATE_CLASS+84)
#define EFX_EFFECT_ROTATE4_R_R_L_R                            (EFX_EFFECT_ROTATE_CLASS+85)
#define EFX_EFFECT_ROTATE4_R_R_L_T                            (EFX_EFFECT_ROTATE_CLASS+86)
#define EFX_EFFECT_ROTATE4_R_R_L_B                            (EFX_EFFECT_ROTATE_CLASS+87)
#define EFX_EFFECT_ROTATE4_R_R_R_L                            (EFX_EFFECT_ROTATE_CLASS+88)
#define EFX_EFFECT_ROTATE4_R_R_R_R                            (EFX_EFFECT_ROTATE_CLASS+89)
#define EFX_EFFECT_ROTATE4_R_R_R_T                            (EFX_EFFECT_ROTATE_CLASS+90)
#define EFX_EFFECT_ROTATE4_R_R_R_B                            (EFX_EFFECT_ROTATE_CLASS+91)
#define EFX_EFFECT_ROTATE4_R_R_T_L                            (EFX_EFFECT_ROTATE_CLASS+92)
#define EFX_EFFECT_ROTATE4_R_R_T_R                            (EFX_EFFECT_ROTATE_CLASS+93)
#define EFX_EFFECT_ROTATE4_R_R_T_T                            (EFX_EFFECT_ROTATE_CLASS+94)
#define EFX_EFFECT_ROTATE4_R_R_T_B                            (EFX_EFFECT_ROTATE_CLASS+95)
#define EFX_EFFECT_ROTATE4_R_R_B_L                            (EFX_EFFECT_ROTATE_CLASS+96)
#define EFX_EFFECT_ROTATE4_R_R_B_R                            (EFX_EFFECT_ROTATE_CLASS+97)
#define EFX_EFFECT_ROTATE4_R_R_B_T                            (EFX_EFFECT_ROTATE_CLASS+98)
#define EFX_EFFECT_ROTATE4_R_R_B_B                            (EFX_EFFECT_ROTATE_CLASS+99)
#define EFX_EFFECT_ROTATE4_R_T_L_L                            (EFX_EFFECT_ROTATE_CLASS+100)
#define EFX_EFFECT_ROTATE4_R_T_L_R                            (EFX_EFFECT_ROTATE_CLASS+101)
#define EFX_EFFECT_ROTATE4_R_T_L_T                            (EFX_EFFECT_ROTATE_CLASS+102)
#define EFX_EFFECT_ROTATE4_R_T_L_B                            (EFX_EFFECT_ROTATE_CLASS+103)
#define EFX_EFFECT_ROTATE4_R_T_R_L                            (EFX_EFFECT_ROTATE_CLASS+104)
#define EFX_EFFECT_ROTATE4_R_T_R_R                            (EFX_EFFECT_ROTATE_CLASS+105)
#define EFX_EFFECT_ROTATE4_R_T_R_T                            (EFX_EFFECT_ROTATE_CLASS+106)
#define EFX_EFFECT_ROTATE4_R_T_R_B                            (EFX_EFFECT_ROTATE_CLASS+107)
#define EFX_EFFECT_ROTATE4_R_T_T_L                            (EFX_EFFECT_ROTATE_CLASS+108)
#define EFX_EFFECT_ROTATE4_R_T_T_R                            (EFX_EFFECT_ROTATE_CLASS+109)
#define EFX_EFFECT_ROTATE4_R_T_T_T                            (EFX_EFFECT_ROTATE_CLASS+110)
#define EFX_EFFECT_ROTATE4_R_T_T_B                            (EFX_EFFECT_ROTATE_CLASS+111)
#define EFX_EFFECT_ROTATE4_R_T_B_L                            (EFX_EFFECT_ROTATE_CLASS+112)
#define EFX_EFFECT_ROTATE4_R_T_B_R                            (EFX_EFFECT_ROTATE_CLASS+113)
#define EFX_EFFECT_ROTATE4_R_T_B_T                            (EFX_EFFECT_ROTATE_CLASS+114)
#define EFX_EFFECT_ROTATE4_R_T_B_B                            (EFX_EFFECT_ROTATE_CLASS+115)
#define EFX_EFFECT_ROTATE4_R_B_L_L                            (EFX_EFFECT_ROTATE_CLASS+116)
#define EFX_EFFECT_ROTATE4_R_B_L_R                            (EFX_EFFECT_ROTATE_CLASS+117)
#define EFX_EFFECT_ROTATE4_R_B_L_T                            (EFX_EFFECT_ROTATE_CLASS+118)
#define EFX_EFFECT_ROTATE4_R_B_L_B                            (EFX_EFFECT_ROTATE_CLASS+119)
#define EFX_EFFECT_ROTATE4_R_B_R_L                            (EFX_EFFECT_ROTATE_CLASS+120)
#define EFX_EFFECT_ROTATE4_R_B_R_R                            (EFX_EFFECT_ROTATE_CLASS+121)
#define EFX_EFFECT_ROTATE4_R_B_R_T                            (EFX_EFFECT_ROTATE_CLASS+122)
#define EFX_EFFECT_ROTATE4_R_B_R_B                            (EFX_EFFECT_ROTATE_CLASS+123)
#define EFX_EFFECT_ROTATE4_R_B_T_L                            (EFX_EFFECT_ROTATE_CLASS+124)
#define EFX_EFFECT_ROTATE4_R_B_T_R                            (EFX_EFFECT_ROTATE_CLASS+125)
#define EFX_EFFECT_ROTATE4_R_B_T_T                            (EFX_EFFECT_ROTATE_CLASS+126)
#define EFX_EFFECT_ROTATE4_R_B_T_B                            (EFX_EFFECT_ROTATE_CLASS+127)
#define EFX_EFFECT_ROTATE4_R_B_B_L                            (EFX_EFFECT_ROTATE_CLASS+128)
#define EFX_EFFECT_ROTATE4_R_B_B_R                            (EFX_EFFECT_ROTATE_CLASS+129)
#define EFX_EFFECT_ROTATE4_R_B_B_T                            (EFX_EFFECT_ROTATE_CLASS+130)
#define EFX_EFFECT_ROTATE4_R_B_B_B                            (EFX_EFFECT_ROTATE_CLASS+131)
#define EFX_EFFECT_ROTATE4_T_L_L_L                            (EFX_EFFECT_ROTATE_CLASS+132)
#define EFX_EFFECT_ROTATE4_T_L_L_R                            (EFX_EFFECT_ROTATE_CLASS+133)
#define EFX_EFFECT_ROTATE4_T_L_L_T                            (EFX_EFFECT_ROTATE_CLASS+134)
#define EFX_EFFECT_ROTATE4_T_L_L_B                            (EFX_EFFECT_ROTATE_CLASS+135)
#define EFX_EFFECT_ROTATE4_T_L_R_L                            (EFX_EFFECT_ROTATE_CLASS+136)
#define EFX_EFFECT_ROTATE4_T_L_R_R                            (EFX_EFFECT_ROTATE_CLASS+137)
#define EFX_EFFECT_ROTATE4_T_L_R_T                            (EFX_EFFECT_ROTATE_CLASS+138)
#define EFX_EFFECT_ROTATE4_T_L_R_B                            (EFX_EFFECT_ROTATE_CLASS+139)
#define EFX_EFFECT_ROTATE4_T_L_T_L                            (EFX_EFFECT_ROTATE_CLASS+140)
#define EFX_EFFECT_ROTATE4_T_L_T_R                            (EFX_EFFECT_ROTATE_CLASS+141)
#define EFX_EFFECT_ROTATE4_T_L_T_T                            (EFX_EFFECT_ROTATE_CLASS+142)
#define EFX_EFFECT_ROTATE4_T_L_T_B                            (EFX_EFFECT_ROTATE_CLASS+143)
#define EFX_EFFECT_ROTATE4_T_L_B_L                            (EFX_EFFECT_ROTATE_CLASS+144)
#define EFX_EFFECT_ROTATE4_T_L_B_R                            (EFX_EFFECT_ROTATE_CLASS+145)
#define EFX_EFFECT_ROTATE4_T_L_B_T                            (EFX_EFFECT_ROTATE_CLASS+146)
#define EFX_EFFECT_ROTATE4_T_L_B_B                            (EFX_EFFECT_ROTATE_CLASS+147)
#define EFX_EFFECT_ROTATE4_T_R_L_L                            (EFX_EFFECT_ROTATE_CLASS+148)
#define EFX_EFFECT_ROTATE4_T_R_L_R                            (EFX_EFFECT_ROTATE_CLASS+149)
#define EFX_EFFECT_ROTATE4_T_R_L_T                            (EFX_EFFECT_ROTATE_CLASS+150)
#define EFX_EFFECT_ROTATE4_T_R_L_B                            (EFX_EFFECT_ROTATE_CLASS+151)
#define EFX_EFFECT_ROTATE4_T_R_R_L                            (EFX_EFFECT_ROTATE_CLASS+152)
#define EFX_EFFECT_ROTATE4_T_R_R_R                            (EFX_EFFECT_ROTATE_CLASS+153)
#define EFX_EFFECT_ROTATE4_T_R_R_T                            (EFX_EFFECT_ROTATE_CLASS+154)
#define EFX_EFFECT_ROTATE4_T_R_R_B                            (EFX_EFFECT_ROTATE_CLASS+155)
#define EFX_EFFECT_ROTATE4_T_R_T_L                            (EFX_EFFECT_ROTATE_CLASS+156)
#define EFX_EFFECT_ROTATE4_T_R_T_R                            (EFX_EFFECT_ROTATE_CLASS+157)
#define EFX_EFFECT_ROTATE4_T_R_T_T                            (EFX_EFFECT_ROTATE_CLASS+158)
#define EFX_EFFECT_ROTATE4_T_R_T_B                            (EFX_EFFECT_ROTATE_CLASS+159)
#define EFX_EFFECT_ROTATE4_T_R_B_L                            (EFX_EFFECT_ROTATE_CLASS+160)
#define EFX_EFFECT_ROTATE4_T_R_B_R                            (EFX_EFFECT_ROTATE_CLASS+161)
#define EFX_EFFECT_ROTATE4_T_R_B_T                            (EFX_EFFECT_ROTATE_CLASS+162)
#define EFX_EFFECT_ROTATE4_T_R_B_B                            (EFX_EFFECT_ROTATE_CLASS+163)
#define EFX_EFFECT_ROTATE4_T_T_L_L                            (EFX_EFFECT_ROTATE_CLASS+164)
#define EFX_EFFECT_ROTATE4_T_T_L_R                            (EFX_EFFECT_ROTATE_CLASS+165)
#define EFX_EFFECT_ROTATE4_T_T_L_T                            (EFX_EFFECT_ROTATE_CLASS+166)
#define EFX_EFFECT_ROTATE4_T_T_L_B                            (EFX_EFFECT_ROTATE_CLASS+167)
#define EFX_EFFECT_ROTATE4_T_T_R_L                            (EFX_EFFECT_ROTATE_CLASS+168)
#define EFX_EFFECT_ROTATE4_T_T_R_R                            (EFX_EFFECT_ROTATE_CLASS+169)
#define EFX_EFFECT_ROTATE4_T_T_R_T                            (EFX_EFFECT_ROTATE_CLASS+170)
#define EFX_EFFECT_ROTATE4_T_T_R_B                            (EFX_EFFECT_ROTATE_CLASS+171)
#define EFX_EFFECT_ROTATE4_T_T_T_L                            (EFX_EFFECT_ROTATE_CLASS+172)
#define EFX_EFFECT_ROTATE4_T_T_T_R                            (EFX_EFFECT_ROTATE_CLASS+173)
#define EFX_EFFECT_ROTATE4_T_T_T_T                            (EFX_EFFECT_ROTATE_CLASS+174)
#define EFX_EFFECT_ROTATE4_T_T_T_B                            (EFX_EFFECT_ROTATE_CLASS+175)
#define EFX_EFFECT_ROTATE4_T_T_B_L                            (EFX_EFFECT_ROTATE_CLASS+176)
#define EFX_EFFECT_ROTATE4_T_T_B_R                            (EFX_EFFECT_ROTATE_CLASS+177)
#define EFX_EFFECT_ROTATE4_T_T_B_T                            (EFX_EFFECT_ROTATE_CLASS+178)
#define EFX_EFFECT_ROTATE4_T_T_B_B                            (EFX_EFFECT_ROTATE_CLASS+179)
#define EFX_EFFECT_ROTATE4_T_B_L_L                            (EFX_EFFECT_ROTATE_CLASS+180)
#define EFX_EFFECT_ROTATE4_T_B_L_R                            (EFX_EFFECT_ROTATE_CLASS+181)
#define EFX_EFFECT_ROTATE4_T_B_L_T                            (EFX_EFFECT_ROTATE_CLASS+182)
#define EFX_EFFECT_ROTATE4_T_B_L_B                            (EFX_EFFECT_ROTATE_CLASS+183)
#define EFX_EFFECT_ROTATE4_T_B_R_L                            (EFX_EFFECT_ROTATE_CLASS+184)
#define EFX_EFFECT_ROTATE4_T_B_R_R                            (EFX_EFFECT_ROTATE_CLASS+185)
#define EFX_EFFECT_ROTATE4_T_B_R_T                            (EFX_EFFECT_ROTATE_CLASS+186)
#define EFX_EFFECT_ROTATE4_T_B_R_B                            (EFX_EFFECT_ROTATE_CLASS+187)
#define EFX_EFFECT_ROTATE4_T_B_T_L                            (EFX_EFFECT_ROTATE_CLASS+188)
#define EFX_EFFECT_ROTATE4_T_B_T_R                            (EFX_EFFECT_ROTATE_CLASS+189)
#define EFX_EFFECT_ROTATE4_T_B_T_T                            (EFX_EFFECT_ROTATE_CLASS+190)
#define EFX_EFFECT_ROTATE4_T_B_T_B                            (EFX_EFFECT_ROTATE_CLASS+191)
#define EFX_EFFECT_ROTATE4_T_B_B_L                            (EFX_EFFECT_ROTATE_CLASS+192)
#define EFX_EFFECT_ROTATE4_T_B_B_R                            (EFX_EFFECT_ROTATE_CLASS+193)
#define EFX_EFFECT_ROTATE4_T_B_B_T                            (EFX_EFFECT_ROTATE_CLASS+194)
#define EFX_EFFECT_ROTATE4_T_B_B_B                            (EFX_EFFECT_ROTATE_CLASS+195)
#define EFX_EFFECT_ROTATE4_B_L_L_L                            (EFX_EFFECT_ROTATE_CLASS+196)
#define EFX_EFFECT_ROTATE4_B_L_L_R                            (EFX_EFFECT_ROTATE_CLASS+197)
#define EFX_EFFECT_ROTATE4_B_L_L_T                            (EFX_EFFECT_ROTATE_CLASS+198)
#define EFX_EFFECT_ROTATE4_B_L_L_B                            (EFX_EFFECT_ROTATE_CLASS+199)
#define EFX_EFFECT_ROTATE4_B_L_R_L                            (EFX_EFFECT_ROTATE_CLASS+200)
#define EFX_EFFECT_ROTATE4_B_L_R_R                            (EFX_EFFECT_ROTATE_CLASS+201)
#define EFX_EFFECT_ROTATE4_B_L_R_T                            (EFX_EFFECT_ROTATE_CLASS+202)
#define EFX_EFFECT_ROTATE4_B_L_R_B                            (EFX_EFFECT_ROTATE_CLASS+203)
#define EFX_EFFECT_ROTATE4_B_L_T_L                            (EFX_EFFECT_ROTATE_CLASS+204)
#define EFX_EFFECT_ROTATE4_B_L_T_R                            (EFX_EFFECT_ROTATE_CLASS+205)
#define EFX_EFFECT_ROTATE4_B_L_T_T                            (EFX_EFFECT_ROTATE_CLASS+206)
#define EFX_EFFECT_ROTATE4_B_L_T_B                            (EFX_EFFECT_ROTATE_CLASS+207)
#define EFX_EFFECT_ROTATE4_B_L_B_L                            (EFX_EFFECT_ROTATE_CLASS+208)
#define EFX_EFFECT_ROTATE4_B_L_B_R                            (EFX_EFFECT_ROTATE_CLASS+209)
#define EFX_EFFECT_ROTATE4_B_L_B_T                            (EFX_EFFECT_ROTATE_CLASS+210)
#define EFX_EFFECT_ROTATE4_B_L_B_B                            (EFX_EFFECT_ROTATE_CLASS+211)
#define EFX_EFFECT_ROTATE4_B_R_L_L                            (EFX_EFFECT_ROTATE_CLASS+212)
#define EFX_EFFECT_ROTATE4_B_R_L_R                            (EFX_EFFECT_ROTATE_CLASS+213)
#define EFX_EFFECT_ROTATE4_B_R_L_T                            (EFX_EFFECT_ROTATE_CLASS+214)
#define EFX_EFFECT_ROTATE4_B_R_L_B                            (EFX_EFFECT_ROTATE_CLASS+215)
#define EFX_EFFECT_ROTATE4_B_R_R_L                            (EFX_EFFECT_ROTATE_CLASS+216)
#define EFX_EFFECT_ROTATE4_B_R_R_R                            (EFX_EFFECT_ROTATE_CLASS+217)
#define EFX_EFFECT_ROTATE4_B_R_R_T                            (EFX_EFFECT_ROTATE_CLASS+218)
#define EFX_EFFECT_ROTATE4_B_R_R_B                            (EFX_EFFECT_ROTATE_CLASS+219)
#define EFX_EFFECT_ROTATE4_B_R_T_L                            (EFX_EFFECT_ROTATE_CLASS+220)
#define EFX_EFFECT_ROTATE4_B_R_T_R                            (EFX_EFFECT_ROTATE_CLASS+221)
#define EFX_EFFECT_ROTATE4_B_R_T_T                            (EFX_EFFECT_ROTATE_CLASS+222)
#define EFX_EFFECT_ROTATE4_B_R_T_B                            (EFX_EFFECT_ROTATE_CLASS+223)
#define EFX_EFFECT_ROTATE4_B_R_B_L                            (EFX_EFFECT_ROTATE_CLASS+224)
#define EFX_EFFECT_ROTATE4_B_R_B_R                            (EFX_EFFECT_ROTATE_CLASS+225)
#define EFX_EFFECT_ROTATE4_B_R_B_T                            (EFX_EFFECT_ROTATE_CLASS+226)
#define EFX_EFFECT_ROTATE4_B_R_B_B                            (EFX_EFFECT_ROTATE_CLASS+227)
#define EFX_EFFECT_ROTATE4_B_T_L_L                            (EFX_EFFECT_ROTATE_CLASS+228)
#define EFX_EFFECT_ROTATE4_B_T_L_R                            (EFX_EFFECT_ROTATE_CLASS+229)
#define EFX_EFFECT_ROTATE4_B_T_L_T                            (EFX_EFFECT_ROTATE_CLASS+230)
#define EFX_EFFECT_ROTATE4_B_T_L_B                            (EFX_EFFECT_ROTATE_CLASS+231)
#define EFX_EFFECT_ROTATE4_B_T_R_L                            (EFX_EFFECT_ROTATE_CLASS+232)
#define EFX_EFFECT_ROTATE4_B_T_R_R                            (EFX_EFFECT_ROTATE_CLASS+233)
#define EFX_EFFECT_ROTATE4_B_T_R_T                            (EFX_EFFECT_ROTATE_CLASS+234)
#define EFX_EFFECT_ROTATE4_B_T_R_B                            (EFX_EFFECT_ROTATE_CLASS+235)
#define EFX_EFFECT_ROTATE4_B_T_T_L                            (EFX_EFFECT_ROTATE_CLASS+236)
#define EFX_EFFECT_ROTATE4_B_T_T_R                            (EFX_EFFECT_ROTATE_CLASS+237)
#define EFX_EFFECT_ROTATE4_B_T_T_T                            (EFX_EFFECT_ROTATE_CLASS+238)
#define EFX_EFFECT_ROTATE4_B_T_T_B                            (EFX_EFFECT_ROTATE_CLASS+239)
#define EFX_EFFECT_ROTATE4_B_T_B_L                            (EFX_EFFECT_ROTATE_CLASS+240)
#define EFX_EFFECT_ROTATE4_B_T_B_R                            (EFX_EFFECT_ROTATE_CLASS+241)
#define EFX_EFFECT_ROTATE4_B_T_B_T                            (EFX_EFFECT_ROTATE_CLASS+242)
#define EFX_EFFECT_ROTATE4_B_T_B_B                            (EFX_EFFECT_ROTATE_CLASS+243)
#define EFX_EFFECT_ROTATE4_B_B_L_L                            (EFX_EFFECT_ROTATE_CLASS+244)
#define EFX_EFFECT_ROTATE4_B_B_L_R                            (EFX_EFFECT_ROTATE_CLASS+245)
#define EFX_EFFECT_ROTATE4_B_B_L_T                            (EFX_EFFECT_ROTATE_CLASS+246)
#define EFX_EFFECT_ROTATE4_B_B_L_B                            (EFX_EFFECT_ROTATE_CLASS+247)
#define EFX_EFFECT_ROTATE4_B_B_R_L                            (EFX_EFFECT_ROTATE_CLASS+248)
#define EFX_EFFECT_ROTATE4_B_B_R_R                            (EFX_EFFECT_ROTATE_CLASS+249)
#define EFX_EFFECT_ROTATE4_B_B_R_T                            (EFX_EFFECT_ROTATE_CLASS+250)
#define EFX_EFFECT_ROTATE4_B_B_R_B                            (EFX_EFFECT_ROTATE_CLASS+251)
#define EFX_EFFECT_ROTATE4_B_B_T_L                            (EFX_EFFECT_ROTATE_CLASS+252)
#define EFX_EFFECT_ROTATE4_B_B_T_R                            (EFX_EFFECT_ROTATE_CLASS+253)
#define EFX_EFFECT_ROTATE4_B_B_T_T                            (EFX_EFFECT_ROTATE_CLASS+254)
#define EFX_EFFECT_ROTATE4_B_B_T_B                            (EFX_EFFECT_ROTATE_CLASS+255)
#define EFX_EFFECT_ROTATE4_B_B_B_L                            (EFX_EFFECT_ROTATE_CLASS+256)
#define EFX_EFFECT_ROTATE4_B_B_B_R                            (EFX_EFFECT_ROTATE_CLASS+257)
#define EFX_EFFECT_ROTATE4_B_B_B_T                            (EFX_EFFECT_ROTATE_CLASS+258)
#define EFX_EFFECT_ROTATE4_B_B_B_B                            (EFX_EFFECT_ROTATE_CLASS+259)
#define EFX_EFFECT_ROTATE_MAX                                 (EFX_EFFECT_ROTATE_CLASS+259)

// Zoom
#define EFX_EFFECT_ZOOM_CLASS                                 9000
#define EFX_EFFECT_ZOOM_TO_C                                  (EFX_EFFECT_ZOOM_CLASS+0)
#define EFX_EFFECT_ZOOM_FROM_C                                (EFX_EFFECT_ZOOM_CLASS+1)
#define EFX_EFFECT_ZOOM_MAX                                   (EFX_EFFECT_ZOOM_CLASS+1)

// Drip
#define EFX_EFFECT_DRIP_CLASS                                 10000
#define EFX_EFFECT_DRIP_T_TO_B                                (EFX_EFFECT_DRIP_CLASS+0)
#define EFX_EFFECT_DRIP_B_TO_T                                (EFX_EFFECT_DRIP_CLASS+1)
#define EFX_EFFECT_DRIP_L_TO_R                                (EFX_EFFECT_DRIP_CLASS+2)
#define EFX_EFFECT_DRIP_R_TO_L                                (EFX_EFFECT_DRIP_CLASS+3)
#define EFX_EFFECT_DRIP_MAX                                   (EFX_EFFECT_DRIP_CLASS+3)

// Blind
#define EFX_EFFECT_BLIND_CLASS                                11000
#define EFX_EFFECT_BLIND_T_TO_B                               (EFX_EFFECT_BLIND_CLASS+0)
#define EFX_EFFECT_BLIND_L_TO_R                               (EFX_EFFECT_BLIND_CLASS+1)
#define EFX_EFFECT_BLIND_GROWING_L_TO_R                       (EFX_EFFECT_BLIND_CLASS+2)
#define EFX_EFFECT_BLIND_GROWING_R_TO_L                       (EFX_EFFECT_BLIND_CLASS+3)
#define EFX_EFFECT_BLIND_GROWING_T_TO_B                       (EFX_EFFECT_BLIND_CLASS+4)
#define EFX_EFFECT_BLIND_GROWING_B_TO_T                       (EFX_EFFECT_BLIND_CLASS+5)
#define EFX_EFFECT_BLIND2_T_TO_B_AND_L_TO_R                   (EFX_EFFECT_BLIND_CLASS+6)
#define EFX_EFFECT_BLIND_MAX                                  (EFX_EFFECT_BLIND_CLASS+6)

// Random
#define EFX_EFFECT_RANDOM_CLASS                               12000
#define EFX_EFFECT_RANDOM_BARS_R_TO_L                         (EFX_EFFECT_RANDOM_CLASS+0)
#define EFX_EFFECT_RANDOM_BARS_L_TO_R                         (EFX_EFFECT_RANDOM_CLASS+1)
#define EFX_EFFECT_RANDOM_BARS_B_TO_T                         (EFX_EFFECT_RANDOM_CLASS+2)
#define EFX_EFFECT_RANDOM_BARS_T_TO_B                         (EFX_EFFECT_RANDOM_CLASS+3)
#define EFX_EFFECT_RANDOM_LINES                               (EFX_EFFECT_RANDOM_CLASS+4)
#define EFX_EFFECT_RANDOM_BLOCKS                              (EFX_EFFECT_RANDOM_CLASS+5)
#define EFX_EFFECT_RANDOM_BLOCKS_R_TO_L                       (EFX_EFFECT_RANDOM_CLASS+6)
#define EFX_EFFECT_RANDOM_BLOCKS_L_TO_R                       (EFX_EFFECT_RANDOM_CLASS+7)
#define EFX_EFFECT_RANDOM_BLOCKS_B_TO_T                       (EFX_EFFECT_RANDOM_CLASS+8)
#define EFX_EFFECT_RANDOM_BLOCKS_T_TO_B                       (EFX_EFFECT_RANDOM_CLASS+9)
#define EFX_EFFECT_RANDOM_MAX                                 (EFX_EFFECT_RANDOM_CLASS+9)

// Checkerboard
#define EFX_EFFECT_CHECKERBOARD_CLASS                         13000
#define EFX_EFFECT_CHECKERBOARD_T_TO_B_THEN_T_TO_B            (EFX_EFFECT_CHECKERBOARD_CLASS+0)
#define EFX_EFFECT_CHECKERBOARD_B_TO_T_THEN_B_TO_T            (EFX_EFFECT_CHECKERBOARD_CLASS+1)
#define EFX_EFFECT_CHECKERBOARD_L_TO_R_THEN_L_TO_R            (EFX_EFFECT_CHECKERBOARD_CLASS+2)
#define EFX_EFFECT_CHECKERBOARD_R_TO_L_THEN_R_TO_L            (EFX_EFFECT_CHECKERBOARD_CLASS+3)
#define EFX_EFFECT_CHECKERBOARD_T_TO_B_THEN_B_TO_T            (EFX_EFFECT_CHECKERBOARD_CLASS+4)
#define EFX_EFFECT_CHECKERBOARD_B_TO_T_THEN_T_TO_B            (EFX_EFFECT_CHECKERBOARD_CLASS+5)
#define EFX_EFFECT_CHECKERBOARD_L_TO_R_THEN_R_TO_L            (EFX_EFFECT_CHECKERBOARD_CLASS+6)
#define EFX_EFFECT_CHECKERBOARD_R_TO_L_THEN_L_TO_R            (EFX_EFFECT_CHECKERBOARD_CLASS+7)
#define EFX_EFFECT_CHECKERBOARD_MAX                           (EFX_EFFECT_CHECKERBOARD_CLASS+7)

// Blocks
#define EFX_EFFECT_BLOCKS_CLASS                               14000
#define EFX_EFFECT_BLOCKS_T_TO_B                              (EFX_EFFECT_BLOCKS_CLASS+0)
#define EFX_EFFECT_BLOCKS_B_TO_T                              (EFX_EFFECT_BLOCKS_CLASS+1)
#define EFX_EFFECT_BLOCKS_L_TO_R                              (EFX_EFFECT_BLOCKS_CLASS+2)
#define EFX_EFFECT_BLOCKS_R_TO_L                              (EFX_EFFECT_BLOCKS_CLASS+3)
#define EFX_EFFECT_BLOCKS_SWIRL_CW_TO_C                       (EFX_EFFECT_BLOCKS_CLASS+4)
#define EFX_EFFECT_BLOCKS_SWIRL_CW_FROM_C                     (EFX_EFFECT_BLOCKS_CLASS+5)
#define EFX_EFFECT_BLOCKS_SWIRL_CCW_TO_C                      (EFX_EFFECT_BLOCKS_CLASS+6)
#define EFX_EFFECT_BLOCKS_SWIRL_CCW_FROM_C                    (EFX_EFFECT_BLOCKS_CLASS+7)
#define EFX_EFFECT_BLOCKS_MAX                                 (EFX_EFFECT_BLOCKS_CLASS+7)

// Circle
#define EFX_EFFECT_CIRCLE_CLASS                               15000
#define EFX_EFFECT_CIRCLE_C_IN                                (EFX_EFFECT_CIRCLE_CLASS+0)
#define EFX_EFFECT_CIRCLE_C_OUT                               (EFX_EFFECT_CIRCLE_CLASS+1)
#define EFX_EFFECT_CIRCLE_L_IN                                (EFX_EFFECT_CIRCLE_CLASS+2)
#define EFX_EFFECT_CIRCLE_L_OUT                               (EFX_EFFECT_CIRCLE_CLASS+3)
#define EFX_EFFECT_CIRCLE_R_IN                                (EFX_EFFECT_CIRCLE_CLASS+4)
#define EFX_EFFECT_CIRCLE_R_OUT                               (EFX_EFFECT_CIRCLE_CLASS+5)
#define EFX_EFFECT_CIRCLE_T_IN                                (EFX_EFFECT_CIRCLE_CLASS+6)
#define EFX_EFFECT_CIRCLE_T_OUT                               (EFX_EFFECT_CIRCLE_CLASS+7)
#define EFX_EFFECT_CIRCLE_B_IN                                (EFX_EFFECT_CIRCLE_CLASS+8)
#define EFX_EFFECT_CIRCLE_B_OUT                               (EFX_EFFECT_CIRCLE_CLASS+9)
#define EFX_EFFECT_CIRCLE_C_INTER_IN_THEN_IN                  (EFX_EFFECT_CIRCLE_CLASS+10)
#define EFX_EFFECT_CIRCLE_C_INTER_OUT_THEN_OUT                (EFX_EFFECT_CIRCLE_CLASS+11)
#define EFX_EFFECT_CIRCLE_C_INTER_IN_THEN_OUT                 (EFX_EFFECT_CIRCLE_CLASS+12)
#define EFX_EFFECT_CIRCLE_C_INTER_OUT_THEN_IN                 (EFX_EFFECT_CIRCLE_CLASS+13)
#define EFX_EFFECT_CIRCLE_L_INTER_IN_THEN_IN                  (EFX_EFFECT_CIRCLE_CLASS+14)
#define EFX_EFFECT_CIRCLE_L_INTER_OUT_THEN_OUT                (EFX_EFFECT_CIRCLE_CLASS+15)
#define EFX_EFFECT_CIRCLE_L_INTER_IN_THEN_OUT                 (EFX_EFFECT_CIRCLE_CLASS+16)
#define EFX_EFFECT_CIRCLE_L_INTER_OUT_THEN_IN                 (EFX_EFFECT_CIRCLE_CLASS+17)
#define EFX_EFFECT_CIRCLE_R_INTER_IN_THEN_IN                  (EFX_EFFECT_CIRCLE_CLASS+18)
#define EFX_EFFECT_CIRCLE_R_INTER_OUT_THEN_OUT                (EFX_EFFECT_CIRCLE_CLASS+19)
#define EFX_EFFECT_CIRCLE_R_INTER_IN_THEN_OUT                 (EFX_EFFECT_CIRCLE_CLASS+20)
#define EFX_EFFECT_CIRCLE_R_INTER_OUT_THEN_IN                 (EFX_EFFECT_CIRCLE_CLASS+21)
#define EFX_EFFECT_CIRCLE_T_INTER_IN_THEN_IN                  (EFX_EFFECT_CIRCLE_CLASS+22)
#define EFX_EFFECT_CIRCLE_T_INTER_OUT_THEN_OUT                (EFX_EFFECT_CIRCLE_CLASS+23)
#define EFX_EFFECT_CIRCLE_T_INTER_IN_THEN_OUT                 (EFX_EFFECT_CIRCLE_CLASS+24)
#define EFX_EFFECT_CIRCLE_T_INTER_OUT_THEN_IN                 (EFX_EFFECT_CIRCLE_CLASS+25)
#define EFX_EFFECT_CIRCLE_B_INTER_IN_THEN_IN                  (EFX_EFFECT_CIRCLE_CLASS+26)
#define EFX_EFFECT_CIRCLE_B_INTER_OUT_THEN_OUT                (EFX_EFFECT_CIRCLE_CLASS+27)
#define EFX_EFFECT_CIRCLE_B_INTER_IN_THEN_OUT                 (EFX_EFFECT_CIRCLE_CLASS+28)
#define EFX_EFFECT_CIRCLE_B_INTER_OUT_THEN_IN                 (EFX_EFFECT_CIRCLE_CLASS+29)
#define EFX_EFFECT_CIRCLE_L_R_INTER                           (EFX_EFFECT_CIRCLE_CLASS+30)
#define EFX_EFFECT_CIRCLE_T_B_INTER                           (EFX_EFFECT_CIRCLE_CLASS+31)
#define EFX_EFFECT_CIRCLE2_L_R_OUT                            (EFX_EFFECT_CIRCLE_CLASS+32)
#define EFX_EFFECT_CIRCLE2_L_R_IN                             (EFX_EFFECT_CIRCLE_CLASS+33)
#define EFX_EFFECT_CIRCLE2_T_B_OUT                            (EFX_EFFECT_CIRCLE_CLASS+34)
#define EFX_EFFECT_CIRCLE2_T_B_IN                             (EFX_EFFECT_CIRCLE_CLASS+35)
#define EFX_EFFECT_CIRCLE_MAX                                 (EFX_EFFECT_CIRCLE_CLASS+35)

// Ellipse
#define EFX_EFFECT_ELLIPSE_CLASS                              16000
#define EFX_EFFECT_ELLIPSE_C_IN                               (EFX_EFFECT_ELLIPSE_CLASS+0)
#define EFX_EFFECT_ELLIPSE_C_OUT                              (EFX_EFFECT_ELLIPSE_CLASS+1)
#define EFX_EFFECT_ELLIPSE_L_IN                               (EFX_EFFECT_ELLIPSE_CLASS+2)
#define EFX_EFFECT_ELLIPSE_L_OUT                              (EFX_EFFECT_ELLIPSE_CLASS+3)
#define EFX_EFFECT_ELLIPSE_R_IN                               (EFX_EFFECT_ELLIPSE_CLASS+4)
#define EFX_EFFECT_ELLIPSE_R_OUT                              (EFX_EFFECT_ELLIPSE_CLASS+5)
#define EFX_EFFECT_ELLIPSE_T_IN                               (EFX_EFFECT_ELLIPSE_CLASS+6)
#define EFX_EFFECT_ELLIPSE_T_OUT                              (EFX_EFFECT_ELLIPSE_CLASS+7)
#define EFX_EFFECT_ELLIPSE_B_IN                               (EFX_EFFECT_ELLIPSE_CLASS+8)
#define EFX_EFFECT_ELLIPSE_B_OUT                              (EFX_EFFECT_ELLIPSE_CLASS+9)
#define EFX_EFFECT_ELLIPSE_C_INTER_IN_THEN_IN                 (EFX_EFFECT_ELLIPSE_CLASS+10)
#define EFX_EFFECT_ELLIPSE_C_INTER_OUT_THEN_OUT               (EFX_EFFECT_ELLIPSE_CLASS+11)
#define EFX_EFFECT_ELLIPSE_C_INTER_IN_THEN_OUT                (EFX_EFFECT_ELLIPSE_CLASS+12)
#define EFX_EFFECT_ELLIPSE_C_INTER_OUT_THEN_IN                (EFX_EFFECT_ELLIPSE_CLASS+13)
#define EFX_EFFECT_ELLIPSE_L_INTER_IN_THEN_IN                 (EFX_EFFECT_ELLIPSE_CLASS+14)
#define EFX_EFFECT_ELLIPSE_L_INTER_OUT_THEN_OUT               (EFX_EFFECT_ELLIPSE_CLASS+15)
#define EFX_EFFECT_ELLIPSE_L_INTER_IN_THEN_OUT                (EFX_EFFECT_ELLIPSE_CLASS+16)
#define EFX_EFFECT_ELLIPSE_L_INTER_OUT_THEN_IN                (EFX_EFFECT_ELLIPSE_CLASS+17)
#define EFX_EFFECT_ELLIPSE_R_INTER_IN_THEN_IN                 (EFX_EFFECT_ELLIPSE_CLASS+18)
#define EFX_EFFECT_ELLIPSE_R_INTER_OUT_THEN_OUT               (EFX_EFFECT_ELLIPSE_CLASS+19)
#define EFX_EFFECT_ELLIPSE_R_INTER_IN_THEN_OUT                (EFX_EFFECT_ELLIPSE_CLASS+20)
#define EFX_EFFECT_ELLIPSE_R_INTER_OUT_THEN_IN                (EFX_EFFECT_ELLIPSE_CLASS+21)
#define EFX_EFFECT_ELLIPSE_T_INTER_IN_THEN_IN                 (EFX_EFFECT_ELLIPSE_CLASS+22)
#define EFX_EFFECT_ELLIPSE_T_INTER_OUT_THEN_OUT               (EFX_EFFECT_ELLIPSE_CLASS+23)
#define EFX_EFFECT_ELLIPSE_T_INTER_IN_THEN_OUT                (EFX_EFFECT_ELLIPSE_CLASS+24)
#define EFX_EFFECT_ELLIPSE_T_INTER_OUT_THEN_IN                (EFX_EFFECT_ELLIPSE_CLASS+25)
#define EFX_EFFECT_ELLIPSE_B_INTER_IN_THEN_IN                 (EFX_EFFECT_ELLIPSE_CLASS+26)
#define EFX_EFFECT_ELLIPSE_B_INTER_OUT_THEN_OUT               (EFX_EFFECT_ELLIPSE_CLASS+27)
#define EFX_EFFECT_ELLIPSE_B_INTER_IN_THEN_OUT                (EFX_EFFECT_ELLIPSE_CLASS+28)
#define EFX_EFFECT_ELLIPSE_B_INTER_OUT_THEN_IN                (EFX_EFFECT_ELLIPSE_CLASS+29)
#define EFX_EFFECT_ELLIPSE_L_R_INTER                          (EFX_EFFECT_ELLIPSE_CLASS+30)
#define EFX_EFFECT_ELLIPSE_T_B_INTER                          (EFX_EFFECT_ELLIPSE_CLASS+31)
#define EFX_EFFECT_ELLIPSE2_L_R_OUT                           (EFX_EFFECT_ELLIPSE_CLASS+32)
#define EFX_EFFECT_ELLIPSE2_L_R_IN                            (EFX_EFFECT_ELLIPSE_CLASS+33)
#define EFX_EFFECT_ELLIPSE2_T_B_OUT                           (EFX_EFFECT_ELLIPSE_CLASS+34)
#define EFX_EFFECT_ELLIPSE2_T_B_IN                            (EFX_EFFECT_ELLIPSE_CLASS+35)
#define EFX_EFFECT_ELLIPSE_MAX                                (EFX_EFFECT_ELLIPSE_CLASS+35)

#define EFFECT_NONE                 EFX_EFFECT_NONE
#define EFFECT_WIPE_DOWN            EFX_EFFECT_WIPE_T_TO_B
#define EFFECT_WIPE_UP              EFX_EFFECT_WIPE_B_TO_T
#define EFFECT_WIPE_RIGHT           EFX_EFFECT_WIPE_L_TO_R
#define EFFECT_WIPE_LEFT            EFX_EFFECT_WIPE_R_TO_L
#define EFFECT_RANDOM_PIXEL         EFX_EFFECT_RANDOM_BLOCK                         // close match - use the grain size
#define EFFECT_RANDOM_SMALLBOX      EFX_EFFECT_RANDOM_BLOCK                         // close match - use the grain size
#define EFFECT_RANDOM_MEDIUMBOX     EFX_EFFECT_RANDOM_BLOCK                         // close match - use the grain size
#define EFFECT_RANDOM_LARGEBOX      EFX_EFFECT_RANDOM_BLOCK                         // close match - use the grain size
#define EFFECT_RANDOM_HLINE         EFX_EFFECT_RANDOM_LINES                         // close match - use the grain size
#define EFFECT_RANDOM_VLINE         EFX_EFFECT_RANDOM_LINES                         // close match - use the grain size
#define EFFECT_VMIDDLE              EFX_EFFECT_WIPE2_C_TO_L_AND_R
#define EFFECT_HMIDDLE              EFX_EFFECT_WIPE2_C_TO_T_AND_B
#define EFFECT_HINTERLACE           EFX_EFFECT_BLIND_T_TO_B                         // close match
#define EFFECT_VINTERLACE           EFX_EFFECT_BLIND_L_TO_R                         // close match
#define EFFECT_HINTERLACE_OUTWARD   EFX_EFFECT_WIPE_INTERV_C_TO_L_AND_R             // close match
#define EFFECT_VINTERLACE_OUTWARD   EFX_EFFECT_WIPE_INTERH_C_TO_T_AND_B             // close match
#define EFFECT_OUTTOIN              EFX_EFFECT_WIPE_RECTANGLE_IN
#define EFFECT_INTOOUT              EFX_EFFECT_WIPE_RECTANGLE_OUT
#define EFFECT_OUTTOIN_INTERLACE    EFX_EFFECT_WIPE_RECTANGLE_INTER_IN_THEN_IN      // close match
#define EFFECT_INTOOUT_INTERLACE    EFX_EFFECT_WIPE_RECTANGLE_INTER_OUT_THEN_OUT    // close match
#define EFFECT_SPIRAL_IN            EFX_EFFECT_BLOCKS_SWIRL_CW_TO_C
#define EFFECT_SPIRAL_OUT           EFX_EFFECT_BLOCKS_SWIRL_CW_FROM_C
#define EFFECT_HMIDDLE_INWARD       EFX_EFFECT_WIPE2_T_AND_B_TO_C
#define EFFECT_VMIDDLE_INWARD       EFX_EFFECT_WIPE2_L_AND_R_TO_C
#define EFFECT_VINTERLACE_INWARD    EFX_EFFECT_WIPE_INTERV_T_AND_B_TO_C             // close match
#define EFFECT_HINTERLACE_INWARD    EFX_EFFECT_WIPE_INTERH_L_AND_R_TO_C             // close match
#define EFFECT_VINTERLACE_RIGHT     EFX_EFFECT_BLIND_T_TO_B                         // close match
#define EFFECT_VINTERLACE_LEFT      EFX_EFFECT_BLIND_T_TO_B                         // close match
#define EFFECT_HINTERLACE_UP        EFX_EFFECT_BLIND_L_TO_R                         // close match
#define EFFECT_HINTERLACE_DOWN      EFX_EFFECT_BLIND_L_TO_R                         // close match


   enum EfxNotifyConstants{
         EFX_NOTIFY_TRANSITION = 0,
         EFX_NOTIFY_IMAGE = 1,
   };

   enum EfxBackStyleConstants{
         EFX_BACKSTYLE_TRANSPARENT = 0,
         EFX_BACKSTYLE_OPAQUE = 1,
         EFX_BACKSTYLE_TRANSLUCENT = 2,
         EFX_BACKSTYLE_TILED_IMAGE = 3,
         EFX_BACKSTYLE_TRANSLUCENT_TILED_IMAGE = 4,
         EFX_BACKSTYLE_GRADIENT = 5,
         EFX_BACKSTYLE_TRANSLUCENT_GRADIENT = 6,
         EFX_BACKSTYLE_MAX = 6
   };

   enum EfxBorderStyleConstants{
         EFX_BORDERSTYLE_TRANSPARENT = 0,
         EFX_BORDERSTYLE_SOLID = 1,
         EFX_BORDERSTYLE_DASH = 2,
         EFX_BORDERSTYLE_DOT = 3,
         EFX_BORDERSTYLE_DASHDOT = 4,
         EFX_BORDERSTYLE_DASHDOTDOT = 5,
         EFX_BORDERSTYLE_MAX = 5
   };

   enum EfxInnerStyleConstants{
         EFX_INNERSTYLE_INSET = 0,
         EFX_INNERSTYLE_RAISED = 1,
         EFX_INNERSTYLE_MAX = 1
   };

   enum EfxOuterStyleConstants{
         EFX_OUTERSTYLE_INSET = 0,
         EFX_OUTERSTYLE_RAISED = 1,
         EFX_OUTERSTYLE_MAX = 1
   };

   enum EfxTextStyleConstants{
         EFX_TEXTSTYLE_NORMAL = 0,
         EFX_TEXTSTYLE_INSETLIGHT = 1,
         EFX_TEXTSTYLE_INSETHEAVY = 2,
         EFX_TEXTSTYLE_RAISEDLIGHT = 3,
         EFX_TEXTSTYLE_RAISEDHEAVY = 4,
         EFX_TEXTSTYLE_DROPSHADOW = 5,
         EFX_TEXTSTYLE_BLOCKSHADOW = 6,
         EFX_TEXTSTYLE_OUTLINEBLOCK = 7,
         EFX_TEXTSTYLE_INSETEXTRALIGHT = 8,
         EFX_TEXTSTYLE_INSETEXTRAHEAVY = 9,
         EFX_TEXTSTYLE_RAISEDEXTRALIGHT = 10,
         EFX_TEXTSTYLE_RAISEDEXTRAHEAVY = 11,
         EFX_TEXTSTYLE_MAX = 11
   };

   enum EfxTextAlignConstants{
         EFX_TEXTALIGN_LEFT_TOP = 0,
         EFX_TEXTALIGN_LEFT_VCENTER = 1,
         EFX_TEXTALIGN_LEFT_BOTTOM = 2,
         EFX_TEXTALIGN_HCENTER_TOP = 3,
         EFX_TEXTALIGN_HCENTER_VCENTER = 4,
         EFX_TEXTALIGN_HCENTER_BOTTOM = 5,
         EFX_TEXTALIGN_RIGHT_TOP = 6,
         EFX_TEXTALIGN_RIGHT_VCENTER = 7,
         EFX_TEXTALIGN_RIGHT_BOTTOM = 8,
         EFX_TEXTALIGN_MAX = 8
   };

   enum AnnHyperlinkTypeConstants{
         ANNLINK_NONE,
         ANNLINK_ANNEVENT,
         ANNLINK_ANNEVENT5,
         ANNLINK_RUN,
         ANNLINK_WEBPAGE
   };

   typedef enum {
      PANWIN_CREATED   = 0,
      PANWIN_UPDATED   = 1,
      PANWIN_DESTROYED = 2
   } PanWinFlagConstants;

#ifndef SAVE_OVERWRITE
   #define SAVE_OVERWRITE 0
   #define SAVE_APPEND    1
   #define SAVE_REPLACE   2
   #define SAVE_INSERT    3
#endif
       
   typedef enum {
      PICTURIZE_LOADALL = 0x0001,
      PICTURIZE_RESIZE = 0x0002,
      PICTURIZE_RESAMPLE = 0x0004,
   } PicturizeFlags;

   typedef enum {
      COLORREF_GRAY16 = 0x02000000,
   } PixelConstants;

#endif
