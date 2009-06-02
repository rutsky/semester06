//  **********************************************************
//  FILE NAME   DiTypes.h
//  PURPOSE     Basic types & defines
//  START       15 jan 2001
//  AUTHOR      Vlad
//  PREFIX      Di
//  NOTES
//  **********************************************************

#ifndef _DITYPES_H_
#define _DITYPES_H_

//  **********************************************************
//  Includes
//  **********************************************************

//  **********************************************************
//  Defines
//  **********************************************************

// Max file name length
#define   diMAX_PATH              260

// Max, min floats
#define   diMAX_FLOAT             (float)(3.4e+38)
#define   diMIN_FLOAT             (float)(-3.4e+38)

// Max, min ints
#define   diMAX_UINT16            65535
#define   diMIN_UINT16            0

// standart values
#ifdef FALSE
  #undef FALSE
#endif
#define FALSE 0

#ifdef TRUE
  #undef TRUE
#endif
#define TRUE   (!FALSE)

#ifdef NULL
  #undef NULL
#endif
#define NULL    ((void*)0)

#ifndef M_PI
  #define M_PI    3.1415926535f
#endif

//  **********************************************************
//  Types
//  **********************************************************

typedef char                      DiChar;

typedef unsigned  char            DiByte;

typedef char                      DiInt8;
typedef short  int                DiInt16;
typedef long   int                DiInt32;
typedef unsigned char             DiUInt8;
typedef unsigned short  int       DiUInt16;
typedef unsigned long   int       DiUInt32;

typedef long  int                 DiBool;
typedef void                      DiVoid;

typedef float                     DiFloat;
typedef double                    DiDouble;

typedef unsigned long int         DiColor;
typedef unsigned long int         DiDword;


#endif  
