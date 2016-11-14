

/************************************************************************
  Copyright    : 2005-2007, Huawei Tech. Co., Ltd.
  File name    : UsimmBase.c
  Author       : zhuli 00100318
  Version      : v00R002
  Date         : 2008-5-15
  Description  : ��C�ļ�������---Baseģ��ʵ��
  Function List:
  History      :
************************************************************************/
#include "pamappom.h"
#include "omprivate.h"
#include "NVIM_Interface.h"
#include "TafNvInterface.h"
#include "CbtPpm.h"
#include "DrvNvInterface.h"
#include "ScInterface.h"
#include "NasNvInterface.h"

#include "msp_nvim.h"

#include "AtOamInterface.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

/*****************************************************************************
    Э��ջ��ӡ��㷽ʽ�µ�.C�ļ��궨��
*****************************************************************************/
#define    THIS_FILE_ID PS_FILE_ID_PAM_APP_OM_C


/* ��¼�յ���Ϣ��Ϣ��buffer����ǰ���� */
OM_RECORD_BUF_STRU                      g_astAcpuRecordInfo[VOS_EXC_DUMP_MEM_NUM_BUTT];

VOS_UINT32                              g_ulAcpuOmFilterFlag;

/* AT<->AT�����δ������Ƶ�GuNasLogFilter.c */
#if (!defined(DMT))
/*****************************************************************************
 Prototype       : OM_RecordInfoEnd
 Description     : A�˱���׮����
 Input           : ulNumer -- �����
 Output          : None
 Return Value    : VOS_VOID

 History         : ---
    Date         : 2012-07-09
    Author       : s00207770
    Modification : Created function
 *****************************************************************************/
VOS_VOID OM_RecordInfoEnd(VOS_EXC_DUMP_MEM_NUM_ENUM_UINT32 enNumber)
{
    VOS_UINT32 *pulBuf;

    if(VOS_EXC_DUMP_MEM_NUM_BUTT <= enNumber)
    {
        return;
    }

    if(VOS_NULL_PTR == g_astAcpuRecordInfo[enNumber].pucBuf)
    {
        return;
    }

    if(0 == g_astAcpuRecordInfo[enNumber].ulLen)
    {
        return;
    }

    /* ��start���Ѿ�����˼�¼endslice�ĳ��ȣ���˴˴������ĸ��ֽ���дendslice��ֵ */
    pulBuf = (VOS_UINT32*)(g_astAcpuRecordInfo[enNumber].pucBuf + g_astAcpuRecordInfo[enNumber].ulLen - sizeof(VOS_UINT32));

    *pulBuf = VOS_GetSlice();

    return;
}

/*****************************************************************************
 Prototype       : OM_RecordInfoStart
 Description     : A�˱���׮����
 Input           : ulNumer -- �����
                   ulSendPid -- ����PID
                   ulRcvPid -- ����PID
                   ulMsgName -- ��Ϣ����
 Output          : None
 Return Value    : VOS_VOID

 History         : ---
    Date         : 2012-07-09
    Author       : s00702770
    Modification : Created function
 *****************************************************************************/
VOS_VOID OM_RecordInfoStart(VOS_EXC_DUMP_MEM_NUM_ENUM_UINT32 enNumber, VOS_UINT32 ulSendPid, VOS_UINT32 ulRcvPid, VOS_UINT32 ulMsgName)
{
    OM_RECORD_INFO_STRU     stRecordInfo;

    if(VOS_EXC_DUMP_MEM_NUM_BUTT <= enNumber)
    {
       return;
    }

    if(VOS_NULL_PTR == g_astAcpuRecordInfo[enNumber].pucBuf)
    {
       return;
    }

    g_astAcpuRecordInfo[enNumber].ulLen %= VOS_TASK_DUMP_INFO_SIZE;

    stRecordInfo.usSendPid      = (VOS_UINT16)ulSendPid;
    stRecordInfo.usRcvPid       = (VOS_UINT16)ulRcvPid;
    stRecordInfo.ulMsgName      = ulMsgName;
    stRecordInfo.ulSliceStart   = VOS_GetSlice();
    stRecordInfo.ulSliceEnd     = 0;

    (VOS_VOID)VOS_MemCpy((g_astAcpuRecordInfo[enNumber].pucBuf + g_astAcpuRecordInfo[enNumber].ulLen),
              &stRecordInfo, sizeof(OM_RECORD_INFO_STRU));

    g_astAcpuRecordInfo[enNumber].ulLen += (VOS_UINT32)sizeof(OM_RECORD_INFO_STRU);

    return;
}

/*****************************************************************************
 Prototype       : OM_RecordMemInit
 Description     : ��ν�ɲ�ռ����
 Input           : None
 Output          : None
 Return Value    : VOS_VOID

 History         : ---
    Date         : 2012-06-28
    Author       : j00168360
    Modification : Created function
 *****************************************************************************/
VOS_VOID OM_RecordMemInit(VOS_VOID)
{
   VOS_UINT32 i;

   (VOS_VOID)VOS_MemSet(g_astAcpuRecordInfo, 0, sizeof(g_astAcpuRecordInfo));

   /* ����ÿ��ģ���¼��ν�ɲ���Ϣ�Ŀռ� */
   for(i = 0; i < VOS_EXC_DUMP_MEM_NUM_BUTT; i++)
   {
      g_astAcpuRecordInfo[i].pucBuf = (VOS_UINT8*)VOS_ExcDumpMemAlloc(i);

      if(VOS_NULL_PTR == g_astAcpuRecordInfo[i].pucBuf)
      {
          return;
      }
   }

   return;
}
#endif

VOS_VOID PAMOM_AcpuTimerMsgProc(MsgBlock* pMsg)
{
    return;
}
#if (!defined(DMT))

VOS_VOID PAMOM_QuereyPidInfo(VOS_VOID)
{
    PAM_VOS_QUEREY_PID_INFO_REQ_STRU    *pstMsg;

    pstMsg = (PAM_VOS_QUEREY_PID_INFO_REQ_STRU *)VOS_AllocMsg(ACPU_PID_PAM_OM,
                            sizeof(PAM_VOS_QUEREY_PID_INFO_REQ_STRU) - VOS_MSG_HEAD_LENGTH);

    /* ������Ϣʧ�� */
    if (VOS_NULL_PTR == pstMsg)
    {
        return;
    }

    pstMsg->ulReceiverPid  = CCPU_PID_PAM_OM;
    pstMsg->usPrimId       = PAM_VOS_QUEREY_PID_INFO_REQ;

    (VOS_VOID)VOS_SendMsg(ACPU_PID_PAM_OM, pstMsg);

    return;
}


VOS_VOID PAMOM_QuereyPidInfoMsgProc(MsgBlock* pMsg)
{
    PAM_VOS_QUEREY_PID_INFO_REQ_STRU    *pstMsg;
    PAM_VOS_QUEREY_PID_INFO_CNF_STRU    *pstCnfMsg;
    VOS_UINT32                           ulLen;

    pstMsg = (PAM_VOS_QUEREY_PID_INFO_REQ_STRU *)pMsg;

    if (PAM_VOS_QUEREY_PID_INFO_REQ == pstMsg->usPrimId)
    {
        ulLen = VOS_QueryPidInfoBufSize();

        pstCnfMsg = (PAM_VOS_QUEREY_PID_INFO_CNF_STRU *)VOS_AllocMsg(ACPU_PID_PAM_OM,
                            sizeof(PAM_VOS_QUEREY_PID_INFO_CNF_STRU) - VOS_MSG_HEAD_LENGTH + ulLen);

        /* ������Ϣʧ�� */
        if (VOS_NULL_PTR == pstCnfMsg)
        {
            return;
        }

        pstCnfMsg->ulReceiverPid  = CCPU_PID_PAM_OM;
        pstCnfMsg->usPrimId       = PAM_VOS_QUEREY_PID_INFO_CNF;
        pstCnfMsg->usLen          = (VOS_UINT16)ulLen;
        VOS_QueryPidInfo((VOS_VOID *)pstCnfMsg->aucValue);

        (VOS_VOID)VOS_SendMsg(ACPU_PID_PAM_OM, pstCnfMsg);
    }
    else if (PAM_VOS_QUEREY_PID_INFO_CNF == pstMsg->usPrimId)
    {
        pstCnfMsg = (PAM_VOS_QUEREY_PID_INFO_CNF_STRU *)pMsg;

        VOS_SetPidInfo((VOS_VOID *)(pstCnfMsg->aucValue), pstCnfMsg->usLen);
    }
    else
    {
        /* blank */
    }

    return;
}
#endif


 VOS_VOID PAMOM_AcpuCcpuPamMsgProc(MsgBlock* pMsg)
 {
    VOS_UINT16                          usPrimId;

    usPrimId = *(VOS_UINT16 *)(pMsg->aucValue);

    if (PAM_VOS_QUEREY_PID_INFO_REQ == usPrimId)
    {
        PAMOM_QuereyPidInfoMsgProc(pMsg);
    }
    else if (PAM_VOS_QUEREY_PID_INFO_CNF == usPrimId)
    {
        PAMOM_QuereyPidInfoMsgProc(pMsg);
    }
    else
    {

    }

    return;
}

/*****************************************************************************
 Prototype       : CBT_AppMsgProc
 Description     : Handle all messages sent to OM.
 Input           : pMsg -- The pointer of the msg.
 Output          : None
 Return Value    : VOS_VOID

 History         : ---
    Date         : 2008-03-20
    Author       : g47350
    Modification : Created function
 *****************************************************************************/
VOS_VOID PAMOM_AppMsgProc(MsgBlock* pMsg)
{
    if (VOS_PID_TIMER == pMsg->ulSenderPid)
    {
        PAMOM_AcpuTimerMsgProc(pMsg);
    }
    else if (CCPU_PID_PAM_OM == pMsg->ulSenderPid)
    {
        PAMOM_AcpuCcpuPamMsgProc(pMsg);
    }
    else
    {
        /* blank */
    }

    return;
}

/*****************************************************************************
 Prototype       : OM_AcpuInit
 Description     : OM' initializtion function
 Input           : None
 Output          : None
 Return Value    : VOS_UINT32

 History         : ---
    Date         : 2011-07-01
    Author       : g47350
    Modification : Created function
 *****************************************************************************/
VOS_UINT32 PAMOM_AcpuInit(VOS_VOID)
{
    NAS_NV_PRIVACY_FILTER_CFG_STRU      stPrivacyFilterCfg  = {0};

    PAMOM_QuereyPidInfo();

#if (FEATURE_ON != FEATURE_SC_SEC_UPDATE)
    /* SC�ļ���Ч���Լ� */
    (VOS_VOID)SC_COMM_Restore();
#endif

    if (NV_OK != NV_ReadEx(MODEM_ID_0,
                           en_NV_Item_Privacy_Log_Filter_Cfg,
                           &stPrivacyFilterCfg,
                           sizeof(NAS_NV_PRIVACY_FILTER_CFG_STRU)))
    {
        stPrivacyFilterCfg.ucFilterEnableFlg = VOS_FALSE;
    }

    if (VOS_FALSE != stPrivacyFilterCfg.ucFilterEnableFlg)
    {
        stPrivacyFilterCfg.ucFilterEnableFlg = VOS_TRUE;
    }

    g_ulAcpuOmFilterFlag = stPrivacyFilterCfg.ucFilterEnableFlg;

    return VOS_OK;
}

/*****************************************************************************
 Prototype       : OM_AcpuPidInit
 Description     : ACPU OM PID' initializtion function
 Input           : None
 Output          : None
 Return Value    : VOS_UINT32

 History         : ---
    Date         : 2011-07-01
    Author       : g47350
    Modification : Created function
 *****************************************************************************/
VOS_UINT32 PAMOM_AppPidInit(enum VOS_INIT_PHASE_DEFINE ip)
{
    switch( ip )
    {
        case VOS_IP_LOAD_CONFIG:
            return PAMOM_AcpuInit();

        default:
            break;
    }

    return VOS_OK;
}

/*****************************************************************************
 Prototype       : OM_AcpuFidInit
 Description     : ACPU OM FID' initializtion function
 Input           : None
 Output          : None
 Return Value    : VOS_UINT32

 History         : ---
    Date         : 2011-07-01
    Author       : g47350
    Modification : Created function
 *****************************************************************************/
VOS_UINT32 PAMOM_APP_FID_Init(enum VOS_INIT_PHASE_DEFINE ip)
{
    VOS_UINT32                          ulRslt;

    switch( ip )
    {
        case VOS_IP_LOAD_CONFIG:
        {
            ulRslt = VOS_RegisterPIDInfo(ACPU_PID_PAM_OM,
                                        (Init_Fun_Type)PAMOM_AppPidInit,
                                        (Msg_Fun_Type)PAMOM_AppMsgProc);
            if( VOS_OK != ulRslt )
            {
                return VOS_ERR;
            }

            ulRslt = VOS_RegisterMsgTaskPrio(ACPU_FID_PAM_OM, VOS_PRIORITY_M2);
            if( VOS_OK != ulRslt )
            {
                return VOS_ERR;
            }

            /* ��Ŀ¼�������򴴽� */
            if (VOS_OK != mdrv_file_access(PAM_LOG_PARTH, PAM_FILE_EXIST))
            {
                (VOS_VOID)mdrv_file_mkdir(PAM_LOG_PARTH);
            }

            break;
        }

        default:
            break;
    }
    return VOS_OK;
}

#if (!defined(DMT))
/*****************************************************************************
 �� �� ��  : VOS_ICCError_CB
 ��������  : ����ICC WIFIͨ�������ݶ�ȡ�ص�����
 �������  : ulChannelID:   ��ǰͨ��ID
             ulLen:         ��ǰͨ�����ݳ���
 �������  : ��
 �� �� ֵ  : VOS_ERROR/VOS_OK
 ���ú���  :
 ��������  :
 �޸���ʷ  :
   1.��    ��  : 2011��3��10��
     ��    ��  : l46160
     �޸�����  : Creat Function
*****************************************************************************/
VOS_UINT VOS_ICCError_CB(VOS_UINT ulChanID, VOS_UINT ulEvent, VOS_VOID* pParam)
{
    LogPrint2("VOS_ICCError_CB: The Error Channel ID is %d,and Event is %d\n",
                (VOS_INT32)ulChanID, (VOS_INT32)ulEvent);

    return VOS_OK;
}

/*****************************************************************************
 �� �� ��  : OM_ComRx_ICC_Init
 ��������  : ����OM��CCPU��ʼ��ICCͨ��
 �������  : ��
 �������  : ��
 �� �� ֵ  : VOS_ERR/VOS_OK
 ���ú���  :
 ��������  :
 �޸���ʷ  :
   1.��    ��  : 2011��3��10��
     ��    ��  : l46160
     �޸�����  : Creat Function
*****************************************************************************/
VOS_UINT32 VOS_ICC_Init(VOS_VOID)
{
    OM_ICC_UDI_CTRL_STRU                astACPUICCCtrlTable;

    /*��ʼ�� ICCͨ��*/
    (VOS_VOID)VOS_MemSet((&astACPUICCCtrlTable), 0, sizeof(astACPUICCCtrlTable));

    astACPUICCCtrlTable.pstICCAttr = (ICC_CHAN_ATTR_S*)VOS_MemAlloc(ACPU_PID_PAM_OM,
                                                                    STATIC_MEM_PT,
                                                                    sizeof(ICC_CHAN_ATTR_S));
    if(VOS_NULL_PTR == astACPUICCCtrlTable.pstICCAttr)
    {
        return VOS_ERR;             /*�����ڴ�ʧ�ܵ������������˲���Ҫ�ͷ�֮ǰ�Ѿ�������ڴ�*/
    }

    astACPUICCCtrlTable.enICCId               = UDI_ICC_GUOM4;
    astACPUICCCtrlTable.pstICCAttr->read_cb   = V_ICC_OSAMsg_CB;
    astACPUICCCtrlTable.pstICCAttr->u32Priority    = OM_ICC_CHANNEL_PRIORITY;
    astACPUICCCtrlTable.pstICCAttr->u32TimeOut     = OM_ICC_HANDSHAKE_TIME_MAX;
    astACPUICCCtrlTable.pstICCAttr->u32FIFOInSize  = OSA_ICC_BUFFER_SIZE;
    astACPUICCCtrlTable.pstICCAttr->u32FIFOOutSize = OSA_ICC_BUFFER_SIZE;
    astACPUICCCtrlTable.pstICCAttr->enChanMode     = ICC_CHAN_MODE_PACKET;
    astACPUICCCtrlTable.pstICCAttr->event_cb       = VOS_ICCError_CB;
    astACPUICCCtrlTable.pstICCAttr->write_cb       = VOS_NULL_PTR;

    if(VOS_ERROR == DRV_ICC_OPEN(astACPUICCCtrlTable.enICCId, astACPUICCCtrlTable.pstICCAttr))
    {
        /* ��ʧ��ʱ��¼��ǰICCͨ����Ϣ */
        VOS_ProtectionReboot(OM_APP_ICC_INIT_ERROR, THIS_FILE_ID, __LINE__,VOS_NULL_PTR,0);

        return VOS_ERR;
    }

    VOS_IccDebugInfoInit();

    return VOS_OK;
}
#endif

#if (VOS_WIN32 != VOS_OS_VER)
/*****************************************************************************
 �� �� ��  : OM_OSAEvent
 ��������  : �ϱ�OSA�Ŀ�ά�ɲ���Ϣ
 �������  :
 �������  : ��
 �� �� ֵ  : ��

 �޸���ʷ      :
  1.��    ��   : 2013��7��27��
    ��    ��   : x51137
    �޸�����   : �����ɺ���
*****************************************************************************/
VOS_VOID OM_OSAEvent(VOS_VOID *pData, VOS_UINT32 ulLength)
{
    DIAG_EVENT_IND_STRU                 stEventInd;

    stEventInd.ulModule = DIAG_GEN_MODULE(DIAG_MODEM_0, DIAG_MODE_COMM);
    stEventInd.ulPid    = ACPU_PID_PAM_OM;
    stEventInd.ulEventId= OAM_EVENT_TIMER;
    stEventInd.ulLength = ulLength;
    stEventInd.pData    = pData;

    (VOS_VOID)DIAG_EventReport(&stEventInd);

    return;
}
#endif

#if (!defined(DMT))
/*****************************************************************************
 �� �� ��  : PAM_OM_AcpuPihToAtMsgFilter
 ��������  : ����pih��at ��Ϣ�ĺ��� CGLA CRSM CRLA
 �������  : pstMsg -- ��Ϣ����
 �������  : NONE
 �� �� ֵ  : VOS_TRUE :��ʾ�����Ϣ�����˵��ˣ�����Ҫ�ϱ���OM
             VOS_FALSE:��ʾ�����Ϣ��Ҫ�ϱ�OM
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2016��08��29��
    ��    ��   : z00316370
    �޸�����   : �����ɺ���
*****************************************************************************/
VOS_UINT32 PAM_OM_AcpuPihToAtMsgFilter(
    const VOS_VOID                      *pMsg
)
{
    MN_APP_PIH_AT_CNF_STRU             *pstEventCnf;

    pstEventCnf = (MN_APP_PIH_AT_CNF_STRU *)pMsg;

    if (PIH_AT_EVENT_CNF != pstEventCnf->ulMsgId)
    {
        return VOS_FALSE;
    }

    switch (pstEventCnf->stPIHAtEvent.EventType)
    {
        case SI_PIH_EVENT_CGLA_SET_CNF:
        case SI_PIH_EVENT_CRSM_SET_CNF:
        case SI_PIH_EVENT_CRLA_SET_CNF:
            OM_NORMAL_LOG1("PAM_OM_AcpuAtToPihMsgFilter: The filter EventType is :", pstEventCnf->stPIHAtEvent.EventType);

            return VOS_TRUE;

        default:
            return VOS_FALSE;
    }
}

/*****************************************************************************
 �� �� ��  : PAM_OM_AcpuAtToPihMsgFilter
 ��������  : ����at��pih ��Ϣ�ĺ��� CGLA CRSM CRLA
 �������  : pstMsg -- ��Ϣ����
 �������  : NONE
 �� �� ֵ  : VOS_TRUE :��ʾ�����Ϣ�����˵��ˣ�����Ҫ�ϱ���OM
             VOS_FALSE:��ʾ�����Ϣ��Ҫ�ϱ�OM
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2016��08��29��
    ��    ��   : z00316370
    �޸�����   : �����ɺ���
*****************************************************************************/
VOS_UINT32 PAM_OM_AcpuAtToPihMsgFilter(
    const VOS_VOID                      *pMsg
)
{
    MSG_HEADER_STRU                     *pstMsg;

    pstMsg = (MSG_HEADER_STRU *)pMsg;

    switch (pstMsg->ulMsgName)
    {
        case OM_SI_PIH_CRSM_SET_REQ:
        case OM_SI_PIH_CRLA_SET_REQ:
        case OM_SI_PIH_CGLA_SET_REQ:
            OM_NORMAL_LOG1("PAM_OM_AcpuAtToPihMsgFilter: The filter ulMsgName is :", pstMsg->ulMsgName);

            return VOS_TRUE;

        default:
            break;
    }

    return VOS_FALSE;
}

/*****************************************************************************
 �� �� ��  : PAM_OM_LayerMsgFilter
 ��������  : ����PAM�е�������Ϣ
 �������  : const VOID *pMsg:��Ϣָ��
 �������  : ��
 �� �� ֵ  : VOS_UINT32:
                ����VOS_TRUE:  ��ʾ����Ϣ��Ҫ���й���
                ����VOS_FALSE: ��ʾ����Ϣ������й���
 ���ú���  :
 ��������  :
 �޸���ʷ      :
  1.��    ��   : 2015��10��28��
    ��    ��   : zhuli
    �޸�����   : �����ɺ���
*****************************************************************************/
VOS_UINT32 PAM_OM_LayerMsgFilter(
    const VOS_VOID                      *pMsg
)
{
    OM_FILTER_MSG_HEAD_STRU             *pstMsgHead;

    if (VOS_FALSE == g_ulAcpuOmFilterFlag)
    {
        return VOS_FALSE;
    }

    pstMsgHead = (OM_FILTER_MSG_HEAD_STRU*)pMsg;

     /* PIH ��Ϣ���� */
    if ( (I0_MAPS_PIH_PID == pstMsgHead->ulSenderPid)
      || (I1_MAPS_PIH_PID == pstMsgHead->ulSenderPid)
      || (I2_MAPS_PIH_PID == pstMsgHead->ulSenderPid))
    {
        return PAM_OM_AcpuPihToAtMsgFilter(pMsg);
    }

    if (WUEPS_PID_AT == pstMsgHead->ulSenderPid)
    {
        return PAM_OM_AcpuAtToPihMsgFilter(pMsg);
    }

    return VOS_FALSE;
}
#endif

/* AT<->AT�����δ������Ƶ�GuNasLogFilter.c */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */
