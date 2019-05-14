// 文件操作

#ifndef __DEAL_FILE_H_
#define __DEAL_FILE_H_

#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>

#include "../base/base.h"


class CDealFile
{
public:
    CDealFile();
    ~CDealFile();

public:
    // 检测文件是否存在
    static bool CheckFileExist(const char* a_p_file_name);

    // 检测读
    static bool CheckFileRead(const char* a_p_file_name);

    // 检测写
    static bool CheckFileWrite(const char* a_p_file_name);

    // 检测执行
    static bool CheckFileExecute(const char* a_p_file_name);

    // 打开
    static bool FileOpen(const char* a_p_file_name, const char* a_p_file_mode, FILE* a_p_file);

    // 关闭
    static bool FileClose(FILE* a_p_file);

    // 创建
    static bool FileCreate(const char* a_p_file_name);

    // 获得文件大小
    static bool FileGetSize(const char* a_p_file_name, uint64& a_i_file_size);

    // 若a_p_buff指向的空间不足,则会收到段错误信号SIGSEGV,进程结束
    // 没什么办法根据指针获取指向空间大小,所以要把地址的空间大小传入

    // 读文件
    static bool FileRead(void* a_p_buff, uint64 a_i_buff_size, uint64 a_i_size, uint64 a_i_count, FILE* a_p_file);

    // 写文件
    static bool FileWrite(const void* a_p_buff, uint64 a_i_buff_size, uint64 a_i_size, uint64 a_i_count, FILE* a_p_file);

private:
    // l_i_mode:待检查的属性
    //     R_OK      测试读许可权
    //     W_OK      测试写许可权
    //     X_OK      测试执行许可权
    //     F_OK      测试文件是否存在
    static bool CheckFileAccess(const char* a_p_file_name, const uint32 a_i_mode);
};

#endif//__DEAL_FILE_H_