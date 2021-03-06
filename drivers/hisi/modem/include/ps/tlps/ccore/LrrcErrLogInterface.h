/******************************************************************************

    Copyright(C)2008,Hisilicon Co. LTD.

 ******************************************************************************
  File Name       : LrrcErrLogInterface.h
  Description     : LrrcErrLogInterface.h header file
  History         :
     1.HANLUFENG 41410       2013-8-27     Draft Enact
     2.
******************************************************************************/

#ifndef __LRRCERRLOGINTERFACE_H__
#define __LRRCERRLOGINTERFACE_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
  1 Include Headfile
*****************************************************************************/
#include  "vos.h"


#include "omerrorlog.h"
#include "AppRrcInterface.h"
#include "OmCommon.h"
//#include "LPsOm.h"


#if (VOS_OS_VER != VOS_WIN32)
#pragma pack(4)
#else
#pragma pack(push, 4)
#endif


/*****************************************************************************
  2 macro
*****************************************************************************/

/*****************************************************************************
  3 Massage Declare
*****************************************************************************/
/* LTE支持CA情况下，支持的最大载频数，V9R1为1，V7R2和K3V3为2，协议规定最大值为5 */
#define LRRC_APP_MAX_NUM 5

/* RRC error发生时，RRC的建链信息记录最大个数，包括建链原因和结果 */
#define RRC_APP_MAX_RRC_EST_INFO_NUM              (5)

/* 协议值:3 */
#define RRC_APP_MAX_NUM_OF_MCC                    (3)

/* RRC error发生时，打印打点信息的最大记录个数 */
#define RRC_APP_ERROR_PRINT_INFO_NUM              (4)

/* RRC error发生时，模块接收最新信息的最大记录个数 */
#define RRC_APP_RECEIVE_MSG_INFO_NUM              (16)

/* RRC error发生时，最新RRC错误码的最大记录个数 */
#define RRC_APP_CODE_NUM                          (4)

/* LTE支持MIMO时，最大同时支持的码字数 */
#define NUM_OF_CODEWORD                           (2)

/* ErrorLog新增丢网信息begin */
/* RRC error发生时，丢网最大记录个数 */
#define RRC_ERRLOG_AREA_LOST_MAX_NUM             (4)
/* ErrorLog新增丢网信息end */

#define LRRC_OM_Q_LOW_CELL_NUM_MAX                (8)
#define LRRC_OM_PING_PONG_INFO_NUM_MAX            (8)
#define LRRC_OM_PING_PONG_CELL_NUM_MAX            (8)
#define LRRC_OM_MOBILITY_2_QLOW_NUM_MAX           (8)
#define LRRC_OM_ESR_NUM_MAX                       (8)
#define LRRC_OM_OUT_OF_SYNC_NUM_MAX               (8)

#define LRRC_OM_MAX_LTE_INTER_FREQ_COUNT           (16)
#define LRRC_OM_MAX_SINGLE_FREQ_CELL_COUNT         (16)

#define LRRC_OM_AIR_MSG_MAX_LEN                    (240)
#define LRRC_OM_AIR_MSG_BUF_LEN                    (720)
#define LRRC_OM_SINGLR_AIR_MSG_MAX_LEN             (360)
#define LRRC_OM_AIR_MSG_MAX_RECORD_TIMEINT         (300)    /*空口消息记录的最大间隔*/

#define LRRC_OM_SINGE_PRINT_LEN                    (24)
#define LRRC_OM_PRINT_INFO_NUM                     (6)
#define LRRC_OM_SRVCC_MAX_MEASID_NUM               (3)

#define  LRRC_CHR_RPT_GUT2L_RESEL_FAIL_MAX_NUM           (10)

#define LRRC_OM_KEY_EVENT_DB_NUM                   56
#define LRRC_OM_UL_AIRMSG_DB_NUM                   60
#define LRRC_OM_AIR_SIGNAL_INFO_ELE_NUM            20
#define LRRC_OM_MEAS_RPT_DB_NUM                    16
#define LL2_OM_VOLTE_PKT_INFO_RPT_NUM              20

#define LRRC_OM_INVALID_LTE_CELLID                 0xFFFFUL

#define LRRC_OM_AIR_SIGNAL_RECORD_INTERVAL         400
#define LRRC_OM_INFO_RECORD_MAXINTVAL              90000   /*事件，空口测量的最大记录间隔*/

#define TLPS_SECU_FUNCTION_ERR_MAXNO               10


/* Volte统计最大的个数 */
#define LRRC_LPDCP_VOLTE_STAT_NUM                  (6)

#define CHR_DIRECTRPT_HDR_LEN                       (sizeof(OM_ERR_LOG_HEADER_STRU) + sizeof(MODEM_ERR_LOG_HEADER_STRU))

/* BAND-38 Scell Not config MFBI record begin */
#define  LRRC_OM_SCELL_NOTCONFIG_MFBI_MAX_NUM             (30)
#define  LRRC_OM_SCELL_NOTCONFIG_MFBI_RPT_TIME_INTERVAL   (6*3600)
/* BAND-38 Scell Not config MFBI record end */


#define LRRC_VOLTE_GEN_STAT_CALL_NUM                16
#define LRRC_VOLTE_GEN_STAT_FAULT_NUM               40


/*****************************************************************************
  4 Enum
*****************************************************************************/
/* HIDS显示的目的 */
typedef VOS_UINT16 LRRC_OM_AIRMSG_TYPE_ENUM_UINT16;

/*****************************************************************************
 枚举名    : LRRC_GUTRRC_CELL_RESEL_RESULT_ENUM
 协议表格  :
 ASN.1描述 :
 枚举说明  : GUT与LTE之间CELL RESEL结果
*****************************************************************************/
enum LRRC_GUTRRC_RESEL_FAIL_CAUSE_ENUM
{
    LRRC_GUTRRC_CELL_RESEL_ARFCN_NOT_SUPPORT            = 0     ,                  /*重选参数检查，重选频点不支持 */
    LRRC_GUTRRC_CELL_LOCKED                          ,                             /*重选参数检查，存在锁小区*/
    LRRC_GUTRRC_FREQ_LOCKED                          ,                             /*重选参数检查，存在锁频点*/
    LRRC_GUTRRC_CELL_BARRED                                       ,                /* 小区在BAR列表中*/


    LRRC_GUTRRC_CELL_SET_WORK_MODE_FAIL                          ,                 /* 向物理层设置切换模式失败*/
    LRRC_GUTRRC_CELL_DSDS_NO_RF_FAIL                             ,                 /* DSDS场景，没有RF资源*/
    LRRC_GUTRRC_CELL_SEARCH_APPLY_RF_FAIL                        ,                 /*小区搜索前，申请资源失败*/
    LRRC_GUTRRC_CELL_SEARCH_RF_OCCUPYED                          ,                 /*小区搜索过程中，资源被抢占导致停止*/
    LRRC_GUTRRC_CELL_SEARCH_RSSI_SORT_FAIL                       ,                 /*物理层上报RSSI IND参数错误*/
    LRRC_GUTRRC_BAND_SCAN_IND_NO_RF                              ,                 /*物理层上报BAND SCAN IND,失败原因为NO-RF*/

    LRRC_GUTRRC_CELL_SEARCH_IND_NO_RF                            ,                 /*物理层上报CELL SEARCH IND,失败原因为NO-RF*/
    LRRC_GUTRRC_CELL_SEARCH_IND_LOCKED                          ,                  /*物理层上报CELL_SEARCH_IND,失败原因为存在锁小区*/
    LRRC_GUTRRC_CELL_SEARCH_IND_FAKE                            ,                  /*物理层上报CELL_SEARCH_IND,失败原因为假小区*/
    LRRC_GUTRRC_CELL_SEARCH_IND_BAND_NOT_SUPP                   ,                  /*物理层上报CELL_SEARCH_IND,失败原因为BAND 不支持*/

    LRRC_GUTRRC_CELL_SEARCH_IND_RSRP_LOW                        ,                  /*物理层上报CELL_SEARCH_IND,失败原因为RSRP值太低*/
    LRRC_GUTRRC_CELL_SEARCH_IND_RSRQ_LOW                        ,                  /*物理层上报CELL_SEARCH_IND,失败原因为RSRQ值太*/
    LRRC_GUTRRC_CELL_SEARCH_IND_ACC_BARED                       ,                  /*物理层上报CELL_SEARCH_IND,原因为小区多次接入失败被BAR*/
    LRRC_GUTRRC_CELL_SEARCH_IND_NO_CELL                         ,                  /*物理层上报CELL_SEARCH_IND,失败原因为小区不存在*/

    LRRC_GUTRRC_CELL_SYSINFO_SIB_RCV_FAIL                         ,                /* 系统消息接收失败*/
    LRRC_GUTRRC_CELL_SYSINFO_CELL_AC_BARRED                       ,                /* 系统消息带BAR指示，导致接收失败*/
    LRRC_GUTRRC_CELL_SYSINFO_CELL_MFBI_ERROR                      ,                /* MFBI场景，接收消息band不在multiband列表中*/
    LRRC_GUTRRC_CELL_SYSINFO_CHINA_MOBILE_FDD                      ,               /* MCC为中国移动，系统消息为FDD*/
    LRRC_GUTRRC_CELL_SYSINFO_SOFT_BANK_ERROR                      ,                /* 软银定制特性异常*/
    LRRC_GUTRRC_CELL_SYSINFO_BAND_NOT_SUPPORT                     ,                /* BAND不支持*/


    LRRC_GUTRRC_CELL_SYSINFO_CELL_FORBID                          ,                /* CELL 被 FORBID*/
    LRRC_GUTRRC_CELL_SYSINFO_NOT_EPLMNS                           ,                /* CELL携带的PLMN信息没有属于RPLMN和EPLMNS */
    LRRC_GUTRRC_CELL_S_CHECK_FAIL                               ,                  /* S准则不满足*/
    LRRC_GUTRRC_CELL_CAMP_FAIL                                  ,                  /* 驻留失败*/
    LRRC_GUTRRC_CELL_PCH_SETUP_FAIL                             ,                  /* 寻呼信道建立失败*/

    LRRC_GUTRRC_CELL_RESEL_UNSPECIFIC                           ,                  /* 其他未定义的错误 */

    LRRC_GUTRRC_CELL_RESEL_RESULT_BUTT
};
typedef VOS_UINT32 LRRC_GUTRRC_RESEL_FAIL_CAUSE_ENUM_UINT32;

enum RRC_APP_FTM_TYPE_ENUM
{
    RRC_APP_FTM_PERIOD_INFO,     /* 周期型上报的工程模式信息，目前Idle状态上报周期为DRX周期，Connected状态下周期为1秒 */
    RRC_APP_FTM_CHANGE_INFO,     /* 更新后上报型的工程模式信息 */
    RRC_APP_FTM_INFO_BUTT
};
typedef VOS_UINT32 RRC_APP_FTM_TYPE_ENUM_UINT32;

/*****************************************************************************
 枚举名    : RRC_APP_PAGING_CYCLE_ENUM
 协议表格  :
 ASN.1描述 :
 枚举说明  : 寻呼周期，RFXX，XX表示无线帧个数，单位为10ms,RF32周期是320ms，界面显示为XXXms
*****************************************************************************/
enum RRC_APP_PAGING_CYCLE_ENUM
{
    RRC_APP_RF32                                     = 0,
    RRC_APP_RF64                                     = 1,
    RRC_APP_RF128                                    = 2,
    RRC_APP_RF256                                    = 3
};
typedef VOS_UINT32 RRC_APP_PAGING_CYCLE_ENUM_UINT32;
/*****************************************************************************
 枚举名    : RRC_APP_TRANSM_MODE_ENUM
 协议表格  :
 ASN.1描述 :
 枚举说明  : 传输模式类型，界面直接显示TMX_TRANSM_MODE即可
*****************************************************************************/
enum RRC_APP_TRANSM_MODE_ENUM
{
    RRC_APP_TM1_TRANSM_MODE                          = 0,
    RRC_APP_TM2_TRANSM_MODE                          = 1,
    RRC_APP_TM3_TRANSM_MODE                          = 2,
    RRC_APP_TM4_TRANSM_MODE                          = 3,
    RRC_APP_TM5_TRANSM_MODE                          = 4,
    RRC_APP_TM6_TRANSM_MODE                          = 5,
    RRC_APP_TM7_TRANSM_MODE                          = 6,
    RRC_APP_TM8_TRANSM_MODE                          = 7,
    RRC_APP_TM9_TRANSM_MODE                          = 8,
    RRC_APP_TRANSM_MODE_BUTT
};
typedef VOS_UINT32 RRC_APP_TRANSM_MODE_ENUM_UINT32;
/*****************************************************************************
 枚举名    : RRC_APP_PROTOCOL_STATE_IND
 协议表格  :
 ASN.1描述 :
 枚举说明  : RRC协议状态类型
*****************************************************************************/
enum RRC_APP_PROTOCOL_STATE_IND_ENUM
{
    RRC_APP_PROTOCOL_IDLE                   = 0 ,
    RRC_APP_PROTOCOL_CONNECTED,
    RRC_APP_PROTOCOL_STATE_BUTT
};
typedef VOS_UINT8 RRC_APP_PROTOCOL_STATE_IND_ENUM_UINT8;

/*****************************************************************************
 枚举名    : RRC_APP_ERROR_CODE_ENUM
 协议表格  :
 ASN.1描述 :
 枚举说明  : RRC错误码类型
*****************************************************************************/
enum RRC_APP_ERROR_CODE_ENUM
{
    RRC_APP_RB_MEAS_CHECK_ERR               = 0, /* 重配置失败--measConfig检查失败 */
    RRC_APP_RB_MOCTRL_CHECK_ERR             = 1, /* 重配置失败--MobilCtrlInfo 切换消息检查失败 */
    RRC_APP_RB_RADIOSRC_CHECK_ERR           = 2, /* 重配置失败--RadioRsrcConfigDedic 消息检查失败 */
    RRC_APP_RB_TTIBUNDING_CHECK_ERR         = 3, /* 重配置失败--TTIBundling 检查失败 */
    RRC_APP_RB_SECU_CHECK_ERR               = 4, /* 重配置失败--SecurCfgHo 切换中安全检查失败 */
    RRC_APP_RB_CFGDSP_ERR                   = 5, /* 重配置失败--配置DSP，DSP返回失败 */
    RRC_APP_RB_CFGL2_ERR                    = 6, /* 重配置失败--配置L2,L2返回失败 */
    RRC_APP_RB_CFGDSP_TIMEROUT_ERR          = 7, /* 重配置失败--配置DSP，保护定时器超时 */
    RRC_APP_RB_CFGL2_TIMEROUT_ERR           = 8, /* 重配置失败--配置L2，保护定时器超时 */

    RRC_APP_EST_PRESENT                         = 12, /* RRC连接建立失败--RRC连接已存在*/
    RRC_APP_EST_ESTING                          = 13, /* RRC连接建立失败--RRC正在建链*/
    RRC_APP_EST_DELING                          = 14, /* RRC连接建立失败--RRC正在释放连接*/
    RRC_APP_EST_DSDS_NO_RF                      = 15, /* RRC连接建立失败--双通:无射频资源*/
    RRC_APP_EST_CELL_SEARCHING                  = 16, /* RRC连接建立失败--RRC正在搜网*/
    RRC_APP_EST_EUTRAN_REJ                      = 17, /* RRC连接建立失败--网侧拒绝建链*/

    RRC_APP_EST_MT_T302_ERR                     = 18, /* RRC连接建立失败--MT Call；T302定时器在运行 */

    RRC_APP_EST_MO_T302_ERR                     = 22, /* RRC连接建立失败--Mo Data；T302定时器在运行 */
    RRC_APP_EST_MO_T303_ERR                     = 23, /* RRC连接建立失败--Mo Data；T303定时器在运行 */
    RRC_APP_EST_MO_ACC_PROBABILITY_ERR          = 24, /* RRC连接建立失败--Mo Data；ACC接入等级问题 */
    RRC_APP_EST_MO_RHPLMN_PROBABILITY_ERR       = 25, /* RRC连接建立失败--Mo Data；Rplmn、Hplmn问题 */
    RRC_APP_EST_MO_ACC_RHPLMN_PROBABILITY_ERR   = 26, /* RRC连接建立失败--Mo Data；AcBarringForMoData存在判断被Bar */

    RRC_APP_EST_MS_T302_ERR                     = 28, /* RRC连接建立失败--Mo Signalling；T302定时器在运行 */
    RRC_APP_EST_MS_T305_ERR                     = 29, /* RRC连接建立失败--Mo Signalling；T305定时器在运行 */
    RRC_APP_EST_MS_ACC_PROBABILITY_ERR          = 30, /* RRC连接建立失败--Mo Signalling；ACC接入等级问题 */
    RRC_APP_EST_MS_RHPLMN_PROBABILITY_ERR       = 31, /* RRC连接建立失败--Mo Signalling；Rplmn、Hplmn问题 */
    RRC_APP_EST_MS_ACC_RHPLMN_PROBABILITY_ERR   = 32, /* Mo Signalling；AcBarringForMoData存在判断被Bar */
    RRC_APP_EST_T300_EXPIRED_ERR                = 33, /* RRC连接建立失败--T300超时*/
    RRC_APP_EST_AREA_LOST_ERR                   = 34, /* 当前已经出服务区直接给NAS回失败 */

    /* ErrorLog新增丢网信息begin */
    RRC_APP_AREA_LOST_ERR                       = 35,  /* 上报NAS丢网 */
    /* ErrorLog新增丢网信息end */

    RRC_APP_REEST_T311_TIMEROUT_ERR             = 49, /* 重建立失败--重建过程中小区搜索T311定时器超时 */
    RRC_APP_REEST_T301_TIMEROUT_ERR             = 50, /* 重建立失败--发送建立Req消息后，等待重建响应消息，T301超时 */
    RRC_APP_REEST_SRB0CFGDSP_ERR                = 51, /* 重建立失败--配置DSP，DSP返回失败 */
    RRC_APP_REEST_SRB0CFGL2_ERR                 = 52, /* 重建立失败--配置L2，L2返回失败 */
    RRC_APP_REEST_MACCFG_CHECK_ERR              = 53, /* 重建立失败--消息检查MasCfg失败 */
    RRC_APP_REEST_PHYDEDIC_CHECK_ERR            = 54, /* 重建立失败--消息检查PhyCfgDedic失败 */
    RRC_APP_REEST_TTIBUND_CHECK_ERR             = 55, /* 重建立失败--ttiBundling检查失败 */
    RRC_APP_REEST_SRB_CHECK_ERR                 = 56, /* 重建立失败--SRB检查失败超时 */
    RRC_APP_REEST_SPS_CHECK_ERR                 = 57, /* 重建立失败--SPS检查失败 */
    RRC_APP_REEST_SRB1CFGDSP_ERR                = 58, /* 重建立失败--配置DSP，DSP返回失败 */
    RRC_APP_REEST_SRB1CFGL2_ERR                 = 59, /* 重建立失败--配置L2，L2返回失败 */
    RRC_APP_REEST_SECU_ERR                      = 60, /* 重建立失败--安全上下文失败 */

    RRC_APP_QLOW_WARN                           = 70,/* 长时间驻留到弱小区报警----服务可能不可用 */
    RRC_APP_PINGPONG_WARN                       = 71,/* 乒乓重选告警----可能导致被叫不通 */
    RRC_APP_CSFB_FAIL_ERR                       = 72,/* CSFB失败 --- 可能导致主叫或被叫不通 */
    RRC_APP_MOBILITY_2_QLOW_WARN                = 73,/* 重选重定向到弱小区----服务可能不可用 */
    RRC_APP_OUT_SYNC_ERR                        = 74,/* 失步错误 */
    RRC_APP_SRCHED_FIRST_FREQ_INFO              = 75,/* 搜网搜到第一个MCC时的频点信息上报 major */
    RRC_APP_SRCHED_SUCC_FREQ_INFO               = 76,/* 搜网成功时频点信息上报 major */
    RRC_APP_SERVICE_RECOVER_WARN                = 77,/* 出服务区后重新恢复 */

    RRC_APP_MAC_RA_MAC_ERR                      = 78,/* 随机接入失败,Mac上报Err_Ind */
    RRC_APP_MAC_RA_CNF_FAIL                     = 79,/* 随机接入失败,随机接入请求，MAC回复CNF指示失败 */
    RRC_APP_MAC_RA_CNF_TIME_OUT                 = 80,/* 随机接入失败,随机接入请求，等待Mac回复Cnf超时 */

    RRC_APP_AIRMSG_DECODE_FAIL                  = 81,/* 空口消息ASN解码失败 */
    RRC_APP_AIRMSG_CHECK_FAIL                   = 82,/* 空口消息检查失败 */
    RRC_APP_SRVCC_FAIL_REEST                    = 83,/* SRVCC 失败 -- HO失败等重建立触发 */
    RRC_APP_SRVCC_FAIL_RF_FAIL                  = 84,/* SRVCC 失败 -- RF失步重建立触发 */
    RRC_APP_SRVCC_FAIL_CONN_REL                 = 85,/* SRVCC 失败 -- 异常释放触发 */
    RRC_APP_SRVCC_FAIL_IRAT_FAIL                = 86,/* SRVCC 失败 -- 异系统切换失败触发 */
    RRC_APP_SRVCC_STAT                          = 87,/* SRVCC 统计信息 */

    RRC_APP_LONG_TIME_INTER_MEAS                = 88,/* 长时间启动异频异系统测量 */
    RRC_APP_TLPS_SECU_FUNCTION_INFO_ERR         = 89,/* 安全函数执行异常 */
    RRC_APP_MOD3_NCELL_IDLE_WARN                = 90,/* Idle态触发的模3相等强干扰邻区告警 */
    RRC_APP_MOD3_NCELL_CONN_WARN                = 91,/* Conn态触发的模3相等强干扰邻区告警 */

    RRC_APP_REEST_CAUSE_MAC_ERR                 = 92,/* Mac上报err导致的无线链路失败重建立 */
    RRC_APP_REEST_CAUSE_RLC_ERR                 = 93,/* Rlc上报err导致的无线链路失败重建立 */
    RRC_APP_REEST_CAUSE_OUT_OF_SYNC             = 94,/* 物理层上报失步触发的重建立 */

    RRC_APP_U2L_RESEL_FAIL                      = 95,/* TW到L重选失败 */
    RRC_APP_G2L_RESEL_FAIL                      = 96,/* G到L重选失败 */

    /* BAND-38 Scell Not config MFBI record begin */
    RRC_APP_BAND38_NCONFIG                      = 97,/* Band38小区未配置MFBI统计 */
    /* BAND-38 Scell Not config MFBI record end */

    RRC_APP_ERR_CODE_BUTT
};
typedef VOS_UINT16 RRC_APP_ERROR_CODE_ENUM_UINT16;

/*****************************************************************************
 枚举名    : LRRC_LNAS_EST_CAUSE_ENUM
 协议表格  :
 ASN.1描述 :
 枚举说明  : RRMM_EST_REQ_STRU消息中的enEstCause
              emergency, highPriorityAccess, mt-Access, mo-Signalling, mo-Data,delayTolerantAccess-v1020 ,
*****************************************************************************/
enum LRRC_APP_LNAS_EST_CAUSE_ENUM
{
    LRRC_APP_LNAS_EST_CAUSE_EMERGENCY_CALL          = 0,
    LRRC_APP_LNAS_EST_CAUSE_HIGH_PRIORITY_ACCESS,
    LRRC_APP_LNAS_EST_CAUSE_MT_ACCESS,
    LRRC_APP_LNAS_EST_CAUSE_MO_SIGNALLING,
    LRRC_APP_LNAS_EST_CAUSE_MO_DATA,
    LRRC_APP_LNAS_EST_CAUSE_BUTT
};
typedef VOS_UINT8 LRRC_APP_LNAS_EST_CAUSE_ENUM_UINT8;

/*****************************************************************************
 枚举名    : LRRC_LNAS_EST_RESULT_ENUM
 协议表格  :
 ASN.1描述 :
 枚举说明  : 信令连接结果枚举类型
*****************************************************************************/
enum LRRC_APP_LNAS_EST_RESULT_ENUM
{
    LRRC_APP_EST_SUCCESS                         = 0,       /* 信令连接建立成功 */
    LRRC_APP_EST_PRESENT                         = 1,       /* 该信令连接已经存在 */
    LRRC_APP_EST_ESTING                          = 2,       /* 该信令连接正在建立 */
    LRRC_APP_EST_DELING                          = 3,       /* 该信令连接正在释放 */
    LRRC_APP_EST_ACCESS_BARRED_MO_CALL           = 4,       /* access barred when MO Call */
    LRRC_APP_EST_ACCESS_BARRED_MO_SIGNAL         = 5,       /* access barred When MO SIGNAL */
    LRRC_APP_EST_ACCESS_BARRED_EMERGENCY_CALL    = 6,       /* access barred When Emergency Call */
    LRRC_APP_EST_ACCESS_BARRED_ALL               = 7,       /* access barred When Rrc Con Rej */
    LRRC_APP_EST_EST_CONN_FAIL                   = 8,       /* RR connection establish failure  */
    LRRC_APP_EST_RJ_INTER_RAT                    = 9,       /* RR connection Reject: 指示进入异系统 */
    LRRC_APP_EST_IMMEDIATE_ASSIGN_REJECT         = 10,      /* 正常的接入过程，发送RA脉冲后，收到立即指派拒绝消息，
                                                                没有收到立即指派消息而导致的接入失败 */
    LRRC_APP_EST_RANDOM_ACCESS_REJECT            = 11,      /* 正常的接入过程，发送RA脉冲后，没有收到网络侧的立即指派，
                                                                也没有收到立即指派拒绝，接入失败 */
    LRRC_APP_EST_CELL_SEARCHING                  = 12,      /* RRC目前不能处理建链请求，NAS收到该原因值，
                                                                直到收到RRC_MM_SYS_INFO_IND再发起建链*/
    LRRC_APP_EST_RESULT_BUTT
};
typedef VOS_UINT8 LRRC_APP_LNAS_EST_RESULT_ENUM_UINT8;

/*****************************************************************************
 枚举名    : LRRC_FLOW_CTRL_TYPE_ENUM
 协议表格  :
 ASN.1描述 :
 枚举说明  : 流程控制类型枚举
             修改枚举时需要注意，失败类型取值 = 启动类型取值+1
 比如: LRRC_FLOW_CTRL_TYPE_G2L_RESEL_FAIL = LRRC_FLOW_CTRL_TYPE_G2L_START_RESEL + 1
*****************************************************************************/
enum LRRC_APP_FLOW_CTRL_TYPE_ENUM
{
    LRRC_APP_FLOW_CTRL_TYPE_L2L_NORMAL            = 0,     /* LTE内流程 */
    LRRC_APP_FLOW_CTRL_TYPE_G2L_START_RESEL,               /* G->L开始重选 */
    LRRC_APP_FLOW_CTRL_TYPE_G2L_RESEL_FAIL,                /* G->L重选失败 */
    LRRC_APP_FLOW_CTRL_TYPE_G2L_STOP_RESEL,                /* G->L停止重选 */
    LRRC_APP_FLOW_CTRL_TYPE_G2L_START_REDIR,               /* G->L开始重定向 */
    LRRC_APP_FLOW_CTRL_TYPE_G2L_REDIR_FAIL,                /* G->L重定向失败 */
    LRRC_APP_FLOW_CTRL_TYPE_G2L_STOP_REDIR,                /* G->L停止重定向 */
    LRRC_APP_FLOW_CTRL_TYPE_G2L_START_CCO,                 /* G->L开始CCO */
    LRRC_APP_FLOW_CTRL_TYPE_G2L_CCO_FAIL,                  /* G->L CCO失败 */
    LRRC_APP_FLOW_CTRL_TYPE_G2L_STOP_CCO,                  /* G->L停止CCO */
    LRRC_APP_FLOW_CTRL_TYPE_G2L_START_HO,                  /* G->L开始HO */
    LRRC_APP_FLOW_CTRL_TYPE_G2L_HO_FAIL,                   /* G->L HO失败 */
    LRRC_APP_FLOW_CTRL_TYPE_G2L_STOP_HO,                   /* G->L停止HO */
    LRRC_APP_FLOW_CTRL_TYPE_W2L_START_RESEL,               /* W->L开始重选 */
    LRRC_APP_FLOW_CTRL_TYPE_W2L_RESEL_FAIL,                /* W->L重选失败 */
    LRRC_APP_FLOW_CTRL_TYPE_W2L_STOP_RESEL,                /* W->L停止重选 */
    LRRC_APP_FLOW_CTRL_TYPE_W2L_START_REDIR,               /* W->L开始重定向 */
    LRRC_APP_FLOW_CTRL_TYPE_W2L_REDIR_FAIL,                /* W->L重定向失败 */
    LRRC_APP_FLOW_CTRL_TYPE_W2L_STOP_REDIR,                /* W->L停止重定向 */
    LRRC_APP_FLOW_CTRL_TYPE_W2L_START_HO,                  /* W->L开始HO */
    LRRC_APP_FLOW_CTRL_TYPE_W2L_HO_FAIL,                   /* W->L HO失败 */
    LRRC_APP_FLOW_CTRL_TYPE_W2L_STOP_HO,                   /* W->L停止HO */
    LRRC_APP_FLOW_CTRL_TYPE_W2L_START_PLMN,                /* W->L开始PLMN搜索 */
    LRRC_APP_FLOW_CTRL_TYPE_W2L_DONE_PLMN,                 /* W->L PLMN搜索完成 */
    LRRC_APP_FLOW_CTRL_TYPE_W2L_STOP_PLMN,                 /* W->L停止PLMN搜索 */
    LRRC_APP_FLOW_CTRL_TYPE_L2G_START_RESEL,               /* L->G开始重选 */
    LRRC_APP_FLOW_CTRL_TYPE_L2G_RESEL_FAIL,                /* L->G重选失败 */
    LRRC_APP_FLOW_CTRL_TYPE_L2G_START_REDIR,               /* L->G重定向 */
    LRRC_APP_FLOW_CTRL_TYPE_L2G_REDIR_FAIL,                /* L->G重定向失败 */
    LRRC_APP_FLOW_CTRL_TYPE_L2G_START_CCO,                 /* L->G开始CCO */
    LRRC_APP_FLOW_CTRL_TYPE_L2G_CCO_FAIL,                  /* L->G CCO失败 */
    LRRC_APP_FLOW_CTRL_TYPE_L2G_START_HO,                  /* L->G开始切换 */
    LRRC_APP_FLOW_CTRL_TYPE_L2G_HO_FAIL,                   /* L->G切换失败 */
    LRRC_APP_FLOW_CTRL_TYPE_L2W_START_RESEL,               /* L->W开始重选 */
    LRRC_APP_FLOW_CTRL_TYPE_L2W_RESEL_FAIL,                /* L->W重选失败 */
    LRRC_APP_FLOW_CTRL_TYPE_L2W_START_REDIR,               /* L->W重定向 */
    LRRC_APP_FLOW_CTRL_TYPE_L2W_REDIR_FAIL,                /* L->W重定向失败 */
    LRRC_APP_FLOW_CTRL_TYPE_L2W_START_HO,                  /* L->W开始切换 */
    LRRC_APP_FLOW_CTRL_TYPE_L2W_HO_FAIL,                   /* L->W切换失败 */
    LRRC_APP_FLOW_CTRL_TYPE_G2L_IDLE_MEAS,                 /* G->L IDLE测量配置 */
    LRRC_APP_FLOW_CTRL_TYPE_W2L_IDLE_MEAS,                 /* W->L IDLE测量配置 */
    LRRC_APP_FLOW_CTRL_TYPE_G2L_CONN_MEAS,                 /* G->L CONN测量配置 */
    LRRC_APP_FLOW_CTRL_TYPE_W2L_CONN_MEAS,                 /* W->L CONN测量配置 */
    LRRC_APP_FLOW_CTRL_TYPE_W2L_RELALL,                    /* W->L RELALL */
    LRRC_APP_FLOW_CTRL_TYPE_L2W_START_CELL_SRCH,           /* L->W 搜网 */
    LRRC_APP_FLOW_CTRL_TYPE_L2W_STOP_CELL_SRCH,            /* L->W 停止搜网 */
    LRRC_APP_FLOW_CTRL_TYPE_L2G_START_CELL_SRCH,           /* L->G 搜网 */
    LRRC_APP_FLOW_CTRL_TYPE_L2G_STOP_CELL_SRCH,            /* L->G 停止搜网 */
    LRRC_APP_FLOW_CTRL_TYPE_T2L_RELALL,                    /* T->L RELALL */
    LRRC_APP_FLOW_CTRL_TYPE_G2L_RELALL,                    /* G->L RELALL */
    LRRC_APP_FLOW_CTRL_TYPE_L2T_START_REDIR,               /* L->T重定向 */
    LRRC_APP_FLOW_CTRL_TYPE_L2T_START_HO,                  /* L->T开始切换 */
    LRRC_APP_FLOW_CTRL_TYPE_L2T_START_RESEL,               /* L->T开始重选 */
    LRRC_APP_FLOW_CTRL_TYPE_T2L_IDLE_MEAS,                 /* T->L IDLE测量配置 */
    LRRC_APP_FLOW_CTRL_TYPE_T2L_CONN_MEAS,                 /* G->L CONN测量配置 */

    LRRC_APP_FLOW_CTRL_TYPE_BUTT
};
typedef VOS_UINT8 LRRC_APP_FLOW_CTRL_TYPE_ENUM_UINT8;

/*****************************************************************************
 枚举名    : RRC_APP_BOOL_ENUM
 协议表格  :
 ASN.1描述 :
 枚举说明  : PS统一布尔类型枚举定义
*****************************************************************************/
enum RRC_APP_BOOL_ENUM
{
    RRC_APP_PS_FALSE                            = 0,
    RRC_APP_PS_TRUE                             = 1,
    RRC_APP_PS_BOOL_BUTT
};
typedef VOS_UINT8   RRC_APP_BOOL_ENUM_UINT8;

/*****************************************************************************
 枚举名    : RRC_UE_CATEGORY_ENUM_UINT8
 协议表格  :
 ASN.1描述 :
 枚举说明  : UE Category，请参见36306协议
*****************************************************************************/
enum RRC_APP_CATEGORY_ENUM
{
    RRC_APP_UE_CATEGORY_1                   = 0,
    RRC_APP_UE_CATEGORY_2                   = 1,
    RRC_APP_UE_CATEGORY_3                   = 2,
    RRC_APP_UE_CATEGORY_4                   = 3,
    RRC_APP_UE_CATEGORY_5                   = 4,
    RRC_APP_UE_CATEGORY_6                   = 5,
    RRC_APP_UE_CATEGORY_7                   = 6,
    RRC_APP_UE_CATEGORY_BUTT
};
typedef VOS_UINT8 RRC_APP_CATEGORY_ENUM_UINT8;
/*****************************************************************************
 枚举名    : RRC_CONN_REEST_IND_ENUM_UINT8
 协议表格  :
 ASN.1描述 :
 枚举说明  : RRC连接重建发起的原因
*****************************************************************************/
enum RRC_APP_CONN_REEST_IND_ENUM
{
    RRC_APP_CONN_REEST_FOR_RBCFG_ERROR      = 1,                                    /* 0:由于RB配置失败导致的RRC连接重建 */
    RRC_APP_CONN_REEST_FOR_HANDOVER_ERROR   = 2,                                    /* 1:由于切换失败导致的RRC连接重建 */
    RRC_APP_CONN_REEST_FOR_OTHER_ERROR      = 3,                                    /* 2:由于无线链路失败或者完整性验证失败导致的RRC连接重建 */
    RRC_APP_CONN_REEST_FOR_BUTT
};
typedef VOS_UINT8 RRC_APP_CONN_REEST_IND_ENUM_UINT8;

/*********************************************************
 枚举名    : RRC_APP_FTM_CHANGE_INFO_TYPE_ENUM
 协议表格  :
 ASN.1描述 :
 枚举说明  : 变更上报工程模式类型定义
**********************************************************/
enum RRC_APP_FTM_CHANGE_INFO_TYPE_ENUM
{
    RRC_APP_FTM_CHANGE_CELL_INFO,  /* RRC驻留小区信息，变化后上报，切换或重选完成后上报 */
    RRC_APP_FTM_CHANGE_STATE_INFO, /* RRC状态信息，变化后上报 */
    RRC_APP_FTM_CHANGE_BUTT
};

typedef VOS_UINT32 RRC_APP_FTM_CHANGE_INFO_TYPE_ENUM_UINT32;
/*********************************************************
 枚举名    : LRRC_LPHY_LTE_BAND_WIDTH_ENUM
 协议表格  :
 ASN.1描述 :
 枚举说明  : LTE带宽，WIDTH_3M就是表示下行带宽是3M，界面显示3M即可
**********************************************************/
enum LRRC_APP_BAND_WIDTH_ENUM
{
    LRRC_APP_LTE_BAND_WIDTH_1D4M = 0,
    LRRC_APP_LTE_BAND_WIDTH_3M,
    LRRC_APP_LTE_BAND_WIDTH_5M,
    LRRC_APP_LTE_BAND_WIDTH_10M,
    LRRC_APP_LTE_BAND_WIDTH_15M,
    LRRC_APP_LTE_BAND_WIDTH_20M,
    LRRC_APP_LTE_BAND_WIDTH_BUTT
};
typedef VOS_UINT16 LRRC_APP_BAND_WIDTH_ENUM_UINT16;

enum LRRC_OM_QLOW_WATCH_TYPE_ENUM
{
    LRRC_OM_QLOW_WATCH_NONE = 0,
    LRRC_OM_QLOW_WATCH_L2L_REDIR,
    LRRC_OM_QLOW_WATCH_L2L_RESEL,
    LRRC_OM_QLOW_WATCH_U2L_REDIR,
    LRRC_OM_QLOW_WATCH_U2L_RESEL,
    LRRC_OM_QLOW_WATCH_G2L_REDIR,
    LRRC_OM_QLOW_WATCH_G2L_RESEL,
    LRRC_OM_QLOW_WATCH_C2L_RESEL,
    LRRC_OM_QLOW_WATCH_BUTT
};
typedef VOS_UINT8 LRRC_OM_QLOW_FLOW_TYPE_ENUM_UINT8;

/*****************************************************************************
 结构名    :LRRC_ERRLOG_ESR_CS_FAIL_TYPE_ENUM32
 结构说明  :
*****************************************************************************/
enum LRRC_OM_RAT_TYPE_ENUM
{
    LRRC_OM_RAT_TYPE_GSM = 1,                         /* GSM接入技术 */
    LRRC_OM_RAT_TYPE_UTRA,                            /* UTRA接入技术 */
    LRRC_OM_RAT_TYPE_LTE,                             /* LTE接入技术 */
    LRRC_OM_RAT_TYPE_HRPD,                            /* CDMA HRPD*/
    LRRC_OM_RAT_TYPE_BUTT
};
typedef VOS_UINT16  LRRC_OM_RAT_TYPE_ENUM_UINT16;

/*****************************************************************************
 结构名    :LRRC_CHR_MEAS_STATE_AND_CNF_ENUM_UINT16
 结构说明  :用于解析LRRC_OM_NCELL_INFO中的usMeasCnfAndState
*****************************************************************************/
enum  LRRC_CHR_MEAS_STATE_AND_CNF_ENUM
{
    LRRC_CHR_MEAS_STATE_BitOpIntraReselCfg = 0x8000,
    LRRC_CHR_MEAS_STATE_bitOpInterMeasCfg = 0x4000,
    LRRC_CHR_MEAS_STATE_bitOp2GMeasCfg = 0x2000,
    LRRC_CHR_MEAS_STATE_bitOp3GMeasCfg = 0x1000,
    LRRC_CHR_MEAS_STATE_bitOpIntraMeasState = 0x0800,
    LRRC_CHR_MEAS_STATE_bitOpInterMeasState = 0x0400,
    LRRC_CHR_MEAS_STATE_bitOp2GMeasState = 0x0200,
    LRRC_CHR_MEAS_STATE_bitOp3GMeasState = 0x0100,
    LRRC_CHR_MEAS_STATE_bitOpValidIntraCell = 0x0080,
    LRRC_CHR_MEAS_STATE_bitOpValidInterCell = 0x0040,
    LRRC_CHR_MEAS_STATE_bitOpValid2GCell = 0x0020,
    LRRC_CHR_MEAS_STATE_bitOpValid3GCell = 0x0010,
    LRRC_CHR_MEAS_STATE_BUTT = 0x0000
};
typedef VOS_UINT16  LRRC_CHR_MEAS_STATE_AND_CNF_ENUM_UINT16;

/*****************************************************************************
   5 STRUCT
*****************************************************************************/
/*****************************************************************************
                  工程模式上报结构体定义
*****************************************************************************/
/*********************************************************************
* ASN 文本 :MCC
*********************************************************************/
typedef struct
{
    VOS_UINT8                           ucCnt;    /*(0, 3)*/
    VOS_UINT8                           aucItem[RRC_APP_MAX_NUM_OF_MCC];
}RRC_APP_MCC_STRU;

/*********************************************************************
* ASN 文本 :PLMN-Identity
*********************************************************************/
typedef struct
{
    RRC_APP_MCC_STRU                         Mcc;                          /* optional */
    RRC_APP_MCC_STRU                         Mnc;
}LRRC_APP_PLMN_ID_STRU;
/*****************************************************************************
 结构名    :APP_RRC_Txpwr
 结构说明  :RRC上报给APP的上行发射功率
*****************************************************************************/
typedef struct
{
    VOS_INT16                        sPuschTxPwr;
    VOS_INT16                        sPucchTxPwr;
    VOS_INT16                        sSrsTxpwr;
    VOS_UINT16                       usReserved;
}RRC_APP_Txpwr_INFO_STRU;
/*****************************************************************************
 结构名    :RRC_APP_PUSCH_BLER_STRU
 结构说明  :RRC上报给APP的PUSCH误码率结构
*****************************************************************************/
typedef struct
{
    VOS_UINT32             ulAckCnt;    /* 收到的ACK个数 */
    VOS_UINT32             ulNackCnt;   /* 收到的NACK个数 */
}RRC_APP_PUSCH_BLER_STRU;

/*****************************************************************************
 结构名    :RRC_APP_PDSCH_BLER_STRU
 结构说明  :RRC上报给APP的PDSCH误码率结构，需要显示初始误码率和累计误码率，需要分码字显示
*****************************************************************************/
typedef struct
{
    VOS_UINT16              ausCWInitTotal[NUM_OF_CODEWORD]; /* 初始误码率，收到的子帧个数，包括错误和正确的 */
    VOS_UINT16              ausCWInitError[NUM_OF_CODEWORD]; /* 初始误码率，收到的错误子帧个数 */
    VOS_UINT16              ausCWReTotal[NUM_OF_CODEWORD];   /* 残留误码率，收到的子帧个数，包括错误和正确的 */
    VOS_UINT16              ausCWReError[NUM_OF_CODEWORD];   /* 残留误码率，收到的错误子帧个数 */
}RRC_APP_PDSCH_BLER_STRU;

/*****************************************************************************
 结构名    :RRC_APP_PDCCH_BLER_STRU
 结构说明  :RRC上报给APP的上下行授权个数
*****************************************************************************/
typedef struct
{
    VOS_UINT32              ulDlGrantCnt; /* 下行授权个数 */
    VOS_UINT32              ulUlGrantCnt; /* 上行授权个数 */
}RRC_APP_PDCCH_BLER_STRU;

/*****************************************************************************
 结构名    :APP_RRC_BLER_INFO_STRU
 结构说明  :RRC上报给APP的误码率结构
*****************************************************************************/
typedef struct
{
    RRC_APP_PUSCH_BLER_STRU         stPuschBlerCnt; /* PUSCH 误块率BLER */
    RRC_APP_PDSCH_BLER_STRU         stPdschBlerCnt; /* PDSCH 误块率BLER */
    RRC_APP_PDCCH_BLER_STRU         stPdcchBlerCnt; /* PDCCH 上下行授权个数 */
}RRC_APP_BLER_INFO_STRU;

/*****************************************************************************
 结构名    :RRC_APP_PERIOD_CELL_INFO_STRU
 结构说明  :RRC周期上报给APP的驻留小区信息
*****************************************************************************/
typedef struct
{
    VOS_INT16                           sRsrp;              /* RSRP测量值范围：(-141,-44)，单位dbm*/
    VOS_INT16                           sRsrq;              /* RSRQ测量值范围：(-40, -6) */
    VOS_INT16                           sRssi;              /* RSSI测量值，单位dbm */
    VOS_UINT16                          usRI;               /* RI值 */
    VOS_UINT16                          ausCQI[2];          /* CQI两个码字，只有一个码字时，数组第一个值有效 */
    VOS_INT32                           lSINR;              /* SINR，小区参考信号信噪比 */
    RRC_APP_Txpwr_INFO_STRU             stTxPwr;            /* LTE上行信道发送功率 */
    RRC_APP_BLER_INFO_STRU              stBler;             /* LTE误码率 */
}RRC_APP_PERIOD_CELL_INFO_STRU;
/*****************************************************************************
 结构名    : RRC_APP_FREQ_PARA_STRU
结构说明  : RRC上报的消息
*****************************************************************************/
typedef struct
{
    VOS_UINT16                              usUlFreq;          /* 上行中心频点 单位:100Khz */
    VOS_UINT16                              usDlFreq;          /* 下行中心频点 单位:100Khz */
    LRRC_APP_BAND_WIDTH_ENUM_UINT16         enDlBandwidth;     /* 下行带宽 */
    VOS_UINT8                               usBand;            /* 频带指示 */
    VOS_UINT8                               aucResv[1];
} RRC_APP_FREQ_PARA_STRU;

/*****************************************************************************
 结构名    : RRC_APP_CHANGE_CELL_INFO_STRU
 结构说明  : 变化上报的工程模式结构--驻留小区信息
*****************************************************************************/
typedef struct
{
    VOS_UINT32                                ulValidNum;                      /* LTE支持CA情况下，有效载频个数 */
    RRC_APP_FREQ_PARA_STRU                    astFreqInfo[LRRC_APP_MAX_NUM];   /* LTE支持CA情况下，上下行频点信息 */
    VOS_UINT16                                astPhyCellId[LRRC_APP_MAX_NUM];  /* LTE支持CA情况下，物理小区ID信息 */
    VOS_UINT8                                 ucSbuFrmAssignmentType;          /* TDD模式下有效，LTE子帧配比，0xFF为无效值 */
    VOS_UINT8                                 ucSpecialSubFrmPatterns;         /* TDD模式下有效，LTE特殊子帧配比，0xFF为无效值 */
    RRC_APP_PAGING_CYCLE_ENUM_UINT32          enPagingCycle;                   /* 寻呼周期 */
    RRC_APP_TRANSM_MODE_ENUM_UINT32           enTranMode;                      /* LTE传输模式 */
}RRC_APP_CHANGE_CELL_INFO_STRU;

/*****************************************************************************
 结构名    : RRC_APP_FTM_CHANGE_INFO_STRU
 结构说明  : 变化上报的工程模式结构
*****************************************************************************/
typedef struct
{
    RRC_APP_FTM_CHANGE_INFO_TYPE_ENUM_UINT32  ulFtmChangeInfoType;

    union
    {
        RRC_APP_CHANGE_CELL_INFO_STRU             stCellInfo;  /* 变化上报的工程模式结构--驻留小区信息 */
        RRC_APP_PROTOCOL_STATE_IND_ENUM_UINT8     enState;     /* 变化上报的工程模式结构--状态信息，包括协议状态和内部代码状态 */
    }u;
}RRC_APP_FTM_CHANGE_INFO_STRU;
/*****************************************************************************
 结构名    : RRC_APP_FTM_PERIOD_INFO_STRU
 结构说明  : 周期上报的工程模式结构
*****************************************************************************/
typedef struct
{
    VOS_UINT32                                 ulValidNum;
    RRC_APP_PERIOD_CELL_INFO_STRU              astCellInfo[LRRC_APP_MAX_NUM]; /* RRC周期上报给APP的驻留小区信息 */
}RRC_APP_FTM_PERIOD_INFO_STRU;

/*****************************************************************************
 结构名    : RRC_APP_FTM_INFO_STRU
 结构说明  : 工程模式上报数据结构
*****************************************************************************/
typedef struct
{
    RRC_APP_FTM_TYPE_ENUM_UINT32             enRrcAppFtmType; /* 工程模式下，上报信息类型--周期上报和变更上报类型 */

    union
    {
        RRC_APP_FTM_PERIOD_INFO_STRU         stPeriodInfo;    /* 工程模式下，周期上报内容， Idle状态上报周期目前为DRX周期，Connected状态下周期为1秒*/
        RRC_APP_FTM_CHANGE_INFO_STRU         stChangeInfo;    /* 工程模式下，变更后上报内容 */
    }u;
}RRC_APP_FTM_INFO_STRU;

/*****************************************************************************
 结构名    : RRC_APP_FTM_INFO_STRU
 结构说明  : 工程模式上报数据结构
*****************************************************************************/
typedef struct
{
    /* 子模块ID */
    OM_ERR_LOG_MOUDLE_ID_ENUM_UINT32    ulMsgModuleID;

    /* 00：主卡, 01：副卡 ,10/11:保留  */
    VOS_UINT16                          usModemId;

    VOS_UINT16                          usProjectID;
    VOS_UINT32                          ulLength;

    RRC_APP_FTM_INFO_STRU               stLrrcFtmInfo;

}LRRC_OM_FTM_REPROT_CONTENT_STRU;

/*****************************************************************************
 结构名    : RRC_APP_FTM_INFO_STRU
 结构说明  : 工程模式上报数据结构
*****************************************************************************/

/* 各组件工程模式主动上报内容 各组件 -> OM*/
typedef struct
{
    VOS_MSG_HEADER
    VOS_UINT32                          ulMsgName;
    VOS_UINT32                          ulMsgType;
    VOS_UINT32                          ulMsgSN;
    VOS_UINT32                          ulRptlen;      /* 工程模式上报的内容长度,如果ulRptlen为0,aucContent内容长度也为0 */
    LRRC_OM_FTM_REPROT_CONTENT_STRU     stLrrcFtmContent;
}LRRC_OM_FTM_REPROT_IND_STRU;

/*****************************************************************************
                   ERROR LOG上报结构体定义
*****************************************************************************/
/*****************************************************************************
 枚举名    : LRRC_OM_ALARMID_ENUM
 协议表格  :
 ASN.1描述 :
 枚举说明  : ErrorLog Cnf消息结构体的AlarmId枚举
*****************************************************************************/
enum LRRC_OM_ALARMID_ENUM
{
    LRRC_OM_ALARMID_ENUM_RECFG          = 1,                /*重配置错误*/
    LRRC_OM_ALARMID_ENUM_REEST          = 2,                /*重建立错误*/
    LRRC_OM_ALARMID_ENUM_EST            = 3,                /*建链错误*/
    LRRC_OM_ALARMID_ENUM_LOSTAREA       = 4,                /*丢网*/
    LRRC_OM_ALARMID_ENUM_OVERFLOW       = 5,                /*消息缓冲区溢出*/
    LRRC_OM_ALARMID_ENUM_QLOW_CELL      = 6,                /*驻留在弱小区告警*/
    LRRC_OM_ALARMID_ENUM_PINGPONG_RESEL = 7,                /*乒乓重选告警*/
    LRRC_OM_ALARMID_ENUM_CSFB_FAIL      = 8,                /*CSFB失败*/
    LRRC_OM_ALARMID_ENUM_MOBILITY_2_QLOW_CELL = 9,          /*重选、重定向到弱小区告警*/
    LRRC_OM_ALARMID_ENUM_OUT_OF_SYNC    = 10,               /*失步报警*/
    LRRC_OM_ALARMID_ENUM_CSEL_INFO      = 11,               /* 小区选择信息 */
    LRRC_OM_ALARMID_ENUM_ISLAND_CELL    = 12,               /* 孤岛小区 */
    LRRC_OM_ALARMID_ENUM_RA             = 13,               /* 随机接入 */
    LRRC_OM_ALARMID_ENUM_AIRMSG_FAIL    = 14,               /* 空口消息异常 */
    LRRC_OM_ALARMID_ENUM_SRVCC_FAIL     = 15,               /* SRVCC 失败 */
    LRRC_OM_ALARMID_ENUM_SRVCC_STAT     = 16,               /* SRVCC 统计信息 */
    LRRC_OM_ALARMID_ENUM_LONGTIMEMEAS_STAT = 17,            /* 长时间启动测量统计信息 */
    LRRC_OM_ALARMID_ENUM_SECU_FUNCTION_ERR_INFO = 18,            /*安全函数运行错误*/
    LRRC_OM_ALARMID_ENUM_MOD3_NCELL     = 19,               /* 模三相等同频强干扰邻区 */
    LRRC_OM_ALARMID_ENUM_RFFAIL_REEST   = 20,               /* UE发送了测量报告，但是并未触发切换引起的重建立 */

    LRRC_OM_ALARMID_ENUM_RESEL_TO_LTE_FAIL = 21,            /*其他模重选到L模失败*/

    /* BAND-38 Scell Not config MFBI record begin */
    LRRC_OM_ALARMID_ENUM_NCONFIG_MFBI_STAT = 22,            /* Band38 Scell未配置MFBI信息统计 */
    /* BAND-38 Scell Not config MFBI record end */

    LRRC_OM_ALARMID_ENUM_BUTT
};
typedef VOS_UINT16 LRRC_OM_ALARMID_ENUM_UINT16;

/*****************************************************************************
 枚举名    : LRRC_OM_ALARMID_ENUM
 协议表格  :
 ASN.1描述 :
 枚举说明  : ErrorLog Cnf消息结构体的AlarmId枚举
*****************************************************************************/
enum LRRC_OM_SRCHED_FREQ_SCENARIO_ENUM
{
    LRRC_OM_RREQ_SCENARIO_ENUM_FIRST_MCC = 1,
    LRRC_OM_RREQ_SCENARIO_ENUM_CSEL_SUCC,
    LRRC_OM_RREQ_SCENARIO_ENUM_BUTT
};
typedef VOS_UINT16 LRRC_OM_SRCHED_FREQ_SCENARIO_ENUM_UINT16;

/*****************************************************************************
 结构名    : LRRC_OM_ACC_INFO_STRU
 协议表格  :
 ASN.1描述 :
 结构说明  : RRC接入信息存储
*****************************************************************************/
typedef struct
{
    VOS_UINT8                              ucAsRelBitmap;      /* 用来设置UE接入层版本，1：支持，0：不支持，从低到高位分别用来指示Rel8,Rel9,.... */
    VOS_UINT8                              aucReserved[3];
    VOS_UINT32                             ulFeatureGroupInds; /* 请参见36.331 B.1  Feature group indicators 章节，按位取值 */
}LRRC_OM_ACC_INFO_STRU;

/*****************************************************************************
 结构名    : RRC_RRC_EST_INFO_STRU
 协议表格  :
 ASN.1描述 :
 结构说明  : RRC建链信息存储
*****************************************************************************/
typedef struct
{
    LRRC_APP_LNAS_EST_CAUSE_ENUM_UINT8     enEstCause;             /* 建链原因 */
    LRRC_APP_LNAS_EST_RESULT_ENUM_UINT8    enResult;               /* 建链结果 */
    VOS_UINT8                              ucNumOfPrembSentR11;
    VOS_UINT8                              ucContentionDetctR11;
    VOS_UINT8                              ucMaxTxPwrReachedR11;
    VOS_UINT8                              aucRsv[3];
}LRRC_OM_RRC_EST_LIST_INFO_STRU;

/*****************************************************************************
 结构名    : LRRC_OM_STATE_INFO_STRU
 结构说明  : ERROR LOG RRC状态信息
*****************************************************************************/
typedef struct
{
    RRC_APP_PROTOCOL_STATE_IND_ENUM_UINT8       enState;            /* RRC协议状态 */
    VOS_UINT8                                   ucErrcTopMsState;   /* RRC主状态机，内部代码的状态机 */
    VOS_UINT8                                   ucErrcTopSsState;   /* RRC子状态机，内部代码的状态机 */
    LRRC_APP_FLOW_CTRL_TYPE_ENUM_UINT8          enErrcFlowCtrlType; /* LTE发生异系统过程时，异系统过程标识 */
}LRRC_OM_STATE_INFO_STRU;

/*****************************************************************************
 结构名    : LRRC_OM_SYNC_INFO_STRU
 结构说明  : ERR LOG 同步信息
*****************************************************************************/
typedef struct
{
    VOS_UINT32          ulInSyncCnt;   /* LRRC收到LPHY的物理层同步个数 */
    VOS_UINT32          ulOutSyncCnt;  /* LRRC收到LPHY的物理层失步个数 */
}LRRC_OM_SYNC_INFO_STRU;

/*****************************************************************************
 枚举名    : LRRC_APP_ERR_LOG_RAT_TYPE_ENUM
 协议表格  :
 ASN.1描述 :
 枚举说明  : 接入技术枚举值，根据检视意见GUTL各自定义但保持各制式顺序一致
*****************************************************************************/
enum LRRC_APP_ERR_LOG_RAT_TYPE_ENUM
{
    LRRC_APP_ERR_LOG_RAT_TYPE_GSM      = 1,      /* GSM 接入技术*/
    LRRC_APP_ERR_LOG_RAT_TYPE_WCDMA,             /* WCDMA 接入技术*/
    LRRC_APP_ERR_LOG_RAT_TYPE_LTE,               /* LTE 接入技术*/
    LRRC_APP_ERR_LOG_RAT_TYPE_TDS,               /* TDS 接入技术*/
    LRRC_APP_ERR_LOG_RAT_TYPE_BUTT
};
typedef VOS_UINT8  LRRC_APP_ERR_LOG_RAT_TYPE_ENUM_UINT8;

/*****************************************************************************
 枚举名    : LRRC_APP_ERR_LOG_AREA_LOST_CAUSE_ENUM
 协议表格  :
 ASN.1描述 :
 枚举说明  : 丢网上报原因值
*****************************************************************************/
enum LRRC_APP_ERR_LOG_AREA_LOST_CAUSE_ENUM
{
    LRRC_APP_ERR_LOG_AREA_LOST_BUTT = 1         /* 当前预留该枚举值*/
};
typedef VOS_UINT8  LRRC_APP_ERR_LOG_AREA_LOST_CAUSE_ENUM_UINT8;

/*****************************************************************************
 结构名    : LRRC_LMAC_RA_TYPE_ENUM
 协议表格  :
 ASN.1描述 :
 结构说明  : RRC请求MAC的随机接入类型
*****************************************************************************/
enum LRRC_OM_RA_TYPE_ENUM
{
    LRRC_OM_RA_TYPE_CONTENTION              = 0,                                    /* 基于竞争的 */
    LRRC_OM_RA_TYPE_NON_CONTENTION          = 1,                                    /* 非竞争的 */
    LRRC_OM_RA_TYPE_BUTT
};
typedef VOS_UINT8 LRRC_OM_RA_TYPE_ENUM_UINT8;

/*****************************************************************************
 结构名    : LRRC_LMAC_RA_CAUSE_ENUM
 协议表格  :
 ASN.1描述 :
 结构说明  : RRC请求MAC随机接入的原因
*****************************************************************************/
enum LRRC_OM_RA_CAUSE_ENUM
{
    LRRC_OM_RA_CAUSE_CONN                   = 0,                                    /* 链接建立的 */
    LRRC_OM_RA_CAUSE_HO                     = 1,                                    /* 切换的 */
    LRRC_OM_RA_CAUSE_REEST                  = 2,                                    /* RRC链接重建立的 */
    LRRC_OM_RA_CAUSE_WAITTA_TIMEOUT         = 3,                                    /* 等待TA超时 */
    LRRC_OM_RA_CAUSE_SENDSR                 = 4,                                    /* 利用随机接入发送SR信息 */
    LRRC_OM_RA_CAUSE_BUTT
};
typedef VOS_UINT8 LRRC_OM_RA_CAUSE_ENUM_UINT8;

/*****************************************************************************
 结构名    : LRRC_OM_ERR_CAUSE_ENUM
 协议表格  :
 ASN.1描述 :
 结构说明  : 随机接入失败的原因值
*****************************************************************************/
enum LRRC_OM_ERR_CAUSE_ENUM
{
    LRRC_OM_ERR_FATAL                        = 0,                                   /*MAC自身发生不可恢复性错误*/
    LRRC_OM_RA_ERR_MAX_TX_NUM                = 1,                                   /*发送随机接入Preamble达到最大次数*/
    LRRC_OM_RA_ERR_MAX_HARQ_NUM              = 2,                                   /*随机接入过程发送消息3达到最大次数*/
    LRRC_OM_ERR_NO_RF_RA                     = 3,                                   /*随机接入时没有RF*/
    LRRC_OM_ERR_NO_RF_TIMEROUT               = 4,                                   /*RF保护定时器超时*/
    LRRC_OM_ERR_RF_RESUME_SYNC               = 5,                                   /*RF恢复后防止与网侧不同步*/
    LRRC_OM_ERR_BUTT
};
typedef VOS_UINT8 LRRC_OM_ERR_CAUSE_ENUM_UINT8;

/*****************************************************************************
 结构名    : LRRC_OM_SRVCC_RST_ENUM
 协议表格  :
 ASN.1描述 :
 结构说明  : SRVCC结果
*****************************************************************************/
enum LRRC_OM_SRVCC_RST_ENUM
{
    LRRC_OM_SRVCC_NO_B1B2                    = 0,                                   /* SRVCC 没有收到B1B2配置 */
    LRRC_OM_SRVCC_NO_RPT                     = 1,                                   /* SRVCC 没有发送B1B2报告 */
    LRRC_OM_SRVCC_NO_HO                      = 2,                                   /* SRVCC 网侧未配置切换 */
    LRRC_OM_SRVCC_HO_FAIL                    = 3,                                   /* SRVCC 切换失败 */
    LRRC_OM_SRVCC_CONN_REL                   = 4,                                   /* Volte 过程中，收到网络的异常连接释放 */
    LRRC_OM_SRVCC_REEST                      = 5,                                   /* Volte 过程中，由于安全完整性失败等其他原因发起了重建立 */
    LRRC_OM_SRVCC_SUCC                       = 6,                                   /* SRVCC 成功 */
    LRRC_OM_SRVCC_FAIL_BUTT
};
typedef VOS_UINT8 LRRC_OM_SRVCC_RST_UINT8;

/*****************************************************************************
 结构名    : LRRC_OM_MEAS_RPT_TYPE_ENUM
 协议表格  :
 ASN.1描述 :
 结构说明  : 测量报告的类型
*****************************************************************************/
enum LRRC_OM_MEAS_RPT_TYPE_ENUM
{
    LRRC_OM_MEAS_RPT_TYPE_EVENT_BEGIN = 0,
    LRRC_OM_MEAS_RPT_TYPE_EVENT_A1,
    LRRC_OM_MEAS_RPT_TYPE_EVENT_A2,
    LRRC_OM_MEAS_RPT_TYPE_EVENT_A3,
    LRRC_OM_MEAS_RPT_TYPE_EVENT_A4,
    LRRC_OM_MEAS_RPT_TYPE_EVENT_A5,
    LRRC_OM_MEAS_RPT_TYPE_EVENT_A6,
    LRRC_OM_MEAS_RPT_TYPE_EVENT_B1,
    LRRC_OM_MEAS_RPT_TYPE_EVENT_B2,

    LRRC_OM_MEAS_RPT_TYPE_PERIOD_BEGIN = 20,
    LRRC_OM_MEAS_RPT_TYPE_REPORT_STRONGESTCELLS,
    LRRC_OM_MEAS_RPT_TYPE_REPORT_STRONGESTCELLS_FOR_SON,
    LRRC_OM_MEAS_RPT_TYPE_REPORT_CGI,

    LRRC_OM_MEAS_RPT_TYPE_BUTT
};
typedef VOS_UINT8 LRRC_OM_MEAS_RPT_TYPE_ENUM_UINT8;

/*****************************************************************************
 结构名    : LL2_OM_VOLTE_AUTOAN_EVENT_ENUM_UINT8
 协议表格  :
 ASN.1描述 :
 结构说明  : L2自诊断的Volte异常事件
*****************************************************************************/
enum LL2_OM_VOLTE_AUTOAN_EVENT_ENUM
{
    LL2_OM_VOLTE_RTP_NO_ABORMAL = 0,                 /* 未见异常，L2 RTP SN连续无抖动 */
    LL2_OM_VOLTE_RTP_LOSTORJITTERLOW,            /* 少量丢包并且无大抖动，都不过门限 */
    LL2_OM_VOLTE_RTP_LOST_PDCP_NOLOST,               /* RTP SN不连续，PDCP连续 */
    LL2_OM_VOLTE_ROHC_FAIL_WITH_BLER,                /* 下行误码ROHC失败 */
    LL2_OM_VOLTE_ROHC_FAIL_NO_BLER,                  /* 无误码或少量误码ROHC失败 */
    LL2_OM_VOLTE_RTP_LOST_WITH_BLER,                 /* 下行误码引起的RTP丢包 */
    LL2_OM_VOLTE_PDCP_LOST_RLC_NOLOST,               /* PDCP不连续,RLC连续 */
    LL2_OM_VOLTE_RTP_BIG_JITTER,                     /* RTP 抖动 >=300 ms*/
    LL2_OM_VOLTE_NO_PACKET_HIGH_QULITY,              /* 语音承载无包，但其他承载有包，或者信号质量良好 */
    LL2_OM_VOLTE_NO_PACKET_LOW_QULITY,               /* 语音承载无包，高误码或者误码不高，但信号质量很差,可能DCI都没解到 */
    LL2_OM_VOLTE_LESS_PACKET_HIGH_QULITY,            /* 无丢包或很少丢包，但包少，信号质量良好场景 */
    LL2_OM_VOLTE_INTERNAL_ERROR                      /* 内部异常，比如查询时间段L2没有数据 */
};
typedef VOS_UINT8  LL2_OM_VOLTE_AUTOAN_EVENT_ENUM_UINT8;

/*****************************************************************************
 结构名    : LPS_LOW_QUILITY_ABNORMAL_STRU
 协议表格  :
 ASN.1描述 :
 结构说明  : 发生弱信号时，提取的异常点
*****************************************************************************/
typedef struct
{
    VOS_UINT8                      enMod3EqualCell;               /* 存在能量相等的模三干扰异常点，网优异常 */
    VOS_UINT8                      enNoIntraFreqHo;               /* A3上报成功，但没有同频切换下发 */
    VOS_UINT8                      enNoInterFreqHo;               /* A4A5上报成功，但没有异频频切换下发 */
    VOS_UINT8                      enNoInterRATHo;                /* B2上报成功，但没有异系统切换下发 */
    VOS_UINT8                      enNoNcellMeasRpt;              /* 没有邻区上报，可能是没测到或不满足条件 */
    VOS_UINT8                      enNoNcellMeasCfg;              /* 没有除A1A2测量配置异常点是否存在*/
    VOS_UINT8                      enMeasRptTryFail;              /* 尝试发送A3之后的测量报告但没有成功 */
    VOS_UINT8                      enUnknowInt;                   /* 信号能量高，没有同频干扰，但RSRQ异常偏低 */
    VOS_UINT8                      enRsv0;                        /* 预留 */
    VOS_UINT8                      enRsv1;                        /* 预留 */
    VOS_UINT8                      enRsv2;                        /* 预留 */
    VOS_UINT8                      enRsv3;                        /* 预留 */
}LPS_LOW_QUILITY_ABNORMAL_STRU;



/*****************************************************************************
 枚举名    : LRRC_LPDCP_VOLTE_FAULT_TRIG_ENUM
 协议表格  :
 ASN.1描述 :
 枚举说明  : PDCP上报的VOLTE_FAULT_IND的触发原因:
             0:查询触发;
             1:主动触发
*****************************************************************************/
enum LRRC_LPDCP_VOLTE_FAULT_TRIG_ENUM
{
    LRRC_LPDCP_VOLTE_FAULT_TRIG_BY_QUERY      = 0,                               /* 查询触发 */
    LRRC_LPDCP_VOLTE_FAULT_TRIG_AUTO,                                            /* 自主触发 */
    LRRC_LPDCP_VOLTE_FAULT_TRIG_BUTT
};
typedef VOS_UINT8 LRRC_LPDCP_VOLTE_FAULT_TRIG_ENUM_UINT8;

/*****************************************************************************
 枚举名    : SECU_FUNC_TYPE_ENUM
 协议表格  :
 ASN.1描述 :
 枚举说明  : 安全函数定义
*****************************************************************************/
enum SECU_FUNC_TYPE_ENUM
{
    MEM_CPY_TYPE  = 0,
    MEM_SET_TYPE     ,
    MEM_MOVE_TYPE    ,
    MEM_SECU_BUTT
};
typedef VOS_UINT8   SECU_FUNC_TYPE_ENUM_UINT8;

/*****************************************************************************
 结构名    : LRRC_OM_OVERFLOW_INFO_STRU
 结构说明  : ERR LOG 缓冲区溢出结构
*****************************************************************************/
typedef struct
{
    OM_ERR_LOG_HEADER_STRU                  stHead;
    VOS_UINT32                              ulCount;    /*缓冲区溢出次数*/
}LRRC_OM_OVERFLOW_INFO_STRU;

/*****************************************************************************
 结构名    : LRRC_OM_CAMP_THRESHOLD_STRU
 结构说明  : 小区重选的的阈值信息数据结构
*****************************************************************************/
typedef struct
{
    VOS_INT16 sQRxLevMin;                      /* 实际值，网络参数*2 */
    VOS_INT16 sQRxLevMinOffset;                /* 实际值，网络参数*2,默认0 */
    VOS_INT16 sQqualMin;                       /* 实际值，默认最小 RRC_INT16_NEGATIVE_INFINITY*/
    VOS_INT16 sQqualMinOffset;                 /* 实际值，默认0 */
}LRRC_OM_CAMP_THRESHOLD_STRU;

/*****************************************************************************
 结构名    : LRRC_OM_TAC_STRU
 结构说明  : TAC 信息
*****************************************************************************/
typedef struct
{
    VOS_UINT8                           ucTac;
    VOS_UINT8                           ucTacCont;
    VOS_UINT8                           aucReserved[2];
}LRRC_OM_TAC_STRU;

/*****************************************************************************
 结构名    : LRRC_OM_CELL_MEAS_INFO
 结构说明  : LTE服务小区测量结果
*****************************************************************************/
typedef struct
{
    VOS_INT16                                           sRsrp;                          /* RSRP值,实际值 */
    VOS_INT16                                           sRsrq;                          /* RSRq值，实际值 */
    VOS_INT16                                           sRssi;                          /* RSSI值，实际值 */
    VOS_INT16                                           sSValue;                        /* S Criteria Value */
    VOS_INT16                                           sRValue;                        /* R Criteria Value */
    VOS_INT16                                           sSqual;                         /* S Criteria RSRQ Value */
}LRRC_OM_CELL_MEAS_INFO;

/*****************************************************************************
 结构名    : LRRC_OM_SCELL_ID
 结构说明  : LTE服务小区识别信息，包括Plmn\Gci\Tac\PhyCellId\Arfcn\Band等
*****************************************************************************/
typedef struct
{
    VOS_UINT32                                          ulGci;                          /* sib1中的GlobalCellId */
    VOS_UINT32                                          ulMcc;                          /* Mcc */
    VOS_UINT32                                          ulMnc;                          /* Mnc */
    LRRC_OM_TAC_STRU                                    stTac;                          /* tac */
    VOS_UINT16                                          usPhyCellId;                    /* 物理小区id */
    VOS_UINT16                                          usUlEarfcn;                     /*上行频率，单位100khz*/
    VOS_UINT16                                          usDlEarfcn;                     /*下行频率，单位100khz*/
    VOS_UINT8                                           ucServerCellBandInd;            /* 所属band*/
    VOS_UINT8                                           ucRsv;
}LRRC_OM_SCELL_ID;

/*****************************************************************************
 结构名    : LRRC_OM_SCELL_ID
 结构说明  : LTE服务小区识别信息，包括Plmn\Gci\Tac\PhyCellId\Arfcn\Band等
*****************************************************************************/
typedef struct
{
    VOS_UINT8    aucPrint[LRRC_OM_SINGE_PRINT_LEN];
}LRRC_OM_SINGLE_PRINT_INFO;


/*****************************************************************************
 结构名    : LRRC_OM_SCELL_ID
 结构说明  : LTE服务小区识别信息，包括Plmn\Gci\Tac\PhyCellId\Arfcn\Band等
*****************************************************************************/
typedef struct
{
    LRRC_OM_SINGLE_PRINT_INFO  astLastPrint[LRRC_OM_PRINT_INFO_NUM];
}LRRC_OM_LATEST_PRINT_INFO;


/*****************************************************************************
 结构名    : LRRC_CHR_SCELL_INFO
 结构说明  : 包括小区，频点，能量，频带，门限，PLMNID等 数据结构
*****************************************************************************/
typedef struct
{
    LRRC_OM_SCELL_ID                                   stSCellIdInfo;                  /*服务小区Id信息*/
    LRRC_OM_CELL_MEAS_INFO                             stSCellMeasRst;                 /*小区测量结果*/
    LRRC_OM_CAMP_THRESHOLD_STRU                        stCampThreshold;                /*驻留门限*/
}LRRC_OM_SCELL_INFO;

/*****************************************************************************
 结构名    : LRRC_OM_AREA_LOST_INFO_STRU
 结构说明  : 丢网上报ERROR LOG包括小区，频点，能量，频带，门限，PLMNID等 数据结构
*****************************************************************************/
typedef struct
{
    VOS_UINT32                                          ulTimeStamp;                    /* 丢网发生的时刻 */
    VOS_UINT32                                          ulGci;                          /* sib1中的GlobalCellId */
    VOS_UINT32                                          ulMcc;                          /* MCC,3 bytes      */
    VOS_UINT32                                          ulMnc;                          /* MNC,2 or 3 bytes */
    LRRC_OM_TAC_STRU                                    stTac;                          /* tac */
    VOS_UINT16                                          usPhyCellId;                    /* 物理小区id */
    VOS_UINT16                                          usUlEarfcn;                     /*上行频率，单位100khz*/
    VOS_UINT16                                          usDlEarfcn;                     /*下行频率，单位100khz*/
    VOS_INT16                                           sRsrp;                          /* RSRP值,实际值 */
    VOS_INT16                                           sRsrq;                          /* RSRq值，实际值 */
    VOS_INT16                                           sRssi;                          /* RSSI值，实际值 */
    LRRC_OM_CAMP_THRESHOLD_STRU                         stCampThreshold;                /* 驻留门限*/
    VOS_UINT8                                           ucServerCellBandInd;            /* 所属band*/
    LRRC_APP_ERR_LOG_AREA_LOST_CAUSE_ENUM_UINT8         enSubCase;                      /* 丢网原因:暂保留，待扩展 */
    VOS_UINT8                                           aucResv[2];
}LRRC_OM_AREA_LOST_INFO_STRU;

/*****************************************************************************
 结构名    : Lte邻区信息
 结构说明  : lte邻区信息
*****************************************************************************/
typedef struct
{
    VOS_UINT16                          usCellCount;
    VOS_UINT8                           aucRsv[2];
    VOS_UINT16                          ausCellIds[LRRC_OM_MAX_SINGLE_FREQ_CELL_COUNT];
}LRRC_OM_FREQ_CELL_LIST_STRU;

/*****************************************************************************
 结构名    : Lte邻频信息
 结构说明  : lte邻频信息
*****************************************************************************/
typedef struct
{
    VOS_UINT16                          usFreqBandInd;
    VOS_UINT16                          usFreqInfo;
    VOS_UINT32                          ulDlArfcn;
    LRRC_OM_FREQ_CELL_LIST_STRU         stNCellList;
}LRRC_OM_LTE_FREQ_PARAM_STRU;

/*****************************************************************************
 结构名    : Lte邻频List
 结构说明  : lte邻频List
*****************************************************************************/
typedef struct
{
    VOS_UINT16                          usInterFreqCount;
    VOS_UINT8                           aucRsv[2];
    LRRC_OM_LTE_FREQ_PARAM_STRU         astLteNFreqParams[LRRC_OM_MAX_LTE_INTER_FREQ_COUNT];
}LRRC_OM_LTE_NCELL_STRU;

/*****************************************************************************
 结构名    : LRRC_OM_COVER_LOST_NCELL_STRU
 结构说明  : 出服务区邻区信息
*****************************************************************************/
typedef struct
{
    LRRC_OM_LTE_NCELL_STRU               stLteNcellInfo;
}LRRC_OM_COVER_LOST_NCELL_STRU;

/*****************************************************************************
 结构名    : LRRC_OM_LOST_COVERAGE_STRU
 结构说明  : 出服务区信息
*****************************************************************************/
typedef struct
{
    OM_ERR_LOG_HEADER_STRU                  stHead;
    RRC_APP_ERROR_CODE_ENUM_UINT16          enErrorCode;            /* 错误码上报结构，每个模块单独编号 */
    VOS_UINT16                              usRsv;
    LRRC_OM_SCELL_ID                        stCoverLostSCellId;     /* 出服务区主小区ID */
    LRRC_OM_COVER_LOST_NCELL_STRU           stCoverLostNCellInfo;   /* 出服务区时邻区信息 */
    LRRC_OM_SCELL_ID                        stCurrSCellId;          /* 重新搜网驻留的主小区ID */
    LRRC_OM_COVER_LOST_NCELL_STRU           stCurrNCellInfo;        /* 重新搜网驻留的邻区信息 */
}LRRC_OM_LOST_COVERAGE_STRU;

/*****************************************************************************
 结构名    : RRC_OM_AREA_LOST_STRU
 结构说明  : 丢网信息
*****************************************************************************/
typedef struct
{
    OM_ERR_LOG_HEADER_STRU                  stHead;
    RRC_APP_ERROR_CODE_ENUM_UINT16          enErrorCode;            /* 错误码上报结构，每个模块单独编号 */
    LRRC_APP_ERR_LOG_RAT_TYPE_ENUM_UINT8    enRatType;              /* 丢网发生时的接入技术 */
    VOS_UINT8                               ucRsv;
    LRRC_OM_AREA_LOST_INFO_STRU             stAreaLostInfo;        /* 丢网信息 */
}RRC_OM_AREA_LOST_STRU;

/*****************************************************************************
 结构名    : LRRC_OM_EST_INFO_STRU
 结构说明  : ERR LOG 连接建立数据结构
*****************************************************************************/
typedef struct
{
    OM_ERR_LOG_HEADER_STRU                  stHead;
    RRC_APP_ERROR_CODE_ENUM_UINT16          enErrorCode;            /* 错误码上报结构，每个模块单独编号 */
    VOS_UINT16                              usRsv;
    LRRC_OM_SCELL_ID                        stPCellInfo;
    LRRC_OM_CELL_MEAS_INFO                  stPCellMeasInfo;
    LRRC_OM_RRC_EST_LIST_INFO_STRU          stEstInfoList;          /* 建链结果 */
    LRRC_OM_ACC_INFO_STRU                   stAccInfo;              /* RRC建链时，UE能力保存结构 */
    LRRC_OM_STATE_INFO_STRU                 stRrcAppStateInfo;      /* 发生故障时，RRC状态信息，包括协议状态机和内部代码状态机 */
}LRRC_OM_EST_INFO_STRU;

/*****************************************************************************
 结构名    : LRRC_OM_REEST_INFO_STRU
 结构说明  : ERR LOG 重建立数据结构
*****************************************************************************/
typedef struct
{
    OM_ERR_LOG_HEADER_STRU              stHead;
    RRC_APP_ERROR_CODE_ENUM_UINT16      enErrorCode;            /* 错误码上报结构，每个模块单独编号 */
    RRC_APP_CONN_REEST_IND_ENUM_UINT8   enReestConnCause;       /* 最近一次重建原因 */
    VOS_UINT8                           ucNumOfPrembSentR11;
    VOS_UINT8                           ucContentionDetctR11;
    VOS_UINT8                           ucMaxTxPwrReachedR11;
    VOS_UINT16                          usRsv;
    LRRC_OM_SCELL_ID                    stPCellInfo;
    LRRC_OM_CELL_MEAS_INFO              stPCellMeasInfo;
    LRRC_OM_SYNC_INFO_STRU              stSyncInfo;             /* 物理层同步信息 */
    LRRC_OM_STATE_INFO_STRU             stRrcAppStateInfo;      /* 发生故障时，RRC状态信息，包括协议状态机和内部代码状态机 */
}LRRC_OM_REEST_INFO_STRU;

/*****************************************************************************
 结构名    : LRRC_OM_RECFG_INFO_STRU
 结构说明  : ERR LOG 重配置数据结构
*****************************************************************************/
typedef struct
{
    OM_ERR_LOG_HEADER_STRU              stHead;
    RRC_APP_ERROR_CODE_ENUM_UINT16      enErrorCode;            /* 错误码上报结构，每个模块单独编号 */
    VOS_UINT8                           ucNumOfPrembSentR11;
    VOS_UINT8                           ucContentionDetctR11;
    VOS_UINT8                           ucMaxTxPwrReachedR11;
    VOS_UINT8                           ucRsv[3];
    LRRC_OM_SCELL_ID                    stPCellInfo;
    LRRC_OM_CELL_MEAS_INFO              stPCellMeasInfo;
    LRRC_OM_STATE_INFO_STRU             stRrcAppStateInfo;      /* 发生故障时，RRC状态信息，包括协议状态机和内部代码状态机 */
}LRRC_OM_RECFG_INFO_STRU;

/*****************************************************************************
 结构名    : LRRC_OM_SRCHED_FREQ_INFO_STRU
 结构说明  : ERR LOG 上报搜网频点信息
*****************************************************************************/
typedef struct
{
    OM_ERR_LOG_HEADER_STRU                      stHead;
    RRC_APP_ERROR_CODE_ENUM_UINT16              enErrorCode;            /* 错误码上报结构，每个模块单独编号 */
    VOS_UINT16                                  usBandInd;
    VOS_UINT16                                  usAllBandSrchIdx;
    PS_BOOL_ENUM_UINT8                          enIsHistoryFreqFlg;
    VOS_UINT8                                   ucRsv;
    VOS_UINT32                                  ulDlArfcn;
    VOS_UINT32                                  ulGci;
    VOS_UINT32                                  ulMcc;                  /* Mcc */
    VOS_UINT32                                  ulMnc;                  /* Mnc */
}LRRC_OM_SRCHED_FREQ_INFO_STRU;

/*****************************************************************************
 结构名    : LRRC_OM_RA_ERROR_INFO_STRU
 结构说明  : ERR LOG 上报随机接入失败原因值
*****************************************************************************/
typedef struct
{
    LRRC_OM_ERR_CAUSE_ENUM_UINT8    enRaErrCause;
    VOS_UINT8                       ucPblSentNum;                               /*Preamble发送次数*/
    PS_BOOL_ENUM_UINT8              enMaxTxPowerReached;                        /*用于指示Preamble是否达到最大发送功率,1:达到，0:没达到*/
    PS_BOOL_ENUM_UINT8              enContentionDetected;                       /*是否有竞争，0:非竞争随机接入，1:竞争随机接入*/
}LRRC_OM_RA_ERROR_INFO_STRU;

/*****************************************************************************
 结构名    : LRRC_OM_RA_INFO_STRU
 结构说明  : ERR LOG 上报随机接入参数信息
*****************************************************************************/
typedef struct
{
    LRRC_OM_RA_TYPE_ENUM_UINT8      enRaType;                                   /* 随机接入方式 */
    LRRC_OM_RA_CAUSE_ENUM_UINT8     enRaCause;                                  /* 随机接入原因 */
    VOS_UINT8                       ucDediPreambleId;
    VOS_UINT8                       ucPrachMaskIndex;
    VOS_INT16                       sPathLoss ;                                 /* 取值不清楚，暂时保留用于选择Group A或B的实际路损，该值需要确认获取方式 */
    VOS_UINT16                      usMsg3Size;                                 /* 用于选择Group A或B的消息3大小值 ，数值使用的单位确认:byte */
    LRRC_OM_RA_ERROR_INFO_STRU      stRaErrInfo;
}LRRC_OM_RA_INFO_STRU;

/*****************************************************************************
 结构名    : LRRC_OM_RA_ERR_INFO_STRU
 结构说明  : ERR LOG 上报随机接入失败信息
*****************************************************************************/
typedef struct
{
    OM_ERR_LOG_HEADER_STRU                      stHead;
    RRC_APP_ERROR_CODE_ENUM_UINT16              enErrorCode;            /* 错误码上报结构，每个模块单独编号 */
    VOS_UINT16                                  usRsv;
    LRRC_OM_SCELL_ID                            stSCellIdInfo;          /* 服务小区Id信息 */
    LRRC_OM_CELL_MEAS_INFO                      stSCellMeasRst;         /* 小区测量结果 */
    LRRC_OM_CAMP_THRESHOLD_STRU                 stCampThreshold;        /* 驻留门限 */
    LRRC_OM_RA_INFO_STRU                        stRaInfo;               /* 随机接入相关信息 */
}LRRC_OM_RA_ERR_INFO_STRU;

/*****************************************************************************
 结构名    : OM_ERR_LOG_REPORT_CNF_STRU
 结构说明  : Balong平台内部交互的ERROR LOG 数据结构
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER
    VOS_UINT32                          ulMsgName;
    VOS_UINT32                          ulMsgType;
    VOS_UINT32                          ulMsgSN;
    VOS_UINT32                          ulRptlen;      /* 故障内容长度,如果ulRptlen为0,aucContent内容长度也为0 */
    VOS_UINT8                           aucContent[4]; /* 故障内容 */
} LRRC_OM_ERR_LOG_REPORT_CNF_STRU;

/*****************************************************************************
 结构名    : LRRC_OM_FAULT_ERR_LOG_IND_STRU
 结构说明  : Balong平台内部交互的主动上报Fault数据结构
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER
    VOS_UINT32                          ulMsgName;
    VOS_UINT32                          ulMsgType;
    VOS_UINT32                          ulMsgSN;
    VOS_UINT32                          ulRptlen;      /* 故障内容长度,如果ulRptlen为0,aucContent内容长度也为0 */
    VOS_UINT8                           aucContent[4]; /* 故障内容 */
}LRRC_OM_FAULT_ERR_LOG_IND_STRU;

typedef LRRC_OM_FAULT_ERR_LOG_IND_STRU LRRC_OM_ALARM_ERR_LOG_IND_STRU;

/*****************************************************************************
结构名    : LRRC_CHR_NCELL_INFO
协议表格  :
ASN.1描述 :
结构说明  : 邻区重选相关信息
*****************************************************************************/
typedef struct
{
    VOS_UINT16                          usMeasCnfAndState;
    VOS_UINT16                          usMeasLevel;
}LRRC_OM_NCELL_INFO;

/*****************************************************************************
结构名    : LRRC_ERRLOG_Q_LOW_CELL_INFO
协议表格  :
ASN.1描述 :
结构说明  : CHR 弱小区记录 rsrq < -15db
*****************************************************************************/
typedef struct
{
    LRRC_OM_SCELL_INFO                 stSCellInfo;                    /* 服务小区信息 */
    LRRC_OM_NCELL_INFO                 stNcellInfo;                    /* 邻区测量信息*/
    VOS_UINT32                         ulCampTimes;                    /* 驻留记录次数，此项*10s为该小区总驻留时长 */
}LRRC_OM_Q_LOW_CELL_INFO;

/*****************************************************************************
结构名    : LRRC_OM_QLOW_CELL_INFO_STRU
协议表格  :
ASN.1描述 :
结构说明  : 弱小区信息
*****************************************************************************/
typedef struct
{
    OM_ERR_LOG_HEADER_STRU              stHead;
    RRC_APP_ERROR_CODE_ENUM_UINT16      enErrorCode;            /* 错误码上报结构，每个模块单独编号 */
    VOS_UINT16                          usQlowCellInfoNum;
    LRRC_OM_Q_LOW_CELL_INFO             astQLowCellInfoList[LRRC_OM_Q_LOW_CELL_NUM_MAX];
}LRRC_OM_QLOW_CELL_INFO_STRU;

/*****************************************************************************
结构名    : LRRC_OM_UTRA_CELL_INFO
结构说明  : 乒乓重选utran小区重选信息
*****************************************************************************/
typedef struct
{
    VOS_UINT16                          usArfcn;
    VOS_UINT16                          usPrimaryCode;                      /* 小区信息 */
}LRRC_OM_UTRA_CELL_INFO;

/*****************************************************************************
结构名    : LRRC_OM_GSM_CELL_INFO
结构说明  : 乒乓重选gsm小区重选信息
*****************************************************************************/
typedef struct
{
    VOS_UINT16                          usArfcn;
    VOS_UINT16                          enBandInd;
    VOS_UINT16                          usNcc;
    VOS_UINT16                          usBcc;
    VOS_UINT16                          usRsv;
    VOS_INT16                           sGeranCellRxlev;
}LRRC_OM_GSM_CELL_INFO;

/*****************************************************************************
结构名    : LRRC_OM_CDMA_CELL_INFO
结构说明  : 乒乓重选cdma小区重选信息
*****************************************************************************/
typedef struct
{
    VOS_UINT8                           ucBand;
    VOS_UINT8                           aucRsv[3];
    VOS_UINT16                          usArfcn;
    VOS_UINT16                          usPn;                               /* 小区信息 */
}LRRC_OM_CDMA_CELL_INFO;

/*****************************************************************************
结构名    : LRRC_OM_PINGPONG_LTE_CELL_INFO
结构说明  : 乒乓重选lte小区重选信息
*****************************************************************************/
typedef struct
{
    LRRC_OM_SCELL_ID                        stLteScellId;
    LRRC_OM_CELL_MEAS_INFO                  stScellMeasInfo;
}LRRC_OM_PINGPONG_LTE_CELL_INFO;

/*****************************************************************************
结构名    : LRRC_OM_PINGPONG_CELL_INFO
结构说明  : 乒乓重选小区重选信息
*****************************************************************************/
typedef struct
{
    LRRC_OM_RAT_TYPE_ENUM_UINT16         usRatType;
    VOS_UINT16                           usRsv;
    union
    {
        LRRC_OM_PINGPONG_LTE_CELL_INFO   stLteCellInfo;
        LRRC_OM_UTRA_CELL_INFO           stUtraCellInfo;
        LRRC_OM_GSM_CELL_INFO            stGsmCellInfo;
        LRRC_OM_CDMA_CELL_INFO           stCdmaCellInfo;
    }u;
}LRRC_OM_PINGPONG_CELL_INFO;

/*****************************************************************************
结构名    : LRRC_OM_PINGPONG_RESEL_RECORD
结构说明  : 乒乓重选记录
*****************************************************************************/
typedef struct
{
    VOS_UINT8                               ucPingPongCellNum;
    VOS_UINT8                               ucRsv;
    VOS_UINT16                              usPingPongTimes;
    LRRC_OM_PINGPONG_CELL_INFO              astPpCellReselCellList[LRRC_OM_PING_PONG_CELL_NUM_MAX];
}LRRC_OM_PINGPONG_RESEL_RECORD;

/*****************************************************************************
结构名    : LRRC_CHR_PINGPONG_RESEL_LIST_INFO
结构说明  : 乒乓重选信息
*****************************************************************************/
typedef struct
{
    OM_ERR_LOG_HEADER_STRU                stHead;
    RRC_APP_ERROR_CODE_ENUM_UINT16        enErrorCode;            /* 错误码上报结构，每个模块单独编号 */
    VOS_UINT16                            usPpCellReselRecordNum;
    LRRC_OM_PINGPONG_RESEL_RECORD         astPpCellReselRecordList[LRRC_OM_PING_PONG_INFO_NUM_MAX];
}LRRC_OM_PINGPONG_RESEL_INFO_STRU;

/*****************************************************************************
结构名    : RRC_ERRLOG_ESR_CS_FAIL_STRU
协议表格  :
ASN.1描述 :
结构说明  : CS失败记录
*****************************************************************************/
typedef struct
{
    LRRC_OM_SCELL_ID                        stScellId;
    VOS_UINT16                              usCsTotalTimes;                 /* 总cs次数 */
    VOS_UINT16                              usCsFailTimes;                  /* cs失败次数 */
    VOS_UINT16                              usConnPagingRelTimes;           /* 连接态寻呼优化生效次数 */
    VOS_UINT16                              usCsReestRelTimes;              /* cs期间重建立优化生效次数 */
    VOS_UINT16                              usCsExpiredRelTimes;            /* cs期间超时释放优化生效次数 */
    VOS_UINT16                              ausRsv[9];
}LRRC_OM_ESR_CS_FAIL_RECORD;

/*****************************************************************************
结构名    : LRRC_OM_CSFB_FAIL_INFO_STRU
协议表格  :
ASN.1描述 :
结构说明  : CSFB失败信息
*****************************************************************************/
typedef struct
{
    OM_ERR_LOG_HEADER_STRU              stHead;
    RRC_APP_ERROR_CODE_ENUM_UINT16      enErrorCode;            /* 错误码上报结构，每个模块单独编号 */
    VOS_UINT16                          usCsfbFailInfoNum;
    LRRC_OM_ESR_CS_FAIL_RECORD          astEsrFailList[LRRC_OM_ESR_NUM_MAX];
}LRRC_OM_CSFB_FAIL_INFO_STRU;

/*****************************************************************************
结构名    : LRRC_CHR_MOBILITY_2_QLOW_CELL_INFO
协议表格  :
ASN.1描述 :
结构说明  : Mobility2QLowCell记录
*****************************************************************************/
typedef struct
{
    LRRC_OM_QLOW_FLOW_TYPE_ENUM_UINT8       usMobiltyFLowType;
    VOS_UINT8                               aucRsv[3];
    LRRC_OM_SCELL_ID                        stScellId;
    LRRC_OM_CELL_MEAS_INFO                  stScellMeasInfo;
}LRRC_OM_MOBILITY_2_QLOW_CELL_RECORD;

/*****************************************************************************
结构名    : LRRC_OM_MOBILITY_2_QLOW_CELL_INFO_STRU
协议表格  :
ASN.1描述 :
结构说明  : Mobility2QLowCell消息
*****************************************************************************/
typedef struct
{
    OM_ERR_LOG_HEADER_STRU                  stHead;
    RRC_APP_ERROR_CODE_ENUM_UINT16          enErrorCode;            /* 错误码上报结构，每个模块单独编号 */
    VOS_UINT16                              usMobility2QLowInfoNum;
    LRRC_OM_MOBILITY_2_QLOW_CELL_RECORD     astMobility2QLowRecordList[LRRC_OM_MOBILITY_2_QLOW_NUM_MAX];
}LRRC_OM_MOBILITY_2_QLOW_CELL_INFO_STRU;

/*****************************************************************************
结构名    : LRRC_CHR_OUT_OF_SYNC_INFO
协议表格  :
ASN.1描述 :
结构说明  : 失步记录
*****************************************************************************/
typedef struct
{
    LRRC_OM_SCELL_ID                        stScellId;
    VOS_UINT32                              ulOutOfSyncNum;
    VOS_UINT32                              ulInSyncNum;
}LRRC_OM_OUT_OF_SYNC_RECORD;

/*****************************************************************************
结构名    : LRRC_CHR_OUT_OF_SYNC_INFO_LIST
协议表格  :
ASN.1描述 :
结构说明  : 失步记录信息
*****************************************************************************/
typedef struct
{
    OM_ERR_LOG_HEADER_STRU                       stHead;
    RRC_APP_ERROR_CODE_ENUM_UINT16               enErrorCode;                   /* 错误码上报结构，每个模块单独编号 */
    VOS_UINT16                                   usOutOfSyncRecordNum;          /*失步记录个数，最大为8*/
    LRRC_OM_OUT_OF_SYNC_RECORD                   astOutOfSyncInfoList[LRRC_OM_OUT_OF_SYNC_NUM_MAX];
}LRRC_OM_OUT_OF_SYNC_INFO_STRU;

/* BAND-38 Scell Not config MFBI record begin */
/*****************************************************************************
结构名    : LRRC_CHR_SCELL_MFBI_RECORD_STRU
协议表格  :
ASN.1描述 :
结构说明  : MFBI info record
*****************************************************************************/
typedef struct
{
    VOS_UINT32                                  ulGci;                          /* sib1中的GlobalCellId */
    VOS_UINT32                                  ulMcc;                          /* Mcc */
    VOS_UINT32                                  ulMnc;                          /* Mnc */
    LRRC_OM_TAC_STRU                            stTac;                          /* tac */
    VOS_UINT16                                  usPhyCellId;                    /* 物理小区id */
    VOS_UINT16                                  usUlEarfcn;                     /*上行频率，单位100khz*/
    VOS_UINT16                                  usDlEarfcn;                     /*下行频率，单位100khz*/
    VOS_UINT8                                   ucServerCellBandInd;            /* 所属band*/
    VOS_UINT8                                   ucRsv;
    VOS_UINT8                                   ucMultiBandIndNum;
    VOS_UINT8                                   aucMultiBandInd[9];             /* 多band列表信息，第一位存放sib1 r9 band信息 */
    VOS_UINT8                                   aucReserved[2];
}LRRC_CHR_SCELL_MFBI_RECORD_STRU;

/*****************************************************************************
结构名    : LRRC_CHR_SCELL_MFBI_INFO_RPT_STRU
协议表格  :
ASN.1描述 :
结构说明  : MFBI info report
*****************************************************************************/
typedef struct
{
    OM_ERR_LOG_HEADER_STRU                       stHead;
    RRC_APP_ERROR_CODE_ENUM_UINT16               enErrorCode;      /* 错误码上报结构，每个模块单独编号 */
    VOS_UINT16                                   usRecordNum;      /* 未配置MFBI的BAND38小区记录个数，最大为30 */
    LRRC_CHR_SCELL_MFBI_RECORD_STRU              astScellInfoList[LRRC_OM_SCELL_NOTCONFIG_MFBI_MAX_NUM];
}LRRC_OM_SCELL_MFBI_INFO_RPT_STRU;
/* BAND-38 Scell Not config MFBI record end */

/*****************************************************************************
 枚举名    : RRC_APP_ERROR_CODE_ENUM
 协议表格  :
 ASN.1描述 :
 枚举说明  : RRC错误码类型
*****************************************************************************/
enum RRC_APP_AIR_MSG_FAIL_ENUM
{
    RRC_APP_AIR_MSG_DECODE_FAIL               = 0,  /* 解码失败 */
    RRC_APP_AIR_MSG_CHECK_FAIL                = 0,  /* 检查失败 */

    RRC_APP_AIR_MSG_FAIL_BUTT
};
typedef VOS_UINT16 RRC_APP_AIR_MSG_FAIL_ENUM_UINT16;

/*****************************************************************************
结构名    : LRRC_CHR_OUT_OF_SYNC_INFO_LIST
协议表格  :
ASN.1描述 :
结构说明  : 失步记录信息
*****************************************************************************/
typedef struct
{
    OM_ERR_LOG_HEADER_STRU                       stHead;
    RRC_APP_ERROR_CODE_ENUM_UINT16               enErrorCode;         /* 错误码上报结构，每个模块单独编号 */
    VOS_UINT16                                   usFailAirMsgType;
    VOS_UINT32                                   ulGci;                          /* sib1中的GlobalCellId */
    VOS_UINT32                                   ulMcc;                          /* MCC,3 bytes      */
    VOS_UINT32                                   ulMnc;                          /* MNC,2 or 3 bytes */
    LRRC_OM_TAC_STRU                             stTac;                          /* tac */
    VOS_UINT16                                   usPhyCellId;                    /* 物理小区id */

    VOS_UINT16                                   usRsv0;
    VOS_UINT16                                   usRsv1;
    VOS_UINT16                                   usRsv2;
    VOS_UINT16                                   usRsv3;
    VOS_UINT16                                   usRsv4;
    VOS_UINT16                                   usRsv5;
    VOS_UINT16                                   usRsv6;
    LRRC_OM_LATEST_PRINT_INFO                    stLatestPrint;
    VOS_UINT8                                    aucBuf[LRRC_OM_AIR_MSG_BUF_LEN];
}LRRC_OM_AIRMSG_PROC_FAIL;
/*****************************************************************************
结构名    : LRRC_OM_SRVCC_MEAS_INFO_STRU
协议表格  :
ASN.1描述 :
结构说明  : SRVCC相关的测量ID
*****************************************************************************/
typedef struct
{
    VOS_UINT16                                   usMeasId;
    VOS_UINT16                                   usMeasObjId;
    VOS_UINT16                                   usMeasRptId;
    LRRC_OM_RAT_TYPE_ENUM_UINT16                 usMeasObjRat;
    VOS_UINT32                                   ulCarrierFreq;
    VOS_UINT16                                   usMeasRptType;
    VOS_INT16                                    sEventThreshold1;
    VOS_INT16                                    sEventThreshold2;
    VOS_INT16                                    sFreqOffset;
    VOS_UINT8                                    ucHysteresis;
    VOS_UINT8                                    ucEventRptFlg;                 /* 该MeasId是否触发过测量报告 */
    VOS_UINT8                                    aucRsv[2];
    VOS_UINT32                                   aulRsv[4];
}LRRC_OM_SRVCC_MEAS_INFO_STRU;

/*****************************************************************************
结构名    : LRRC_OM_SRVCC_INFO_STRU
协议表格  :
ASN.1描述 :
结构说明  : SRVCC失败信息
*****************************************************************************/
typedef struct
{
    OM_ERR_LOG_HEADER_STRU                       stHead;
    RRC_APP_ERROR_CODE_ENUM_UINT16               enErrorCode;                   /* 错误码上报结构，每个模块单独编号 */
    VOS_INT16                                    sRsrp;                         /* 当前服务小区rsrp */
    VOS_INT16                                    sRsrq;                         /* 当前服务小区rsrq */
    VOS_INT16                                    sRssi;
    VOS_UINT32                                   ulGci;                         /* sib1中的GlobalCellId */
    LRRC_OM_TAC_STRU                             stTac;                         /* tac */
    VOS_UINT32                                   ulMcc;                         /* Mcc */
    VOS_UINT32                                   ulMnc;                         /* Mnc */
    VOS_UINT32                                   ulDlEarfcn;                    /*下行频点，单位100khz */
    VOS_UINT16                                   usPhyCellId;                   /* 物理小区，单位100khz */
    LRRC_OM_SRVCC_RST_UINT8                      ucFailCause;                   /* 失败原因值 */
    VOS_UINT8                                    ucSrvccState;                  /* 预留 */
    VOS_UINT16                                   usRsv;
    VOS_UINT16                                   usMeasIdNum;
    LRRC_OM_SRVCC_MEAS_INFO_STRU                 astMeasIdList[LRRC_OM_SRVCC_MAX_MEASID_NUM];
    VOS_UINT32                                   aulRsv[8];
}LRRC_OM_SRVCC_FAIL_INFO_STRU;

/*****************************************************************************
结构名    : LRRC_OM_MOD3_NCELL_INFO_STRU
协议表格  :
ASN.1描述 :
结构说明  : 模三相等强干扰小区
*****************************************************************************/
typedef struct
{
    OM_ERR_LOG_HEADER_STRU                       stHead;
    RRC_APP_ERROR_CODE_ENUM_UINT16               enErrorCode;                   /* 错误码上报结构，每个模块单独编号 */
    VOS_UINT16                                   usRsv0;
    VOS_UINT32                                   ulGci;                         /* sib1中的GlobalCellId */
    LRRC_OM_TAC_STRU                             stTac;                         /* tac */
    VOS_UINT32                                   ulMcc;                         /* Mcc */
    VOS_UINT32                                   ulMnc;                         /* Mnc */
    VOS_UINT32                                   ulDlarfcn;                     /* 下行频点 */
    VOS_INT16                                    sRsrp;                         /* 当前主小区rsrp */
    VOS_INT16                                    sRsrq;                         /* 当前主小区rsrq */
    VOS_INT16                                    sRssi;
    VOS_INT16                                    sNcellRsrp;                    /* 模三相等同频邻小区rsrp */
    VOS_INT16                                    sNcellRsrq;                    /* 模三相等同频邻小区rsrq */
    VOS_INT16                                    sNcellRssi;
    VOS_UINT16                                   usPCellId;                     /* 主小区ID */
    VOS_UINT16                                   usNCellId;                     /* 邻小区ID */

    PS_BOOL_ENUM_UINT8                           ucVolteFlag;
    VOS_UINT8                                    ucRsv0;
    VOS_UINT8                                    ucRsv1;
    VOS_UINT8                                    ucRsv2;
    VOS_UINT8                                    ucRsv3;
    VOS_UINT8                                    ucRsv4;
    VOS_UINT8                                    ucRsv5;
    VOS_UINT8                                    ucRsv6;
    VOS_UINT16                                   usRsv1;
    VOS_UINT16                                   usRsv2;
    VOS_UINT16                                   usRsv3;
    VOS_UINT16                                   usRsv4;
    VOS_UINT32                                   ulRsv0;
    VOS_UINT32                                   ulRsv1;
}LRRC_OM_MOD3_NCELL_INFO_STRU;

/*****************************************************************************
结构名    : LRRC_OM_RF_FAIL_REEST_INFO_STRU
协议表格  :
ASN.1描述 :
结构说明  : 无线链路失败导致的重建立，用以统计同频A3测量报告未响应的情况
*****************************************************************************/
typedef struct
{
    OM_ERR_LOG_HEADER_STRU                       stHead;
    RRC_APP_ERROR_CODE_ENUM_UINT16               enErrorCode;                   /* 错误码上报结构，每个模块单独编号 */
    VOS_UINT16                                   usRsv0;

    VOS_UINT32                                   ulGci;                         /* sib1中的GlobalCellId */
    LRRC_OM_TAC_STRU                             stTac;                         /* tac */
    VOS_UINT32                                   ulMcc;                         /* Mcc */
    VOS_UINT32                                   ulMnc;                         /* Mnc */
    VOS_UINT32                                   ulDlarfcn;                     /* 下行频点 */

    VOS_INT16                                    sRsrp;                         /* 当前主小区rsrp */
    VOS_INT16                                    sRsrq;                         /* 当前主小区rsrq */

    VOS_INT16                                    sRssi;
    VOS_INT16                                    sStrongestIntraNcellRsrp;      /* 最强同频邻小区rsrp */

    VOS_INT16                                    sStrongestIntraNcellRsrq;      /* 最强同频邻小区rsrq */
    VOS_INT16                                    sStrongestIntraNcellRssi;

    VOS_UINT16                                   usPCellId;                     /* 主小区ID */
    VOS_UINT16                                   usStrongestIntraNCellId;       /* 最强同频邻小区ID */

    VOS_UINT16                                   usA3RptCnt;                    /* A3上报次数 */
    VOS_UINT16                                   usRsv7;

    VOS_UINT32                                   ulA3ToReestInterTime;          /* 第一次上报A3到重建立的时间间隔，单位ms */
    VOS_UINT32                                   ulT310Len;                     /* T310时长，单位ms */

    VOS_UINT8                                    ucA3RptFlag;                   /* 同频A3测量上报情况:0:未配置A3测量 1:有测量但未上报 2:有测量有上报 */
    PS_BOOL_ENUM_UINT8                           ucPcellStrongestFlag;          /* 主小区同频信号质量最强标志，仅考虑rsrp */
    PS_BOOL_ENUM_UINT8                           ucVolteFlag;
    VOS_UINT8                                    ucRsv0;

    VOS_UINT16                                   usA3NCellId0;                  /* A3上报过的邻区小区ID0，无效值0xFF */
    VOS_UINT16                                   usA3NCellId1;

    VOS_UINT16                                   usA3NCellId2;
    VOS_UINT16                                   usA3NCellId3;

    VOS_UINT16                                   usA3NCellId4;
    VOS_UINT16                                   usA3NCellId5;

    VOS_UINT8                                    ucRsv1;                        /* 相当于usA3NCellId6 */
    VOS_UINT8                                    ucRsv2;                        /* 相当于usA3NCellId7 */
    VOS_UINT8                                    ucRsv3;
    VOS_UINT8                                    ucRsv4;

    VOS_UINT8                                    ucRsv5;
    VOS_UINT8                                    ucRsv6;
    VOS_UINT8                                    ucRsv7;
    VOS_UINT8                                    ucRsv8;

    VOS_UINT16                                   usRsv1;
    VOS_UINT16                                   usRsv2;

    VOS_UINT16                                   usRsv3;
    VOS_UINT16                                   usRsv4;

    VOS_UINT16                                   usRsv5;
    VOS_UINT16                                   usRsv6;

    VOS_UINT32                                   ulRsv0;                        /* 最后一次成功上报A3到重建立的时间间隔，单位ms */
    VOS_UINT32                                   ulRsv1;
}LRRC_OM_RF_FAIL_REEST_INFO_STRU;
/*****************************************************************************
结构名    : LRRC_GUTRRC_CELL_RESEL_RESULT
协议表格  :
ASN.1描述 :
结构说明  : GUT2L 重选选失败原因值上下文
*****************************************************************************/
typedef struct
{
    RRC_APP_ERROR_CODE_ENUM_UINT16              enErrorCode;               /* 错误码上报结构，每个模块单独编号 */
    LRRC_APP_ERR_LOG_RAT_TYPE_ENUM_UINT8        enSrcIratType;             /* 源小区制式类型1为G, 2为W, 4为T */
    VOS_UINT8                                   ucRsv;                     /* 保留 */

    VOS_UINT32                                  ulGci;                    /* sib1中的GlobalCellId */
    VOS_UINT32                                  ulMcc;                    /* Mcc */
    VOS_UINT32                                  ulMnc;                    /* Mnc */
    LRRC_OM_TAC_STRU                            stTac;                    /* tac */
    VOS_UINT16                                  usPhyCellId;              /* 物理小区id */
    VOS_UINT16                                  usDlEarfcn;               /*下行频率，单位100khz*/
    VOS_UINT16                                  usBandInd;                /* 所属band*/
    VOS_UINT16                                  usRsv0;

    VOS_INT16                                   sCurrSCellRsrp;            /* 小区Rsrp */
    VOS_INT16                                   sCurrSCellRsrq;            /* 小区Rsrq */

    LPS_32K_TIMESTAMP                           stCurrTimeStamp;            /* GUT重选到L失败的时间 */
    LRRC_GUTRRC_RESEL_FAIL_CAUSE_ENUM_UINT32    enReselFailCause;           /* GUT重选到L失败的原因 */
}LRRC_GUTRRC_CELL_RESEL_RESULT;

/*****************************************************************************
结构名    : LRRC_OM_GUT2L_RESEL_INFO_STRU
协议表格  :
ASN.1描述 :
结构说明  : 用以统计其他模向L重选失败的情况
*****************************************************************************/
typedef struct
{
    OM_ERR_LOG_HEADER_STRU                       stHead;
    VOS_UINT16                                   usRptNum;
    LRRC_GUTRRC_CELL_RESEL_RESULT                stCellReselResult[LRRC_CHR_RPT_GUT2L_RESEL_FAIL_MAX_NUM];
}LRRC_OM_GUT2L_RESEL_INFO_STRU;

/*****************************************************************************
结构名    : LRRC_OM_SRVCC_STAT_INFO_STRU
协议表格  :
ASN.1描述 :
结构说明  : SRVCC统计信息
*****************************************************************************/
typedef struct
{
    OM_ERR_LOG_HEADER_STRU                       stHead;
    RRC_APP_ERROR_CODE_ENUM_UINT16               enErrorCode;                   /* 错误码上报结构，每个模块单独编号 */
    VOS_UINT16                                   usRsv;
    VOS_UINT32                                   ulGci;
    LRRC_OM_TAC_STRU                             stTac;                         /* tac */
    VOS_UINT32                                   ulPreDelay;                    /* SRVCC准备时延 */
    VOS_INT16                                    sMeasCfgRsrp;                  /* 配置B1B2时rsrp */
    VOS_INT16                                    sMeasCfgRsrq;                  /* 配置B1B2时rsrq */
    VOS_INT16                                    sMeasRptRsrp;                  /* 测量报告时rsrq */
    VOS_INT16                                    sMeasRptRsrq;                  /* 测量报告时rsrp */
    VOS_INT16                                    sEventThreshold1;
    VOS_INT16                                    sEventThreshold2;
    VOS_UINT32                                   aulRsv[8];
}LRRC_OM_SRVCC_STAT_INFO_STRU;

/*****************************************************************************
结构名    : LRRC_OM_INTER_FREQ_ORRAT_MEAS_FOR_LONG_TIME
协议表格  :
ASN.1描述 :
结构说明  :长时间处于异系统测量而不重选可能引起功耗升高的现网统计
*****************************************************************************/
typedef struct
{
    OM_ERR_LOG_HEADER_STRU                       stHead;
    RRC_APP_ERROR_CODE_ENUM_UINT16               enErrorCode;                   /* 错误码上报结构，每个模块单独编号 */
    VOS_UINT16                                   usRsv;
    VOS_UINT32                                   ulGci;
    LRRC_OM_TAC_STRU                             stTac;                         /* tac */
    VOS_UINT32                                   ulMeasTimeLen;                 /* 低优先级异系统测量总时间 */
    VOS_UINT32                                   ulCampTimeLen;                 /* 当前小区驻留总时间 */
    VOS_UINT32                                   ulConnStateTimeLen;            /* 连接态总时间*/
    VOS_UINT32                                   ulCometoConnTimes;             /* 期间进入连接态的次数*/

    VOS_UINT32                                   ulCampTstampStartHighBits;
    VOS_UINT32                                   ulCampTstampStartLowBits;

    VOS_UINT32                                   ulCampTstampEndHighBits;
    VOS_UINT32                                   ulCampTstampEndLowBits;


    VOS_INT16                                    sRxLevelMin;
    VOS_INT16                                    sRxLevelQMin;
    VOS_INT16                                    sNonIntraSearch;
    VOS_INT16                                    sNonIntraSearchQ;
    VOS_INT16                                    sThreshServingLowP;
    VOS_INT16                                    sThreshServingLowQ;

    VOS_UINT16                                   usLowUmtsFreqNum;
    VOS_UINT16                                   usLowGsmFreqNum;


    VOS_UINT16                                   usUtranMode;
    VOS_UINT16                                   usRsv0;

    VOS_UINT16                                   usRsv1;
    VOS_UINT16                                   usRsv2;


    VOS_UINT32                                   ulRsv0;
    VOS_UINT32                                   ulRsv1;
    VOS_UINT32                                   ulRsv2;
    VOS_UINT32                                   ulRsv3;

}LRRC_OM_INTER_FREQORRAT_MEAS_FOR_LONG_TIME;

typedef struct
{
    VOS_UINT32   ulTimeStamp;
    VOS_UINT32   ulFileID;
    VOS_UINT32   ulLineNO;
    SECU_FUNC_TYPE_ENUM_UINT8 enSecuFuncType;/*函数类型*/
    VOS_UINT8    ucRsv;
    VOS_INT16    sErrNo;/*错误值*/
    VOS_VOID *   pDst;/*目的地址*/
    VOS_UINT32   ulMaxBuffer;/*目的缓存长度*/
    VOS_VOID *   pSrc;/*源地址*/
    VOS_UINT32   ulLength;/*实际长度*/
}TLPS_SECU_FUNCTION_INFO_STRU;

/*****************************************************************************
 结构名    : LRRC_OM_SECU_FUNCTION_ERROR_INFO_STRU
 结构说明  : 安全函数异常运行错误信息
*****************************************************************************/
typedef struct
{
    OM_ERR_LOG_HEADER_STRU                     stHead;                /* Errorlog 消息头*/
    RRC_APP_ERROR_CODE_ENUM_UINT16             enErrorCode;             /* 错误码*/
    VOS_UINT16                                 usRsv;
    TLPS_SECU_FUNCTION_INFO_STRU               stTlpsSecuFunctionInfo;   /* 安全函数运行异常时状态信息 */
}LRRC_OM_SECU_FUNCTION_ERROR_INFO_STRU;

/*****************************************************************************
 结构名    : LRRC_OM_AIR_SIGNAL_INFO_STRU
 结构说明  : 一次信号轨迹的记录信息
*****************************************************************************/
typedef struct
{
    VOS_UINT16        usCellId;
    VOS_UINT16        usDLEarfcn;
    VOS_INT16         sRsrp;
    VOS_INT16         sRsrq;
    VOS_UINT16        usStrongestIntraCell;
    VOS_INT16         sStrongestIntrRsrp;
    VOS_UINT16        usMod3EualIntraCell;
    VOS_INT16         sMod3EualIntraRsrp;
    VOS_UINT32        ulTimeStamp;
    VOS_INT8          sPusPwr;
    VOS_UINT8         ucBler;
    VOS_UINT8         ucMeasLevel;
    VOS_INT8          sSnr;
    LPS_OM_SENSOR_STAT_ENUM_UINT8   enSensorState;
    VOS_INT8          sResv;
    VOS_UINT16        usResv;
}LRRC_OM_AIR_SIGNAL_INFO_STRU;

/*****************************************************************************
 结构名    : LRRC_OM_LAYER_EVENT_INFO_STRU
 结构说明  : 一次Key 记录信息
*****************************************************************************/
typedef struct
{
    OM_PS_KEY_EVENT_ENUM_UINT16   usEvent;
    VOS_UINT16                    usRsv;
    VOS_UINT32                    ulTimeStamp;
}LRRC_OM_LAYER_EVENT_INFO_STRU;

/*****************************************************************************
 枚举名    : RRC_MR_TRIG_TYPE_ENUM
 协议表格  :
 ASN.1描述 :
 枚举说明  : 触发测量报告上报的条件
*****************************************************************************/
enum LRRC_MR_TRIG_TYPE_ENUM
{
    LRRC_MR_TRIG_TYPE_A1 = 0,
    LRRC_MR_TRIG_TYPE_A2 ,
    LRRC_MR_TRIG_TYPE_A3 ,
    LRRC_MR_TRIG_TYPE_A4 ,
    LRRC_MR_TRIG_TYPE_A5 ,
    LRRC_MR_TRIG_TYPE_A6 ,
    LRRC_MR_TRIG_TYPE_A7 ,
    LRRC_MR_TRIG_TYPE_A8 ,
    LRRC_MR_TRIG_TYPE_A9 ,

    LRRC_MR_TRIG_TYPE_B1 ,
    LRRC_MR_TRIG_TYPE_B2 ,
    LRRC_MR_TRIG_TYPE_B3 ,
    LRRC_MR_TRIG_TYPE_B4 ,


    LRRC_MR_TRIG_TYPE_C1 ,
    LRRC_MR_TRIG_TYPE_C2 ,
    LRRC_MR_TRIG_TYPE_C3 ,
    LRRC_MR_TRIG_TYPE_C4 ,

    LRRC_MR_TRIG_TYPE_PERIOD_STR ,
    LRRC_MR_TRIG_TYPE_PERIOD_CGI ,

    LRRC_MR_TRIG_TYPE_BUTT
};
typedef VOS_UINT8   LRRC_MR_TRIG_TYPE_ENUM_UINT8;

/*****************************************************************************
 枚举名    : RRC_CMM_MEAS_OBJ_TYPE_ENUM
 协议表格  :
 ASN.1描述 :
 枚举说明  :基于协议定义
*****************************************************************************/
enum LRRC_OM_MEAS_OBJ_TYPE_ENUM
{
    RRC_OM_MEAS_OBJ_EUTRA     = 1,
    RRC_OM_MEAS_OBJ_UTRA      ,
    RRC_OM_MEAS_OBJ_GERAN     ,
    RRC_OM_MEAS_OBJ_CDMA2000  ,
    RRC_OM_MEAS_OBJ_BUTT
};
typedef VOS_UINT8 LRRC_OM_MEAS_OBJ_TYPE_ENUM_UINT8;


/*****************************************************************************
 结构名    : LRRC_OM_MR_BASIC_INFO
 结构说明  : 如果是测量报告，记录一些基本信息
*****************************************************************************/
typedef struct
{
    VOS_UINT32                       ulTimeStamp;
    VOS_UINT16                       usSrvingCellId;       /* 发送测量报告时的PCELL ID */
    VOS_UINT16                       usSrvingEarfcn;       /* 发送测量报告时的PCELL的ARFCN */

    VOS_UINT32                       ulObjEarfcn;

    RRC_APP_BOOL_ENUM_UINT8          enCnfFlg;             /* 确认标志 */
    LRRC_OM_MEAS_RPT_TYPE_ENUM_UINT8 enEvtType;
    VOS_UINT8                        ucIfScell;            /* 如果是A1-A3是否是SCELL*/
    LRRC_OM_MEAS_OBJ_TYPE_ENUM_UINT8 enObjType;            /* ObjType*/

    VOS_UINT8                        ucPcellRsrp;
    VOS_UINT8                        ucPcellRsrq;
    VOS_UINT8                        ucNcellEnergy;        /* 非A1,A2时间报最强邻区能量 */
    VOS_UINT8                        ucNcellQulity;        /* 非A1,A2时间报最强邻区能量 */

    VOS_UINT8                        ucNcellEnergySnd;     /* 次强邻区能量 */
    VOS_UINT8                        ucRsv0;
    VOS_UINT8                        ucRsv1;
    VOS_UINT8                        ucRsv2;

    VOS_UINT16                       usNcellId;             /* NcellId */
    VOS_UINT16                       usNcellIdSnd;          /* 次强NcellId */
}LRRC_OM_MEASRPT_INFO;

/*****************************************************************************
 结构名    : LRRC_OM_UL_AIRMSG_INFO_STRU
 结构说明  :
*****************************************************************************/
typedef struct
{
    VOS_UINT32                       ulTimeStamp;
    LRRC_OM_AIRMSG_TYPE_ENUM_UINT16  usMsgType;
    RRC_APP_BOOL_ENUM_UINT8          enCnfFlg;             /* 确认标志 */
    VOS_UINT8                        ucRsv;

    LRRC_OM_MEASRPT_INFO            *pstMrInfo;             /* 测量报告提供额外的信息，
                                                              先在初始化分配好，内存不够再优化*/
}LRRC_OM_UL_AIRMSG_INFO_STRU;

/*****************************************************************************
 结构名    :LRRC_OM_REEST_TRIG_ENUM
 结构说明  :重建立触发的直接原因
*****************************************************************************/
enum LRRC_OM_REEST_TRIG_ENUM
{
    LRRC_OM_REEST_TRIG_LMAC_ERR_FATAL = 0,                  /*有邻区但网络不指示切换*/
    LRRC_OM_REEST_TRIG_LMAC_RA_ERR_MAX_TX,                        /*确实没有信号*/
    LRRC_OM_REEST_TRIG_LMAC_RA_ERR_MAX_HARQ,
    LRRC_OM_REEST_TRIG_LMAC_ERR_NO_RF_RA,
    LRRC_OM_REEST_TRIG_LMAC_ERR_NO_RF_TIMEROUT,
    LRRC_OM_REEST_TRIG_LMAC_ERR_RF_RESUME_SYNC,

    LRRC_OM_REEST_TRIG_LRLC_ERR_RL_FAIL = 20,

    LRRC_OM_REEST_TRIG_LPDCP_INTEGRITY_VERIFY = 30,

    LRRC_OM_REEST_TRIG_OUT_OF_SYNC       = 40,
    LRRC_OM_REEST_TRIG_LPHY_TIMING_ERR,
    LRRC_OM_REEST_TRIG_SERV_MEAS_DOWN,            /* 服务区测量掉底 */

    LRRC_OM_REEST_TRIG_HIGH_LAYER        = 60,    /* 高层频段禁止等原因 */
    LRRC_OM_REEST_TRIG_INTERNAL_CONFLICT,         /* 内部流程对冲 */

    /* RRC原因 */
    LRRC_OM_REEST_TRIG_INTRAFREQ_HO_FAIL = 70,         /* LTE模内同频切换失败*/
    LRRC_OM_REEST_TRIG_INTERFREQ_HO_FAIL,         /* LTE模内异频切换失败*/
    LRRC_OM_REEST_TRIG_L2W_HO_FAIL,           /* 系统间切换失败*/
    LRRC_OM_REEST_TRIG_L2G_HO_FAIL,           /* 系统间切换失败*/
    LRRC_OM_REEST_TRIG_L2G_CCO_FAIL,          /* 系统间切换失败*/
    LRRC_OM_REEST_TRIG_L2L_T304_EXPIRED,      /* T304 Expired */
    LRRC_OM_REEST_TRIG_CCO_T304_EXPIRED,
    LRRC_OM_REEST_TRIG_X2LHO_T304_EXPIRED,


    /*=========================内部异常=======================================*/
    LRRC_OM_REEST_TRIG_X2L_CHECKFAIL      = 160,         /* L2X切换，信元检查失败 */
    LRRC_OM_REEST_TRIG_X2L_SUSPENDFAIL,       /* L2X挂起失败 */

    LRRC_OM_REEST_TRIG_CFG_PHY_FAIL,
    LRRC_OM_REEST_TRIG_CFG_L2_FAIL,
    LRRC_OM_REEST_TRIG_CFG_PDCP_FAIL,
    LRRC_OM_REEST_TRIG_SCELL_RELCFG_FAIL,
    LRRC_OM_REEST_TRIG_RELNEWRB_FAIL,
    LRRC_OM_REEST_TRIG_SAVERECFG_FAIL,
    LRRC_OM_REEST_TRIG_STOP_PDCP_FAIL,
    LRRC_OM_REEST_TRIG_HO_CFGMACRLC_FAIL,
    LRRC_OM_REEST_TRIG_HO_CFGRB_FAIL,
    LRRC_OM_REEST_TRIG_HO_CFGSMC_FAIL,
    LRRC_OM_REEST_TRIG_HO_CFGSCELL_FAIL,
    LRRC_OM_REEST_TRIG_HO_RELNEWRB_FAIL,
    LRRC_OM_REEST_TRIG_BUTT
};
typedef VOS_UINT8  LRRC_OM_REEST_TRIG_ENUM_UINT8;

/*****************************************************************************
 结构名    :LRRC_OM_REEST_SUBTRIG_ENUM
 结构说明  :重建立触发的子原因
*****************************************************************************/
enum LRRC_OM_REEST_SUBTRIG_ENUM
{
    LRRC_OM_REEST_SUBTRIG_NORMAL,
    LRRC_OM_REEST_SUBTRIG_BUTT
};
typedef VOS_UINT8  LRRC_OM_REEST_SUBTRIG_ENUM_UINT8;

/*****************************************************************************
 结构名    :LRRC_OM_REEST_GUESSTRIG_ENUM
 结构说明  :重建立触发的推测原因值
*****************************************************************************/
enum LRRC_OM_REEST_GUESSTRIG_ENUM
{
    LRRC_OM_REEST_GUESSTRIG_NORMAL = 1,
    LRRC_OM_REEST_GUESSTRIG_BUTT,
};
typedef VOS_UINT8  LRRC_OM_REEST_GUESSTRIG_ENUM_UINT8;

/*****************************************************************************
 结构名    :LRRC_OM_REEST_FLOW_IND
 结构说明  :重建立过程指示
*****************************************************************************/
enum LRRC_OM_REEST_FLOW_IND_ENUM
{
    LRRC_OM_REEST_FLOW_IND_SUCC = 1,                         /*重建成功*/
    LRRC_OM_REEST_FLOW_IND_REJ,                              /*重建立被拒*/

    LRRC_OM_REEST_FLOW_IND_T311_EXPIRE,                      /* 重建立失败--重建过程中小区搜索T311定时器超时 */
    LRRC_OM_REEST_FLOW_IND_T301_EXPIRE,                      /* 重建立失败--发送建立Req消息后，等待重建响应消息，T301超时 */
    LRRC_OM_REEST_FLOW_IND_INTR_ERROR,                       /* 内部异常，信元检查失败等 */
    LRRC_OM_REEST_FLOW_IND_BUTT,
};
typedef VOS_UINT8  LRRC_OM_REEST_FLOW_IND_ENUM_UINT8;



/*****************************************************************************
结构名    : LRRC_OM_REESTAB_STAT_INFO_STRU
协议表格  :
ASN.1描述 :
结构说明  : 重建立统计信息
*****************************************************************************/
typedef struct
{
    VOS_UINT32                                   ulMcc;                         /* Mcc */
    VOS_UINT32                                   ulMnc;

    VOS_UINT32                                   ulGci;                         /* 重建前小区信息 */
    LRRC_OM_TAC_STRU                             stTac;

    LRRC_OM_REEST_TRIG_ENUM_UINT8                enReesTrig;                    /* 触发直接原因*/
    LRRC_OM_REEST_SUBTRIG_ENUM_UINT8             enReesSubTrig;                 /* 触发子原因，待补充*/
    LRRC_OM_REEST_GUESSTRIG_ENUM_UINT8           enReesGuessTrig;               /* 通过自动化分析得出的推测原因值，比如网络不切换*/
    LRRC_OM_REEST_FLOW_IND_ENUM_UINT8            enReesFlowInd;                 /* 重建立本身流程成功失败原因值*/

    VOS_UINT32                                   ulReesFailSubRslt;             /* 如果重建失败记录一个子原因 */

    VOS_UINT8                                    enOrigFreqRees;                /* 是否是原频点重建 */
    VOS_UINT8                                    enIfRssiStrPointToSelect;      /* 是否有能量更强的小区可以优先选择 */
    VOS_UINT16                                   usResv;

    VOS_INT16                                    sTrigRsrp;                     /* 触发点上的小区能量 */
    VOS_INT16                                    sTrigRsrq;                     /* 触发点上的小区质量 */

    VOS_INT16                                    sRsrpAfterReest;               /* 重建后的小区能量 */
    VOS_INT16                                    sRsrqAfterReest;               /* 重建后的小区质量 */

    /* 以下为自诊断的原因值 */
    LPS_LOW_QUILITY_ABNORMAL_STRU                stLowQuiltyCause;

    VOS_UINT8                                    enGoodSigRlError;              /* 好信号无线链路问题，可能是UE底层异常 */
    VOS_UINT8                                    enNotMaxTxPwrRl;               /* 上行信号未推到最大发生RL异常点 */
    VOS_UINT8                                    enFastFading;
    VOS_UINT8                                    enRsv1;

    VOS_UINT8                                    enRsv2;
    VOS_UINT8                                    enRsv3;
    VOS_UINT8                                    enRsv4;
    VOS_UINT8                                    enRsv5;

    VOS_UINT32                                   ulRsv0;
    VOS_UINT32                                   ulRsv1;

    VOS_UINT16                                   usRsv1;
    VOS_UINT16                                   usRsv2;

    VOS_UINT32                                   ul1stFreq;
    VOS_INT16                                    s1stRssi;
    VOS_UINT16                                   us1stPssRatio;

    VOS_UINT32                                   ul2ndFreq;
    VOS_INT16                                    s2ndRssi;
    VOS_UINT16                                   us2ndPssRatio;

    VOS_UINT32                                   ul3rdFreq;
    VOS_INT16                                    s3rdRssi;
    VOS_UINT16                                   us3rdPssRatio;

    VOS_UINT32                                   ulFreqBeforeRees;               /* 重建前频点 */
    VOS_UINT16                                   usPCellIdBeforeRees;            /* 重建前小区ID */
    VOS_UINT16                                   usBandBeforeRees;
    VOS_UINT32                                   ulEarfcnBeforeRees;

    VOS_UINT32                                   ulFreqAfterRees;               /* 重建选择的频点 */
    VOS_UINT16                                   usPCellIdAfterRees;            /* 重建选择的频点的小区ID */
    VOS_UINT16                                   usBandAfterRees;
    VOS_UINT32                                   ulGciAfterRees;                /* 重建前小区信息 */

    VOS_UINT32                                   ulSigDbNum;
    LRRC_OM_AIR_SIGNAL_INFO_STRU                 astAirSigDb[LRRC_OM_AIR_SIGNAL_INFO_ELE_NUM]; /* 重建立触发20s的信号db*/

    VOS_UINT32                                   ulEvtNum;
    LRRC_OM_LAYER_EVENT_INFO_STRU                astLayerEventDb[LRRC_OM_KEY_EVENT_DB_NUM];    /* 截止重建立流程结束的事件DB*/

    VOS_UINT32                                   ulMeasRptNum;
    LRRC_OM_MEASRPT_INFO                         astMeasRptDb[LRRC_OM_MEAS_RPT_DB_NUM];        /* 上行MeasRpt信息库 */

    /* VOS_UINT8                                    aucAirMsgBuff[LRRC_OM_AIR_MSG_BUF_LEN];  */     /* 原始AirMsg数据流*/
}LRRC_OM_REESTAB_STAT_INFO_STRU;

/*****************************************************************************
 结构名    : LRRC_OM_REESTAB_STAT_INFO_STRU
 结构说明  : 上报HIDS的结构
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgId;            /*_H2ASN_Skip*/
    LRRC_OM_REESTAB_STAT_INFO_STRU      stReestabStatInfo;
}LRRC_AUTOAN_REESTAB_INFO_IND;

/*****************************************************************************
 结构名    : LL2_OM_VOLTE_STAT_INFO_STRU
 协议表格  :
 ASN.1描述 :
 结构说明  : L2 Volte单秒统计信息
*****************************************************************************/
typedef struct
{
    VOS_UINT32                                ulMsTimeStamp;                    /* 转换成MS的时间戳 */

    LL2_OM_VOLTE_AUTOAN_EVENT_ENUM_UINT8      ucEvent;
    VOS_UINT8                                 ucRtpPktCnt;                      /* 1秒内下行接收的RTP报文数 */
    VOS_UINT8                                 ucRtpLossPktCnt;                  /* 1秒内下行接收的连续RTP丢包个数 */
    VOS_UINT8                                 ucRohcDecompFailCnt;              /* 1秒内报文ROHC解压缩失败的个数统计 */

    VOS_UINT16                                usTotalMACTb;                     /* 1秒内下行MAC TB块总的个数统计，包括其他数据 */
    VOS_UINT16                                usCrcErrMACTb;                    /* 1秒内下行MAC TB块CRC错的个数统计，包括其他数据 */
    VOS_UINT16                                usRtpJitter;                      /* 1秒内报文RTP抖动时间，MS */
    VOS_UINT16                                usRsv0;

    VOS_UINT32                                ulPdcpSendIpPktCnt;               /* PDCP 语音承载下行发送的报文总数 */

    VOS_UINT8                                 ucSilenceFrameCnt;                /*1秒内下行静默帧个数统计*/
    VOS_UINT8                                 ucRsv;
    VOS_UINT16                                usRsv;
}LL2_OM_VOLTE_STAT_INFO_STRU;

/*****************************************************************************
结构名    : LPS_OM_VOLTE_FAULT_AUTOAN_INFO_STRU
协议表格  :
ASN.1描述 :
结构说明  : Volte接入层详细自诊断信息，用户CHR或HIDS上报
*****************************************************************************/
typedef struct
{
    VOS_UINT32                                   ulMcc;                         /* Mcc */
    VOS_UINT32                                   ulMnc;

    VOS_UINT32                                   ulGci;
    LRRC_OM_TAC_STRU                             stTac;

    LRRC_LPDCP_VOLTE_FAULT_TRIG_ENUM_UINT8       enTrigSrc;                     /* 触发源:用户面识别的异常还是BSD识别的异常 */
    VOS_UINT8                                    ucRsv;
    LPS_OM_VOLTE_FAULT_CAUSE_ENUM_UINT16         enFaultCause;                  /* 综合分析的FaultCause */

    /* 自诊断分析识别出的异常点,信令面异常 */
    LPS_LOW_QUILITY_ABNORMAL_STRU                stLowQuiltyCause;

    VOS_UINT8                                    enGoodSigRlError;              /* 好信号无线链路问题，可能是UE底层异常 */
    VOS_UINT8                                    enGoodSigOutofSync;            /* 好信号失步 */
    VOS_UINT8                                    enGoodSigBler;                 /* 好信号误码较高 */
    VOS_UINT8                                    enNotMaxTxPwrRl;               /* 上行信号未推到最大发生RL异常点 */

    VOS_UINT8                                    enSrvccFail;                   /* SRVCC失败 */
    VOS_UINT8                                    enSrvccDelayAbnormal;          /* SRVCC时延异常 */
    VOS_UINT8                                    enSrvccSucc;                   /* SRVCC成功 */
    VOS_UINT8                                    enAirMsgCheckFail;             /* 空口消息ASN解码或检查失败 */

    VOS_UINT8                                    enInteVeriFail;                /* 完整性验证失败 */
    VOS_UINT8                                    enIntraHoFail;                 /* 发生了模内切换失败 */
    VOS_UINT8                                    enIntraHoSucc;                 /* IntraHo成功 */
    VOS_UINT8                                    enReestHappen;                 /* 发生了重建立 */

    VOS_UINT8                                    enOutOfSync;                   /* 发生了失步*/
    VOS_UINT8                                    enNotReasonableHoInd;          /* 切换的目标小区非合理 */
    VOS_UINT8                                    enLowQuilBeforeHo;             /* 切换前SNR低 */
    VOS_UINT8                                    enLowQuilBeforeSrvcc;          /* SRVCC前SNR低 */

    VOS_UINT8                                    enLowQuilty;                   /* 问题时段存在弱信号 */
    LRRC_OM_REEST_TRIG_ENUM_UINT8                enReesTrig;                    /* 如果发生重建立，记录重建立触发原因 */
    VOS_UINT8                                    enFastFading;                  /* 信号快衰 */
    VOS_UINT8                                    enRsv1;

    /* 自诊断分析识别出的异常点,用户面异常*/
    LL2_OM_VOLTE_AUTOAN_EVENT_ENUM_UINT8         enL2FaultEvent;
    VOS_UINT8                                    enDecipherFail;                /* 发生解密失败 */
    VOS_UINT8                                    enRohcFail;                    /* 发生头压缩失败 */
    VOS_UINT8                                    enRtpPktLostPdcpNoLost;        /* RTP有丢包，PDCP无丢包 */

    VOS_UINT32                                   usRtpConPktLostnum;            /* 问题时段，单位时间最大连续丢包个数 */
    VOS_UINT32                                   usRtpPktTotalLostnum;          /* 问题时段，总的RTP丢包数 */

    VOS_UINT8                                    enJitterFlg;                   /* 是否存在抖动 */
    VOS_UINT8                                    enJitterOrPktLostReachThre;    /* 综合抖动或丢包是否超过门限 */
    VOS_UINT16                                   usJitterTime;                  /* 问题时段，单位时间最大抖动时间 */

    VOS_UINT8                                    enRsv2;
    VOS_UINT8                                    enRsv3;
    VOS_UINT8                                    enRsv4;
    VOS_UINT8                                    enRsv5;

    /* 信息数据库中的原始信息 */
    VOS_UINT32                                   ulSigDbNum;
    LRRC_OM_AIR_SIGNAL_INFO_STRU                 astAirSigDb[LRRC_OM_AIR_SIGNAL_INFO_ELE_NUM]; /* Volte故障发生时刻20s左右的信号变化*/

    VOS_UINT32                                   ulEvtNum;
    LRRC_OM_LAYER_EVENT_INFO_STRU                astLayerEventDb[LRRC_OM_KEY_EVENT_DB_NUM];    /* Volte故障发生时刻20s左右关键事件DB */

    VOS_UINT32                                   ulMeasRptNum;
    LRRC_OM_MEASRPT_INFO                         astMeasRptDb[LRRC_OM_MEAS_RPT_DB_NUM];        /* Volte故障发生时刻测量报告交互过程 */

    VOS_UINT16                                   usFaultMsStamp;
    VOS_UINT16                                   usValidPktRptNum;
    LL2_OM_VOLTE_STAT_INFO_STRU                  astLL2PktOmDb[LRRC_LPDCP_VOLTE_STAT_NUM];     /* 逐秒统计L2用户面数据包统计信息 */
}LPS_OM_VOLTE_FAULT_AUTOAN_INFO_STRU;

/*****************************************************************************
 结构名    : LRRC_OM_VOLTE_AUTOAN_INFO_IND
 结构说明  : 上报HIDS的结构
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                               ulMsgId;            /*_H2ASN_Skip*/
    LPS_OM_VOLTE_FAULT_AUTOAN_INFO_STRU      stVolteFaultInd;
}LRRC_OM_VOLTE_AUTOAN_INFO_IND;


/*****************************************************************************
结构名    : LRRC_OM_VOLTE_GENERAL_STAT_INFO_STRU
协议表格  :
ASN.1描述 :
结构说明  : 重建立统计信息
*****************************************************************************/
typedef struct
{
    VOS_UINT32                                   ulGci;
    LRRC_OM_TAC_STRU                             stTac;

    VOS_UINT8                                    ucHour;
    VOS_UINT8                                    ucMin;
    VOS_UINT8                                    ucIfMoCall;
    VOS_UINT8                                    ucRsv0;
}LRRC_OM_VOLTE_SINGLE_CALL_INFO_STRU;

/*****************************************************************************
结构名    : LRRC_OM_VOLTE_SINGLE_FAULT_INFO_STRU
协议表格  :
ASN.1描述 :
结构说明  : 重建立统计信息
*****************************************************************************/
typedef struct
{
    VOS_UINT8                                    ucHour;
    VOS_UINT8                                    ucMin;
    VOS_UINT8                                    ucSec;
    LPS_OM_SENSOR_STAT_ENUM_UINT8                enSensorState;
    LPS_OM_VOLTE_FAULT_CAUSE_ENUM_UINT16         enCause;
    VOS_UINT8                                    ucRsrq;   /* 按照测量报告计算方法计算 */
    VOS_UINT8                                    ucRsrp;
}LRRC_OM_VOLTE_SINGLE_FAULT_INFO_STRU;


/*****************************************************************************
结构名    : LRRC_OM_VOLTE_GENERAL_STAT_INFO_STRU
协议表格  :
ASN.1描述 :
结构说明  : 重建立统计信息
*****************************************************************************/
typedef struct
{
    VOS_UINT32                                   ulMcc;                         /* Mcc */
    VOS_UINT32                                   ulMnc;

    VOS_UINT16                                   usYear;
    VOS_UINT8                                    ucMon;
    VOS_UINT8                                    ucDayOfMon;

    VOS_UINT16                                   usVolteCallNum;                /* VolteCall总次数 */
    VOS_UINT16                                   usMoCallNum;
    VOS_UINT32                                   ulTotalTimeLenInSec;           /* VolteCall总时长，秒单位 */

    VOS_UINT16                                   usBsdFaultTimes;               /* BSD识别的异常次数 */
    VOS_UINT16                                   usAutoFaultTimes;              /* 自诊断触发识别的异常次数 */

    VOS_UINT16                                   usPktLostMoreThan50Times;
    VOS_UINT16                                   usPktLostBet30and50Times;
    VOS_UINT16                                   usPktLostBet20and30Times;
    VOS_UINT16                                   usPktLostBet10and20Times;
    VOS_UINT16                                   usPktLostBet5and10Times;

    VOS_UINT16                                   usJitterMoreThan3SecTimes;
    VOS_UINT16                                   usJitterMoreBet2and3SecTimes;
    VOS_UINT16                                   usJitterMoreBet1and2SecTimes;
    VOS_UINT16                                   usJitterMoreBet500msand1SecTimes;
    VOS_UINT16                                   usJitterMoreBet300and500msTimes;

    VOS_UINT16                                   usStartHour;
    VOS_UINT16                                   usStartDay;
    VOS_UINT16                                   usDurationLenInMin;
    VOS_UINT16                                   usRsv3;

    VOS_UINT16                                   usNoPktTimes;
    VOS_UINT16                                   usLessPktTimes;

    VOS_UINT32                                   ulFaultCallNum;
    LRRC_OM_VOLTE_SINGLE_CALL_INFO_STRU          astFaultCallInfo[LRRC_VOLTE_GEN_STAT_CALL_NUM];

    VOS_UINT32                                   ulFaultInfoNum;
    LRRC_OM_VOLTE_SINGLE_FAULT_INFO_STRU         astFaultPointInfo[LRRC_VOLTE_GEN_STAT_FAULT_NUM];
}LPS_OM_VOLTE_GENERAL_STAT_INFO_STRU;



/*****************************************************************************
  6 UNION
*****************************************************************************/


/*****************************************************************************
  7 Extern Global Variable
*****************************************************************************/


/*****************************************************************************
  8 Fuction Extern
*****************************************************************************/


/*****************************************************************************
  9 OTHERS
*****************************************************************************/


#if (VOS_OS_VER == VOS_WIN32)
#pragma pack()
#else
#pragma pack(0)
#endif




#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of LrrcErrLogInterface.h */


