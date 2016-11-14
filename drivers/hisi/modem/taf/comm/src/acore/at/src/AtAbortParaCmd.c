

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "AtParse.h"
#include "ATCmdProc.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
    Э��ջ��ӡ��㷽ʽ�µ�.C�ļ��궨��
*****************************************************************************/
#define    THIS_FILE_ID        PS_FILE_ID_AT_ABORTPARACMD_C

/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/

/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/
#if (FEATURE_ON == FEATURE_CSG)
/*****************************************************************************
 �� �� ��  : AT_AbortCsgIdSearchPara
 ��������  : ���csg�б�������������
 �������  : ucIndex - AT�˿ڵ�index
 �������  : ��
 �� �� ֵ  : VOS_UINT32
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2015��9��6��
    ��    ��   : z00161729
    �޸�����   : ֧��LTE CSG��������
*****************************************************************************/
VOS_UINT32 AT_AbortCsgIdSearchPara(
    VOS_UINT8                           ucIndex
)
{
    /* ��ǰֻ�ܴ����б��ѵĴ��, ���������򲻽��д�ϲ��� */
    if ( AT_CMD_CSG_LIST_SEARCH == gastAtClientTab[ucIndex].CmdCurrentOpt )
    {
        /* AT��MMA���ʹ���б��ѵ����� */
        if (VOS_TRUE == TAF_MMA_AbortCsgListSearchReq(WUEPS_PID_AT, gastAtClientTab[ucIndex].usClientId, 0))
        {
            /* ֹͣcsg�б���AT�ı�����ʱ�� */
            AT_StopRelTimer(ucIndex, &gastAtClientTab[ucIndex].hTimer);

            /* ���µ�ǰ�������� */
            gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_ABORT_CSG_LIST_SEARCH;

            return AT_WAIT_ASYNC_RETURN;
        }

    }

    return AT_FAILURE;
}
#endif


/*****************************************************************************
 �� �� ��  : At_AbortCopsPara
 ��������  : COPS����Ĵ�ϴ�������
 �������  : ucIndex: AT�˿ڵ�index
 �������  : ��
 �� �� ֵ  : VOS_UINT32
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��09��19��
    ��    ��   : l00171473
    �޸�����   : V7R1C50_At_Abort, ���COPS����Ĵ�������
*****************************************************************************/
VOS_UINT32 At_AbortCopsPara(
    VOS_UINT8                           ucIndex
)
{
    /* ��ǰֻ�ܴ����б��ѵĴ��, ���������򲻽��д�ϲ��� */
    if ( AT_CMD_COPS_TEST == gastAtClientTab[ucIndex].CmdCurrentOpt )
    {
        /* AT��MMA���ʹ���б��ѵ����� */
        if (VOS_TRUE == TAF_MMA_AbortPlmnListReq(WUEPS_PID_AT, gastAtClientTab[ucIndex].usClientId, 0))
        {
            /* ֹͣ�б���AT�ı�����ʱ�� */
            AT_StopRelTimer(ucIndex, &gastAtClientTab[ucIndex].hTimer);

            /* ���µ�ǰ�������� */
            gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_COPS_ABORT_PLMN_LIST;

            return AT_WAIT_ASYNC_RETURN;
        }
        else
        {
            return AT_FAILURE;
        }
    }
    else
    {
        return AT_FAILURE;
    }

}


#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif
