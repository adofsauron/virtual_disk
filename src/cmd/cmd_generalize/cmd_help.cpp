#include "cmd_help.h"

CCmdHelp::CCmdHelp(CHandleFileSys* l_p_handle_file_sys)
    :CCmdBase(CONST_CMD_HELP, l_p_handle_file_sys)
{
}

bool CCmdHelp::CheckFeasibility(const std::vector<std::string>& a_vec_args, std::string& a_str_proc_resault)
{
    return true;
}

bool CCmdHelp::Dispose(const std::vector<std::string>& a_vec_args, std::string& a_str_proc_resault)
{

    const char* l_p_help =
    "\
    dir                                                                                                                                       \n\
        功能：列出目录中的文件和子目录列表                                                                                                        \n\
        格式：dir [/s] [/ad] [path1] [path2] ....                                                                                              \n\
        /ad 只输出其子目录                                                                                                                     \n\
        /s  输出目录及其所有子目录下所有文件                                                                                                   \n\
                                                                                                                                            \n\
    md                                                                                                                                         \n\
        功能：创建目录                                                                                                                         \n\
        格式：mkdir path                                                                                                                       \n\
                                                                                                                                            \n\
    cd                                                                                                                                         \n\
        功能：显示当前目录名或者切换当前目录                                                                                                   \n\
        格式：cd [path]                                                                                                                        \n\
        需要支持的额外参数：无.                                                                                                                \n\
        缺省切换到根路径                                                                                                                       \n\
                                                                                                                                            \n\
    copy                                                                                                                                       \n\
        功能：任意目录下的文件内容，拷贝到任意指定目录中。																							\n\
        格式：copy path1 path2                                                                                                                 \n\
        需要支持的额外参数：无                                                                                                                 \n\
        [以@开头为真实磁盘],目前目标文件不可为物理磁盘                                                                                       \n\
                                                                                                                                            \n\
    del                                                                                                                                        \n\
        功能：删除一个或者多个文件                                                                                                             \n\
        格式：del [/s] path [path1] ...                                                                                                        \n\
        需要支持的额外参数：                                                                                                                   \n\
        /s 递归删除目录及其所有子目录下指定文件                                                                                                \n\
        /f 删除链接文件链接的源文件                                                                                                            \n\
                                                                                                                                            \n\
    ren                                                                                                                                        \n\
        功能：给一个文件或者目录改名                                                                                                           \n\
        格式：rd src dst                                                                                                                       \n\
                                                                                                                                            \n\
    move                                                                                                                                       \n\
        功能：移动一个或者多个(文件/文件夹)                                                                                                    \n\
        格式：move /y src dst                                                                                                                  \n\
        /y 遇到同名的文件或者文件夹覆盖                                                                                                        \n\
                                                                                                                                            \n\
    mklink                                                                                                                                     \n\
        功能：创建一个符号链接，将虚拟磁盘中的一个路径（srcPath）和symbolLinkPath建立链接。                                                      \n\
        格式：mklink srcPath symbolLinkPath                                                                                                      \n\
                                                                                                                                                \n\
    save                                                                                                                                       \n\
        功能：将整个虚拟磁盘序列化到一个文件                                                                                                   \n\
        格式：save dstPath                                                                                                                     \n\
        保存路径由配置文件指定,不可手动输入                                                                                                    \n\
                                                                                                                                            \n\
    load                                                                                                                                       \n\
        功能：从磁盘载入文件，重建虚拟磁盘                                                                                                     \n\
        格式：load                                                                                                                          \n\
        保存路径由配置文件指定,不可手动输入                                                                                                      \n\
                                                                                                                                            \n\
    cls                                                                                                                                        \n\
        功能：clear screen 执行后屏幕清空，光标定位在屏幕第一行                                                                               \n\
        格式：cls                                                                                                                              \n\
                                                                                                                                            \n\
    ";

	a_str_proc_resault = l_p_help;

    LOG_RECORD(LOG_DEBUG,"");
    return true;
}
