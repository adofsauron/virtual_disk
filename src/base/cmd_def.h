#ifndef __CMD_DEF__
#define __CMD_DEF__

#include <string>

// 命令定义

const static std::string CONST_CMD_CD          =    "cd";
const static std::string CONST_CMD_CLS         =    "cls";
const static std::string CONST_CMD_COPY        =    "copy";
const static std::string CONST_CMD_DEL         =    "del";
const static std::string CONST_CMD_DIR         =    "dir";
const static std::string CONST_CMD_EXIT        =    "exit";
const static std::string CONST_CMD_LOAD        =    "load";
const static std::string CONST_CMD_MD          =    "md";
const static std::string CONST_CMD_MKLINK      =    "mklink";
const static std::string CONST_CMD_MOVE        =    "move";
const static std::string CONST_CMD_REN         =    "ren";
const static std::string CONST_CMD_SAVE        =    "save";
const static std::string CONST_CMD_HELP        =    "help";


enum  ENUM_CMD_NO
{
	ENUM_CMD_NO_INVL	= -1, // 错误
	ENUM_CMD_NO_CD		= 0,
	ENUM_CMD_NO_CLS		= 1,
	ENUM_CMD_NO_COPY	= 2,
	ENUM_CMD_NO_DEL		= 3,
	ENUM_CMD_NO_DIR		= 4,
	ENUM_CMD_NO_EXIT	= 5,
	ENUM_CMD_NO_LOAD	= 6,
	ENUM_CMD_NO_MD		= 7,
	ENUM_CMD_NO_MKLINK 	= 8,
	ENUM_CMD_NO_MOVE	= 9,
	ENUM_CMD_NO_REN		= 10,
	ENUM_CMD_NO_SAVE	= 11,
	ENUM_CMD_NO_HELP	= 12,
	ENUM_CMD_NO_MAX,
};


#endif // __CMD_DEF__
