#ifndef __MACRO_H__
#define __MACRO_H__

#include "platform.h"

#define DELETE_PTR(ptr)		\
do							\
{							\
	if (NULL != ptr)		\
	{						\
		delete ptr;			\
		ptr = NULL;			\
	}						\
} while (0);

#define FREE_PTR(ptr)		\
do							\
{							\
	if (NULL != ptr)		\
	{						\
		free(ptr);			\
		ptr = NULL;			\
	}						\
} while (0);

// 单独的节点占用的空间,不包含子节点空间
#define DEF_JUST_NODE_SIZE (sizeof(SCateNode)) 

// 一个目录节点一共占用的空间
#define DEF_NODE_SIZE (node)\
	( (node.m_i_son_num > 0) ? (DEF_JUST_NODE_SIZE + (sizeof(uint32) * node.m_i_son_num)) : DEF_JUST_NODE_SIZE )
	

#endif//__MACRO_H__