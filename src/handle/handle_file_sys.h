// 文件系统操作类

#ifndef __HANDLE_FILE_SYS_H__
#define __HANDLE_FILE_SYS_H__

#include <set>
#include <string>
#include <map>

#include "../base/pack_def.h"
#include "./file_sys/handle_catalog.h"
#include "./file_sys/handle_aval_disk.h"
#include "./file_sys/handle_disk_mirror.h"
#include "../tools/serialize.h"
#include "../base/env.h"
#include "../tools/logger.h"
#include "../tools/deal_string.h"
#include "../tools/date.h"

class CHandleCatelog;
class CHandleAvalDisk;
class CHandleDiskMirror;
class CEnv;
class CDealString;
class CDate;

class CHandleFileSys
{
public:
    CHandleFileSys();
    ~CHandleFileSys();

public: // 对外暴露的操作接口
    inline bool InitOver() {return m_b_init_over;};

    // 路径是否存在,若存在返回节点属性
    bool CheckPathExist(const std::string& a_str_path, SCateNode*& a_p_cata_node);

    // 找出给定路径的绝对路径，只用于路径转换,不判断路径是否存在 TODO: 在路径转换时,中间不可存在通配符，绝对路径也不会去识别
    bool GetFullPath(const std::string& a_str_give_path, std::string& a_str_full_path);

    // 根据绝对路径名,转换为文件名
    bool GetOnlyNameByFullName(const std::string& a_str_full_path, std::string& a_str_only_name);

    // 根据绝对路径找到父路径,全名
    bool GetFatherNameByFullName(const std::string& a_str_full_path, std::string& a_str_father_name);

    // 添加一个文件[含目录]
    // a_str_full_name : 绝对路径+名字
    // a_p_node 返回生成的新的节点的地址,外部设置更详细的属性
    // a_i_type 文件类型
    // a_i_access 文件访问权限 (ACCESS_EXECUTE | ACCESS_WRITE | ACCESS_RREAD)
    bool AddNewFile(const std::string& a_str_full_name, const ENUM_CNODE_TYPE a_i_type, const uint8 a_i_access, SCateNode*& a_p_node);

    // 获得可用的空间大小
    bool GetAvalSpace(uint64& l_i_aval_space);

    // 删除文件
    // input:文件的绝对路径
    bool DelFile(const std::string& a_str_full_name);

    // 申请a_i_size大小的可用空间, a_p_aval_space是可用空间的首地址
    bool ApplayAvalSpace(const uint64 a_i_size, byte* a_p_aval_space, uint64& a_i_incex);

    // 根据索引，获得可用空间的首地址
    bool GetAvalSpaceByIndex(const uint64 a_i_incex, byte* a_p_space);

    // 输出目录下文件, 不包含子目录
    bool CollectDirBrief(const std::string& a_str_full_name, std::vector<uint32>& a_vec_son_name);

    // 输出目录下文件，含子目录
    bool CollectDirTotal(const std::string& a_str_full_name, std::map<std::string, std::vector<uint32> >& a_map_son);

    // 重新加载文件系统,服务运行时，load指令
    bool ReloadFileSys();

    // 打印单个节点信息
    bool PrintNode(const SCateNode* l_p_node, std::string& a_str_info);

    // 打印单个节点信息
    bool PrintNode(const uint32 l_i_id, std::string& a_str_info);

    // 改变node归属
    bool MoveNode(const uint32 l_i_id, const uint32 l_i_old_fid, const uint32 l_i_new_fid);

    // 重建目录树的 缓存系统 文件名->id, TODO: 应急做法
    bool ReBuildCatelogCache();

    // 保存文件系统
    bool SaveFileSys();

    static void PrintSFileSysInfo(const SFileSysInfo& a_o_info, std::string& a_str_log);

private: // 对外隐藏, 用户不能直接操作文件系统, 只能由暴露出的函数处理
    // 初始化文件系统信息,格式化磁盘时使用
    void InitFileSysInfo();

    // 初始化文件系统,必须初始化后才可使用
    bool InitFileSys();

    // 创建文件系统
    bool CreateFileSys();

    // 加载文件系统, 服务启动时
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