#ifndef LINUX_MMC_DSM_SDCARD_H
#define LINUX_MMC_DSM_SDCARD_H

#include <dsm/dsm_pub.h>

#define DSM_REPORT_UEVENT_TRUE 		  1
#define DSM_REPORT_UEVENT_FALSE		  0	

enum DSM_SDCARD_STATUS
{
	DSM_SDCARD_CMD8				= 0,
	DSM_SDCARD_CMD55			= 1,
	DSM_SDCARD_ACMD41			= 2,
	DSM_SDCARD_CMD2_R0 			= 3,
	DSM_SDCARD_CMD2_R1 			= 4,
	DSM_SDCARD_CMD2_R2 			= 5,
	DSM_SDCARD_CMD2_R3 			= 6,
	DSM_SDCARD_CMD3    			= 7,
	DSM_SDCARD_CMD9_R0			= 8,
	DSM_SDCARD_CMD9_R1			= 9,
	DSM_SDCARD_CMD9_R2			= 10,
	DSM_SDCARD_CMD9_R3			= 11,
	DSM_SDCARD_CMD7				= 12,
	DSM_SDCARD_CMD6_CMDERR      = 13,
	DSM_SDCARD_CMD6_DATERR      = 14,
	DSM_SDCARD_STATUS_BLK_STUCK_IN_PRG_ERR      = 15,
	DSM_SDCARD_STATUS_BLK_WR_SPEED_ERR      = 16,
	DSM_SDCARD_STATUS_BLK_RW_CHECK_ERR      = 17,
	DSM_SDCARD_STATUS_RO_ERR      = 18,
	DSM_SDCARD_STATUS_FILESYSTEM_ERR      = 19,
	DSM_SDCARD_STATUS_LOWSPEED_SPEC_ERR      = 20,
	DSM_SDCARD_REPORT_UEVENT      = 21,
	DSM_SDCARD_CMD_MAX,
};

enum DSM_SDCARD_ERR
{
	DSM_SDCARD_CMD2_RESP_ERR 		= 21300,
	DSM_SDCARD_CMD3_RESP_ERR 		= 21301,
	DSM_SDCARD_CMD6_RESP_ERR        = 21302,
	DSM_SDCARD_CMD7_RESP_ERR 		= 21303,
	DSM_SDCARD_CMD8_RESP_ERR 		= 21304,
	DSM_SDCARD_CMD9_RESP_ERR 		= 21305,
	DSM_SDCARD_CMD55_RESP_ERR		= 21306,
	DSM_SDCARD_ACMD41_RESP_ERR		= 21307,
	DSM_SDCARD_BLK_STUCK_IN_PRG_ERR		= 21308,
	DSM_SDCARD_BLK_WR_SPEED_ERR		= 21309,
	DSM_SDCARD_BLK_RW_CHECK_ERR		= 21310,
	DSM_SDCARD_RO_ERR		= 21311,
	DSM_SDCARD_FILESYSTEM_ERR		= 21312,
	DSM_SDCARD_LOWSPEED_SPEC_ERR		= 21313,
	DSM_SDCARD_NO_UEVENT_REPORT		= 21314,
};

struct dsm_sdcard_cmd_log
{
	char *log;
	u32  value;
};

extern struct dsm_client *sdcard_dclient;

extern char g_dsm_log_sum[1024];
extern struct dsm_sdcard_cmd_log dsm_sdcard_cmd_logs[];

extern char *dsm_sdcard_get_log(int cmd,int err);
extern void dsm_sdcard_report(int cmd, int err);
extern void dsm_sdcard_init(void);

#endif /* LINUX_MMC_DSM_SDCARD_H */
