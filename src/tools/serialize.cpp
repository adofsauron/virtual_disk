#include "serialize.h"

// 反序列化文件系统信息
bool CSerialize::UnPackFileSysInfo(byte* a_p_space, const uint32 a_i_size,  SFileSysInfo& a_o_file_sys_info)
{
	if (NULL == a_p_space)
	{
		return false;
	}

	if (0 >= a_i_size)
	{
		return false;
	}

	if (MCRO_STRUCT_SIZE(SFileSysInfo) > a_i_size)
	{
		return false;
	}

	memcpy(&a_o_file_sys_info, a_p_space, MCRO_STRUCT_SIZE(SFileSysInfo));

	SFileSysInfo::Print(&a_o_file_sys_info);

	return true;
}

// 反序列化可用磁盘信息
bool CSerialize::UnPackAvalDiskInfo(byte* a_p_space, const uint32 a_i_size,  SAvalDiskInfo& a_o_aval_disk_info)
{
	if (NULL == a_p_space)
	{
		return false;
	}

	if (0 >= a_i_size)
	{
		return false;
	}

	if (MCRO_STRUCT_SIZE(SAvalDiskInfo) > a_i_size)
	{
		return false;
	}

	memcpy(&a_o_aval_disk_info, a_p_space, MCRO_STRUCT_SIZE(SAvalDiskInfo));	
	return true;
}

// 反序列化目录树节点
bool CSerialize::UnPackCatelog(byte* a_p_space, const uint32 a_i_size,  const uint32 a_i_cate_node_num , std::map<uint32, SCateNode>& a_map_catalog)
{
	if (NULL == a_p_space)
	{
		LOG_ERR("NULL == a_p_space");
		return false;
	}

	if (0 >= a_i_size)
	{
		LOG_ERR("0 >= a_i_size");
		return false;
	}

	if (0 >= a_i_cate_node_num) // 至少要存在一个跟节点，不可能等于0
	{
		LOG_ERR("0 >= a_i_cate_node_num");
		return false;
	}

	a_map_catalog.clear();

	uint64 l_i_index = 0;
	for (uint32 i = 0; i < a_i_cate_node_num; ++i)
	{
		SCateNode l_o_node; // 注意内存空间开辟的时机
		memset(&l_o_node, 0x00, MCRO_STRUCT_SIZE(l_o_node));
		memcpy(&l_o_node, a_p_space + l_i_index, MCRO_STRUCT_SIZE(l_o_node));
		l_i_index += MCRO_STRUCT_SIZE(l_o_node);

		l_o_node.m_p_son_set = NULL; // 注意声明周期,数组根据结构体,所以此处不可有所指向指向

		SCateNode::Print(&l_o_node);
		std::cout << "------------------------------------------------""" << std::endl;

		a_map_catalog[l_o_node.m_i_id] = l_o_node;

		if (0 >= l_o_node.m_i_id) // 0时认为拷贝结束,已经由参数指定了节点数量,不应该发生.此处为了容错
		{
			break;
		}

		if (0 < l_o_node.m_i_son_num)
		{
			std::cout << l_o_node.m_i_son_num << std::endl;

			// 生命周期跟随目录节点,节点内存被清理时释放开辟的空间
			uint32* l_p_son_set = (uint32*) malloc (sizeof(uint32) * l_o_node.m_i_son_num);
			if (NULL == l_p_son_set)
			{
				LOG_ERR("NULL == l_p_son_set");
				return false;
			}

			memcpy(l_p_son_set, a_p_space + l_i_index, sizeof(uint32) * l_o_node.m_i_son_num);
			a_map_catalog[l_o_node.m_i_id].m_p_son_set = l_p_son_set; // 注意此处数组地址赋值
			l_i_index += sizeof(uint32) * l_o_node.m_i_son_num; // 下标偏移
		}
	}

	return true;
}

// 序列化文件系统信息
bool CSerialize::PackFileSysInfo(byte* a_p_space, const uint32 a_i_size, const SFileSysInfo& a_o_file_sys_info)
{
	if (NULL == a_p_space)
	{
		LOG_ERR("a_p_space null");
		return false;
	}

	if (0 >= a_i_size)
	{
		LOG_ERR("a_i_size error");
		return false;
	}

	if (MCRO_STRUCT_SIZE(SFileSysInfo) > a_i_size)
	{
		 std::cout<< "PackFileSysInfo err: size err, MCRO_STRUCT_SIZE(SFileSysInfo):" 
		 	<<MCRO_STRUCT_SIZE(SFileSysInfo) << ", a_i_size:"<<a_i_size << std::endl;
		return false;
	}

	memcpy(a_p_space, &a_o_file_sys_info, MCRO_STRUCT_SIZE(SFileSysInfo));	
;
	SFileSysInfo::Print((SFileSysInfo*)&a_o_file_sys_info);

	return true;
}

// 序列化可用磁盘信息
bool CSerialize::PackAvalDiskInfo(byte* a_p_space, const uint32 a_i_size, const SAvalDiskInfo& a_o_aval_disk_info)
{
	if (NULL == a_p_space)
	{
		return false;
	}

	if (0 >= a_i_size)
	{
		return false;
	}

	if (MCRO_STRUCT_SIZE(SAvalDiskInfo) > a_i_size)
	{
		return false;
	}

	memcpy(a_p_space, &a_o_aval_disk_info, MCRO_STRUCT_SIZE(SAvalDiskInfo));

	return true;
}

// 序列化目录树节点
bool CSerialize::PackCatelog(byte* a_p_space, const uint32 a_i_size, const std::map<uint32, SCateNode>& a_map_catalog)
{
	if (NULL == a_p_space)
	{
		return false;
	}

	if (0 >= a_i_size)
	{
		return false;
	}

	if (a_map_catalog.empty())
	{
		LOG_ERR("a_map_catalog.empty()");
		return false;
	}

	// 检测空间容量
	uint32 l_i_catalog_total_size = 0;
	std::map<uint32, SCateNode>::const_iterator l_iter = a_map_catalog.begin();
	for (; l_iter != a_map_catalog.end(); ++l_iter) 
	{
		l_i_catalog_total_size += MACRO_CATE_NODE_SIZE(l_iter->second);
	}

	if (l_i_catalog_total_size > a_i_size)
	{
		LOG_ERR("l_i_catalog_total_size > a_i_size");
		return false;
	}

	// 执行内存拷贝
	uint64 l_i_index = 0;
	l_iter = a_map_catalog.begin();
	for (; l_iter != a_map_catalog.end(); ++l_iter) 
	{
		const SCateNode& l_o_node = l_iter->second;

		SCateNode::Print((SCateNode*)&l_o_node);
		std::cout << "------------------------------------------------" << std::endl;

		// 拷贝结构体
		memcpy(a_p_space + l_i_index, &l_o_node, MCRO_STRUCT_SIZE(SCateNode));
		l_i_index += MCRO_STRUCT_SIZE(SCateNode);

		// 拷贝子目录id空间
		if (l_o_node.m_i_son_num > 0)
		{
			memcpy(a_p_space + l_i_index, l_o_node.m_p_son_set, (sizeof(uint32) * l_o_node.m_i_son_num) );
			l_i_index += ((sizeof(uint32) * l_o_node.m_i_son_num));
		}
	}

	return true;
}
