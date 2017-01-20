/******************************************************************************

                  版权所有 (C), 2001-2013, 华为技术有限公司

 ******************************************************************************
  文 件 名   : MtcNasInterface.h
  版 本 号   : 初稿
  作    者   : z00301431
  生成日期   : 2015年05月03日
  最近修改   :
  功能描述   : NAS模块与MTC模块内部接口头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2015年05月03日
    作    者   : z00301431
    修改内容   : 创建文件

******************************************************************************/

#ifndef _MTCNASINTERFACE_H_
#define _MTCNASINTERFACE_H_

/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/
#include "vos.h"
#include "PsTypeDef.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#pragma pack(4)

/*****************************************************************************
  2 宏定义
*****************************************************************************/

/*****************************************************************************
  3 枚举定义
*****************************************************************************/
/*****************************************************************************
枚举名    : MTC_NAS_MSG_ID_ENUM
结构说明  : NAS和MTC之间的消息

  1.日    期   : 2015年05月03日
    作    者   : z00301431
    修改内容   : 定义NAS和MTC之间的消息
*****************************************************************************/
enum MTC_NAS_MSG_ID_ENUM
{
    /* 消息名称 */                         /* 消息ID */                         /* 备注 */
    /* NAS --> MTC */
    ID_NAS_MTC_TC_STATUS_INFO_IND          = 0x0001,                            /* _H2ASN_MsgChoice NAS_MTC_TC_STATUS_INFO_IND_STRU */

    /* MTC --> NAS */

    /* MTC --> MMC */
    ID_MTC_MMC_SCREEN_STA_CHANGE_NOTIFY = 0x0002,                            /* _H2ASN_MsgChoice MTC_MMC_SCREEN_STA_CHANGE_NOTIFY_STRU */

    ID_MTC_NAS_MSG_ID_BUTT
};
typedef VOS_UINT32  MTC_NAS_MSG_ID_ENUM_UINT32;

/*****************************************************************************
枚举名    : MTC_NAS_TC_STATUS_ENUM_UINT16
结构说明  : 环回状态上报

  1.日    期   : 2015年05月3日
    作    者   : z00301431
    修改内容   : 环回模式
*****************************************************************************/
enum MTC_NAS_TC_STATUS_ENUM
{
    MTC_NAS_TC_STOP                      = 0,                                   /* 环回停止 */
    MTC_NAS_TC_START,                                                           /* 环回启动 */
    MTC_NAS_TC_BUTT
};
typedef VOS_UINT16 MTC_NAS_TC_STATUS_ENUM_UINT16;


/*****************************************************************************
枚举名    : MTC_NAS_RAT_MODE_ENUM_UINT16
结构说明  : 接入模式

  1.日    期   : 2015年05月3日
    作    者   : z00301431
    修改内容   : 接入模式
*****************************************************************************/
enum MTC_NAS_RAT_MODE_ENUM
{
    MTC_NAS_RAT_MODE_GSM                     = 0,                               /* GSM */
    MTC_NAS_RAT_MODE_WCDMA,                                                     /* WCDMA */
    MTC_NAS_RAT_MODE_TDSCDMA,                                                   /* TDSCDMA */
    MTC_NAS_RAT_MODE_LTE,                                                       /* LTE */
    MTC_NAS_RAT_MODE_CDMA,                                                      /* CDMA */
    MTC_NAS_RAT_MODE_BUTT
};
typedef VOS_UINT16 MTC_NAS_RAT_MODE_ENUM_UINT16;

/*****************************************************************************
枚举名    : MTC_NAS_SCREEN_STATE_ENUM_UINT8
结构说明  : 亮灭屏状态

  1.日    期   : 2016年6月13日
    作    者   : c00318887
    修改内容   : 亮灭屏状态
*****************************************************************************/
enum MTC_NAS_SCREEN_STATE_ENUM
{
    MTC_NAS_SCREEN_STATE_OFF            = 0x0,              /* 灭屏 */
    MTC_NAS_SCREEN_STATE_ON,                                /* 亮屏 */

    MTC_NAS_SCREEN_STATE_BUTT
};
typedef VOS_UINT8 MTC_NAS_SCREEN_STATE_ENUM_UINT8;


/*****************************************************************************
  4 全局变量声明
*****************************************************************************/


/*****************************************************************************
  5 消息头定义
*****************************************************************************/


/*****************************************************************************
  6 消息定义
*****************************************************************************/


/*****************************************************************************
  7 STRUCT定义
*****************************************************************************/
/*******************************************************************************
 结构名    : NAS_MTC_TC_STATUS_INFO_IND_STRU
 结构说明  : ID_NAS_MTC_TC_STATUS_INFO_IND的消息结构

 1.日    期   : 2015年05月03日
   作    者   : z00301431
   修改内容   : 新生成
*******************************************************************************/
typedef struct
{
    VOS_MSG_HEADER
    VOS_UINT32                          ulMsgName;
    MTC_NAS_TC_STATUS_ENUM_UINT16       enTcStatus;
    MTC_NAS_RAT_MODE_ENUM_UINT16        enRatMode;
}NAS_MTC_TC_STATUS_INFO_IND_STRU;

/*******************************************************************************
 结构名    : MTC_MMC_SCREEN_STA_CHANGE_NOTIFY_STRU
 结构说明  : ID_MTC_MMC_SCREEN_STA_CHANGE_NOTIFY 的消息结构

 1.日    期   : 2016年06月13日
   作    者   : c00318887
   修改内容   : 新生成
*******************************************************************************/
typedef struct
{
    MSG_HEADER_STRU                     stMsgHeader;                            /*_H2ASN_Skip*/
    MTC_NAS_SCREEN_STATE_ENUM_UINT8     enScrnState;
    VOS_UINT8                           aucReserved[3];
}MTC_MMC_SCREEN_STA_CHANGE_NOTIFY_STRU;

/*****************************************************************************
  8 UNION定义
*****************************************************************************/


/*****************************************************************************
  9 OTHERS定义
*****************************************************************************/

/*****************************************************************************
  H2ASN顶级消息结构定义
*****************************************************************************/
typedef struct
{
    MTC_NAS_MSG_ID_ENUM_UINT32          enMsgID;                                /*_H2ASN_MsgChoice_Export MTC_NAS_MSG_ID_ENUM_UINT32*/
    VOS_UINT8                           aucMsgBlock[4];
    /***************************************************************************
        _H2ASN_MsgChoice_When_Comment          MTC_NAS_MSG_ID_ENUM_UINT32
    ****************************************************************************/
} MTC_NAS_MSG_DATA;
/*_H2ASN_Length UINT32*/

typedef struct
{
    VOS_MSG_HEADER
    MTC_NAS_MSG_DATA                    stMsgData;
} MtcNasInterface_MSG;

/*****************************************************************************
  10 函数声明
*****************************************************************************/

#if ((VOS_OS_VER == VOS_WIN32) || (VOS_OS_VER == VOS_NUCLEUS))
#pragma pack()
#else
#pragma pack(0)
#endif

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of MtcNasInterface.h */

