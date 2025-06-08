#define EXTERN extern
#include "defs.h"
/************* STUFF FOR PARSING COMMAND LINE ARGS ************************/
char *GetStrOption(char *name,char *def)
// char *name;
// char *def;
{
  int i;
  char *result=def;
  int l=strlen(name);
  
  for(i=1;i<arg_c;i++)
    if(arg_v[i][0]=='-' && (int)strlen(arg_v[i])>=l+1)
      if(!strncmp(arg_v[i]+1,name,l))
	if(arg_v[i][l+1]=='=')
	  result=arg_v[i]+l+2;
	else
	  result=arg_v[i]+l+1;  
  return result;
}
int GetBoolOption(char *name)
// char *name;
{
  char *s;
  s=GetStrOption(name,"off");
  return strcmp(s,"off");
}
int GetIntOption(char *name,int def)
// char *name;
// int def;
{
  char *s;
  char buffer[40];
  sprintf(buffer,"%d",def);
  s=GetStrOption(name,buffer);
  return atof(s);
}
