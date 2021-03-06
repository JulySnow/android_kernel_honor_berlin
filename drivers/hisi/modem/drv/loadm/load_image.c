/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2012-2015. All rights reserved.
 * foss@huawei.com
 *
 * If distributed as part of the Linux kernel, the following license terms
 * apply:
 *
 * * This program is free software; you can redistribute it and/or modify
 * * it under the terms of the GNU General Public License version 2 and
 * * only version 2 as published by the Free Software Foundation.
 * *
 * * This program is distributed in the hope that it will be useful,
 * * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * * GNU General Public License for more details.
 * *
 * * You should have received a copy of the GNU General Public License
 * * along with this program; if not, write to the Free Software
 * * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA
 *
 * Otherwise, the following license terms apply:
 *
 * * Redistribution and use in source and binary forms, with or without
 * * modification, are permitted provided that the following conditions
 * * are met:
 * * 1) Redistributions of source code must retain the above copyright
 * *    notice, this list of conditions and the following disclaimer.
 * * 2) Redistributions in binary form must reproduce the above copyright
 * *    notice, this list of conditions and the following disclaimer in the
 * *    documentation and/or other materials provided with the distribution.
 * * 3) Neither the name of Huawei nor the names of its contributors may
 * *    be used to endorse or promote products derived from this software
 * *    without specific prior written permission.
 *
 * * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include "product_config.h"

#include <linux/io.h>
#include <linux/jiffies.h>
#include <linux/slab.h>
#include <linux/file.h>
#include <linux/fs.h>
#ifdef CONFIG_COMPRESS_CCORE_IMAGE
#include <linux/zlib.h>
#endif
#include <linux/dma-mapping.h>
#include <asm/dma-mapping.h>
#include <asm/cacheflush.h>
#include <soc_onchiprom.h>
#include <bsp_shared_ddr.h>
#include <bsp_reset.h>
#include <bsp_sec.h>
#include <bsp_rfile.h>
#include <bsp_version.h>
#include <bsp_ddr.h>
#include <bsp_efuse.h>
#include "load_image.h"
#include "modem_dtb.h"

/* Dalls之后手机和MBB融合代码 */
#ifndef CONFIG_MODEM_LOAD_OLD_AUSTIN

#ifdef BSP_CONFIG_PHONE_TYPE
#define SECBOOT_BUFLEN  (0x100000)		/*1MB*/
#else
#define SECBOOT_BUFLEN  (0x100000/8)	/*128KB*/
#endif

#define MODEM_IMAGE_PATH	"/modem_fw/"
#define VRL_SIZE					(0x1000)  /*VRL 4K*/

/* 带安全OS需要安全加载，预留连续内存，否则在系统长时间运行后，单独复位时可能申请不到连续内存 */
#if (defined CONFIG_TZDRIVER) && (defined CONFIG_LOAD_SEC_IMAGE)
static  u8 SECBOOT_BUFFER[SECBOOT_BUFLEN];
#endif

struct image_type_name
{
	enum SVC_SECBOOT_IMG_TYPE etype;
	u32 run_addr;
	u32 ddr_size;
	const char* name;
};

 struct image_type_name modem_images[] =
{
	{MODEM, DDR_MCORE_ADDR,			DDR_MCORE_SIZE,			"balong_modem.bin"},
	{HIFI,	DDR_HIFI_ADDR,			DDR_HIFI_SIZE,			"hifi.img"},/* 预留 */
#ifdef CONFIG_TLPHY_LOAD
	{DSP,	DDR_TLPHY_IMAGE_ADDR,	DDR_TLPHY_IMAGE_SIZE,	"lphy.bin"},
#endif
#ifdef CONFIG_CPHY_LOAD
	{XDSP,	DDR_CBBE_IMAGE_ADDR,	DDR_CBBE_IMAGE_SIZE,	"xphy_mcore.bin"},
#endif
	{TAS,	0,						0,						"tas.bin"},
	{WAS,	0,						0,						"was.bin"},
	{CAS,	0,						0,						"cas.bin"},	/* 预留 */
	{MODEM_DTB,	DDR_MCORE_DTS_ADDR,	DDR_MCORE_DTS_SIZE,		"modem_dt.img"},
	{SOC_MAX,		0,				0,						""},
};

/*lint -save -e651 -e708 -e570 -e64 -e785*/
static DEFINE_MUTEX(load_proc_lock);
/*lint -restore */

static int get_image(struct image_type_name** image, enum SVC_SECBOOT_IMG_TYPE etype,u32 run_addr, u32 ddr_size)
{
	int i;
	struct image_type_name* img;

	img = modem_images;
	for(i=0; i<SOC_MAX; i++)
	{
		if(img->etype == etype)
		{
			break;
		}
		img++;
	}
	if(i == SOC_MAX)
	{
		sec_print_err("can not find image of type id %d\n", etype);
		return -ENOENT;
	}
	/*如果是tas was镜像的话要*/
	if(!img->run_addr)
	{
		img->run_addr = run_addr ;
		img->ddr_size = ddr_size ;
	}
	*image = (struct image_type_name*)img;

	return 0;
}

static int get_file_size(const char *filename)
{
	struct rfile_stat_stru st;
	s32 ret;
	memset(&st,0x00,sizeof (struct rfile_stat_stru));
	ret = bsp_stat((s8*)filename, (void *)&st);
	if(ret)
	{
		sec_print_err("file bsp_stat error .\n");
		return ret;
	}

	return (int)st.size;
}

static int get_file_name(char *file_name, const struct image_type_name *image, bool *is_sec)
{
	/* 尝试以sec_开头的安全镜像 */
	*is_sec = true;
	file_name[0] = '\0';
	strncat(file_name, MODEM_IMAGE_PATH, strlen(MODEM_IMAGE_PATH));
	strncat(file_name, "sec_", strlen("sec_"));
	strncat(file_name, image->name, strlen(image->name));
	sec_print_info("loading %s  image\n", file_name);
	if(bsp_access((s8*) file_name, RFILE_RDONLY))
	{
		sec_print_info("file %s can't access, try unsec image\n", file_name);

		/* 尝试以非安全镜像 */
		*is_sec = false;
		file_name[0] = '\0';
		strncat(file_name, MODEM_IMAGE_PATH, strlen(MODEM_IMAGE_PATH));
		strncat(file_name, image->name, strlen(image->name));

		if(bsp_access((s8*) file_name, RFILE_RDONLY))
		{
			sec_print_err("error: file %s can't access, return\n", file_name);
			return -EACCES;
		}
	}

	return 0;
}

static int read_file(const char *file_name, unsigned int offset,
		unsigned int length, char *buffer)
{
	struct file * fp;
	int retval;

	fp = filp_open(file_name, O_RDONLY, 0600);
	if (IS_ERR(fp)) {
		retval = (int)PTR_ERR(fp);
		sec_print_err("filp_open(%s) failed, fp:%p", file_name, fp);
		return retval;
	}

	retval = kernel_read(fp, (loff_t)offset, buffer, (unsigned long)length);

	if (retval != (int)length) {
		sec_print_err("kernel_read(%s) failed, fp:%p, retval %d, require len %u\n",
			file_name, fp, retval, length);
		if (retval >= 0)
			retval = -EIO;
	}

	if(fp->f_inode)
	{
		invalidate_mapping_pages(fp->f_inode->i_mapping, 0, -1);/*lint !e747 !e570*/
	}

	filp_close(fp, NULL);
	return retval;
}

int gzip_header_check(unsigned char* zbuf)
{
	if (zbuf[0] != 0x1f || zbuf[1] != 0x8b || zbuf[2] != 0x08) {
		return 0;
	} else {
		return 1;
	}
}

#if (defined CONFIG_TZDRIVER) && (defined CONFIG_LOAD_SEC_IMAGE)

static TEEC_Session load_session;
static TEEC_Context load_context;

/*
 * Function name:TEEK_init.
 * Discription:Init the TEEC and get the context
 * Parameters:
 *	  @ session: the bridge from unsec world to sec world.
 *	  @ context: context.
 * return value:
 *	  @ TEEC_SUCCESS-->success, others-->failed.
 */
static int TEEK_init(void)
{
	TEEC_Result result;
	TEEC_UUID svc_uuid = TEE_SERVICE_SECBOOT;
	TEEC_Operation operation = {0};
	const char* package_name = "sec_boot";
	u32 root_id = 0;

	result = TEEK_InitializeContext(
			NULL,
			&load_context);

	if(result != TEEC_SUCCESS) {
		sec_print_err("TEEK_InitializeContext failed, result %#x\n", result);
		goto error;
	}

	operation.started = 1;
	operation.cancel_flag = 0;
	operation.paramTypes =
		(u32)TEEC_PARAM_TYPES(TEEC_NONE, TEEC_NONE, TEEC_MEMREF_TEMP_INPUT, TEEC_MEMREF_TEMP_INPUT);/*lint !e845*/

	operation.params[2].tmpref.buffer = (void *)(&root_id);
	operation.params[2].tmpref.size = sizeof(root_id);
	operation.params[3].tmpref.buffer = (void *)(package_name);
	operation.params[3].tmpref.size = strlen(package_name) + 1;
	result = TEEK_OpenSession(
			&load_context,
			&load_session,
			&svc_uuid,
			TEEC_LOGIN_IDENTIFY,
			NULL,
			&operation,
			NULL);

	if (result != TEEC_SUCCESS)
	{
		sec_print_err("TEEK_OpenSession failed, result %#x\n", result);
		TEEK_FinalizeContext(&load_context);
	}

error:

	return (int)result;
}

static void TEEK_uninit(void)
{
	TEEK_CloseSession(&load_session);
	TEEK_FinalizeContext(&load_context);
}


/*
 * Function name:trans_vrl_to_os.
 * Discription:transfer vrl data to sec_OS
 * Parameters:
 *	  @ session: the bridge from unsec world to sec world.
 *	  @ image: the data of the image to transfer.
 *	  @ buf: the buf in  kernel to transfer
 *	  @ size: the size to transfer.
 * return value:
 *	  @ TEEC_SUCCESS-->success, others--> failed.
 */
static int trans_vrl_to_os(enum SVC_SECBOOT_IMG_TYPE  image,
		  void * buf,
		  const unsigned int size)
{
	TEEC_Session *session = &load_session;
	TEEC_Result result;
	TEEC_Operation operation;
	u32 origin;

	operation.started = 1;
	operation.cancel_flag = 0;
	operation.paramTypes =
		(u32)TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_MEMREF_TEMP_INPUT, TEEC_NONE, TEEC_NONE);/*lint !e845*/

	operation.params[0].value.a = image;
	operation.params[1].tmpref.buffer = (void *)buf;
	operation.params[1].tmpref.size = size;

	result = TEEK_InvokeCommand(
				session,
				SECBOOT_CMD_ID_COPY_VRL_TYPE,
				&operation,
				&origin);
	if (result != TEEC_SUCCESS) {
		sec_print_err("invoke failed, result %#x\n", result);
	}

	return (int)result;
}

/*
 * Function name:trans_data_to_os.
 * Discription:transfer image data to sec_OS
 * Parameters:
 *	  @ session: the bridge from unsec world to sec world.
 *	  @ image: the data of the image to transfer.
 *	  @ run_addr: the image entry address.
 *	  @ buf: the buf in  kernel to transfer
 *	  @ offset: the offset to run_addr.
 *	  @ size: the size to transfer.
 * return value:
 *	  @ TEEC_SUCCESS-->success, others--> failed.
 */
static int trans_data_to_os(enum SVC_SECBOOT_IMG_TYPE  image,
						u32 run_addr,
						void * buf,
						const unsigned int offset,
						const unsigned int size)
{
	TEEC_Session *session = &load_session;
	TEEC_Result result;
	TEEC_Operation operation;
	u32 origin;
	unsigned long paddr;
	paddr = MDDR_FAMA(run_addr);

	operation.started = 1;
	operation.cancel_flag = 0;
	operation.paramTypes =
		(u32)TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_VALUE_INPUT, TEEC_VALUE_INPUT, TEEC_VALUE_INPUT);/*lint !e845*/

	operation.params[0].value.a = image;
	operation.params[0].value.b = (u32)(paddr & 0xFFFFFFFF);;
	operation.params[1].value.a = (u32)((u64)paddr >> 32);/* 手机和MBB 兼容 */
	operation.params[1].value.b = offset;
	operation.params[2].value.a = (u32)virt_to_phys(buf);/* 手机和MBB 兼容 */
	operation.params[2].value.b = (u64)virt_to_phys(buf) >> 32;/* 手机和MBB 兼容 */
	operation.params[3].value.a = size;
	result = TEEK_InvokeCommand(
				session,
				SECBOOT_CMD_ID_COPY_DATA_TYPE,
				&operation,
				&origin);
	if (result != TEEC_SUCCESS) {
		sec_print_err("invoke failed, result %#x\n", result);
	}

	return (int)result;
}

/*
 * Function name:start_soc_image.
 * Discription:start the image verification, if success, unreset the soc
 * Parameters:
 *	  @ session: the bridge from unsec world to sec world.
 *	  @ image: the image to verification and unreset
 *	  @ run_addr: the image entry address
 * return value:
 *	  @ TEEC_SUCCESS-->success, others-->failed.
 */
static int verify_soc_image(enum SVC_SECBOOT_IMG_TYPE  image,
						u32 run_addr)
{
	TEEC_Session *session = &load_session;
	TEEC_Result result;
	TEEC_Operation operation;
	u32 origin;
	unsigned long paddr;
    int ret;
    paddr = MDDR_FAMA(run_addr);

    ret = bsp_efuse_ops_prepare();
    if(ret)
    {
        return ret;
    }
	operation.started = 1;
	operation.cancel_flag = 0;
	operation.paramTypes =
		(u32)TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_VALUE_INPUT, TEEC_NONE, TEEC_NONE);/*lint !e845*/

	 operation.params[0].value.a = image;
	 operation.params[0].value.b = 0;/*SECBOOT_LOCKSTATE , not used currently*/
	 operation.params[1].value.a = (u32)(paddr & 0xFFFFFFFF);
	 operation.params[1].value.b = (u32)((u64)paddr >> 32);/* 手机和MBB 兼容 */
	 result = TEEK_InvokeCommand(session,
								   SECBOOT_CMD_ID_VERIFY_DATA_TYPE,
									&operation,
									&origin);
	 if (result != TEEC_SUCCESS){
		sec_print_err("start  failed, result is 0x%x!\n", result);
	}
    bsp_efuse_ops_complete();
	return (int)result;
}

/******************************************************************************
Function:		load_data_to_secos
Description:	从指定偏移开始传送指定大小的镜像
Input:
			part_name   - 要发送镜像的名称
			offset	  - 偏移地址
			sizeToRead	- 输入参数，要写入的镜像的bytes大小

Output:			none
Return:			SEC_OK: OK  SEC_ERROR: ERROR码
******************************************************************************/
static int load_data_to_secos(const char* file_name, u32 offset, u32 size,
			const struct image_type_name* image, bool is_sec)
{
	int ret;
	int read_bytes;
	int readed_bytes;
	int remain_bytes;
	u32 file_offset = 0;
	u32 skip_offset = 0;
	u32 load_position_offset = 0;
	u32 is_compress_check_need = 0;

	/* 读取指定偏移的指定大小 */
	if(0 != offset)
	{
		skip_offset = offset;
		remain_bytes = (int)size;
	}
	else	/* 读取整个文件 */
	{
        is_compress_check_need = 1;	/* 只有从起始位置加载需要检查是否有gzip的头 */
		remain_bytes = get_file_size(file_name);
		if (remain_bytes <=0)
		{
			sec_print_err("error file_size 0x%x\n", remain_bytes);
			return remain_bytes;
		}

		if(is_sec)
		{
			if(remain_bytes <= VRL_SIZE)
			{
				sec_print_err("error file_size (0x%x) less than VRL_SIZE\n", remain_bytes);
				return -EIO;
			}
			remain_bytes -= VRL_SIZE;
			skip_offset = VRL_SIZE;
		}
	}
	sec_print_info("need read file %s 0x%x to 0x%x though secos from offset 0x%x\n",file_name, image->run_addr, remain_bytes, skip_offset);

	/* 检查读取的大小是否超过ddr分区大小 */
	if((u32)remain_bytes > image->ddr_size)
	{
		sec_print_err("remain_bytes larger than ddr size:  remain_bytes 0x%x > ddr_size 0x%x!\n", remain_bytes, image->ddr_size);
		return -ENOMEM;
	}

	/*split the size to be read to each SECBOOT_BUFLEN bytes.*/
	while (remain_bytes)
	{
		if (remain_bytes > SECBOOT_BUFLEN)
			read_bytes = SECBOOT_BUFLEN;
		else
			read_bytes = remain_bytes;

		readed_bytes = read_file(file_name, skip_offset + file_offset, (u32)read_bytes, (s8 *)SECBOOT_BUFFER);
		if (readed_bytes < 0 || readed_bytes != read_bytes) {
			sec_print_err("read_file %s err: readed_bytes 0x%x\n", file_name, readed_bytes);
			return readed_bytes;
		}

		if ((is_compress_check_need) && (readed_bytes >= 10)) {
			is_compress_check_need = 0;

			if (gzip_header_check((unsigned char*)SECBOOT_BUFFER)) {
                /* 将整个gzip格式的压缩镜像放在DDR空间结束位置 */
				load_position_offset = (u32)(image->ddr_size - (u32)remain_bytes);
			}
		}

		ret = trans_data_to_os(image->etype, image->run_addr,  (void *)(SECBOOT_BUFFER), load_position_offset+file_offset, (u32)read_bytes);
		sec_print_info("trans data ot os: etype 0x%x ,run_addr 0x%x, from 0x%p to secos file_offset 0x%x, bytes 0x%x success\n",
			image->etype, image->run_addr, (void *)(SECBOOT_BUFFER), file_offset, read_bytes);

		if (ret)
		{
			sec_print_err("modem image trans to os is failed, error code 0x%x\n", ret);
			return ret;
		}

		remain_bytes -= read_bytes;
		file_offset += (u32)read_bytes;
	}

	return SEC_OK;
}

static int ccpu_reset(enum SVC_SECBOOT_IMG_TYPE  image)
{
	TEEC_Session *session = &load_session;
	TEEC_Result result;
	TEEC_Operation operation;
	u32 origin;

	operation.started = 1;
	operation.cancel_flag = 0;

	operation.paramTypes =
		(u32)TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_NONE, TEEC_NONE, TEEC_NONE);/*lint !e845*/

	operation.params[0].value.a = image;
	result = TEEK_InvokeCommand(
				session,
				SECBOOT_CMD_ID_RESET_IMAGE,
				&operation,
				&origin);
	if (result != TEEC_SUCCESS) {
		sec_print_err("invoke failed, result %#x\n", result);
	}

	return (int)result;
}

s32 load_image(enum SVC_SECBOOT_IMG_TYPE ecoretype, u32 run_addr, u32 ddr_size)
{
	s32 ret;
	bool is_sec;
	char file_name[256] = {0};
	int readed_bytes;
	struct image_type_name *image;

	ret = get_image(&image, ecoretype, run_addr, ddr_size);
	if(ret)
	{
		sec_print_err("can't find image\n");
		return ret;
	}

	if(!run_addr)
	{
		run_addr = image->run_addr;
	}

	ret = get_file_name(file_name, image, &is_sec);
	if(ret)
	{
		sec_print_err("can't find image\n");
		return ret;
	}
	sec_print_info("find file %s, is_sec: %d\n", file_name, is_sec);

	/*load vrl data to sec os*/
	if(is_sec)
	{
		readed_bytes = read_file(file_name, 0, VRL_SIZE, (char*)SECBOOT_BUFFER);
		if(readed_bytes < 0 || readed_bytes != VRL_SIZE)
		{
			sec_print_err("read_file %s error, readed_bytes 0x%x!\n", file_name, readed_bytes);
			ret = readed_bytes;
			goto error;
		}

		ret = trans_vrl_to_os(image->etype, (void *)(SECBOOT_BUFFER), VRL_SIZE);
		if(ret)
		{
			sec_print_err("trans_vrl_to_os error, ret 0x%x!\n", ret);
			goto error;
		}
		sec_print_err("trans vrl to secos success, from buf 0x%p\n", (void *)(SECBOOT_BUFFER));
	}

	/* load image data to sec os */
	ret = load_data_to_secos(file_name, 0, 0, image, is_sec);
	if(ret)
	{
		sec_print_err("load image %s to secos failed, ret = 0x%x\n", file_name, ret);
		goto error;
	}
	sec_print_err("load image %s to secos success\n", file_name);

	/*end of trans all data, start verify*/
	ret = verify_soc_image(ecoretype, run_addr);
	if(ret)
	{
		sec_print_err("verify image %s fail, ret = 0x%x\n", file_name, ret);
		goto error;
	}
	sec_print_err("verify image %s success\n", file_name);

error:

	return ret;
}

#else

/* 不带安全OS的镜像加载 */
s32 load_image(enum SVC_SECBOOT_IMG_TYPE ecoretype, u32 run_addr, u32 ddr_size)
{
	s32 ret;
	bool is_sec;
	unsigned long paddr;
	int file_size;
	int readed_bytes;
	char file_name[256] = {0};
	struct image_type_name *image;
	void *vaddr = NULL;
	void *vaddr_load;
	unsigned int offset=0;

	ret = get_image(&image, ecoretype,run_addr,ddr_size);
	if(ret)
	{
		sec_print_err("can't find image\n");
		return ret;
	}

	/* load image data to sec os */
	if(!run_addr)
	{
		run_addr = image->run_addr;
		ddr_size = image->ddr_size;
	}

	ret = get_file_name(file_name, image, &is_sec);
	if(ret)
	{
		sec_print_err("can't find image\n");
		return ret;
	}
	sec_print_info("find file %s, is_sec: %d\n", file_name, is_sec);

	/*得到文件的大小*/
	file_size = get_file_size(file_name);
	sec_print_info("file_size 0x%x\n",file_size);
	if (file_size <=0)
	{
		sec_print_err("error %s size < 0 \n", file_name);
		return file_size;
	}

	/* 检查文件大小是否超过ddr分区大小 */
	if(file_size > ddr_size)
	{
		sec_print_err("image larger than ddr size:  file_size 0x%x > ddr_size %#x !\n", file_size, ddr_size);
		return -ENOMEM;
	}

	paddr =MDDR_FAMA(run_addr);
#ifdef CONFIG_ARM64
	vaddr = ioremap_wc(paddr,ddr_size);
	sec_print_err("image larger paddr 0x%lx ",paddr);
#else
	vaddr = ioremap_cached(paddr, ddr_size);
#endif
	if (vaddr == NULL)
	{
		sec_print_err("ioremap_cached error .\n");
		return -ENOMEM;
	}

	vaddr_load = vaddr;

#ifdef CONFIG_COMPRESS_CCORE_IMAGE
	if(MODEM == ecoretype)
	{
		vaddr_load = vaddr - file_size + ddr_size;
	}
#endif

	/*如果是安全的话就要把头去掉*/
	if(is_sec)
	{
		offset = VRL_SIZE;
		file_size = file_size - VRL_SIZE;
	}

	readed_bytes = read_file(file_name, offset, (u32)file_size, (s8 *)vaddr_load);
	if (readed_bytes < 0 || readed_bytes != file_size) {
		ret = -EIO;
		sec_print_err("read_file %s err: readed_bytes 0x%x\n", file_name, file_size);
		goto error_unmap;
	}

	ret = bsp_sec_check(vaddr_load, file_size);
	if (ret)
	{
		sec_print_err("fail to check image %s, error code 0x%x\n", file_name, ret);
		goto error_unmap;
	}

#ifdef CONFIG_COMPRESS_CCORE_IMAGE
	if(MODEM == ecoretype)
	{
	    char* zlib_next_in = NULL;
	    unsigned int zlib_avail_in = 0;

        sec_print_err(">>start to decompress ccore image ...from 0x%p to 0x%p\n", vaddr_load, vaddr);
    	if (gzip_header_check((unsigned char*)vaddr_load)) {
            /* skip over gzip header (1f,8b,08... 10 bytes total +
    	     * possible asciz filename)
    	     */
            zlib_next_in = (char *)(vaddr_load + 10);
            zlib_avail_in = (unsigned)(file_size - 10 - 8);
    	    /* skip over asciz filename */
        	if (zlib_next_in[3] & 0x8) {
        		do {
        			/*
        			 * If the filename doesn't fit into the buffer,
        			 * the file is very probably corrupt. Don't try
        			 * to read more data.
        			 */
        			if (zlib_avail_in == 0) {
        				sec_print_err("gzip header error");
        				ret = -EIO;
                        goto error_unmap;
        			}
        			--zlib_avail_in;
        		} while (*zlib_next_in++);
        	}
    		ret = zlib_inflate_blob(vaddr, ddr_size, (void *)zlib_next_in, zlib_avail_in);
    		if (ret < 0) {
                sec_print_err("fail to decompress ccore image, error code %d\n", ret);
                goto error_unmap;
    		} else {
        		sec_print_err("decompress ccore image success. file length = 0x%x\n", (unsigned)ret);
                ret = 0;
    		}
    	}
	}
#endif
	sec_print_err("load image %s success\n", file_name);
#ifdef CONFIG_ARM64	/* 手机平台非安全启动在Kernel直接激活A9 */
	if(MODEM == ecoretype)
	{
		writel(0xB140, bsp_sysctrl_addr_byindex(sysctrl_mdm)+0x24);
	}
#else
	/*刷新cache*/
	dmac_map_area(vaddr, ddr_size, DMA_TO_DEVICE);
	outer_clean_range(run_addr, ddr_size);
#endif

error_unmap:
	iounmap(vaddr);

	return ret;
}

#endif

#ifdef CONFIG_MODEM_DTB_LOAD_IN_KERNEL
static int get_dtb_entry(unsigned int modemid, struct modem_dt_table_t *dt_hdr, struct modem_dt_entry_t *dt_entry_ccore)
{
	uint32_t i;
	uint8_t sec_id[4]={0};
	struct modem_dt_entry_t *dt_entry_ptr;

	sec_id[0] = MODEMID_K_BITS(modemid);
	sec_id[1] = MODEMID_H_BITS(modemid);
	sec_id[2] = MODEMID_M_BITS(modemid);
	sec_id[3] = MODEMID_L_BITS(modemid);

	/* 获取与modemid匹配的acore/ccore dt_entry 指针,复用dtctool，modem config.dts中将boardid配置为对应modem_id值 */
	dt_entry_ptr = (struct modem_dt_entry_t *)((char *)dt_hdr + sizeof(struct modem_dt_table_t));	/*lint !e826*/
	for (i = 0; i < dt_hdr->num_entries; i++)
	{
		if ((dt_entry_ptr->boardid[0] == sec_id[0]) &&
		(dt_entry_ptr->boardid[1] == sec_id[1]) &&
		(dt_entry_ptr->boardid[2] == sec_id[2]) &&
		(dt_entry_ptr->boardid[3] == sec_id[3]))
		{
			sec_print_info("[%d],modemid(0x%x, 0x%x, 0x%x, 0x%x)\n",
				i, dt_entry_ptr->boardid[0], dt_entry_ptr->boardid[1], dt_entry_ptr->boardid[2], dt_entry_ptr->boardid[3]);

			memcpy((void *)dt_entry_ccore, (void *)dt_entry_ptr, sizeof(modem_dt_entry_t));
			break;
		}
		dt_entry_ptr++;
	}

	if(i == dt_hdr->num_entries) {
		return -ENOENT;
	}

	return SEC_OK;
}

static s32 load_and_verify_dtb_data(void)
{
	s32 ret;
	u32 modem_id = 0;
	struct modem_dt_table_t *header;
	struct modem_dt_entry_t dt_entry_ptr = {{0}};

#if (!defined CONFIG_TZDRIVER) || (!defined CONFIG_LOAD_SEC_IMAGE)
	void *vaddr;
	unsigned long paddr;
#endif

	char file_name[256] = {0};
	struct image_type_name *image;
	bool is_sec;
	u32 offset = 0;
	int readed_bytes;

	ret = get_image(&image, MODEM_DTB,0,0);
	if(ret)
	{
		sec_print_err("can't find image\n");
		return ret;
	}

	ret = get_file_name(file_name, image, &is_sec);
	if(ret)
	{
		sec_print_err("can't find image\n");
		return ret;
	}
	sec_print_info("find file %s, is_sec: %d\n", file_name, is_sec);

	/*get the head*/
	header  = (struct modem_dt_table_t *)kmalloc((size_t)MODEM_DTB_HEADER_LEN, GFP_KERNEL);
	if(NULL == header)
	{
	   sec_print_err("dtb header malloc fail\n");
	   return -ENOMEM;
	}
	memset((void*)header, 0, (size_t)MODEM_DTB_HEADER_LEN);

	/* 安全版本跳过sec VRL头 */
	if(is_sec)
	{
		offset = VRL_SIZE;
	}

	readed_bytes = read_file(file_name, offset, MODEM_DTB_HEADER_LEN, (char*)header);
	if (readed_bytes < 0 || readed_bytes != MODEM_DTB_HEADER_LEN)
	{
	   sec_print_err("fail to read the head of modem dtb image\n");
	   ret = readed_bytes;
	   goto err_out;
	}

	/* 需要mask掉射频扣板ID号或modemid的bit[9:0] */
	modem_id = bsp_get_version_info()->board_id & MODEMID_VENDOR_MASK;
	sec_print_info("modem_id 0x%x \n", modem_id);

	memset((void *)&dt_entry_ptr, 0, sizeof(dt_entry_ptr));

	ret = get_dtb_entry(modem_id, header, &dt_entry_ptr);
	if (ret)
	{
		sec_print_err("fail to get_dtb_entry\n");
		goto err_out;
	}

#if (defined CONFIG_TZDRIVER) && (defined CONFIG_LOAD_SEC_IMAGE)
	/* 安全版本且使能了签名 */
	if(is_sec && 0 != dt_entry_ptr.vrl_size)
	{
		/*load vrl data to sec os*/
		if(dt_entry_ptr.vrl_size > SECBOOT_BUFLEN)
		{
			sec_print_err("modem dtb vrl_size too large %d\n", dt_entry_ptr.vrl_size);
			ret = -ENOMEM;
			goto err_out;
		}
		sec_print_info("modem dtb vrl_offset %d, vrl_size %d\n", dt_entry_ptr.vrl_offset,dt_entry_ptr.vrl_size);

		readed_bytes = read_file(file_name, offset + dt_entry_ptr.vrl_offset,dt_entry_ptr.vrl_size, (char*)SECBOOT_BUFFER);
		if (readed_bytes < 0 || (u32)readed_bytes != dt_entry_ptr.vrl_size)
		{
			sec_print_err("fail to read the dtb vrl\n");
			ret = readed_bytes;
			goto err_out;
		}

		ret = trans_vrl_to_os(image->etype, (void *)(SECBOOT_BUFFER), VRL_SIZE);
		if(ret)
		{
			sec_print_err("trans_vrl_to_os error, ret 0x%x!\n", ret);
			goto err_out;
		}
		sec_print_err("trans vrl to secos success, from buf 0x%p\n", (void *)(SECBOOT_BUFFER));

	}

	/* load image data to sec os */
	ret = load_data_to_secos(file_name, offset + dt_entry_ptr.dtb_offset, dt_entry_ptr.dtb_size, image, is_sec);
	if(ret)
	{
		sec_print_err("load image %s to secos failed, ret = 0x%x\n", file_name, ret);
		goto err_out;
	}
	sec_print_err("load image %s to secos success\n", file_name);

	if(is_sec && 0 != dt_entry_ptr.vrl_size)
	{
		ret = verify_soc_image(image->etype, image->run_addr);
		if (ret)
		{
			sec_print_err("fail to verify modem dtb image\n");
			goto err_out;
		}
	}
#else
	/* Kernel非安全世界直接映射、加载 */
	paddr =MDDR_FAMA(image->run_addr);
	sec_print_err("modem ddr paddr 0x%lx ",paddr);

	vaddr = ioremap(paddr, image->ddr_size);
	if (vaddr == NULL)
	{
		sec_print_err("ioremap_cached error .\n");
		return -ENOMEM;
	}

	if(dt_entry_ptr.dtb_size > image->ddr_size)
	{
		sec_print_err("modem dtb dtb_size too large %d than ddr_size %d\n", dt_entry_ptr.vrl_size, image->ddr_size);
		ret = -ENOMEM;
		goto err_unmap;
	}
	readed_bytes = read_file(file_name, offset + dt_entry_ptr.dtb_offset, dt_entry_ptr.dtb_size, (s8 *)vaddr);
	if (readed_bytes < 0 || readed_bytes != dt_entry_ptr.dtb_size) {
		sec_print_err("read_file %s err: readed_bytes 0x%x\n", file_name, readed_bytes);
		ret = -EIO;
		goto err_unmap;
	}
err_unmap:
	iounmap(vaddr);

#endif
err_out:
	kfree(header);
	return ret;
}
#endif

/*****************************************************************************
 函 数 名  : Modem相关镜像加载接口
 功能描述  : Modem相关镜像加载接口
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 成功返回OK,失败返回ERROR
*****************************************************************************/
int bsp_load_modem_images(void)
{
	int ret;

	mutex_lock(&load_proc_lock);

#if (defined CONFIG_TZDRIVER) && (defined CONFIG_LOAD_SEC_IMAGE)
	ret = TEEK_init();
	if(ret)
	{
		mutex_unlock(&load_proc_lock);
		sec_print_err("TEEK_init failed! ret %#x\n" ,ret);
		return ret;
	}

	ret = ccpu_reset(MODEM);
    if(ret)
	{
		mutex_unlock(&load_proc_lock);
		sec_print_err("ccpu_reset failed, ret %#x\n", ret);
		return ret;
	}
#endif

#if ! defined(DRV_BUILD_SEPARATE)
#ifdef CONFIG_TLPHY_LOAD
	ret = load_image(DSP, 0, 0);
	if(ret)
	{
		goto error;
	}
#endif

#ifdef CONFIG_CPHY_LOAD
	ret = load_image(XDSP, 0, 0);
	if(ret)
	{
		goto error;
	}
#endif

#endif

#ifdef CONFIG_MODEM_DTB_LOAD_IN_KERNEL
	ret = load_and_verify_dtb_data();
	if(ret)
	{
		goto error;
	}
#endif
	ret = load_image(MODEM, 0, 0);
	if(ret)
	{
		goto error;
	}

error:
#if (defined CONFIG_TZDRIVER) && (defined CONFIG_LOAD_SEC_IMAGE)
	TEEK_uninit();
#endif

	mutex_unlock(&load_proc_lock);

	return ret;
}

/*****************************************************************************
 函 数 名  : was.img、tas.img等动态加载镜像接口
 功能描述  : Modem相关镜像加载接口
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 成功返回OK,失败返回ERROR
*****************************************************************************/
int bsp_load_modem_single_image(enum SVC_SECBOOT_IMG_TYPE ecoretype, u32 run_addr, u32 ddr_size)
{
	int ret;

	mutex_lock(&load_proc_lock);

#if (defined CONFIG_TZDRIVER) && (defined CONFIG_LOAD_SEC_IMAGE)
	ret = TEEK_init();
	if(ret)
	{
		mutex_unlock(&load_proc_lock);
		sec_print_err("TEEK_InitializeContext failed!\n");
		return ret;
	}
#endif

	ret = load_image(ecoretype, run_addr, ddr_size);
	if(ret)
	{
		goto error;
	}

error:
#if (defined CONFIG_TZDRIVER) && (defined CONFIG_LOAD_SEC_IMAGE)
	TEEK_uninit();
#endif

	mutex_unlock(&load_proc_lock);

	return ret;
}

#else

/* Austin平台老代码 */
#include <../../adrv/adrv.h>
#ifdef CONFIG_CPHY_LOAD
/*****************************************************************************
 函 数 名  : 手机平台上A核通过安全OS加载dsp镜像,校验成功返回OK,校验失败返回ERROR
 功能描述  : 手机平台上A核通过安全OS加载dsp镜像,校验成功返回OK,校验失败返回ERROR
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 成功返回OK,失败返回ERROR
 调用函数  :
 被调函数  :
*****************************************************************************/
int his_load_cdsp_image(void)
{
	int ret = 0;
	u64 jiffies_old = 0,jiffies_new=0;
	u64 jiffies_time=0;
	BSP_CORE_TYPE_E ecoretype = BSP_CDSP;
	jiffies_old = get_jiffies_64();
	ret = bsp_reset_loadimage(ecoretype);
	jiffies_new = get_jiffies_64();
	jiffies_time =jiffies_new -jiffies_old;
	if(ret)
	{
		printk(KERN_ERR ">>load his_load_cdsp_image error, ret 0x%x\n", ret);
	}
	else
	{
		printk(KERN_ERR ">>load his_load_cdsp_image success, jiffies_time 0x%llx\n", jiffies_time);
	}
	return ret;
}
#endif

#ifdef CONFIG_TLPHY_LOAD

/*****************************************************************************
 函 数 名  : 手机平台上A核通过安全OS加载dsp镜像,校验成功返回OK,校验失败返回ERROR
 功能描述  : 手机平台上A核通过安全OS加载dsp镜像,校验成功返回OK,校验失败返回ERROR
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 成功返回OK,失败返回ERROR
 调用函数  :
 被调函数  :
*****************************************************************************/
int his_load_tldsp_image(void)
{
	int ret = 0;
	u64 jiffies_old = 0,jiffies_new=0;
	u64 jiffies_time=0;
	BSP_CORE_TYPE_E ecoretype = BSP_BBE;
	jiffies_old = get_jiffies_64();
	ret = bsp_reset_loadimage(ecoretype);
	jiffies_new = get_jiffies_64();
	jiffies_time =jiffies_new -jiffies_old;
	if(ret)
	{
		printk(KERN_ERR ">>load his_load_tldsp_image error, ret 0x%x\n", ret);
	}
	else
	{
		printk(KERN_ERR ">>load his_load_tldsp_image success, jiffies_time 0x%llx\n", jiffies_time);
	}
	return ret;
}

#endif

/*****************************************************************************
 函 数 名  : 手机平台上A核通过安全OS加载modem镜像,校验成功解复位modem,校验失败返回ERROR
 功能描述  : 手机平台上A核通过安全OS首先把modem镜像加载到内存中,再来校验镜像,校验成功解复位modem,校验失败返回ERROR
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 成功返回0,失败返回非0
 调用函数  :
 被调函数  :
*****************************************************************************/

static int his_load_modem_image(void)
{
	int ret = 0;
	u64 jiffies_old = 0,jiffies_new=0;
	u64 jiffies_time=0;
	BSP_CORE_TYPE_E ecoretype = BSP_CCORE;
	jiffies_old = get_jiffies_64();
	ret = bsp_reset_loadimage(ecoretype);
	jiffies_new = get_jiffies_64();
	jiffies_time =jiffies_new -jiffies_old;
	if(ret)
	{
		printk(KERN_ERR ">>load his_load_modem_image error, ret 0x%x\n", ret);
	}
	else
	{
		printk(KERN_ERR ">>load his_load_modem_image success, jiffies_time 0x%llx\n", jiffies_time);
	}
	return ret;
}

/*****************************************************************************
 函 数 名  : 给modem单独复位提供镜像加载接口
 功能描述  : 给modem单独复位提供镜像加载接口
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 成功返回OK,失败返回ERROR
*****************************************************************************/
int bsp_load_modem_images(void)
{
	int ret = 0;

#if ! defined(DRV_BUILD_SEPARATE)
#ifdef CONFIG_CPHY_LOAD
	ret = his_load_cdsp_image();
	if(ret)
	{
		return -1;
	}
#endif
#ifdef CONFIG_TLPHY_LOAD
	ret = his_load_tldsp_image();
	if(ret)
	{
		return -1;
	}
#endif
#endif

	ret = his_load_modem_image();
	if(ret)
	{
		return -1;
	}
	return ret;
}

#endif

