#pragma once
/*! \file def_glob.h
  \brief globals
  
*/
#ifdef __unix__
EXTERN struct tms life_start;
EXTERN struct tms life_end;
EXTERN struct tms start_time;
EXTERN struct tms end_time;
#endif
#ifdef _WIN64
EXTERN clock_t life_end;
EXTERN clock_t life_start;
EXTERN clock_t start_time;
EXTERN clock_t end_time;
#endif
EXTERN class wl_memory *wl_mem;

// from extern.h

/*! \var first_definition
  \brief All definition are stores in a linked list starting at first_definition.
   
*/

EXTERN ptr_definition first_definition;

/*! \var arg_c
  \brief set from argc in either life.c or lib.c

*/

EXTERN int arg_c;

/*! \var arg_v
  \brief set from argv in either life.c or lib.c

*/

EXTERN char *arg_v[ARGNN]; // Modified DJD


/*! \var error_psi_term
  \brief symbol in bi module

*/

EXTERN ptr_psi_term error_psi_term;


/*! \var abortsym
  \brief symbol in bi module

*/

EXTERN ptr_definition abortsym; /* 26.1 */

/*! \var aborthooksym
  \brief symbol in bi module

*/

EXTERN ptr_definition aborthooksym; /* 26.1 */

/* The following variables are used to make built-in type comparisons */
/* as fast as possible.  They are defined in built_ins.c.  */

/*! \var add_module1
  \brief symbol in bi module for features

*/

EXTERN ptr_definition add_module1;  /*  RM: Mar 12 1993  */

/*! \var add_module2
  \brief symbol in bi module for str2psi

*/

EXTERN ptr_definition add_module2;

/*! \var add_module3
  \brief symbol in bi module for feature_values

*/

EXTERN ptr_definition add_module3;

/*! \var parser_stack_index
  \brief index to parser stack

*/

EXTERN long long parser_stack_index; /* 26.1 */

/*! \var and
  \brief symbol in syntax module

*/

EXTERN ptr_definition wl_and;

/*! \var apply
  \brief symbol in bi module

*/

EXTERN ptr_definition apply;

/*! \var boolean
  \brief symbol in bi module

*/

EXTERN ptr_definition boolean;

/*! \var boolpredsym
  \brief symbol in bi module

*/

EXTERN ptr_definition boolpredsym;

/*! \var built_in
  \brief symbol in bi module

*/

EXTERN ptr_definition built_in;

/*! \var calloncesym
  \brief symbol in bi module

*/

EXTERN ptr_definition calloncesym;

/*! \var colonsym
  \brief symbol in syntax module

*/

EXTERN ptr_definition colonsym;

/*! \var commasym
  \brief symbol in syntax module

*/

EXTERN ptr_definition commasym;

/*! \var comment
  \brief symbol in bi module

*/

EXTERN ptr_definition comment;
/*  extern  ptr_definition conjunction; 19.8 */

/*! \var constant
  \brief symbol in bi module

*/

EXTERN ptr_definition constant;

/*! \var cut
  \brief symbol in syntax module

*/

EXTERN ptr_definition cut;

/*! \var disjunction
  \brief symbol in bi module

*/

EXTERN ptr_definition disjunction;

/*! \var disj_nil
  \brief symbol in syntax module

*/

EXTERN ptr_definition disj_nil; /*  RM: Feb 16 1993  */

/*! \var eof
  \brief symbol in syntax module

*/

EXTERN ptr_definition eof;

/*! \var eqsym
  \brief symbol in syntax module

*/

EXTERN ptr_definition eqsym;

/*! \var leftarrowsym
  \brief symbol in syntax module

*/

EXTERN ptr_definition leftarrowsym; /* PVR 15.9.93 */

/*! \var lf_false
  \brief symbol in bi module

*/

EXTERN ptr_definition lf_false;

/*! \var funcsym
  \brief symbol in syntax module

*/

EXTERN ptr_definition funcsym;

/*! \var functor
  \brief symbol in bi module

*/

EXTERN ptr_definition functor;

/*! \var iff
  \brief symbol in bi module

*/

EXTERN ptr_definition iff;

/*! \var integer
  \brief symbol in bi module

*/

EXTERN ptr_definition integer;

/*! \var alist
  \brief symbol in bi module

*/

EXTERN ptr_definition alist;

/*! \var life_or
  \brief symbol in syntax module

*/

EXTERN ptr_definition life_or; /*  RM: Apr  6 1993  */

/*! \var minus_symbol
  \brief symbol in syntax module

*/

EXTERN ptr_definition minus_symbol;/*  RM: Jun 21 1993  */

/*! \var nil
  \brief symbol in bi module

*/

EXTERN ptr_definition nil;    /*** RM 9 Dec 1992 ***/

/*! \var nothing
  \brief symbol in bi module

*/

EXTERN ptr_definition nothing;

/*! \var predsym
  \brief symbol in syntax module

*/

EXTERN ptr_definition predsym;

/*! \var quote
  \brief symbol in syntax module

*/

EXTERN ptr_definition quote;

/*! \var quoted_string
  \brief symbol in bi module

*/

EXTERN ptr_definition quoted_string;

/*! \var real
  \brief symbol in bi module

*/

EXTERN ptr_definition real;

/*! \var stream
  \brief symbol in bi module

*/

EXTERN ptr_definition stream;

/*! \var succeed
  \brief symbol in bi module

*/

EXTERN ptr_definition succeed;

/*! \var such_that
  \brief symbol in syntax module

*/

EXTERN ptr_definition such_that;

/*! \var top
  \brief symbol in syntax module

*/

EXTERN ptr_definition top;

/*! \var lf_true
  \brief symbol in bi module

*/

EXTERN ptr_definition lf_true;

/*! \var timesym
  \brief symbol in bi module

*/

EXTERN ptr_definition timesym;

/*! \var tracesym
  \brief symbol in bi module

*/

EXTERN ptr_definition tracesym; /* 26.1 */

/*! \var typesym
  \brief symbol in syntax module

*/

EXTERN ptr_definition typesym;

/*! \var variable
  \brief symbol in bi module

*/

EXTERN ptr_definition variable;

/*! \var opsym
  \brief symbol in bi module

*/

EXTERN ptr_definition opsym;

/*! \var loadsym
  \brief symbol in bi module

*/

EXTERN ptr_definition loadsym;

/*! \var dynamicsym
  \brief symbol in bi module

*/

EXTERN ptr_definition dynamicsym;

/*! \var staticsym
  \brief symbol in bi module

*/

EXTERN ptr_definition staticsym;

/*! \var encodesym
  \brief symbol in bi module

*/

EXTERN ptr_definition encodesym;

/*! \var listingsym
  \brief symbol in bi module

*/

EXTERN ptr_definition listingsym;

/*! \var delay_checksym
  \brief symbol in bi module

*/

EXTERN ptr_definition delay_checksym;

/*! \var eval_argsym
  \brief symbol in bi module

*/

EXTERN ptr_definition eval_argsym;

/*! \var inputfilesym
  \brief symbol in bi module

*/

EXTERN ptr_definition inputfilesym;

/*! \var call_handlersym
  \brief symbol in bi module

*/

EXTERN ptr_definition call_handlersym;

/*! \var xf_sym
  \brief symbol in bi module

*/

EXTERN ptr_definition xf_sym;

/*! \var fx_sym
  \brief symbol in bi module

*/

EXTERN ptr_definition fx_sym;

/*! \var yf_sym
  \brief symbol in bi module

*/

EXTERN ptr_definition yf_sym;

/*! \var fy_sym
  \brief symbol in bi module

*/

EXTERN ptr_definition fy_sym;

/*! \var xfx_sym
  \brief symbol in bi module

*/

EXTERN ptr_definition xfx_sym;

/*! \var xfy_sym
  \brief symbol in bi module

*/

EXTERN ptr_definition xfy_sym;

/*! \var yfx_sym
  \brief symbol in bi module

*/

EXTERN ptr_definition yfx_sym;

/*! \var nullsym
  \brief symbol in bi module

*/

EXTERN ptr_definition nullsym;

/*! \var sys_bitvector
  \brief symbol in sys module

*/

EXTERN ptr_definition sys_bitvector;

/*! \var sys_regexp
  \brief symbol in sys module

*/

EXTERN ptr_definition sys_regexp;

/*! \var sys_stream
  \brief symbol in sys module

*/

EXTERN ptr_definition sys_stream;

/*! \var sys_file_stream
  \brief symbol in sys module

*/

EXTERN ptr_definition sys_file_stream;

/*! \var sys_socket_stream
  \brief symbol in sys module

*/

EXTERN ptr_definition sys_socket_stream;

/*  RM: Jul  7 1993  */

/*! \var final_dot
  \brief symbol in syntax module

*/

EXTERN ptr_definition final_dot;

/*! \var final_question
  \brief symbol in syntax module

*/

EXTERN ptr_definition final_question;

/*! \var sys_process_no_children
  \brief symbol in sys module

*/

EXTERN ptr_definition sys_process_no_children;

/*! \var sys_process_exited
  \brief symbol in sys module

*/

EXTERN ptr_definition sys_process_exited;

/*! \var sys_process_signaled
  \brief symbol in sys module

*/

EXTERN ptr_definition sys_process_signaled;

/*! \var sys_process_stopped
  \brief symbol in sys module

*/

EXTERN ptr_definition sys_process_stopped;

/*! \var sys_process_continued
  \brief symbol in sys module
  
*/

EXTERN ptr_definition sys_process_continued;

/*! \var null_psi_term
  \brief Used to represent an empty parse token 
  
*/
EXTERN ptr_psi_term null_psi_term; 

/*! \var old_state
  \brief used in token.c to save state to restore it later
  
*/

EXTERN ptr_psi_term old_state; /*  RM: Feb 17 1993  */

// from interrupt.h

/*! \var interrupted
  \brief used in handling user using ctrl-c

*/

EXTERN long long interrupted;
// from login.h

EXTERN long long stepflag;
EXTERN long long ignore_eff;
EXTERN long long goal_count;

// from modules.h

/*! \var bi_module
  \brief Module for public built-ins 

*/

EXTERN ptr_module bi_module;      

/*! \var user_module
  \brief Default module for user input 

*/

EXTERN ptr_module user_module;    

/*! \var no_module
  \brief ???

*/

EXTERN ptr_module no_module;     

/*! \var x_module
  \brief 'ifdef X11' unnecessary

*/

EXTERN ptr_module x_module;     

/*! \var syntax_module
  \brief Module for minimal Prolog syntax

*/

EXTERN ptr_module syntax_module;  

/*! \var module_table
  \brief The table of modules 

*/

EXTERN ptr_node module_table;     

/*! \var current_module
  \brief The current module for the tokenizer 

*/

EXTERN ptr_module current_module; 

/*! \var current_module
  \brief system module

*/
EXTERN ptr_module sys_module;

/*! \var display_modules
  \brief whether to display modules with symbols
*/

EXTERN long long display_modules;

/*! \var display_persistent
  \brief if true print persistent values preceded by "$"

*/

EXTERN long long display_persistent;

/*! \var trace_input
  \brief whether to echo characters read

  I can use this to get equivalent of swi prolog's "protocol" !!!
*/

EXTERN long long trace_input;

// from parser.h

/*! \var parse_ok
  \brief indicates whether there was a syntax error

*/

EXTERN long long parse_ok;

// from print.h

/*! \var no_name
  \brief the string "pointer"

*/

EXTERN char *no_name;

/*! \var buffer
  \brief buffer used only in print.c - there is local with same name in xpred.c

*/

EXTERN char *buffer;

/*! \var print_depth
  \brief Global flag that modifies how writing is done. 
*/

EXTERN long long print_depth;

/*! \var indent
  \brief Global flag that modifies how writing is done. 
*/

EXTERN long long indent;

/*! \var const_quote
  \brief Global flag that modifies how writing is done. 
*/

EXTERN long long const_quote;

/*! \var write_stderr
  \brief Global flag that modifies how writing is done. 
*/

EXTERN long long write_stderr;

/*! \var write_corefs
  \brief Global flag that modifies how writing is done. 
*/

EXTERN long long write_corefs;

/*! \var write_resids
  \brief Global flag that modifies how writing is done. 
*/

EXTERN long long write_resids;

/*! \var write_canon
  \brief Global flag that modifies how writing is done. 
*/

EXTERN long long write_canon;

// from token.h

/*! \var stdin_terminal
  \brief set in init_io in lib.c to true - never changed - used in token.c

*/

EXTERN long long stdin_terminal;

/*! \var var_occurred
  \brief ???

*/

EXTERN long long var_occurred;
/* Part of global input file state */

/*! \var start_of_line
  \brief ???

*/
EXTERN long long start_of_line;


EXTERN long long saved_char;
EXTERN long long old_saved_char;
EXTERN ptr_psi_term saved_psi_term;
EXTERN ptr_psi_term old_saved_psi_term;
EXTERN long long eof_flag;

/* File state ADT */
EXTERN ptr_psi_term input_state;
EXTERN ptr_psi_term stdin_state;
/* For parsing from a string */
EXTERN long long stringparse;
EXTERN char *stringinput;

// from types.h

// from lefun.h
EXTERN ptr_goal resid_aim;
EXTERN ptr_resid_list resid_vars; /* 21.9 */
EXTERN ptr_goal resid_limit;
EXTERN long long curried;
EXTERN long long can_curry;

// from templates.h

EXTERN char *numbers[21];
EXTERN long long set_extra_args[6];

// from lib.c
EXTERN jmp_buf env;
EXTERN char prompt_buffer[PROMPT_BUFFER];

// from lub.c

// from arity.c
EXTERN FILE *features;

// from built_ins.c
EXTERN FILE *bi_list;

EXTERN long long (* c_rule[MAX_BUILT_INS])();


EXTERN char *one;
EXTERN char *two;
EXTERN char *three;
EXTERN char *year_attr;
EXTERN char *month_attr;
EXTERN char *day_attr;
EXTERN char *hour_attr;
EXTERN char *minute_attr;
EXTERN char *second_attr;
EXTERN char *weekday_attr;

// from copy.c
/* TRUE means: heap_flag==TRUE & only copy to heap those objects not */
/* already on heap, i.e. incremental copy to heap.                   */
EXTERN long long to_heap;


// from error.c
EXTERN long long psi_term_line_number;

EXTERN long long warningflag;
EXTERN long long quietflag;  // 21.1 
EXTERN long long cygwin_flag;  // 21.1 
EXTERN long long trace;
EXTERN long long verbose; // 21.1 

EXTERN long long steptrace;
EXTERN long long stepcount;

// from hash_table.c

EXTERN long long rand_array[256];

// from lefun.c
/* ptr_goal resid_limit; 12.6 */

/* ptr_psi_term match_date; 13.6 */
/* ptr_choice_point cut_point; 13.6 */

// from lib.c

EXTERN long long c_query_level;

// from login.c
/* Statistics on trail cleaning */
EXTERN long long clean_iter;
EXTERN long long clean_succ;

/* ptr_choice_point prompt_choice_stack; 12.7 */

// EXTERN struct tms start_time,end_time;

EXTERN long long xeventdelay;
EXTERN long long xcount;

EXTERN long long more_u_attr; /* TRUE if U has attributes V doesn't */
EXTERN long long more_v_attr; /* Vice-versa */

EXTERN long long u_func,v_func;  /* TRUE if U or V is a curried function */
EXTERN long long new_stat;

EXTERN ptr_definition *gamma_table;

// from modules.c
EXTERN string module_buffer;              /* Temporary storage place for strings */

EXTERN long long cmp_debug_flag;

// from parser.c
EXTERN psi_term psi_term_stack[PARSER_STACK_SIZE];
EXTERN long long int_stack[PARSER_STACK_SIZE];
EXTERN wl_operator op_stack[PARSER_STACK_SIZE];

EXTERN long long no_var_tree;

// from print.c

EXTERN char *name;
EXTERN char seg_format[PRINT_POWER+4];

EXTERN item pretty_things[PRETTY_SIZE];
EXTERN ptr_item indx;

/* The output stream for a given print command is put in here */
/* This will be set to stdout, to stderr, or to output_stream */
EXTERN FILE *outfile;

// from sys.c

/*! \var sys_bytedata
  \brief symbol in sys module

*/

EXTERN ptr_definition sys_bytedata; /* DENYS: BYTEDATA */

// from token.c
EXTERN ptr_node symbol_table;

/* For parsing from a string */
/****************************************************************************/

/* Abstract Data Type for the Input File State */

/* FILE *last_eof_read; */

/* Global input file state information */
/* Note: all characters should be stored in long longs.  This ensures
   that noncharacters (i.e., EOF) can also be stored. */
/* For parsing from a string */

// from types.h
EXTERN ptr_int_list adults,children;

// EXTERN long long parser_stack_index;

EXTERN ptr_node var_tree;
EXTERN ptr_node printed_vars;
EXTERN ptr_node printed_pointers;
EXTERN ptr_node pointer_names;
EXTERN long long gen_sym_counter;

EXTERN long long noisy;
EXTERN long long types_done;

EXTERN FILE *input_stream;
EXTERN long long line_count;
EXTERN string input_file_name;
EXTERN FILE *output_stream;
EXTERN char *prompt;
EXTERN long long page_width;

EXTERN long long type_count;
EXTERN long long types_modified;
EXTERN long long main_loop_ok;
EXTERN ptr_goal aim;
EXTERN ptr_goal goal_stack;
EXTERN ptr_choice_point choice_stack;
EXTERN ptr_stack undo_stack;
#ifdef TS
EXTERN unsigned long long global_time_stamp; /* 9.6 */
#endif

EXTERN long long assert_first;
EXTERN long long assert_ok;
EXTERN long long file_date;

// from xpred.h
EXTERN ptr_psi_term xevent_list, xevent_existing;

EXTERN ptr_definition
xevent, xkeyboard_event, xbutton_event, /* RM: 7.12.92 */
  xexpose_event, xdestroy_event, xmotion_event,
  xdisplay, xdrawable, xwindow, xpixmap, xconfigure_event,
  xenter_event,xleave_event, xmisc_event,  /* RM: 3rd May 93 */
  xgc, xdisplaylist;

EXTERN long long x_window_creation;

// EXTERN char  *numbers[21];

// EXTERN long long  set_extra_args[6];

