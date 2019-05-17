
#ifndef __PACK_DEF_H__
#define __PACK_DEF_H__

#include <memory.h>

#include "platform.h"
#include "config.h"
#include "macro.h"


// 目录节点类型
typedef enum ENUM_CNODE_TYPE
{
	CNODE_INVIAL	= 0, // 非法类型
	CNODE_DIR		= 1, // [d]目录类型
	CNODE_FILE		= 2, // [-]文本文件类型(含二进制文件)
	CNODE_LINK		= 3, // [l]软链接文件, 保存的inode应该与被链接的节点inode相同
}ENUM_CNODE_TYPE; // 兼容c

// 目录节点权限
typedef enum ENUM_CNODE_ACCESS
{
	ACCESS_INVIAL	= 0, // [-],无任何权限
	ACCESS_EXECUTE	= 1, // [x],二进制001，执行
	ACCESS_WRITE	= 3, // [w],二进制010, 读
	ACCESS_RREAD	= 4, // [r],二进制100, 写
}ENUM_CNODE_ACCESS;


// 目录树节点存储结构

#pragma pack(1) // 按1字节对齐,便于内存拷贝
typedef struct SCateNode // 目录树节点
{
	SCateNode()
		:m_i_id(0),
		m_i_disk_index(0),
		m_i_file_size(0),
		m_i_type(CNODE_INVIAL),
		m_i_access(ACCESS_INVIAL),
		m_i_create_time(0),
		m_i_update_time(0),
		m_i_link_id(0),
		m_i_parent_id(0),
		m_i_son_num(0),
		m_p_son_set(NULL)
	{
		memset(m_p_name, 0x00, config::CONST_MAX_SIZE_FILE_NAME + 1);
		memset(m_p_full_name, 0x00, config::CONST_MAX_SIZE_FILE_FULL_NAME + 1);
	};

	~SCateNode()
	{
		// new -> delete
		// malloc -> free
		if (m_i_son_num>0)
		{
			FREE_PTR(m_p_son_set);
		}
		
	}

	uint32				m_i_id;														// 节点id
	char				m_p_name[config::CONST_MAX_SIZE_FILE_NAME + 1];				// 文件名
	char				m_p_full_name[config::CONST_MAX_SIZE_FILE_FULL_NAME + 1];	// 绝对路径+文件名
	uint64				m_i_disk_index;												// 在可用磁盘中的地址的位置，目录文件不占用磁盘
	uint32				m_i_file_size;												// 文件占用的空间大小；[连续空间]
	ENUM_CNODE_TYPE		m_i_type;													// 节点类型
	uint8				m_i_access;													// 节点权限
	uint32				m_i_create_time;											// 创建时间
	uint32				m_i_update_time;											// 更新时间
	uint32				m_i_link_id;												// 软练级文件被链接的节点id
	uint32				m_i_parent_id;												// 父节点,根节点无
	uint32				m_i_son_num;												// 儿子节点的数量
	uint32*				m_p_son_set;												// 儿子节点的地址空间,需释放
} SCateNode;

// 磁盘信息
typedef struct SAvalDiskInfo
{
	SAvalDiskInfo()
		:m_i_aval_disk_size(0),
		m_i_aval_index(0)
	{
	}

	uint64 	m_i_aval_disk_size;		// 可用磁盘空间的大小
	uint64	m_i_aval_index; 		// 可用磁盘中可用空间的位置索引
} SAvalDiskInfo;

// 文件系统信息
typedef struct SFileSysInfo
{
	SFileSysInfo()
		:m_i_disk_total_size(0),
		m_i_aval_disk_info_index(0),
		m_i_catelog_space_index(0),
		m_i_cate_node_num(0),
		m_i_aval_disk_space_index(0),
		m_i_disk_block_size(0)
	{
	}

	uint64	m_i_disk_total_size;				// 总的磁盘大小
	uint64	m_i_aval_disk_info_index;			// 可用磁盘信息在整个磁盘中的位置索引
	uint64 	m_i_catelog_space_index;			// 目录树在整个磁盘中的位置索引
	uint64 	m_i_cate_node_num; 					// 文件节点的数量
	uint64 	m_i_aval_disk_space_index;			// 可用磁盘空间在整个磁盘中的位置索引
	uint64	m_i_disk_block_size;				// 磁盘块的大小,按块写入
} SFileSysInfo;


// 整个磁盘的占用
// ---------------------------------------------------------
// | 文件系统[5%] |        可用磁盘空间                    |
// ---------------------------------------------------------

// 文件系统内部数据占用
// ---------------------------------------------------------
// |   文件系统信息   |    可用磁盘信息     |   目录树      |   
// ---------------------------------------------------------


#endif//__PACK_DEF_H__
