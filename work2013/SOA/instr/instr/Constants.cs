namespace instr
{
    static class Constants
    {
        public const int ONEBLOCKSIZE = 100;
        public const int SPACEX = 10;
        public const int SPACEY = 30;
        // UndoRedo最大値
        public const int UNDOMAX = 5;
        //メニューメインコマンド
        public const int NOOPCMD = 0;
        public const int MAINFILECMD = 100;
        public const int MAINEDITCMD = 200;
        public const int MAINSLCTCMD = 300;
        public const int MAINFIGUCMD = 400;
        public const int MAINTEXTCMD = 500;
        public const int MAINIMAGECMD = 600;
        // メニューサブコマンド
        public const int FILENEWCMD = 101;
        public const int FILELOADCMD = 102;
        public const int FILESAVECMD = 103;
        public const int FILEEXPORTCMD = 111;
        public const int PRINTSETUPCMD = 121;
        public const int PRINTOUTCMD = 122;
        public const int EDITUNDOCMD = 201;
        public const int EDITREDOCMD = 202;
        public const int EDITCUTCMD = 211;
        public const int EDITCOPYCMD = 212;
        public const int EDITPASTCMD = 213;
        public const int EDITDELETECMD = 221;
        public const int SLCTALLCMD = 301;
        public const int SLCTAREACMD = 311;
        public const int FIGULINECMD = 401;
        public const int FIGUBOXSCMD = 402;
        public const int FIGUELPSCMD = 403;
        public const int TEXTCREATECMD = 501;
        public const int IMAGEBKLOADCMD = 601;
        public const int IMAGEBKDELETECMD = 602;
        public const int IMAGELOADCMD = 611;
        public const int IMAGECLIPCMD = 621;
        public const int IMAGEALPHACMD = 631;

        //動作イベント定数
        public const int INITEVENT = 0;
        public const int DOWNEVENT = 1;
        public const int MOVEEVENT = 2;
        public const int UPMOVEEVENT = 3;
        public const int UPEVENT = 4;
        public const int ENDEVENT = 99;

        //オブジェクト種別ID
        public const int OBJDATA = 1;
        public const int OBJVECTLIST = 20;
        public const int OBJSTRING = 30;
        public const int OBJIMAGE = 40;
        public const int OBJVECT = 21;
        public const int OBJMOJI = 31;
        //オブジェクト状態フラグ
        public const int NOACTFALG = 0;
        public const int ACTIVEFALG = 1;

        // 判定用数値画面上のドット数
        public const double LIMITAREA = 8;
        public const double SLCTCRCLR = 4;

        //文字列属性
        public const int YOKOGAKI = 0;
        public const int TATEGAKI = 1;

        //線種種別
        public const int LINEKIND = 0;
        public const int BEZEKIND = 1;

        public const double NUMMAX = 100000.0;
        //ES書体関係定義
        public const int FIDXSIZE = 6;
        public const int ESF0MESH = 1024;
        public const int ESF0SIZADD = 3;
        public const int ESF0ATRADD = 0;
        public const int ESF0XCDADD = 1;
        public const int ESF0YCDADD = 2;
        public const int F0EORMASK = 0x01;
        public const int PATRMASK = 0x0e;
        public const int MOVEATR = 0x00;
        public const int LINEATR = 0x01;
        public const int CONTATR = 0x02;
        public const int CURVATR = 0x04;
        public const int BEZEATR = 0x08;
        public const double NORAG = 9999.0;
        //選択処理ローカルコマンド
        public const int SLCTMODE = 10;
        public const int MOVEMODE = 20;
        public const int TRNSMODE = 30;
        //支点種類
        public const int OUTSIDE = 0;
        public const int LEFTUP = 11;
        public const int LEFTDOWN = 13;
        public const int RIGHTUP = 31;
        public const int RIGHTDOWN = 33;
        public const int LEFTCNTR = 12;
        public const int RIGHTCNTR = 32;
        public const int CNTRUP = 21;
        public const int CNTRDOWN = 23;
        public const int CNTRCNTR = 22;

        public const string URLPRODUCTPAGE = "http://www.tatsumidev.com/";
    }
}
