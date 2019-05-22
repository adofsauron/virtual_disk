#ifndef __CMD_DIR_H__
#define __CMD_DIR_H__

#include <stdio.h>
#include <stdlib.h>

#include <map>

#include "../cmd_base.h"

class CCmdBase;

class CCmdDir : public CCmdBase
{
public:
    explicit CCmdDir(CHandleFileSys* l_p_handle_file_sys);

public:
    virtual bool CheckFeasibility(const std::vector<std::string>& a_vec_args, std::string& a_str_proc_resault);
    virtual bool Dispose(const std::vector<std::string>& a_vec_args, std::string& a_str_proc_resault);

    // 处理通配符的请求,列出当前路径下，符合通配符的文件, ?单个字符，*通配
    bool DisposeWildcard(const std::string& a_str_prex, std::string& a_str_proc_resault);
};

#endif//__CMD_DIR_H__