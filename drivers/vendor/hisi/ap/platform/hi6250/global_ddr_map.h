#ifndef _HI_GLOBAL_MEM_MAP_INCLUDE_H_
#define _HI_GLOBAL_MEM_MAP_INCLUDE_H_ 
#define HISI_RESERVED_FASTBOOT_PHYMEM_BASE 0x0
#define HISI_RESERVED_FASTBOOT_PHYMEM_SIZE (0x400000)
#define HISI_RESERVED_LPMCU_PHYMEM_BASE 0x3c0000
#define HISI_RESERVED_LPMCU_PHYMEM_SIZE 0x40000
#define HISI_RESERVED_DTB_PHYMEM_BASE 0x07A00000
#define HISI_RESERVED_DTB_PHYMEM_SIZE (0x07B00000 - 0x07A00000)
#define HISI_RESERVED_DDR_TRAINING1_PHYMEM_BASE 0x07B00000
#define HISI_RESERVED_DDR_TRAINING1_PHYMEM_SIZE (0x07B08000 - 0x07B00000)
#define HISI_RESERVED_TUI_CMA_PHYMEM_BASE 0x30000000
#define HISI_RESERVED_TUI_CMA_PHYMEM_SIZE (0x31000000 - 0x30000000)
#define HISI_RESERVED_GRAPHIC_PHYMEM_BASE 0x31000000
#define HISI_RESERVED_GRAPHIC_PHYMEM_SIZE (0x32a40000 - 0x31000000)
#define HISI_RESERVED_MNTN_PHYMEM_BASE 0x34000000
#define HISI_RESERVED_MNTN_PHYMEM_SIZE (0x34800000 - 0x34000000)
#define HISI_RESERVED_PSTORE_PHYMEM_BASE 0x34800000
#define HISI_RESERVED_PSTORE_PHYMEM_SIZE (0x34900000 - 0x34800000)
#define HISI_RESERVED_LPMX_CORE_PHYMEM_BASE 0x34900000
#define HISI_RESERVED_LPMX_CORE_PHYMEM_SIZE (0x34A00000 - 0x34900000)
#define HISI_RESERVED_FAST_KER_AND_PHYMEM_BASE 0x34A00000
#define HISI_RESERVED_FAST_KER_AND_PHYMEM_SIZE (0x34A80000 - 0x34A00000)
#define HISI_SUB_RESERVED_FASTBOOT_LOG_PYHMEM_BASE 0x34A00000
#define HISI_SUB_RESERVED_FASTBOOT_LOG_PYHMEM_SIZE (0x34A20000 - 0x34A00000)
#define HISI_SUB_RESERVED_SCHARGE_PYHMEM_BASE 0x34A20000
#define HISI_SUB_RESERVED_SCHARGE_PYHMEM_SIZE (0x34A21000 - 0x34A20000)
#define HISI_SUB_RESERVED_BL31_SHARE_MEM_PHYMEM_BASE 0x34A21000
#define HISI_SUB_RESERVED_BL31_SHARE_MEM_PHYMEM_SIZE (0x34A31000 - 0x34A21000)
#define HISI_SUB_RESERVED_UNUSED_PHYMEM_BASE 0x34A31000
#define HISI_SUB_RESERVED_UNUSED_PHYMEM_SIZE (0x34A78000 - 0x34A31000)
#define HISI_RESERVED_SMMU_PHYMEM_BASE 0x34A78000
#define HISI_RESERVED_SMMU_PHYMEM_SIZE (0x34A80000 - 0x34A78000)
#define HISI_RESERVED_SENSORHUB_SHARE_MEM_PHYMEM_BASE 0x34A80000
#define HISI_RESERVED_SENSORHUB_SHARE_MEM_PHYMEM_SIZE (0x34B00000 - 0x34A80000)
#define HISI_RESERVED_ISP_BOOT_PHYMEM_BASE 0x34B00000
#define HISI_RESERVED_ISP_BOOT_PHYMEM_SIZE (0x34B20000 - 0x34B00000)
#define HISI_RESERVED_IVP_PHYMEM_BASE 0x34E00000
#define HISI_RESERVED_IVP_PHYMEM_SIZE (0x34F00000 - 0x34E00000)
#define HISI_RESERVED_HIFI_DATA_PHYMEM_BASE 0x34F00000
#define HISI_RESERVED_HIFI_DATA_PHYMEM_SIZE (0x35300000 - 0x34F00000)
#define HISI_RESERVED_HIFI_IPC_PHYMEM_BASE 0x35300000
#define HISI_RESERVED_HIFI_IPC_PHYMEM_SIZE (0x35400000 - 0x35300000)
#define HISI_RESERVED_HIFI_PHYMEM_BASE 0x35400000
#define HISI_RESERVED_HIFI_PHYMEM_SIZE (0x35C00000 - 0x35400000)
#define HISI_RESERVED_BL31_PHYMEM_BASE 0x35C00000
#define HISI_RESERVED_BL31_PHYMEM_SIZE (0x35CC0000 - 0x35C00000)
#define HISI_RESERVED_SENSORHUB_PHYMEM_BASE 0x36000000
#define HISI_RESERVED_SENSORHUB_PHYMEM_SIZE (0x36200000 - 0x36000000)
#define HISI_RESERVED_SECOS_PHYMEM_BASE 0x36200000
#define HISI_RESERVED_SECOS_PHYMEM_SIZE (0x38000000 - 0x36200000)
#define HISI_RESERVED_MODEM_PHYMEM_BASE 0x38000000
#define HISI_RESERVED_MODEM_PHYMEM_SIZE (0x3EC00000 - 0x38000000)
#define HISI_RESERVED_MODEM_SHARE_PHYMEM_BASE 0x3EC00000
#define HISI_RESERVED_MODEM_SHARE_PHYMEM_SIZE (0x3F100000 - 0x3EC00000)
#define HISI_RESERVED_MODEM_SOCP_PHYMEM_BASE 0x3F100000
#define HISI_RESERVED_MODEM_SOCP_PHYMEM_SIZE (0x40100000 - 0x3F100000)
#define HISI_RESERVED_LPMX_CORE_PHYMEM_BASE_UNIQUE (HISI_RESERVED_LPMX_CORE_PHYMEM_BASE)
#define HISI_RESERVED_LPMCU_PHYMEM_BASE_UNIQUE (HISI_RESERVED_LPMCU_PHYMEM_BASE)
#define HISI_RESERVED_MNTN_PHYMEM_BASE_UNIQUE (HISI_RESERVED_MNTN_PHYMEM_BASE)
#endif
