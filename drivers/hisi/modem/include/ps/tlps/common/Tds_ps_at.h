/******************************************************************************

   Copyright(C)2008,Hisilicon Co. LTD.

 ******************************************************************************
  File Name       : Tds_ps_at.h
  Description     :  非信令维修工具AT命令头文件
  History           :
     1.Name+ID      2012-09-20 l00213440 Draft Enact
******************************************************************************/

#ifndef __TDS_PS_AT_H__
#define __TDS_PS_AT_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/*****************************************************************************
  1 Include Headfile
*****************************************************************************/
#include "at_common.h"

#if (VOS_OS_VER != VOS_WIN32)
#pragma pack(4)
#else
#pragma pack(push, 4)
#endif

/*****************************************************************************
  2 Macro
*****************************************************************************/

/*****************************************************************************
  3 Massage Declare
*****************************************************************************/

/*****************************************************************************
  4 Enum
*****************************************************************************/

/*****************************************************************************
  5 STRUCT
*****************************************************************************/

/*****************************************************************************
  6 UNION
*****************************************************************************/

/*****************************************************************************
  7 Extern Global Variable
*****************************************************************************/

/*****************************************************************************
  8 Fuction Extern
*****************************************************************************/
extern VOS_UINT32 TDS_SwitchSlave(VOS_VOID);
extern VOS_UINT32 TDS_SwitchMaster(VOS_VOID);
extern VOS_UINT32 TDS_AtPsSendMsg(VOS_UINT32 ulLen, TDS_AT_PS_MSG_INFO_STRU * pstTdsAppPsMsg);
/*extern VOS_UINT32 TDS_PsAtSendMsg(VOS_UINT32 ulLen, TDS_AT_PS_MSG_INFO_STRU * pstTdsAppPsMsg);
 */
extern VOS_UINT32 Tds_PsZspOmMboxWrite(VOS_UINT32 ulLength, VOS_UINT8 *pucBuf);

/*****************************************************************************
  9 OTHERS
*****************************************************************************/


#if (VOS_OS_VER != VOS_WIN32)
#pragma pack()
#else
#pragma pack(pop)
#endif


#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif




