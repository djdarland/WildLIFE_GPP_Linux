#include "str_const.h"
str_const::str_const()
{
	num_tst = str_const::cvt("1234");
	LOCALSETUPFILE = str_const::cvt("./.set_up");
	PROMPT = str_const::cvt("> ");
	PROMPT2 = str_const::cvt("|    ");
	STREAM = str_const::cvt("stream");
	INPUT_FILE_NAME = str_const::cvt("input_file_name");
	LINE_COUNT = str_const::cvt("line_count");
	START_OF_LINE = str_const::cvt("start_of_line");
	SAVED_CHAR = str_const::cvt("saved_char");
	OLD_SAVED_CHAR = str_const::cvt("old_saved_char");
	SAVED_PSI_TERM = str_const::cvt("saved_psi_term");
	OLD_SAVED_PSI_TERM = str_const::cvt("old_saved_psi_term");
	EOF_FLAG = str_const::cvt("eof_flag");
	CURRENT_MODULE = str_const::cvt("current_module");
	WL_DOTDOT = str_const::cvt(": ");   /*  RM: Dec 14 1992, should be " : "  */
	STDIN = str_const::cvt("stdin");
	STDOUT = str_const::cvt("stdout");
	STDERR = str_const::cvt("stderr");
	ONE = str_const::cvt("1");
	TWO = str_const::cvt("2");
	THREE = str_const::cvt("3");
	FOUR = str_const::cvt("4");


}
char* str_const::cvt(const char* str)
{
	char* it;
	it = (char*)malloc(strlen(str+1));
	strcpy(it, str);

	return it;
}
