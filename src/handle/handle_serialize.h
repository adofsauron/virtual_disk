// 序列化

#ifndef __SERIALIZE_H__
#define __SERIALIZE_H__

#include "../base/base.h"

bool Serialize(byte* buff, const int len, int& index, const SCateNode& node)
{

	int32 need_num = sizeof(SCateNode) + (sizeof(int32) * node.m_i_son_num);

	if (need_num > len)
	{
		return false;
	}

	memcpy(buff + index, &node, sizeof(SCateNode));
	index += sizeof(SCateNode);

	if (node.m_i_son_num > 0)
	{
		memcpy(buff + index, node.m_p_son_set, sizeof(uint32) * node.m_i_son_num);
		index += node.m_i_son_num;
	}

}

bool Serialize(byte* buff, const int len, int& index, SCateNode& node)
{
	// TODO: check
	memcpy(&node, buff + index, sizeof(SCateNode));
	index += sizeof(SCateNode);

	if (node.m_i_son_num > 0)
	{
		uint32* lpson = (uint32*)malloc(sizeof(uint32) * node.m_i_son_num);
		memcpy(lpson, buff + index, sizeof(uint32) * node.m_i_son_num);
		node.m_p_son_set = lpson;
	}
}

bool DeleteNode(SCateNode* node)
{
	if (NULL == node)
	{
		return false;
	}

	DELETE_PTR(node->m_p_son_set);
	DELETE_PTR(node);
	return true;
}


#endif//__SERIALIZE_H__