// 文件系统操作类

#ifndef __HANDLE_FILE_SYS_H__
#define __HANDLE_FILE_SYS_H__

#include "../base/pack_def.h"
#include "./file_sys/handle_catalog.h"
#include "./file_sys/handle_aval_disk.h"
#include "./file_sys/handle_disk_mirror.h"
#include "../tools/serialize.h"

class CHandleCatelog;
class CHandleAvalDisk;
class CHandleDiskMirror;

class CHandleFileSys
{
public:
    CHandleFileSys();
    ~CHandleFileSys();

public: // 对外暴露的操作接口
    inline bool InitOver() {return m_b_init_over;};

private: // 对外隐藏, 用户不能直接操作文件系统, 只能由暴露出的函数处理
    // 初始化文件系统信息,格式化磁盘时使用
    void InitFileSysInfo();

    // 初始化文件系统,必须初始化后才可使用
    bool InitFileSys();

    // 创建文件系统
    bool CreateFileSys();

    // 加载文件系统
    bool LoadFileSys();

private:
    CHandleCatelog*         m_p_hdle_catelog;
    CHandleAvalDisk*        m_p_hdle_aval_disk;
    CHandleDiskMirror*      m_p_hdle_disk_mirror;

private:
    SFileSysInfo            m_o_file_sys_info; // 文件系统信息,建立之后，如果不重建文件系统,则按此值读取;仅修改相关配置不生效
    bool                    m_b_init_over;
};

#endif//__HANDLE_FILE_SYS_H__