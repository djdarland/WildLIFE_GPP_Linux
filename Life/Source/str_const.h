#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <string>
class str_const
{
public:
  char* num_tst;
  char * LOCALSETUPFILE;       //	"./.set_up"
  char * PROMPT;               // "> "
  char * PROMPT2;              // "|    "
  char * STREAM;               // "stream"
  char * INPUT_FILE_NAME;      // "input_file_name"
  char * LINE_COUNT;           // "line_count"
  char * START_OF_LINE;        // "start_of_line"
  char * SAVED_CHAR;           // "saved_char"
  char * OLD_SAVED_CHAR;       // "old_saved_char"
  char * SAVED_PSI_TERM;       // "saved_psi_term"
  char * OLD_SAVED_PSI_TERM;   // "old_saved_psi_term"
  char * EOF_FLAG;             // "eof_flag"
  char * CURRENT_MODULE;       // "current_module"
  char * WL_DOTDOT;            // ": "   /*  RM: Dec 14 1992, should be " : "  */
  char * STDIN;                // "stdin"
  char * STDOUT;               // "stdout"
  char * STDERR;               // "stderr"
  char * ONE;                  // "1"
  char * TWO;                  // "2"
  char * THREE;                // "3"
  char * FOUR;                 // "4"
  
  char* cvt(const char*str);
  str_const();
  
};

