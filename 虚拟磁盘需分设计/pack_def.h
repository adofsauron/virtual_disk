
#ifndef __PACK_DEF_H__
#define __PACK_DEF_H__

#include <memory.h>

#include "platform.h"
#include "../tools/config.h"
#include "macro.h"


// 目录节点类型
typedef enum ENUM_CNODE_TYPE
{
	CNODE_INVIAL	= 0, // 非法类型
	CNODE_DIR		= 1, // [d]目录类型, 不关联inode
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
		memset(m_p_name, 0x00, DEF_MAX_SIZE_FILE_NAME + 1);
	};

	~SCateNode()
	{
		DELETE_PTR(m_p_son_set);
	}

	uint32				m_i_id;									// 节点id
	char				m_p_name[DEF_MAX_SIZE_FILE_NAME + 1];	// 节点文件名
	uint64				m_i_disk_index;							// 在可用磁盘中的地址的位置，目录文件不占用磁盘
	uint32				m_i_file_size;							// 文件占用的空间大小；[连续空间]
	ENUM_CNODE_TYPE		m_i_type;								// 节点类型
	uint8				m_i_access;								// 节点权限
	uint32				m_i_create_time;						// 创建时间
	uint32				m_i_update_time;						// 更新时间
	uint32				m_i_link_id;							// 软练级文件被链接的节点id
	uint32				m_i_parent_id;							// 父节点,根节点无
	uint32				m_i_son_num;							// 儿子节点的数量
	uint32*				m_p_son_set;							// 儿子节点的地址空间,需释放
}SCateNode;


// 单独的节点占用的空间,不包含子节点空间
#define DEF_JUST_NODE_SIZE (sizeof(SCateNode)) 

// 一个目录节点一共占用的空间
#define DEF_NODE_SIZE (node)\
	( (node.m_i_son_num > 0) ? (DEF_JUST_NODE_SIZE + (sizeof(uint32) * node.m_i_son_num)) : DEF_JUST_NODE_SIZE )
	

#endif//__PACK_DEF_H__