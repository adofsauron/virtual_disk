
// 配置类

#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "base.h"

namespace config
{
	static const uint32    CONST_MAX_SIZE_FILE_NAME		    = 255;	                                // 文件名字的最大长度
	static const uint32    CONST_MAX_SIZE_INPUT			    = 1024;	                                // 输入的最大长度
	static const uint32    CONST_MAX_SIZE_FILE_FULL_NAME	= 2048;									// 绝对路径的最大长度

	static const uint64    CONST_MAX_SIZE_DISK_TOTAL		= 1024 * 1024;                    		// 总磁盘空间, 1M
	static const uint64    CONST_MAX_SIZE_FILE_SYS		    = CONST_MAX_SIZE_DISK_TOTAL * 0.05;     // 文件系统占用空间
	static const uint64	   CONST_DISK_BLOCK_SIZE            = 1024;									// 磁盘块,按块写入镜像文件
	static const uint64	   CONST_MAX_SIZE_MIRROR_BUFF		= CONST_MAX_SIZE_DISK_TOTAL;			// 磁盘镜像加载进内存的空间缓存

	static const char*     CONST_DISK_MIRROR_NAME           = "virtual_disk.data";   // 虚拟磁盘被序列化后存储的文件名称
	static const char*     CONST_LOG_FILE_PATH              = "log/";                       	// 日志文件保存路径
	static const char*     CONST_LOG_FILE_NAME              = "vdisk";                              // 日志文件名称,后边需追加年月日时分秒
};


#endif//__CONFIG_H__