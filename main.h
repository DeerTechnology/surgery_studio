#ifndef MAIN_H
#define MAIN_H

#define __USEPAD
//#define __ENGLISH
//#define __LOOPTEST //测试使用
#ifdef __USEPAD
    #define __FULLSCREEN
    #define __TOUCHSCREEN
    //#define __AUTOCONNECT
#endif
#define __TOUCHSCREEN
#ifdef __ENGLISH
#define __MAINNAME ""
#else
#define __MAINNAME "KUNIN微量注射泵"
#endif

#define CHECK_TIME 100

#define __VERSION_MASTER "V 1.1.5"//移动变换
#define __VERSION_SLAVE "V 1.1.2"//移动变换

#endif // MAIN_H
//cd /d D:\codes\......
//windeployqt ......
