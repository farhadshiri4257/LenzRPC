#ifndef __LIB_DEF_H_
#define __LIB_DEF_H_

/*
*********************************************************************************************************
*                                          STANDARD DEFINES
*********************************************************************************************************
*/

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
#endif


#define  DEF_NULL                                 ((void *)0)


                                                                /* ----------------- BOOLEAN DEFINES ------------------ */
#define  DEF_FALSE                                         0u
#define  DEF_TRUE                                          1u

#define  DEF_NO                                            0u
#define  DEF_YES                                           1u

#define  DEF_DISABLED                                      0u
#define  DEF_ENABLED                                       1u

#define  DEF_INACTIVE                                      0u
#define  DEF_ACTIVE                                        1u

#define  DEF_INVALID                                       0u
#define  DEF_VALID                                         1u

#define  DEF_OFF                                           0u
#define  DEF_ON                                            1u

#define  DEF_CLR                                           0u
#define  DEF_SET                                           1u

#define  DEF_FAIL                                          0u
#define  DEF_OK                                            1u
#define  DEF_TIMEOUT                                      2u

                                                                /* ------------------- BIT DEFINES -------------------- */
#define  DEF_BIT_NONE                                   0x00u

#define  DEF_BIT_00                                     0x01u
#define  DEF_BIT_01                                     0x02u
#define  DEF_BIT_02                                     0x04u
#define  DEF_BIT_03                                     0x08u
#define  DEF_BIT_04                                     0x10u
#define  DEF_BIT_05                                     0x20u
#define  DEF_BIT_06                                     0x40u
#define  DEF_BIT_07                                     0x80u

#define  DEF_BIT_08                                   0x0100u
#define  DEF_BIT_09                                   0x0200u
#define  DEF_BIT_10                                   0x0400u
#define  DEF_BIT_11                                   0x0800u
#define  DEF_BIT_12                                   0x1000u
#define  DEF_BIT_13                                   0x2000u
#define  DEF_BIT_14                                   0x4000u
#define  DEF_BIT_15                                   0x8000u

#define  DEF_BIT_16                               0x00010000u
#define  DEF_BIT_17                               0x00020000u
#define  DEF_BIT_18                               0x00040000u
#define  DEF_BIT_19                               0x00080000u
#define  DEF_BIT_20                               0x00100000u
#define  DEF_BIT_21                               0x00200000u
#define  DEF_BIT_22                               0x00400000u
#define  DEF_BIT_23                               0x00800000u

#define  DEF_BIT_24                               0x01000000u
#define  DEF_BIT_25                               0x02000000u
#define  DEF_BIT_26                               0x04000000u
#define  DEF_BIT_27                               0x08000000u
#define  DEF_BIT_28                               0x10000000u
#define  DEF_BIT_29                               0x20000000u
#define  DEF_BIT_30                               0x40000000u
#define  DEF_BIT_31                               0x80000000u


/*
*********************************************************************************************************
*                                              DEF_MIN()
*
* Description : Determine the minimum of two values.
*
* Argument(s) : a           First  value.
*
*               b           Second value.
*
* Return(s)   : Minimum of the two values.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

#define  DEF_MIN(a, b)                                  (((a) < (b)) ? (a) : (b))


/*
*********************************************************************************************************
*                                              DEF_MAX()
*
* Description : Determine the maximum of two values.
*
* Argument(s) : a           First  value.
*
*               b           Second value.
*
* Return(s)   : Maximum of the two values.
*
* Note(s)     : none.
*********************************************************************************************************
*/

#define  DEF_MAX(a, b)                                  (((a) > (b)) ? (a) : (b))


#endif

