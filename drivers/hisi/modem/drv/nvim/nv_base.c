

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/io.h>
#include <linux/err.h>
#include <linux/printk.h>
#include <linux/kthread.h>
#include <linux/vmalloc.h>
#include <linux/sched.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include <linux/cdev.h>
#include <linux/proc_fs.h>
#include <product_config.h>
#include <mdrv_rfile_common.h>
#include <osl_thread.h>
#include <bsp_nvim.h>
#include "nv_comm.h"
#include "nv_file.h"
#include "nv_ctrl.h"
#include "bsp_blk.h"
#include "nv_xml_dec.h"
#include "nv_debug.h"
#include "nv_index.h"
#include "nv_crc.h"
#include "nv_partition_img.h"
#include "nv_partition_bakup.h"
#include "NVIM_ResumeId.h"
#include "bsp_dump.h"
#include "nv_msg.h"
#include "nv_proc.h"



u32 nv_readEx(u32 modem_id,u32 itemid,u32 offset,u8* pdata,u32 datalen)
{
    u32 ret;
    nv_file_info_s file_info = {0};
    nv_item_info_s item_info = {0};
    nv_rd_req      rreq;
    nv_ctrl_info_s* ctrl_info = (nv_ctrl_info_s*)NV_GLOBAL_CTRL_INFO_ADDR;

    nv_debug(NV_FUN_READ_EX,0,itemid,modem_id,datalen);

    if((NULL == pdata)||(0 == datalen))
    {
        nv_debug(NV_FUN_READ_EX,1,itemid,0,0);
        return BSP_ERR_NV_INVALID_PARAM;
    }

    ret = nv_search_byid(itemid,((u8*)NV_GLOBAL_CTRL_INFO_ADDR),&item_info,&file_info);
    if(ret)
    {
        nv_printf("can not find 0x%x !\n",itemid);
        return BSP_ERR_NV_NO_THIS_ID;
    }

    if((offset + datalen) > item_info.nv_len)
    {
        ret = BSP_ERR_NV_ITEM_LEN_ERR;
        nv_debug(NV_FUN_READ_EX,3,offset,datalen,item_info.nv_len);
        goto nv_readEx_err;
    }

    if((modem_id == 0) || (modem_id > ctrl_info->modem_num))
    {
        ret = BSP_ERR_NV_INVALID_MDMID_ERR;
        nv_debug(NV_FUN_READ_EX,4,ret,itemid,modem_id);
        goto nv_readEx_err;
    }

    if(modem_id > item_info.modem_num)
    {
#if (FEATURE_OFF == FEATURE_MULTI_MODEM)
        ret = BSP_ERR_NV_INVALID_MDMID_ERR;
        nv_debug(NV_FUN_READ_EX,5,ret,itemid,modem_id);
        goto nv_readEx_err;
#else
        modem_id = 1;
#endif
    }

    rreq.itemid  = itemid;
    rreq.modemid = modem_id;
    rreq.offset  = offset;
    rreq.pdata   = pdata;
    rreq.size    = (datalen < item_info.nv_len) ? datalen : item_info.nv_len;
    (void)nv_read_from_mem(&rreq, &item_info);

    nv_debug_trace(pdata, datalen);

    return NV_OK;

nv_readEx_err:
    nv_mntn_record("\n[%s]:[0x%x]:[%d] 0x%x\n",__FUNCTION__,itemid,modem_id, ret);
    nv_help(NV_FUN_READ_EX);
    return ret;
}

u32 nv_writeEx(u32 modem_id, u32 itemid, u32 offset, u8* pdata, u32 datalen)
{
    u32 ret;
    nv_file_info_s  file_info = {0};
    nv_item_info_s  item_info = {0};
    nv_wr_req       wreq;
    u8  test_byte;
    nv_ctrl_info_s* ctrl_info = (nv_ctrl_info_s*)NV_GLOBAL_CTRL_INFO_ADDR;

    nv_debug(NV_FUN_WRITE_EX,0,itemid,modem_id,datalen);
    nv_debug_record(NV_DEBUG_WRITEEX_START|itemid<<16);

    if((NULL == pdata)||(0 == datalen))
    {
        nv_debug(NV_FUN_WRITE_EX,1,itemid,datalen,0);
        return BSP_ERR_NV_INVALID_PARAM;
    }

    /* test pdata is accessable */
    test_byte = *pdata;
    UNUSED(test_byte);

    ret = nv_search_byid(itemid,((u8*)NV_GLOBAL_CTRL_INFO_ADDR),&item_info,&file_info);
    if(ret)
    {
        nv_printf("can not find 0x%x !\n",itemid);
        nv_debug(NV_FUN_WRITE_EX,2,itemid,modem_id,offset);
        return BSP_ERR_NV_NO_THIS_ID;
    }

    nv_debug_trace(pdata, datalen);

    if((datalen + offset) >item_info.nv_len)
    {
        ret = BSP_ERR_NV_ITEM_LEN_ERR;
        nv_debug(NV_FUN_WRITE_EX,3,itemid,datalen,item_info.nv_len);
        goto nv_writeEx_err;
    }

    if((modem_id == 0) || (modem_id > ctrl_info->modem_num))
    {
        ret = BSP_ERR_NV_INVALID_MDMID_ERR;
        nv_debug(NV_FUN_WRITE_EX,4,itemid,datalen,item_info.nv_len);
        goto nv_writeEx_err;
    }

    if(modem_id > item_info.modem_num)
    {
#if (FEATURE_OFF == FEATURE_MULTI_MODEM)
        ret = BSP_ERR_NV_INVALID_MDMID_ERR;
        nv_debug(NV_FUN_WRITE_EX,5,itemid,datalen,item_info.nv_len);
        goto nv_writeEx_err;
#else
        modem_id = 1;
#endif
    }

    /* check crc before write */
    if(nv_crc_need_check_inwr(&item_info, datalen))
    {
        ret = nv_crc_check_item(&item_info, modem_id);
        if(ret)
        {
            nv_debug(NV_FUN_WRITE_EX, 6, itemid,datalen,ret);
            ret = nv_resume_item(&item_info, itemid, modem_id);
            if(ret)
            {
                nv_debug(NV_FUN_WRITE_EX,7, itemid, modem_id, ret);
                goto nv_writeEx_err;
            }
        }
    }

    wreq.itemid    = itemid;
    wreq.modemid   = modem_id;
    wreq.offset    = offset;
    wreq.pdata     = pdata;
    wreq.size      = datalen;
    nv_debug_record(NV_DEBUG_WRITEEX_MEM_START);
    ret = nv_write_to_mem(&wreq, &item_info);
    if(ret)
    {
        nv_debug(NV_FUN_WRITE_EX,8,itemid,datalen,0);
        goto nv_writeEx_err;
    }

    nv_debug_record(NV_DEBUG_WRITEEX_FILE_START);
    ret = nv_write_to_file(&wreq, &item_info);
    if(ret)
    {
        nv_debug(NV_FUN_WRITE_EX,9,itemid,datalen,ret);
        goto nv_writeEx_err;
    }
    nv_debug_record(NV_DEBUG_WRITEEX_END|itemid<<16);

    return NV_OK;

nv_writeEx_err:
    nv_mntn_record("\n[%s]:[0x%x]:[%d]\n",__FUNCTION__,itemid,modem_id);
    nv_help(NV_FUN_WRITE_EX);
    return ret;
}



u32 bsp_nvm_get_nv_num(void)
{
    struct nv_ctrl_file_info_stru* ctrl_info = (struct nv_ctrl_file_info_stru*)NV_GLOBAL_CTRL_INFO_ADDR;
    return ctrl_info->ref_count;
}

u32 bsp_nvm_get_nvidlist(NV_LIST_INFO_STRU*  nvlist)
{
    u32 i;
    struct nv_ctrl_file_info_stru* ctrl_info = (struct nv_ctrl_file_info_stru*)NV_GLOBAL_CTRL_INFO_ADDR;
    struct nv_ref_data_info_stru* ref_info   = (struct nv_ref_data_info_stru*)((unsigned long)NV_GLOBAL_CTRL_INFO_ADDR+NV_GLOBAL_CTRL_INFO_SIZE\
        +NV_GLOBAL_FILE_ELEMENT_SIZE*ctrl_info->file_num);

    if(NULL == nvlist)
    {
        return NV_ERROR;
    }

    for(i = 0;i<ctrl_info->ref_count;i++)
    {
        nvlist[i].usNvId       = ref_info[i].itemid;
        nvlist[i].ucNvModemNum = ref_info[i].modem_num;
    }
    return NV_OK;
}

u32 bsp_nvm_get_len(u32 itemid,u32* len)
{
    u32 ret;
    struct nv_ref_data_info_stru ref_info = {0};
    struct nv_file_list_info_stru file_info = {0};

    nv_debug(NV_API_GETLEN,0,itemid,0,0);
    if(NULL == len)
    {
        nv_debug(NV_API_GETLEN,1,itemid,0,0);
        return BSP_ERR_NV_INVALID_PARAM;
    }

    /*check init state*/
    if(false == nv_read_right())
    {
        nv_debug(NV_API_GETLEN,3,itemid,0,0);
        return BSP_ERR_NV_MEM_INIT_FAIL;
    }
    ret = nv_search_byid(itemid,(u8*)NV_GLOBAL_CTRL_INFO_ADDR,&ref_info, &file_info);
    if(NV_OK == ret)
    {
        *len = ref_info.nv_len;
        return NV_OK;
    }
    return ret;
}

u32 bsp_nvm_authgetlen(u32 itemid,u32* len)
{
    return bsp_nvm_get_len(itemid,len);
}



u32 bsp_nvm_dcread_direct(u32 modem_id,u32 itemid, u8* pdata,u32 datalen)
{
    return nv_readEx(modem_id,itemid,0,pdata,datalen);
}

u32 bsp_nvm_dcread(u32 modem_id,u32 itemid, u8* pdata,u32 datalen)
{
    /*check init state*/
    if(false == nv_read_right())
    {
        return BSP_ERR_NV_MEM_INIT_FAIL;
    }

    return nv_readEx(modem_id,itemid,0,pdata,datalen);
}

u32 bsp_nvm_auth_dcread(u32 modem_id,u32 itemid, u8* pdata,u32 datalen)
{
    return bsp_nvm_dcread(modem_id,itemid,pdata,datalen);
}

u32 bsp_nvm_dcreadpart(u32 modem_id,u32 itemid,u32 offset,u8* pdata,u32 datalen)
{
    /*check init state*/
    if(false == nv_read_right())
    {
        return BSP_ERR_NV_MEM_INIT_FAIL;
    }

    return nv_readEx(modem_id,itemid,offset,pdata,datalen);
}

u32 bsp_nvm_dcwrite(u32 modem_id,u32 itemid, u8* pdata,u32 datalen)
{
    if(false == nv_write_right())
    {
        return BSP_ERR_NV_MEM_INIT_FAIL;
    }

    return nv_writeEx(modem_id,itemid,0,pdata,datalen);
}

u32 bsp_nvm_auth_dcwrite(u32 modem_id,u32 itemid, u8* pdata,u32 datalen)
{
    return bsp_nvm_dcwrite(modem_id,itemid,pdata,datalen);
}

u32 bsp_nvm_dcwritepart(u32 modem_id,u32 itemid, u32 offset,u8* pdata,u32 datalen)
{
    if(false == nv_write_right())
    {
        return BSP_ERR_NV_MEM_INIT_FAIL;
    }

    return nv_writeEx(modem_id,itemid,offset,pdata,datalen);
}

u32 bsp_nvm_dcwrite_direct(u32 modem_id,u32 itemid, u8* pdata,u32 datalen)
{
    return nv_writeEx(modem_id,itemid,0,pdata,datalen);
}



u32 bsp_nvm_flush(void)
{
    u32 ret;
    struct nv_global_ddr_info_stru* ddr_info = (struct nv_global_ddr_info_stru*)NV_GLOBAL_INFO_ADDR;

    ret = nv_flush_wrbuf(ddr_info);
    if (ret) {
        nv_printf("Fail to flush low priority write buffer \n");
    }

    ret = nv_send_msg_sync(NV_TASK_MSG_FLUSH, 0, 0);
    return ret;
}


u32 bsp_nvm_flushSys(void)
{
    u32 ret = NV_ERROR;
    FILE* fp = NULL;
    u32 ulTotalLen = 0;
    struct nv_global_ddr_info_stru* ddr_info = (struct nv_global_ddr_info_stru*)NV_GLOBAL_INFO_ADDR;

    nv_create_flag_file((s8*)NV_SYS_FLAG_PATH);

    nv_debug(NV_FUN_FLUSH_SYS,0,0,0,0);
    if(nv_file_access((s8*)NV_FILE_SYS_NV_PATH,0))
    {
        fp = nv_file_open((s8*)NV_FILE_SYS_NV_PATH,(s8*)NV_FILE_WRITE);
    }
    else
    {
        fp = nv_file_open((s8*)NV_FILE_SYS_NV_PATH,(s8*)NV_FILE_RW);
    }
    if(NULL == fp)
    {
        nv_debug(NV_FUN_FLUSH_SYS,1,ret,0,0);
        ret = BSP_ERR_NV_NO_FILE;
        goto nv_flush_err;
    }
    ulTotalLen = ddr_info->file_len;
    /*在nvdload分区文件末尾置标志0xabcd8765*/
    *( unsigned int* )( NV_GLOBAL_CTRL_INFO_ADDR + ddr_info->file_len )
        = ( unsigned int )NV_FILE_TAIL_MAGIC_NUM;
    ulTotalLen += sizeof(unsigned int);
    /*系统分区数据不做CRC校验，因此回写时不考虑CRC校验码的存放位置*/
    ret = (u32)nv_file_write((u8*)NV_GLOBAL_CTRL_INFO_ADDR,1,ulTotalLen,fp);
    nv_file_close(fp);
    if(ret != ulTotalLen)
    {
        nv_debug(NV_FUN_FLUSH_SYS,3,ret,ulTotalLen,0);
        ret = BSP_ERR_NV_WRITE_FILE_FAIL;
        goto nv_flush_err;
    }

    nv_delete_flag_file((s8*)NV_SYS_FLAG_PATH);
    return NV_OK;

nv_flush_err:
    nv_mntn_record("\n[%s]\n",__func__);
    nv_help(NV_FUN_FLUSH_SYS);
    return ret;
}


/*
* Function   : bsp_nvm_backup
* Discription: backup the memory to file/flash/rfile
* Parameter  : none
* Output     : result
* History    : yuyangyang  00228784  create
* 备注       : 由于备份内存nv到备份区的过程中，需要保证内存中的NV数据不被改写，备份过程中需要锁住内存，
*              那么对该接口的使用，要有场景上的限制，目前使用场景包括:
*              1. 提供给用户接口mdrv_nv_backup使用，目前没人使用；
*              2. bsp_nvm_update_default，产线专用接口，用于at命令^inforbu，不会有并发写操作；
*              3. nv_file_flag_check，启动过程中检查写入完成性的操作，不会有并发写操作；
*              4. nv_data_writeback, 升级完成之后，讲升级后的数据写回备份区，不会有并发写操作；
*                                    启动加载过程中，从img分区和备份分区都加载失败，需要从出厂区做紧急恢复，
*                                    恢复完成后写回备份区，不会有并发写操作。
*               从现有场景看，bsp_nvm_backup接口过程中锁内存不会产生风险，维护时需要持续关注。
*/
u32 bsp_nvm_backup(u32 crc_flag)
{
    u32 ret = NV_ERROR;
    nv_global_info_s* ddr_info = (nv_global_info_s*)NV_GLOBAL_INFO_ADDR;
    FILE* fp = NULL;
    u32 writeLen = 0;

    nv_debug(NV_API_BACKUP,0,0,0,0);

    if( (ddr_info->acore_init_state != NV_INIT_OK)&&
        (ddr_info->acore_init_state != NV_KERNEL_INIT_DOING))
    {
        return NV_ERROR;
    }

    nv_create_flag_file((s8*)NV_BACK_FLAG_PATH);

    if(nv_file_access((s8*)NV_BACK_PATH,0))
    {
        fp = nv_file_open((s8*)NV_BACK_PATH,(s8*)NV_FILE_WRITE);
    }
    else
    {
        fp = nv_file_open((s8*)NV_BACK_PATH,(s8*)NV_FILE_RW);
    }
    if(NULL == fp)
    {
        ret = BSP_ERR_NV_NO_FILE;
        nv_debug(NV_API_BACKUP,1,ret,0,0);
        goto nv_backup_fail;
    }

    writeLen = ddr_info->file_len;

    if(NV_FLAG_NEED_CRC == crc_flag)
    {
        ret = nv_crc_check_ddr(NV_RESUME_NO);
        if(ret)
        {
            nv_debug(NV_API_BACKUP,2,ret,0, 0);
            (void)nv_debug_store_ddr_data();
            goto nv_backup_fail;
        }
    }

    /* 如果需要进行CRC校验, 备份数据到备份区，内存中的数据较备份区更新，所以crc check不能带自动恢复,
       要保证写入备份区的数据，crc校验正确，同时备份过程中内存数据不被改写，所以需要锁住内存 */
    nv_ipc_sem_take(IPC_SEM_NV_CRC, IPC_SME_TIME_OUT);
    ret = (u32)nv_file_write((u8*)NV_GLOBAL_CTRL_INFO_ADDR,1,writeLen,fp);
    nv_ipc_sem_give(IPC_SEM_NV_CRC);
    nv_file_close(fp);
    fp = NULL;
    if(ret != writeLen)
    {
        nv_debug(NV_API_BACKUP,3,ret,writeLen,0);
        ret = BSP_ERR_NV_WRITE_FILE_FAIL;
        goto nv_backup_fail;
    }

    (void)nv_bakup_info_reset();

    if(nv_file_update(NV_BACK_PATH))
    {
        nv_debug(NV_API_BACKUP, 4 , 0, 0, 0);
    }

    nv_delete_flag_file((s8*)NV_BACK_FLAG_PATH);

    return NV_OK;
nv_backup_fail:
    if(fp){nv_file_close(fp);}
    nv_mntn_record("\n[%s]\n",__FUNCTION__);
    nv_help(NV_API_BACKUP);
    return ret;

}

#ifdef CONFIG_MULTI_CARRIER
/*
* Function   : bsp_nvm_mtcarrier_backup
* Discription: flush the memory to sys sec for system use
* Parameter  : none
* Output     : result
* History    :
*/
u32 bsp_nvm_mtcarrier_backup(char* back_path)
{
    u32 ret = NV_ERROR;
    s32 routine_len = 0;
    struct nv_global_ddr_info_stru* ddr_info = (struct nv_global_ddr_info_stru*)NV_GLOBAL_INFO_ADDR;
    FILE* mt_fp = NULL;
    char  mtca_rontine[MTCA_ROUTINE_MAX_LENGTH];

    if(NULL == back_path)
    {
        ret = BSP_ERR_NV_INVALID_PARAM;
        printk("here path error\n");
        nv_debug(NV_FUN_MTCARRIER_BACKUP,1,ret,0,0);  /* para2 is 0,is right? */
        return NV_ERROR;
    }

    if( (ddr_info->acore_init_state != NV_INIT_OK)&&
        (ddr_info->acore_init_state != NV_KERNEL_INIT_DOING))
    {
        printk("%s init error\n",__FUNCTION__);
        return NV_ERROR;
    }

    if(nv_file_access((s8*)NV_BACK_MTCARRIER_FLAG_PATH,0))
    {
        mt_fp = nv_file_open((s8*)NV_BACK_MTCARRIER_FLAG_PATH,(s8*)NV_FILE_WRITE);
    }
    else
    {
        mt_fp = nv_file_open((s8*)NV_BACK_MTCARRIER_FLAG_PATH,(s8*)NV_FILE_RW);
    }
    ret = (u32)nv_file_write((u8*)NV_BACK_MTCARRIER_FLAG_PATH,1,sizeof(back_path),mt_fp);
    nv_file_close(mt_fp);
    if(ret!=sizeof(back_path))
    {
        printk("%s save rountine error, ret is %d\n",mtca_rontine,ret);
        return NV_ERROR;
    }

    /* packet a nv.bin rountine */
    routine_len = snprintf(mtca_rontine,(MTCA_ROUTINE_MAX_LENGTH-1),"%s/nv.bin",back_path);
    if(!routine_len)
    {
        printk("%s rountine error\n",mtca_rontine);
        return NV_ERROR;
    }

    if(nv_file_access((s8*)mtca_rontine,0))
    {
        mt_fp = nv_file_open((s8*)mtca_rontine,(s8*)NV_FILE_WRITE);
    }
    else
    {
        mt_fp = nv_file_open((s8*)mtca_rontine,(s8*)NV_FILE_RW);
    }

    if(NULL == mt_fp)
    {
        ret = BSP_ERR_NV_NO_FILE;
        nv_debug(NV_FUN_MTCARRIER_BACKUP,2,ret,0,0);
        printk("open error\n");
        goto nv_backup_mtcarrierr_fail;
    }

    ret = (u32)nv_file_write((u8*)NV_GLOBAL_CTRL_INFO_ADDR,1,ddr_info->file_len,mt_fp);
    nv_file_close(mt_fp);
    mt_fp = NULL;
    if(ret != ddr_info->file_len)
    {
        nv_debug(NV_FUN_MTCARRIER_BACKUP,3,ret,ddr_info->file_len,0);
        ret = BSP_ERR_NV_WRITE_FILE_FAIL;
        printk("write error\n");
        goto nv_backup_mtcarrierr_fail;
    }

    //nv_delete_flag_file((s8*)NV_BACK_MTCARRIER_FLAG_PATH);

    return NV_OK;
nv_backup_mtcarrierr_fail:
    nv_mntn_record("\n[%s]\n",__FUNCTION__);
    nv_help(NV_FUN_MTCARRIER_BACKUP);
    return ret;
}
#endif

/*
* Function   : bsp_nvm_update_default
* Discription: update mem data to default section
* Parameter  : none
* Output     : result
* History    : yuyangyang  00228784  create
* 备注       : 产线专用接口，用于at命令^inforbu，此时要保证没有其他写NV的动作，
*              备份出厂分区的过程中会锁住NV内存不让写，
*              如果此时有写NV动作，可能会导致获取内存写权限时间超长，引起未知的程序错误
*/
u32 bsp_nvm_update_default(void)
{
    u32 ret;
    FILE* fp = NULL;
    u32 datalen = 0;
    struct nv_global_ddr_info_stru* ddr_info = (struct nv_global_ddr_info_stru*)NV_GLOBAL_INFO_ADDR;
    NV_SELF_CTRL_STRU self_ctrl = {0};

    nv_debug(NV_FUN_UPDATE_DEFAULT,0,0,0,0);

    if(ddr_info->acore_init_state != NV_INIT_OK)
    {
        return NV_ERROR;
    }

#if defined(BSP_CONFIG_PHONE_TYPE)
    if(false == nv_check_update_default_right())
    {
        nv_printf("no right to write factory data!\n");
    }
#endif

    ret = bsp_nvm_read(NV_ID_DRV_SELF_CTRL, (u8*)&self_ctrl, (u32)sizeof(NV_SELF_CTRL_STRU));
    if(ret)
    {
        nv_printf("read nv 0x%x fail,ret = 0x%x\n", NV_ID_DRV_SELF_CTRL, ret);
        return NV_ERROR;
    }
    self_ctrl.ulResumeMode = NV_MODE_USER;
    ret = bsp_nvm_write(NV_ID_DRV_SELF_CTRL, (u8*)&self_ctrl, (u32)sizeof(NV_SELF_CTRL_STRU));
    if(ret)
    {
        nv_printf("write nv 0x%x fail,ret = 0x%x\n", NV_ID_DRV_SELF_CTRL, ret);
        return NV_ERROR;
    }

    /*在写入文件前进行CRC校验，以防数据不正确*/
    ret = nv_crc_check_ddr(NV_RESUME_NO);
    if(ret)
    {
        nv_debug(NV_FUN_UPDATE_DEFAULT,3,ret,ddr_info->file_len,0);
        (void)nv_debug_store_ddr_data();
        goto nv_update_default_err;
    }

    if(nv_file_access((s8*)NV_DEFAULT_PATH,0))
    {
        fp = nv_file_open((s8*)NV_DEFAULT_PATH,(s8*)NV_FILE_WRITE);
    }
    else
    {
        fp = nv_file_open((s8*)NV_DEFAULT_PATH,(s8*)NV_FILE_RW);
    }
    if(NULL == fp)
    {
        ret = BSP_ERR_NV_NO_FILE;
        nv_debug(NV_FUN_UPDATE_DEFAULT,2,ret,0,0);
        goto nv_update_default_err;
    }

    /* 锁住NV内存，在写入文件前进行CRC校验，以防数据不正确，
       同时要保证当前的crc check不做自动恢复，带自动恢复的crc check会在恢复过程中获取ipc semaphore，导致死锁*/
    ret = nv_ipc_sem_take(IPC_SEM_NV_CRC, IPC_SME_TIME_OUT);
    if(ret)
    {
        nv_debug(NV_FUN_UPDATE_DEFAULT, 3, ret,0,0);
        goto nv_update_default_err;
    }
    datalen = (u32)nv_file_write((u8*)NV_GLOBAL_CTRL_INFO_ADDR,1,ddr_info->file_len,fp);
    nv_ipc_sem_give(IPC_SEM_NV_CRC);

    nv_file_close(fp);
    if(datalen != ddr_info->file_len)
    {
        nv_debug(NV_FUN_UPDATE_DEFAULT,6,ret,ddr_info->file_len,0);
        ret = BSP_ERR_NV_WRITE_FILE_FAIL;
        goto nv_update_default_err;
    }

    ret = bsp_nvm_backup(NV_FLAG_NO_CRC);
    if(ret)
    {
        nv_debug(NV_FUN_UPDATE_DEFAULT,7,ret,0,0);
        goto nv_update_default_err;
    }

#if defined(BSP_CONFIG_PHONE_TYPE)
    nv_delete_update_default_right();
#endif

    return NV_OK;
nv_update_default_err:
    /* coverity[deref_arg] */
    if(fp){nv_file_close(fp);}
    nv_mntn_record("\n[%s]\n",__FUNCTION__);
    nv_help(NV_FUN_UPDATE_DEFAULT);
    return ret;
}



u32 bsp_nvm_revert_default(void)
{
    u32 ret;

    /* 先把出厂区所有NV项恢复回来 */
    ret = nv_revert_data((s8*)NV_DEFAULT_PATH, NULL, 0, NV_FLAG_NEED_CRC);
    if(ret)
    {
        nv_mntn_record("revert from default partition failed, ret=0x%x\n!", ret);
        return ret;
    }

    /*机要nv项不恢复,重新恢复成img分区里的数据，需要重新生成CRC校验码*/
    ret = nv_revert_data(NV_IMG_PATH,g_ausNvResumeSecureIdList,\
        (u32)bsp_nvm_getRevertNum((unsigned long)NV_SECURE_ITEM), NV_FLAG_NEED_CRC);
    if(ret)
    {
        nv_mntn_record("revert keeping secure list failed, ret=0x%x\n!", ret);
        return ret;
    }

    ret = nv_img_flush_all();
    ret |= bsp_nvm_flushSys();

    nv_mntn_record("bsp_nvm_revert_default success!\n!");

    return ret;
}

u32 nv_write2file_handle(nv_cmd_req *msg)
{
    u32 ret;
    nv_item_info_t *nv_info;
    nv_flush_item_s flush_item;

    nv_info = &msg->nv_item_info;
    flush_item.itemid = nv_info->itemid;
    flush_item.modemid = nv_info->modem_id;
    ret = nv_flushItem(&flush_item);
    if (ret)
    {
        return ret;
    }

    if (true == nv_isSysNv(nv_info->itemid))
    {
        ret = bsp_nvm_flushSys();
    }

    return ret;
}

void bsp_nvm_icc_task(void* parm)
{
    u32 ret = NV_ERROR;
    nv_cmd_req *msg;
    nv_item_info_t *nv_info;
	
    /* coverity[self_assign] */
    parm = parm;

    /* coverity[no_escape] */
    for(;;)
    {
        osl_sem_down(&g_nv_ctrl.task_sem);

        g_nv_ctrl.opState = NV_OPS_STATE;

        /*如果当前处于睡眠状态，则等待唤醒处理*/
        if(g_nv_ctrl.pmState == NV_SLEEP_STATE)
        {
            printk("%s cur state in sleeping,wait for resume end!\n",__func__);
            osl_sem_down(&g_nv_ctrl.suspend_sem);
        }

        msg = nv_get_cmd_req();
        if (msg == NULL) {
            g_nv_ctrl.opState = NV_IDLE_STATE;
            continue;
        }

        nv_debug_printf("msg type:0x%x\n", msg->msg_type);
        nv_info = &msg->nv_item_info;
        switch (msg->msg_type) {
            case NV_TASK_MSG_WRITE2FILE:
                ret = nv_write2file_handle(msg);
                break;

            case NV_TASK_MSG_FLUSH:
                /* there is no actual nv operation, return NV_OK and notify
                           * NV writing process the result actually
                           */
                 ret = NV_OK;
                break;

            case NV_TASK_MSG_RESUEM:
                ret = nv_resume_ddr_from_img();
                break;

            case NV_TASK_MSG_RESUEM_ITEM:
                ret = nv_resume_item(NULL, nv_info->itemid, nv_info->modem_id);
                break;

            default:
                nv_printf("msg type invalid, msg type;0x%x\n", msg->msg_type);
                break;
        }

        nv_debug_printf("deal msg ok\n");
        if (ret) {
            nv_mntn_record("flush nv to file fail, msg type:0x%x errno:0x%x\n", msg->msg_type, ret);
        }

        if (msg->nv_msg_callback) {
            msg->nv_msg_callback(ret, msg->sn);
        }

        nv_put_cmd_req(msg);
        g_nv_ctrl.task_proc_count++;
        g_nv_ctrl.opState = NV_IDLE_STATE;
    }
}


u32 bsp_nvm_xml_decode(void)
{
    u32 ret = NV_ERROR;

    if(!nv_file_access(NV_XNV_CARD1_PATH,0))
    {
        ret = nv_xml_decode(NV_XNV_CARD1_PATH,NV_XNV_CARD1_MAP_PATH,NV_USIMM_CARD_1);
        if(ret)
        {
            return ret;
        }
    }

    if(!nv_file_access(NV_XNV_CARD2_PATH,0))
    {
        ret = nv_xml_decode(NV_XNV_CARD2_PATH,NV_XNV_CARD2_MAP_PATH,NV_USIMM_CARD_2);
        if(ret)
        {
            return ret;
        }
    }

    if(!nv_file_access(NV_XNV_CARD3_PATH,0))
    {
        ret = nv_xml_decode(NV_XNV_CARD3_PATH, NV_XNV_CARD3_MAP_PATH, NV_USIMM_CARD_3);
        if(ret)
        {
            return ret;
        }
    }

    /*CUST XML 无对应MAP文件，传入空值即可*/
    if(!nv_file_access(NV_CUST_CARD1_PATH,0))
    {
        ret = nv_xml_decode(NV_CUST_CARD1_PATH,NULL,NV_USIMM_CARD_1);
        if(ret)
        {
            return ret;
        }
    }

    if(!nv_file_access(NV_CUST_CARD2_PATH,0))
    {
        ret = nv_xml_decode(NV_CUST_CARD2_PATH,NULL,NV_USIMM_CARD_2);
        if(ret)
        {
            return ret;
        }
    }

    if(!nv_file_access(NV_CUST_CARD3_PATH,0))
    {
        ret = nv_xml_decode(NV_CUST_CARD3_PATH, NULL, NV_USIMM_CARD_3);
        if(ret)
        {
            return ret;
        }
    }

    return NV_OK;
}

#ifdef CONFIG_MULTI_CARRIER
/*
* Function   : bsp_nvm_mtcarrier_cust_xml_decode
* Discription: flush the memory to sys sec for system use
* Parameter  : none
* Output     : result
* History    :
*/
u32 bsp_nvm_mtcarrier_cust_xml_decode(void)
{
    u32 ret = NV_ERROR;
    s32 routine_len ;
    s32 read_len= 0;
    s8* norm_routine = NULL;
    s8  mtca_cust_rontine[MTCA_ROUTINE_MAX_LENGTH] = {};
    FILE* mt_fp = NULL;

    if(nv_file_access((s8*)NV_BACK_MTCARRIER_FLAG_PATH,0))
    {
        mt_fp = nv_file_open((s8*)NV_BACK_MTCARRIER_FLAG_PATH,(s8*)NV_FILE_READ);
    }
    else
    {
        printk("get %s rountine error\n",NV_BACK_MTCARRIER_FLAG_PATH);
        return NV_ERROR;
    }
    read_len = nv_get_file_len(mt_fp);
    if(read_len<0)
    {
        printk("read length error, read_len is %d\n",read_len);
        return NV_ERROR;
    }
    norm_routine = (s8*)nv_malloc((size_t)read_len);
    if(!norm_routine)
    {
        printk("malloc fail, len:0x%x\n",read_len);
        return NV_ERROR;
    }
    ret = nv_file_read(norm_routine, 1, read_len, mt_fp);
    if(ret != read_len)
    {
        nv_free(norm_routine);
        printk("read length not equal,ret is %d,read_len is %d \n",ret,read_len);
        return NV_ERROR;
    }
    nv_file_close(mt_fp);


    /*get path*/
    routine_len = snprintf(mtca_cust_rontine,(MTCA_ROUTINE_MAX_LENGTH-1),"%s/cust.xml",norm_routine);
    nv_free(norm_routine);
    norm_routine = NULL;
    if(!routine_len)
    {
        printk("%s rountine error\n",mtca_cust_rontine);
        return NV_ERROR;
    }

    if(NULL==mtca_cust_rontine)
    {
        ret = BSP_ERR_NV_INVALID_PARAM;
        return NV_ERROR;
    }

    /*MUTI CARRIER CUST XML 无对应MAP文件，传入空值即可*/
    if(!nv_file_access(mtca_cust_rontine,0))
    {
        ret = nv_xml_decode(mtca_cust_rontine,NULL,NV_USIMM_CARD_1);
        if(ret)
        {
            return ret;
        }
    }

    /*CUST XML 无对应MAP文件，传入空值即可*/
    if(!nv_file_access(mtca_cust_rontine,0))
    {
        ret = nv_xml_decode(mtca_cust_rontine,NULL,NV_USIMM_CARD_2);
        if(ret)
        {
            return ret;
        }
    }

    return ret;
}

/*
* Function   : bsp_nvm_revert_mtcarrier_user
* Discription: flush the memory to sys sec for system use
* Parameter  : none
* Output     : result
* History    :
*/
u32 bsp_nvm_revert_mtcarrier_user(void)
{
    s8  mtca_user_rontine[MTCA_ROUTINE_MAX_LENGTH] = {};
    s32 ret = 0;
    s32 read_len= 0;
    u32 file_length = 0;
    struct nv_mtcarrier_userlist_bin_stru nv_mt_bin_list = {0};
    FILE* mt_fp = NULL;
    s8* norm_routine = NULL;
    u16* nvidlist = NULL;
    s32 routine_len ;

    /*get path*/
    if(nv_file_access((s8*)NV_BACK_MTCARRIER_FLAG_PATH,0))
    {
        mt_fp = nv_file_open((s8*)NV_BACK_MTCARRIER_FLAG_PATH,(s8*)NV_FILE_READ);
    }
    else
    {
        printk("get %s rountine error\n",NV_BACK_MTCARRIER_FLAG_PATH);
        return NV_ERROR;
    }
    read_len = nv_get_file_len(mt_fp);
    if(read_len<0)
    {
        printk("read length error, read_len is %d\n",read_len);
        return NV_ERROR;
    }
    norm_routine = (s8*)nv_malloc((size_t)read_len);
    if(!norm_routine)
    {
        printk("malloc fail, len:0x%x\n",read_len);
        return NV_ERROR;
    }
    ret = nv_file_read(norm_routine, 1, read_len, mt_fp);
    if(ret != read_len)
    {
        nv_free(norm_routine);
        printk("read length not equal,ret is %d,read_len is %d \n",ret,read_len);
        return NV_ERROR;
    }
    nv_file_close(mt_fp);
    nv_file_remove((s8*)NV_BACK_MTCARRIER_FLAG_PATH);

    routine_len = snprintf(mtca_user_rontine,(MTCA_ROUTINE_MAX_LENGTH-1),"%s/usr_list.bin",norm_routine);
    nv_free(norm_routine);
    norm_routine = NULL;
    if(!routine_len)
    {
        printk("%s rountine error\n",mtca_user_rontine);
        return NV_ERROR;
    }

    if(NULL==mtca_user_rontine)
    {
        ret = BSP_ERR_NV_INVALID_PARAM;
        nv_debug(NV_FUN_MTCARRIER_USR_REVERT,1,ret,0,0);  /* para2 is 0,is right? */
        return NV_ERROR;
    }

    /* */
    if(nv_file_access(mtca_user_rontine,0))
    {
        mt_fp = nv_file_open(mtca_user_rontine,(s8*)NV_FILE_WRITE);
    }

    if(NULL == mt_fp)
    {
        ret = BSP_ERR_NV_NO_FILE;
        nv_debug(NV_FUN_MTCARRIER_USR_REVERT,2,ret,0,0);
        goto nv_revert_mtcarrier_fail;
    }

    file_length = nv_get_file_len(mt_fp);
    if((file_length > NV_MAX_FILE_SIZE)||(file_length == 0))
    {
        ret = BSP_ERR_NV_FILE_ERROR;
        nv_debug(NV_FUN_MTCARRIER_USR_REVERT,3,ret,0,0);
        goto nv_revert_mtcarrier_fail;
    }

    ret = (u32)nv_file_read((u8*)&nv_mt_bin_list,1,sizeof(nv_mt_bin_list),mt_fp);
    if(ret<0)
    {
        ret = BSP_ERR_NV_READ_FILE_FAIL;
        nv_debug(NV_FUN_MTCARRIER_USR_REVERT,4,ret,0,0);

        goto nv_revert_mtcarrier_fail;
    }

    if(NV_MTCARRIER_USR_BIN_MAGIC != nv_mt_bin_list.magic_head)
    {
        ret = BSP_ERR_NV_READ_FILE_FAIL;
        nv_debug(NV_FUN_MTCARRIER_USR_REVERT,5,ret,0,0);

        goto nv_revert_mtcarrier_fail;
    }

    if(0==nv_mt_bin_list.nvid_num)
    {
        ret = BSP_ERR_NV_FILE_ERROR;
        nv_debug(NV_FUN_MTCARRIER_USR_REVERT,6,ret,0,0);

        goto nv_revert_mtcarrier_fail;
    }

    nvidlist = (u16 *)nv_malloc((size_t)(nv_mt_bin_list.nvid_num * sizeof(u16)));
    if(NULL == nvidlist)
    {
        ret = BSP_ERR_NV_FILE_ERROR;
        nv_debug(NV_FUN_MTCARRIER_USR_REVERT,7,ret,0,0);

        goto nv_revert_mtcarrier_fail;
    }

    /* seek nv file */
    nv_file_seek(mt_fp,0,SEEK_SET);

    ret = (u32)nv_file_read((u8*)(nvidlist),1,nv_mt_bin_list.nvid_num * sizeof(u16),mt_fp);
    if(ret<0)
    {
        nv_free(nvidlist);
        ret = BSP_ERR_NV_READ_FILE_FAIL;
        nv_debug(NV_FUN_MTCARRIER_USR_REVERT,8,ret,0,0);

        goto nv_revert_mtcarrier_fail;
    }

    ret = nv_revert_data(NV_BACK_PATH,nvidlist,nv_mt_bin_list.nvid_num, NV_FLAG_NO_CRC);
    if(ret<0)
    {
        ret = BSP_ERR_NV_BACKUP_DATA_FAIL;
        nv_debug(NV_FUN_MTCARRIER_USR_REVERT,9,ret,0,0);

        goto nv_revert_mtcarrier_fail;
    }

    nv_free(nvidlist);

    return NV_OK;
nv_revert_mtcarrier_fail:
    nv_mntn_record("\n[%s]\n",__FUNCTION__);
    nv_help(NV_FUN_MTCARRIER_USR_REVERT);
    return ret;
}
#endif

u32 bsp_nvm_upgrade(void)
{
    u32 ret;
    struct nv_global_ddr_info_stru* ddr_info = (struct nv_global_ddr_info_stru*)NV_GLOBAL_INFO_ADDR;

    nv_debug(NV_FUN_UPGRADE_PROC,0,0,0,0);
    nv_mntn_record("Balong nv upgrade start!\n");

    /*判断fastboot阶段xml 解析是否异常，若出现异常，则需要重新解析xml*/
    if(ddr_info->xml_dec_state != NV_XML_DEC_SUCC_STATE)
    {
        nv_mntn_record("fastboot xml decode fail ,need kernel decode again!\n");
        ret = bsp_nvm_xml_decode();
        if(ret)
        {
            nv_mntn_record("kernel xml decode failed 0x%x!\n", ret);
            nv_debug(NV_FUN_UPGRADE_PROC,1,ret,0,0);
            goto upgrade_fail_out;
        }
    }

#ifdef CONFIG_MULTI_CARRIER
    ret = bsp_nvm_mtcarrier_cust_xml_decode();
    if(ret)
    {
        nv_debug(NV_FUN_UPGRADE_PROC,3,ret,0,0);
        goto upgrade_fail_out;
    }
#endif

    /*升级恢复处理，烧片版本直接返回ok*/
    ret = nv_upgrade_revert_proc();
    if(ret)
    {
        nv_mntn_record("upgrade revert failed 0x%x!\n", ret);
        nv_debug(NV_FUN_UPGRADE_PROC,4,ret,0,0);
        goto upgrade_fail_out;
    }
    else
    {
        nv_mntn_record("upgrade revert success!\n");
    }
#ifdef CONFIG_MULTI_CARRIER
    ret = bsp_nvm_revert_mtcarrier_user();
    if(ret)
    {
        nv_debug(NV_FUN_UPGRADE_PROC,5,ret,0,0);
        goto upgrade_fail_out;
    }
#endif

    (void)nv_crc_make_ddr();
    nv_mntn_record("upgrade mkddr crc success!\n");

    /*将最新数据写入各个分区*/
    ret = nv_data_writeback();
    if(ret)
    {
        nv_mntn_record("upgrade writeback failed 0x%x!\n", ret);
        nv_debug(NV_FUN_UPGRADE_PROC,7,ret,0,0);
        goto upgrade_fail_out;
    }
    else
    {
        nv_mntn_record("upgrade writeback success!\n");
    }

    /*置升级包无效*/
    ret = (u32)nv_modify_upgrade_flag((bool)false);
    if(ret)
    {
        nv_mntn_record("upgrade set dload packet invalid failed 0x%x!\n", ret);
        nv_debug(NV_FUN_UPGRADE_PROC,8,ret,0,0);
        goto upgrade_fail_out;
    }

    ret = nv_file_update(NV_DLOAD_PATH);
    if(ret)
    {
        nv_mntn_record("upgrade nv_file_update failed 0x%x!\n", ret);
        nv_debug(NV_FUN_UPGRADE_PROC, 9,ret,0,0);
        goto upgrade_fail_out;
    }

    return NV_OK;
upgrade_fail_out:
    nv_mntn_record("\n%s\n",__func__);
    nv_help(NV_FUN_UPGRADE_PROC);
    return NV_ERROR;
}

/*
* Function   : bsp_nvm_resume_bakup
* Discription: resume all nv data from bakup partition, when image partition is invalid
* Parameter  : none
* Output     : result
* History    : yuyangyang  00228784  create
* 备注       : reload在初始化过程中，需要从流程上保证此时不会有写NV的操作，
               从而做crc check的时候内存不会被改写。
*/
u32 bsp_nvm_resume_bakup(void)
{
    u32 ret = NV_ERROR;
    FILE* fp = NULL;
    u32 datalen = 0;

    if(true == nv_check_file_validity((s8 *)NV_BACK_PATH, (s8 *)NV_BACK_FLAG_PATH))
    {
        nv_mntn_record("load from %s\n",NV_BACK_PATH);
        fp = nv_file_open((s8*)NV_BACK_PATH,(s8*)NV_FILE_READ);
        if(!fp)
        {
            nv_debug(NV_FUN_MEM_INIT,5,0,0,0);
            goto load_err_proc;
        }

        ret = nv_read_from_file(fp,(u8*)NV_GLOBAL_CTRL_INFO_ADDR,&datalen);
        nv_file_close(fp);
        if(ret)
        {
            nv_debug(NV_FUN_MEM_INIT,6,0,0,0);
            goto load_err_proc;
        }

        ret = nv_crc_check_ddr(NV_RESUME_NO);
        if(ret)
        {
            nv_debug(NV_FUN_MEM_INIT,8,ret,0,0);
            goto load_err_proc;
        }

        /*从备份区加载需要首先写入工作区*/
        ret = nv_img_flush_all();
        if(ret)
        {
            nv_debug(NV_FUN_MEM_INIT,9,0,0,0);
            goto load_err_proc;
        }

        return NV_OK;
    }

load_err_proc:
    ret = nv_load_err_proc();
    if(ret)
    {
        nv_mntn_record("%s %d ,err revert proc ,ret :0x%x\n",__func__,__LINE__,ret);
        nv_help(NV_FUN_MEM_INIT);
    }

    return ret;
}


/*
* Function   : bsp_nvm_reload
* Discription: init mem,include first load the file to dload section,
*              this step ensure the nv data copy to ddr,enhance the dload file check
* Parameter  : none
* Output     : result
* History    : yuyangyang  00228784  create
* 备注       : reload在初始化过程中，需要从流程上保证此时不会有写NV的操作，
               从而做crc check的时候内存不会被改写。
*/
u32 bsp_nvm_reload(void)
{
    u32 ret = NV_ERROR;
    FILE* fp = NULL;
    u32 datalen = 0;

    /*工作分区数据存在，且无未写入完成的标志文件*/
    if( true == nv_check_file_validity((s8 *)NV_IMG_PATH, (s8 *)NV_IMG_FLAG_PATH))
    {
        nv_mntn_record("load from %s current slice:0x%x\n",NV_IMG_PATH, bsp_get_slice_value());
        fp = nv_file_open((s8*)NV_IMG_PATH,(s8*)NV_FILE_READ);
        if(!fp)
        {
            nv_mntn_record("[%s]open %s fail\n", __FUNCTION__, NV_IMG_PATH);
            goto load_bak;
        }

        ret = nv_read_from_file(fp,(u8*)NV_GLOBAL_CTRL_INFO_ADDR,&datalen);
        nv_file_close(fp);
        if(ret)
        {
            nv_mntn_record("[%s] read %s fail, ret = 0x%x\n", __FUNCTION__, NV_IMG_PATH, ret);
            goto load_bak;
        }

        /*reload在初始化过程中，需要从流程上保证此时不会有写NV的操作，从而做crc check的时候内存不会被改写
          带自动恢复的crc check，会在恢复过程中获取crc ipc semaphore，所以不能锁住内存做crc check*/
        ret = nv_crc_check_ddr(NV_RESUME_BAKUP);
        if(BSP_ERR_NV_CRC_RESUME_SUCC == ret)
        {
            ret = nv_img_flush_all();
            if(ret)
            {
                nv_mntn_record("nv resume write back failed 0x%x\n", ret);
                return ret;
            }
            nv_printf("img check crc error, but resume success, write back to img!\n");
        }
        else if(ret)
        {
            nv_mntn_record("nv image check crc failed %d...current slice:0x%x\n", ret, bsp_get_slice_value());

            /* 保存错误镜像，然后从bakup分区恢复 */
            (void)nv_debug_store_file(NV_IMG_PATH);
            if(nv_debug_is_resume_bakup())
            {
                ret = bsp_nvm_resume_bakup();
                if(ret)
                {
                    nv_mntn_record("nv resume bakup failed %d...current slice:0x%s \n", ret, bsp_get_slice_value());
                }
            }
            else
            {
                nv_mntn_record("config don't resume bakup...slice:0x%x \n",bsp_get_slice_value());
            }

            /* 复位系统 */
            if(nv_debug_is_reset())
            {
                system_error(DRV_ERRNO_NV_CRC_ERR, NV_FUN_MEM_INIT, 3, NULL, 0);
            }
        }

        return ret;
    }

load_bak:

    return bsp_nvm_resume_bakup();
}
/*****************************************************************************
 函 数 名  : bsp_nvm_write_buf_init
 功能描述  : 初始化写入NV时使用的buf和信号量
 输入参数  :
 输出参数  : 无
 返 回 值  : 无
*****************************************************************************/
u32 bsp_nvm_buf_init(void)
{
    /*create sem*/
    osl_sem_init(1,&g_nv_ctrl.nv_list_sem);
    INIT_LIST_HEAD(&g_nv_ctrl.nv_list);

    return NV_OK;
}

s32 bsp_nvm_kernel_init(void)
{
    u32 ret;
    struct nv_global_ddr_info_stru* ddr_info = (struct nv_global_ddr_info_stru*)NV_GLOBAL_INFO_ADDR;
    nv_debug(NV_FUN_KERNEL_INIT,0,0,0,0);

    /*sem & lock init*/
    spin_lock_init(&g_nv_ctrl.spinlock);
    osl_sem_init(0,&g_nv_ctrl.task_sem);
    osl_sem_init(0,&g_nv_ctrl.suspend_sem);
    osl_sem_init(1,&g_nv_ctrl.rw_sem);
    osl_sem_init(0,&g_nv_ctrl.cc_sem);
    wake_lock_init(&g_nv_ctrl.wake_lock,WAKE_LOCK_SUSPEND,"nv_wakelock");
    g_nv_ctrl.shared_addr = (struct nv_global_ddr_info_stru *)NV_GLOBAL_INFO_ADDR;

    nv_mntn_record("Balong nv init  start! %s %s\n",__DATE__,__TIME__);

    (void)nv_debug_init();

    /*file info init*/
    ret = nv_file_init();
    if(ret)
    {
        nv_debug(NV_FUN_KERNEL_INIT,1,ret,0,0);
        goto out;
    }
    if(ddr_info->acore_init_state != NV_BOOT_INIT_OK)
    {
        nv_mntn_record("fast boot nv init fail !\n");
        nv_show_fastboot_err();
        /* coverity[secure_coding] */
        memset(ddr_info,0,sizeof(struct nv_global_ddr_info_stru));
    }

    ddr_info->acore_init_state = NV_KERNEL_INIT_DOING;
    nv_flush_cache((void*)NV_GLOBAL_INFO_ADDR, (u32)NV_GLOBAL_INFO_SIZE);

    ret = (u32)bsp_ipc_sem_create((u32)IPC_SEM_NV_CRC);
    if(ret)
    {
        nv_debug(NV_FUN_KERNEL_INIT, 2, ret, 0, 0);
        goto out;
    }

    if((ddr_info->mem_file_type == NV_MEM_DLOAD) &&
       (!nv_file_access((s8*)NV_DLOAD_PATH,0)) &&/*升级分区存在数据*/
       (true == nv_get_upgrade_flag())/*升级文件有效*/
       )
    {
        ret = bsp_nvm_upgrade();
        if(ret)
        {
            nv_mntn_record("upgrade faided! 0x%x\n", ret);
            nv_debug(NV_FUN_KERNEL_INIT,3,ret,0,0);
            goto out;
        }
        else
        {
            nv_mntn_record("upgrade success!\n");
        }

        /*读取NV自管理配置*/
        ret = bsp_nvm_read(NV_ID_DRV_SELF_CTRL,(u8*)(&(g_nv_ctrl.nv_self_ctrl)),(u32)sizeof(NV_SELF_CTRL_STRU));
        if(ret)
        {
            g_nv_ctrl.nv_self_ctrl.ulResumeMode = NV_MODE_USER;
            nv_printf("read 0x%x fail,use default value! ret :0x%x\n",NV_ID_DRV_SELF_CTRL,ret);
        }
    }
    else
    {
        /*读取NV自管理配置*/
        ret = bsp_nvm_read(NV_ID_DRV_SELF_CTRL,(u8*)(&(g_nv_ctrl.nv_self_ctrl)), (u32)sizeof(NV_SELF_CTRL_STRU));
        if(ret)
        {
            g_nv_ctrl.nv_self_ctrl.ulResumeMode = NV_MODE_USER;
            nv_printf("read 0x%x fail,use default value! ret :0x%x\n",NV_ID_DRV_SELF_CTRL,ret);
        }

        /*重新加载最新数据*/
        ret = bsp_nvm_reload();
        if(ret)
        {
            nv_debug(NV_FUN_KERNEL_INIT,4,ret,0,0);
            goto out;
        }
        else
        {
            nv_printf("reload success!\n");
        }
    }

    /*初始化双核使用的链表*/
    nv_flushListInit();

    ret = bsp_nvm_buf_init();
    if(ret)
    {
        nv_debug(NV_FUN_KERNEL_INIT,10,ret,0,0);
        goto out;
    }

    /*置初始化状态为OK*/
    ddr_info->acore_init_state = NV_INIT_OK;
    nv_flush_cache((void*)NV_GLOBAL_INFO_ADDR, (u32)NV_GLOBAL_INFO_SIZE);

    /*保证各分区数据正常写入*/
    nv_file_flag_check();

    INIT_LIST_HEAD(&g_nv_ctrl.stList);
    ret = (u32)osl_task_init("drv_nv",15,1024,(OSL_TASK_FUNC)bsp_nvm_icc_task,NULL,(OSL_TASK_ID*)&g_nv_ctrl.task_id);
    if(ret)
    {
        nv_mntn_record("[%s]:nv task init err! ret :0x%x\n",__func__,ret);
        goto out;
    }
    ret = nv_icc_chan_init(NV_RECV_FUNC_AC);
    if(ret)
    {
        nv_debug(NV_FUN_KERNEL_INIT,5,ret,0,0);
        goto out;
    }

    ret = nv_msg_init();
    if (ret) {
        nv_debug(NV_FUN_KERNEL_INIT,6,ret,0,0);
        goto out;
    }

    /*to do:nv id use macro define*/
    ret = bsp_nvm_read(NV_ID_MSP_FLASH_LESS_MID_THRED,(u8*)(&(g_nv_ctrl.mid_prio)),(u32)sizeof(u32));
    if(ret)
    {
        g_nv_ctrl.mid_prio = 20;
        nv_printf("read 0x%x fail,use default value! ret :0x%x\n",NV_ID_MSP_FLASH_LESS_MID_THRED,ret);
    }

    nv_mntn_record("Balong nv init ok!\n");

//    nv_show_ddr_info();

    return NV_OK;

out:
    nv_mntn_record("\n[%s]\n",__FUNCTION__);
    ddr_info->acore_init_state = NV_INIT_FAIL;
    nv_help(NV_FUN_KERNEL_INIT);
    nv_show_ddr_info();
    return -1;
}

static void bsp_nvm_exit(void)
{
    struct nv_global_ddr_info_stru* ddr_info = (struct nv_global_ddr_info_stru*)NV_GLOBAL_INFO_ADDR;

    /* coverity[self_assign] */
    ddr_info = ddr_info;

    /*关机写数据*/
    (void)bsp_nvm_flush();
    /*清除标志*/
    /* coverity[secure_coding] */
    memset(ddr_info,0,sizeof(struct nv_global_ddr_info_stru));
}

void modem_nv_delay(void)
{
    u32  blk_size;
    char *blk_label;
    int i, ret = -1;

    /*最长等待时长10s*/
    for(i=0;i<10;i++)
    {
        nv_printf("modem nv wait for nv block device %d s\n",i);

        blk_label = (char*)NV_BACK_SEC_NAME;
        ret = bsp_blk_size(blk_label, &blk_size);
        if (ret) {
            nv_taskdelay(1000);
            nv_printf("get block device %s fail\n", blk_label);
            continue;
        }

        blk_label = (char*)NV_DLOAD_SEC_NAME;
        ret = bsp_blk_size(blk_label, &blk_size);
        if (ret) {
            nv_taskdelay(1000);
            nv_printf("get block device %s fail\n", blk_label);
            continue;
        }

        blk_label = (char*)NV_SYS_SEC_NAME;
        ret = bsp_blk_size(blk_label, &blk_size);
        if (ret) {
            nv_taskdelay(1000);
            nv_printf("get block device %s fail\n", blk_label);
            continue;
        }

        blk_label = (char*)NV_DEF_SEC_NAME;
        ret = bsp_blk_size(blk_label, &blk_size);
        if (ret) {
            nv_taskdelay(1000);
            nv_printf("get block device %s fail\n", blk_label);
            continue;
        }
        return;
    }
}

/*lint -save -e715*//*715表示入参dev未使用*/
static int  modem_nv_probe(struct platform_device *dev)
{
    int ret;

    g_nv_ctrl.pmState = NV_WAKEUP_STATE;
    g_nv_ctrl.opState = NV_IDLE_STATE;

    modem_nv_delay();

    /* coverity[Event check_return] *//* coverity[Event unchecked_value] */
    if(mdrv_file_access("/modem_log/drv/nv",0))
        (void)mdrv_file_mkdir("/modem_log/drv/nv");


    ret = bsp_nvm_kernel_init();

    ret |= modemNv_ProcInit();

    return ret;
}

#define NV_SHUTDOWN_STATE   NV_BOOT_INIT_OK
static void modem_nv_shutdown(struct platform_device *dev)
{
    struct nv_global_ddr_info_stru* ddr_info = (struct nv_global_ddr_info_stru*)NV_GLOBAL_INFO_ADDR;

    printk("%s shutdown start %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n",__func__);

    /*read only*/
    ddr_info->acore_init_state = NV_SHUTDOWN_STATE;
    ddr_info->ccore_init_state = NV_SHUTDOWN_STATE;
    ddr_info->mcore_init_state = NV_SHUTDOWN_STATE;
}
/*lint -restore*/

/*lint -save -e785*//*785表示对结构体初始化的不完全modem_nv_pm_ops和modem_nv_drv modem_nv_device*/
#ifdef CONFIG_PM
/*lint -save -e715*//*715表示入参dev未使用*/
static s32 modem_nv_suspend(struct device *dev)
{
    static int count = 0;
    if(g_nv_ctrl.opState == NV_OPS_STATE)
    {
        printk(KERN_ERR"%s Modem nv in doing !\n",__func__);
        return -1;
    }
    g_nv_ctrl.pmState = NV_SLEEP_STATE;
    printk(KERN_ERR"Modem nv enter suspend! %d times\n",++count);
    return 0;
}
static s32 modem_nv_resume(struct device *dev)
{
    static int count = 0;
    
    g_nv_ctrl.pmState = NV_WAKEUP_STATE;
    if(NV_OPS_STATE== g_nv_ctrl.opState)
    {
        printk(KERN_ERR"%s need to enter task proc!\n",__func__);
        osl_sem_up(&g_nv_ctrl.suspend_sem);
    }
    printk(KERN_ERR"Modem nv enter resume! %d times\n",++count);
    return 0;
}
/*lint -restore*/
static const struct dev_pm_ops modem_nv_pm_ops ={
    .suspend = modem_nv_suspend,
    .resume  = modem_nv_resume,
};

#define MODEM_NV_PM_OPS (&modem_nv_pm_ops)
#else
#define MODEM_NV_PM_OPS  NULL
#endif

static struct platform_driver modem_nv_drv = {
    .shutdown   = modem_nv_shutdown,
    .driver     = {
        .name     = "modem_nv",
        .owner    = (struct module *)(unsigned long)THIS_MODULE,
        .pm       = MODEM_NV_PM_OPS,
    },
};


static struct platform_device modem_nv_device = {
    .name = "modem_nv",
    .id = 0,
    .dev = {
    .init_name = "modem_nv",
    },
};
/*lint -restore*/

int  modem_nv_init(void)
{
    struct platform_device *dev = NULL;
    int ret;
    if(0 == g_nv_ctrl.initStatus)
    {
        g_nv_ctrl.initStatus = 1;
    }
    else
    {
        show_stack(current, NULL);
    }

    ret = modem_nv_probe(dev);

    return ret;
}
/*仅用于初始化nv设备*/
int nv_init_dev(void)
{
    u32 ret;
    ret = (u32)platform_device_register(&modem_nv_device);
    if(ret)
    {
        printk(KERN_ERR"platform_device_register modem_nv_device fail !\n");
        return -1;
    }

    ret = (u32)platform_driver_register(&modem_nv_drv);
    if(ret)
    {
        printk(KERN_ERR"platform_device_register modem_nv_drv fail !\n");
        platform_device_unregister(&modem_nv_device);
        return -1;
    }
    nv_printf("init modem nv dev ok\n");
    return NV_OK;
}
void  modem_nv_exit(void)
{
    bsp_nvm_exit();
    platform_device_unregister(&modem_nv_device);
    platform_driver_unregister(&modem_nv_drv);
}

device_initcall(nv_init_dev);
#if (FEATURE_OFF == FEATURE_DELAY_MODEM_INIT)
module_init(modem_nv_init);
module_exit(modem_nv_exit);
#endif

void bsp_nvm_make_pclint_happy(void)
{
    (void)__initcall_nv_init_dev6();
}
EXPORT_SYMBOL(bsp_nvm_backup);
EXPORT_SYMBOL(bsp_nvm_dcread);
EXPORT_SYMBOL(bsp_nvm_kernel_init);
EXPORT_SYMBOL(bsp_nvm_update_default);
EXPORT_SYMBOL(bsp_nvm_revert_default);
EXPORT_SYMBOL(bsp_nvm_dcreadpart);
EXPORT_SYMBOL(bsp_nvm_get_len);
EXPORT_SYMBOL(bsp_nvm_dcwrite);
EXPORT_SYMBOL(bsp_nvm_flush);
EXPORT_SYMBOL(bsp_nvm_reload);
EXPORT_SYMBOL(nvm_read_rand);
EXPORT_SYMBOL(nvm_read_randex);
EXPORT_SYMBOL(bsp_nvm_dcread_direct);
EXPORT_SYMBOL(bsp_nvm_dcwrite_direct);
EXPORT_SYMBOL(bsp_nvm_auth_dcread);
EXPORT_SYMBOL(bsp_nvm_auth_dcwrite);
EXPORT_SYMBOL(bsp_nvm_dcwritepart);
EXPORT_SYMBOL(bsp_nvm_get_nvidlist);
EXPORT_SYMBOL(bsp_nvm_authgetlen);
EXPORT_SYMBOL(bsp_nvm_xml_decode);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif



