#include "deal_file.h"

bool CDealFile::CheckFileAccess(const char* a_p_file_name, const uint32 a_i_mode)
{
    if (NULL == a_p_file_name)
    {
        return false;
    }

    if (strlen(a_p_file_name) > FILENAME_MAX )
    {
        return false;
    }

    if ((R_OK != a_i_mode) && (W_OK != a_i_mode) && (X_OK != a_i_mode) && (F_OK != a_i_mode))
    {
        return false;
    }

    if (F_OK != a_i_mode)
    {
        if (0 != access(a_p_file_name, F_OK))
        {
            return false;
        }
    }

    if (0 != access(a_p_file_name, a_i_mode))
    {
        return false;
    }
    
    return true;
}

bool CDealFile::CheckFileExist(const char* a_p_file_name)
{
    return CheckFileAccess(a_p_file_name, F_OK);
}

bool CDealFile::CheckFileRead(const char* a_p_file_name)
{
    return CheckFileAccess(a_p_file_name, R_OK);
}

bool CDealFile::CheckFileWrite(const char* a_p_file_name)
{
    return CheckFileAccess(a_p_file_name, W_OK);
}

bool CDealFile::CheckFileExecute(const char* a_p_file_name)
{
    return CheckFileAccess(a_p_file_name, X_OK);
}

bool CDealFile::FileOpen(const char* a_p_file_name, const char* a_p_file_mode, FILE*& a_p_file)
{
    if (!CheckFileExist(a_p_file_name)) // 文件不存在
    {
        return false;
    }

    if (NULL != a_p_file) // 文件句柄已存在
    {
        return false;
    }

    a_p_file = fopen(a_p_file_name, a_p_file_mode);

    if (NULL == a_p_file)
    {
        return false;
    }

    return true;
}

bool CDealFile::FileClose(FILE* a_p_file)
{
    if (NULL == a_p_file)
    {
        return false;
    }

    if (0 != fclose(a_p_file))
    {
        return false;
    }

    return true;
}

bool CDealFile::FileCreate(const char* a_p_file_name)
{
    if (CheckFileExist(a_p_file_name)) // 文件已存在
    {
        return false;
    }

    FILE* l_p_file = fopen(a_p_file_name, "w");

    if (NULL == l_p_file)
    {
        return false;
    }

    if (0 != fclose(l_p_file))
    {
        return false;
    }
    
    return true;
}

bool CDealFile::FileGetSize(const char* a_p_file_name, uint64& a_i_file_size)
{
    if (!CheckFileExist(a_p_file_name)) // 文件不存在
    {
        return false;
    }

    struct stat statbuf;
    if (0 != stat(a_p_file_name,&statbuf))
    {
        return false;
    }

    a_i_file_size = statbuf.st_size;

    return true;
}

bool CDealFile::FileWrite(const void* a_p_buff, uint64 a_i_buff_size, uint64 a_i_size, uint64 a_i_count, FILE*& a_p_file)
{
    if (NULL == a_p_file)
    {
        return false;
    }

    if (NULL == a_p_buff)
    {
        return false;
    }
    
    if (a_i_buff_size <= 0)
    {
        return false;
    }

    if (a_i_size <= 0)
    {
        return false;
    }

    if (a_i_count <= 0)
    {
        return false;
    }

    // 地址空间越界
    if (a_i_buff_size < (a_i_size * a_i_count))
    {
        return false;
    }

    uint64 l_i_write_count = fwrite(a_p_buff, a_i_size, a_i_count, a_p_file);
    if (l_i_write_count != a_i_count)
    {
        return false;
    }

    return true;
}

bool CDealFile::FileRead(void* a_p_buff, uint64 a_i_buff_size, uint64 a_i_size, uint64 a_i_count, FILE*& a_p_file)
{
    if (NULL == a_p_file)
    {
        return false;
    }

    if (NULL == a_p_buff)
    {
        return false;
    }
    
    if (a_i_buff_size <= 0)
    {
        return false;
    }

    if (a_i_size <= 0)
    {
        return false;
    }

    if (a_i_count <= 0)
    {
        return false;
    }

    // 地址空间越界
    if (a_i_buff_size < (a_i_size * a_i_count))
    {
        return false;
    }

    uint64 l_i_read_count = fread(a_p_buff, a_i_size, a_i_count, a_p_file);
    if (l_i_read_count != a_i_count)
    {
        return false;
    }

    return true;
}
