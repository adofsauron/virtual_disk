#ifndef __CMD_COPY_H__
#define __CMD_COPY_H__

#include "../cmd_base.h"

class CCmdBase;

// TODO: 目前仅支持从物理磁盘向虚拟磁盘拷贝文件，不可从虚拟磁盘向物理磁盘拷贝
// TODO: 第二版本支持
// 若炎症正确性, 使用cat命令

class CCmdCopy : public CCmdBase
{
public:
    explicit CCmdCopy(CHandleFileSys* l_p_handle_file_sys);
    ~CCmdCopy();

public:
    virtual bool CheckFeasibility(const std::vector<std::string>& a_vec_args, std::string& a_str_proc_resault);
    virtual bool Dispose(const std::vector<std::string>& a_vec_args, std::string& a_str_proc_resault);
};

#endif//__CMD_COPY_H__