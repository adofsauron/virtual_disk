// 序列化

#ifndef __HANDLE_SERIZLIZE_H__
#define __HANDLE_SERIZLIZE_H__

#include <map>

#include "../base/base.h"
#include "../handle/handle_file_sys.h"

class CSerialize
{
// 工具类创建对象无意义
// 创建构造函数和拷贝构造函数但不实现,避免编译器自动创建
// 不创建析构函数,编译器不会自动创建析构函数
private:
	CSerialize();
	CSerialize(const CSerialize&);
	CSerialize& operator = (const CSerialize&);

public:
	// 反序列化文件系统信息
	static bool UnPackFileSysInfo(byte* a_p_space, const uint32 a_i_size,  SFileSysInfo& a_o_file_sys_info);

	// 反序列化可用磁盘信息
	static bool UnPackAvalDiskInfo(byte* a_p_space, const uint32 a_i_size,  SAvalDiskInfo& a_o_aval_disk_info);

	// 反序列化目录树节点
	static bool UnPackCatelog(byte* a_p_space, const uint32 a_i_size,  const uint32 a_i_cate_node_num , std::map<uint32, SCateNode>& a_map_catalog);

	// 序列化文件系统信息
	static bool PackFileSysInfo(byte* a_p_space, const uint32 a_i_size, const SFileSysInfo& a_o_file_sys_info);

	// 序列化可用磁盘信息
	static bool PackAvalDiskInfo(byte* a_p_space, const uint32 a_i_size, const  SAvalDiskInfo& a_o_aval_disk_info);

	// 序列化目录树节点
	static bool PackCatelog(byte* a_p_space, const uint32 a_i_size, const std::map<uint32, SCateNode>& a_map_catalog);
};


#endif//__HANDLE_SERIZLIZE_H__