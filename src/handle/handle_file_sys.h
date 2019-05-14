// 文件系统操作类

#ifndef __HANDLE_FILE_SYS_H__
#define __HANDLE_FILE_SYS_H__

#include "./file_sys/handle_catalog.h"
#include "./file_sys/handle_aval_disk.h"

class CHandleCatelog;
class CHandleAvaliableDisk;

class CHandleFileSys
{
public:
    explicit CHandleFileSys(CHandleCatelog* l_p_hdle_clog, CHandleAvaliableDisk* l_p_hdle_aval_disk);
    ~CHandleFileSys();

public:

private:
    CHandleCatelog*         m_p_hdle_catelog;
    CHandleAvaliableDisk*   m_p_hdle_aval_disk;
};

#endif//__HANDLE_FILE_SYS_H__