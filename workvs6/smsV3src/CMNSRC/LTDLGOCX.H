// LEAD コモンダイアログコントロール定数定義ファイル
  typedef enum {
           DLG_HELP_FILEOPEN    = 0x00000001,
           DLG_HELP_FILEOPEN95,
           DLG_HELP_FILESAVE,
           DLG_HELP_FILESAVE95,
           DLG_HELP_COLORRES,
           DLG_HELP_GETANGLE,     
           DLG_HELP_GETSIZE,      
           DLG_HELP_GETFILTER,    
           DLG_HELP_GETCHANGE,    
           DLG_HELP_GETRANGE,     
           DLG_HELP_GETGAMMA,     
           DLG_HELP_GETNOISE,     
           DLG_HELP_GETEMBOSS,    
           DLG_HELP_WINDOWLEVEL,    
   }HelpEventConstants;

   typedef enum {
           FLT_SPATIAL    = 0,
           FLT_BINARY     = 1,
   }FilterTypeConstants;

   typedef enum {
           DLG_FO_AUTOPROCESS    = 0x00000001,   /* Dlg should autoprocess image and fill the pBitmap */
           DLG_FO_PROGRESSIVE    = 0x00000002,   /* FileOpen Dlg should have Prog. File options */
           DLG_FO_MULTIPAGE      = 0x00000004,   /* FileOpen Dlg should have Multipage options */
           DLG_FO_LOADROTATED    = 0x00000008,   /* FileOpen Dlg should have Load Rotated option */
           DLG_FO_LOADCOMPRESSED = 0x00000010,   /* FileOpen Dlg should have Load 1-bit compressed option */
           DLG_FO_FILEINFO       = 0x00000020,   /* FileOpen Dlg should have FileInfo button */
           DLG_FO_SHOWPREVIEW    = 0x00000040,   /* FileOpen Dlg should have ThumbNail and should create the */
                                                 /* preview if none is stored in the file */
           DLG_FO_SHOWSTAMP      = 0x00000080,   /* FileOpen Dlg should have ThumbNail and should only load */
                                                 /* the preview stamp if one is stored in the file */
           DLG_FO_95STYLE        = 0x00000100,   /* FileOpen Dlg should have Windows95 Explorer style look */
           DLG_FO_DELPAGE        = 0x00000200,   /* FileOpen Dlg should have Delete Page Button */
   }FileOpenDlgConstants;

   typedef enum {
           DLG_FS_AUTOPROCESS  =  0x00000001,   /* Dlg should autoprocess image and fill the pBitmap */
           DLG_FS_PROGRESSIVE  =  0x00000002,   /* FileSave Dlg should have Prog. File options */
           DLG_FS_MULTIPAGE    =  0x00000004,   /* FileSave Dlg should have Multipage options */
           DLG_FS_STAMP        =  0x00000008,   /* FileSave Dlg should have save with stamp options */
           DLG_FS_QFACTOR      =  0x00000010,   /* FileSave Dlg should have QFactor option */
           DLG_FS_95STYLE      =  0x00000020,   /* FileSave Dlg should have Windows95 Explorer style look */
   }FileSaveDlgConstants;

   typedef enum {
           DLG_FS_LEAD       =    0x00000002,
           DLG_FS_LEAD1BIT   =    0x00000004,
           DLG_FS_JFIF       =    0x00000008,
           DLG_FS_CALS       =    0x00000010,
           DLG_FS_PNG        =    0x00000020,
           DLG_FS_DICOM      =    0x00000040,
           DLG_FS_FAX        =    0x00000080,
           DLG_FS_EPS        =    0x00000100,
           DLG_FS_EXIF       =    0x00000200,
           DLG_FS_FPX        =    0x00000400,
           DLG_FS_IMG        =    0x00000800,
           DLG_FS_GIF        =    0x00001000,
           DLG_FS_IOCA       =    0x00002000,
           DLG_FS_PCT        =    0x00004000,
           DLG_FS_MAC        =    0x00008000,
           DLG_FS_AWD        =    0x00010000,
           DLG_FS_MSP        =    0x00020000,
           DLG_FS_OS2BMP     =    0x00040000,
           DLG_FS_PCX        =    0x00080000,
           DLG_FS_PSD        =    0x00100000,
           DLG_FS_RAS        =    0x00200000,
           DLG_FS_TGA        =    0x00400000,
           DLG_FS_TIFF       =    0x00800000,
           DLG_FS_BMP        =    0x01000000,
           DLG_FS_WFX        =    0x02000000,
           DLG_FS_WMF        =    0x04000000,
           DLG_FS_WPG        =    0x08000000,
           DLG_FS_ICO        =    0x10000000,
           DLG_FS_CUR        =    0x20000000,
           DLG_FS_ALL        =    0xFFFFFFFF  /* all file formats */
   }FileSaveDlgFormatConstants;                  

// possible values for uSaveMulti
#ifndef SAVE_OVERWRITE
#define SAVE_OVERWRITE 0
#define SAVE_APPEND    1
#define SAVE_REPLACE   2
#define SAVE_INSERT    3
#endif

   typedef enum {
           DLG_CR_SHOWPREVIEW  =   0x00000001,   /* shows preview of image */
           DLG_CR_AUTOPROCESS  =   0x00000002    /* process the image on OK */
   }ColorResDlgConstants;

   typedef enum {
           DLG_CR_1BIT       =      0x00000004,   /* 1-bit is an option */
           DLG_CR_2BIT       =      0x00000008,   /* 2-bit is an option */
           DLG_CR_3BIT       =      0x00000010,   /* 3-bit is an option */
           DLG_CR_4BIT       =      0x00000020,   /* 4-bit is an option */
           DLG_CR_5BIT       =      0x00000040,   /* 5-bit is an option */
           DLG_CR_6BIT       =      0x00000080,   /* 6-bit is an option */
           DLG_CR_7BIT       =      0x00000100,   /* 7-bit is an option */
           DLG_CR_8BIT       =      0x00000200,   /* 8-bit is an option */
           DLG_CR_12BIT      =      0x00000400,   /* 12-bit is an option */
           DLG_CR_16BIT      =      0x00000800,   /* 16-bit is an option */
           DLG_CR_24BIT      =      0x00001000,   /* 24-bit is an option */
           DLG_CR_32BIT      =      0x00002000,   /* 32-bit is an option */
           DLG_CR_BITALL     =      0x00003FFC,   /* All BitsPerPixel are options */
   }ColorResDlgBitConstants;

   typedef enum {
           DLG_CR_DITHER_NONE      = 0x00004000,   /* no dithering is an option */
           DLG_CR_DITHER_FLOYD     = 0x00008000,   /* floyd dithering is an option */
           DLG_CR_DITHER_STUCKI    = 0x00010000,   /* stucki dithering is an option */
           DLG_CR_DITHER_BURKES    = 0x00020000,   /* burkes dithering is an option */
           DLG_CR_DITHER_SIERRA    = 0x00040000,   /* sierra dithering is an option */
           DLG_CR_DITHER_STEVENSON = 0x00080000,   /* stevenson dithering is an option */
           DLG_CR_DITHER_JARVIS    = 0x00100000,   /* jarvis dithering is an option */
           DLG_CR_DITHER_CLUSTER   = 0x00200000,   /* clustered dithering is an option */
           DLG_CR_DITHER_ORDERED   = 0x00400000,   /* ordered dithering is an option */
           DLG_CR_DITHER_ALL       = 0x007FC000,   /* All dithering methods are options */
   }ColorResDlgDitherConstants;

   typedef enum {
           DLG_CR_PAL_FIXED     = 0x00800000,   /* fixed palette is an option */
           DLG_CR_PAL_OPTIMIZED = 0x01000000,   /* fixed palette is an option */
           DLG_CR_PAL_IDENTITY  = 0x02000000,   /* identity palette is an option */
           DLG_CR_PAL_NETSCAPE  = 0x04000000,   /* netscape palette is an option */
           DLG_CR_PAL_ALL       = 0x07800000,   /* All palette options are options */
   }ColorResDlgPaletteConstants;

   typedef enum {
           DLG_CR_ORDER = 0x08000000   /* color order is an option */
   }ColorResDlgOrderConstants;

   typedef enum {
           DLG_IMG_AUTOPROCESS  =   0x80000000,   /* Dlg should autoprocess image and fill the pBitmap */
           DLG_IMG_SHOWPREVIEW  =   0x40000000,   /* ImageProcess Dlg should preview of image */
           DLG_IMG_HIDEBACKCOLOR=   0x20000000,   /* GetAngle Dlg should hide Background Color button */
                                
           DLG_IMG_RESIZE       =   0x00000001,   /* resize dialog */
           DLG_IMG_RESAMPLE     =   0x00000002,   /* resample dialog */
           DLG_IMG_ROTATE       =   0x00000003,   /* rotate dialog */
           DLG_IMG_SHEAR        =   0x00000004,   /* shear dialog */
                                
           DLG_IMG_GRADIENT     =   0x00000001,   /* gradient dialog */
           DLG_IMG_LAPLACIAN    =   0x00000002,   /* laplacian dialog */
           DLG_IMG_SOBEL        =   0x00000004,   /* sobel dialog */
           DLG_IMG_PREWITT      =   0x00000008,   /* prewitt dialog */
           DLG_IMG_SHIFTDIFF    =   0x00000010,   /* shiftdiff dialog */
           DLG_IMG_LINESEG      =   0x00000020,   /* lineseg dialog */
           DLG_IMG_EROSION      =   0x00000040,   /* erosion dialog */
           DLG_IMG_DILATION     =   0x00000080,   /* dilation dialog */
                                
           DLG_IMG_SHARPEN      =   0x00000001,   /* sharpen dialog */
           DLG_IMG_BRIGHTNESS   =   0x00000002,   /* brightness dialog */
           DLG_IMG_CONTRAST     =   0x00000004,   /* contrast dialog */
           DLG_IMG_SATURATION   =   0x00000008,   /* saturation dialog */
           DLG_IMG_HISTOCONTRAST=   0x00000010,   /* histo contrast dialog */
           DLG_IMG_HUE          =   0x00000020,   /* hue dialog */
           DLG_IMG_HALFTONE     =   0x00000040,   /* halftone dialog */
           DLG_IMG_POSTERIZE    =   0x00000080,   /* posterize dialog */
           DLG_IMG_MOSAIC       =   0x00000100,   /* mosaic dialog */
           DLG_IMG_AVERAGE      =   0x00000200,   /* average dialog */
           DLG_IMG_MEDIAN       =   0x00000400,   /* median dialog */
           DLG_IMG_OILIFY       =   0x00000800,   /* oilify dialog */
           DLG_IMG_MIN          =   0x00001000,   /* min filter dialog */
           DLG_IMG_MAX          =   0x00002000,   /* max filter dialog */
           DLG_IMG_SOLARIZE     =   0x00004000,   /* solarize dialog */
                                
           DLG_IMG_ADDNOISE     =   0x0000001C,   /* add noise dialog */
           DLG_IMG_GAMMACORRECT =   0x0000001D,   /* gamma correct dialog */
           DLG_IMG_EMBOSS       =   0x0000001E,   /* emboss filter dialog */
           DLG_IMG_INTENSITYDETECT =0x0000001F    /* intensity detect dialog */
      } ImgProcDlgConstants;                        

   typedef enum {
           DLG_WL_SHOWPREVIEW = 0x00000001,        /* shows preview of image */
           DLG_WL_AUTOPROCESS = 0x00000002,        /* process the image on OK */
      } WindowLevelDlgConstants;                        

   typedef enum {
           DLG_OFN_OVERWRITEPROMPT = 0x00000002,  /* FileSave will prompt if file exists */
           DLG_OFN_NOCHANGEDIR = 0x00000008,      /* restore dir to original after close */
           DLG_OFN_ALLOWMULTI = 0x00000200,       /* allow multiple files to be selected */
      } FileDlgFlagConstants;                        

// ActiveX error codes
#include "L_OcxErr.h"

   typedef enum{
           DLG_EFX_SHOWPREVIEW      = 0x00000001,   /* shows preview of image */
           DLG_EFX_AUTOPREVIEW      = 0x00000002,   /* auto preview when options change */
           DLG_EFX_DELAY            = 0x00000004,   /* effect delay is an option */
           DLG_EFX_GRAIN            = 0x00000008,   /* effect grain is an option */
           DLG_EFX_PASSES           = 0x00000010,   /* effect passes is an option */
           DLG_EFX_TRANSPARENT      = 0x00000020,   /* transparency is an option */
           DLG_EFX_WAND             = 0x00000040,   /* wand is an option */
           DLG_EFX_NO_TREEVIEW      = 0x00000080,   /* DO NOT use TreeView */
           DLG_EFX_CLASS_NONE       = 0x00000100,   /* no effect class is option */
           DLG_EFX_CLASS_WIPE       = 0x00000200,   /* linear wipe class is option */
           DLG_EFX_CLASS_WIPERECT   = 0x00000400,   /* rect wipe class is option */
           DLG_EFX_CLASS_WIPECIRCLE = 0x00000800,   /* circle wipe class is option */
           DLG_EFX_CLASS_PUSH       = 0x00001000,   /* push class is option */
           DLG_EFX_CLASS_SLIDE      = 0x00002000,   /* slide class is option */
           DLG_EFX_CLASS_ROLL       = 0x00004000,   /* roll class is option */
           DLG_EFX_CLASS_ROTATE     = 0x00008000,   /* rotate class is option */
           DLG_EFX_CLASS_ZOOM       = 0x00010000,   /* zoom class is option */
           DLG_EFX_CLASS_DRIP       = 0x00020000,   /* drip class is option */
           DLG_EFX_CLASS_BLIND      = 0x00040000,   /* blind class is option */
           DLG_EFX_CLASS_RANDOM     = 0x00080000,   /* random class is option */
           DLG_EFX_CLASS_CHECK      = 0x00100000,   /* check class is option */
           DLG_EFX_CLASS_BLOCKS     = 0x00200000,   /* blocks class is option */
           DLG_EFX_CLASS_CIRCLE     = 0x00400000,   /* circle class is option */
           DLG_EFX_CLASS_ELLIPSE    = 0x00800000,   /* ellipse class is option */
      }PaintEffectDlgConstants;                                 

   typedef enum{
           DLG_GRAD_SHOWPREVIEW           = 0x00000001,   /* shows preview of image */
           DLG_GRAD_AUTOPREVIEW           = 0x00000002,   /* auto preview when options change */
           DLG_GRAD_STARTCOLOR            = 0x00000004,   /* start color is an option */
           DLG_GRAD_ENDCOLOR              = 0x00000008,   /* end color is an option */
           DLG_GRAD_STEPS                 = 0x00000010,   /* gradient steps is an option */
           DLG_GRAD_NO_TREEVIEW           = 0x00000020,   /* DO NOT use TreeView */
           DLG_GRAD_CLASS_LINEAR          = 0x00000040,   /* linear gradients are an option */
           DLG_GRAD_CLASS_ANGULAR         = 0x00000080,   /* angular gradients are an option */
           DLG_GRAD_CLASS_RECTANGULAR     = 0x00000100,   /* rectangular gradients are an option */
           DLG_GRAD_CLASS_ELLIPTICAL      = 0x00000200,   /* elliptical gradients are an option */
           DLG_GRAD_CLASS_CONICAL         = 0x00000400,   /* conical gradients are an option */
      }GradientDlgConstants;                                 

   typedef enum{
           DLG_TRANS_SHOWPREVIEW     = 0x00000001,   /* shows preview of image */
           DLG_TRANS_AUTOPREVIEW     = 0x00000002,   /* auto preview when options change */
           DLG_TRANS_FORECOLOR       = 0x00000004,   /* forecolor is an option */
           DLG_TRANS_BACKCOLOR       = 0x00000008,   /* backcolor is an option */
           DLG_TRANS_DELAY           = 0x00000010,   /* transition delay is an option */
           DLG_TRANS_GRAIN           = 0x00000020,   /* transition grain is an option */
           DLG_TRANS_EFFECT          = 0x00000040,   /* paint effect is an option */
           DLG_TRANS_PASSES          = 0x00000080,   /* transition passes is an option */
           DLG_TRANS_WAND            = 0x00000100,   /* wand is an option */
           DLG_TRANS_TRANSPARENT     = 0x00000200,   /* transparency is an option */
           DLG_TRANS_GRADIENT        = 0x00000400,   /* gradient is an option */
      }TransitionDlgConstants;                                 

   typedef enum{
           DLG_SHAPE_SHOWPREVIEW          = 0x00000001,   /* shows preview of image */
           DLG_SHAPE_AUTOPREVIEW          = 0x00000002,   /* auto preview when options change */
           DLG_SHAPE_BACKSTYLE            = 0x00000004,   /* backstyle is an option */
           DLG_SHAPE_FILLSTYLE            = 0x00000008,   /* fillstyle is an option */
           DLG_SHAPE_FORE_BACK_COLOR      = 0x00000010,   /* fore and back colors are options */
           DLG_SHAPE_BORDERSTYLE          = 0x00000020,   /* borderstyle is an option */
           DLG_SHAPE_BORDERWIDTH          = 0x00000040,   /* border width is an option */
           DLG_SHAPE_BORDERCOLOR          = 0x00000080,   /* bordercolor is an option */
           DLG_SHAPE_INNERSTYLE           = 0x00000100,   /* innerbandstyle is an option */
           DLG_SHAPE_INNERWIDTH           = 0x00000200,   /* innerband width is an option */
           DLG_SHAPE_INNER_HILITE_SHADOW  = 0x00000400,   /* innerband colors are options */
           DLG_SHAPE_OUTERSTYLE           = 0x00000800,   /* outerbandstyle is an option */
           DLG_SHAPE_OUTERWIDTH           = 0x00001000,   /* outerband width is an option */
           DLG_SHAPE_OUTER_HILITE_SHADOW  = 0x00002000,   /* outerband colors are options */
           DLG_SHAPE_SHADOWCOLOR          = 0x00004000,   /* shadow color is an option */
           DLG_SHAPE_SHADOW_X_Y           = 0x00008000,   /* shadow x and y are options */
           DLG_SHAPE_BROWSEIMAGE          = 0x00010000,   /* browse image button is option */
           DLG_SHAPE_NO_TREEVIEW          = 0x00020000,   /* DO NOT use TreeView */
           DLG_SHAPE_CLASS_SQUARE         = 0x00040000,   /* squares are options */
           DLG_SHAPE_CLASS_RECTANGLE      = 0x00080000,   /* rectangles are options */
           DLG_SHAPE_CLASS_PARALLELOGRAM  = 0x00100000,   /* parallelograms are options */
           DLG_SHAPE_CLASS_TRAPEZOID      = 0x00200000,   /* trapezoids are options */
           DLG_SHAPE_CLASS_TRIANGLE       = 0x00400000,   /* triangles are options */
           DLG_SHAPE_CLASS_OTHER          = 0x00800000,   /* other polygons are options */
           DLG_SHAPE_CLASS_CIRCLE         = 0x01000000,   /* circles are options */
           DLG_SHAPE_CLASS_ELLIPSE        = 0x02000000,   /* ellipses are options */
           DLG_SHAPE_CLASS_STAR           = 0x04000000,   /* stars are options */
           DLG_SHAPE_CLASS_CROSS          = 0x08000000,   /* crosses are options */
           DLG_SHAPE_CLASS_ARROW          = 0x10000000,   /* arrows are options */
      }ShapeDlgConstants;

   typedef enum{
           DLG_TEXT_SHOWPREVIEW           = 0x00000001,   /* shows preview of image */
           DLG_TEXT_AUTOPREVIEW           = 0x00000002,   /* auto preview when options change */
           DLG_TEXT_SAMPLETEXT            = 0x00000004,   /* sample text edit box is an option */
           DLG_TEXT_STYLE                 = 0x00000008,   /* text style is an option */
           DLG_TEXT_COLOR                 = 0x00000010,   /* text color is an option */
           DLG_TEXT_BORDERCOLOR           = 0x00000020,   /* bordercolor is an option */
           DLG_TEXT_ALIGN                 = 0x00000040,   /* alignment is an option */
           DLG_TEXT_ANGLE                 = 0x00000080,   /* angle is an option */
           DLG_TEXT_WORDWRAP              = 0x00000100,   /* wordwrap is an option */
           DLG_TEXT_FONT                  = 0x00000200,   /* font is an option */
           DLG_TEXT_FOREIMAGE             = 0x00000400,   /* foreground image is an option */
           DLG_TEXT_BROWSEIMAGE           = 0x00000800,   /* browse image button is option */
           DLG_TEXT_SHADOWCOLOR           = 0x00001000,   /* shadow color is an option */
           DLG_TEXT_SHADOW_X_Y            = 0x00002000,   /* shadow x and y are options */
      }TextDlgConstants;

#ifndef L_SUPPORT_EXPRESS
   #define L_SUPPORT_EXPRESS        0
   #define L_SUPPORT_GIFLZW         1
   #define L_SUPPORT_TIFLZW         2
   #define L_SUPPORT_FPXEXTENSIONS  3
   #define L_SUPPORT_OCR            4
   #define L_SUPPORT_VIDEO          5
   #define L_SUPPORT_MEDICAL        6
#endif
