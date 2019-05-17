// 底层处理的错误码标识
// 注意: 只能 CHandleAvalDisk,CHandleCatelog,CHandleDiskMirror 这三个底层类使用

enum ENUM_ERRNO
{
    ENO_OK = 0, // 正确
};

extern ENUM_ERRNO errno;
