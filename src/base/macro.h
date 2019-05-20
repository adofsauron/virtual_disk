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

#define DELETE_PTR_ARRAY(ptr)	\
do								\
{								\
	if (NULL != ptr)			\
	{							\
		delete[] ptr;			\
		ptr = NULL;				\
	}							\
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


// 计算结构体空间大小
#define MCRO_STRUCT_SIZE(a_struct)	(sizeof(a_struct))

// 一个目录节点一共占用的空间
#define MACRO_CATE_NODE_SIZE(a_o_node) \
	( (a_o_node.m_i_son_num > 0) ? (sizeof(a_o_node) + (sizeof(uint32) * a_o_node.m_i_son_num)) : sizeof(a_o_node) )
	

#endif//__MACRO_H__
