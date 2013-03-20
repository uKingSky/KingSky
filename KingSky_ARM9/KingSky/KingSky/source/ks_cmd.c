#include "KingSky.h"


#if KS_CMD_EN > 0
static int  cmd_len(void);
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
#define CMD_LEN           cmd_len()

static cmd_tbl_t  *__ks_boot_cmd_start = __ks_boot_cmd;
static cmd_tbl_t  *__ks_boot_cmd_end = (__ks_boot_cmd+CMD_MAX_LEN);

//#define DEBUG_PARSER               1


/*add the cmd*/
void add_cmd(char *name,int maxargs,int repeatable,char *usage,char     *help,CMD cmd)
{
#ifdef DEBUG_PARSER
	ks_log("the CMD_LEN is %d\n",CMD_LEN);
#endif
	int num = CMD_LEN;
	if(num >= CMD_MAX_LEN) return;
	(__ks_boot_cmd_start+num)->name       = name;
	(__ks_boot_cmd_start+num)->maxargs    = maxargs;
	(__ks_boot_cmd_start+num)->cmd        = cmd;
	(__ks_boot_cmd_start+num)->repeatable = repeatable;
	(__ks_boot_cmd_start+num)->usage      = usage;
	(__ks_boot_cmd_start+num)->help       = help;
}
static int  cmd_len(void)
{
	int len;
	for(len = 0;len<ARRAY_SIZE(__ks_boot_cmd);len++)
		if (__ks_boot_cmd[len].name== NULL) break;
		return len;
}

 void init_cmd(void)
{
	int len;
	for(len = 0;len<ARRAY_SIZE(__ks_boot_cmd);len++){
		__ks_boot_cmd[len].name       =  NULL;
		__ks_boot_cmd[len].cmd        =  NULL;
		__ks_boot_cmd[len].usage      =  NULL;
		__ks_boot_cmd[len].help       =  NULL;
		__ks_boot_cmd[len].maxargs    =  0;
		__ks_boot_cmd[len].repeatable =  0;
	}
}

static char * str_find(const char * s, int c)
{
	for(; *s != (char) c; ++s)
		if (*s == '\0')
			return NULL;
		return (char *) s;
}

/***************************************************************************
* find command table entry for a command
*/

void show_all_cmd(void)
{
	cmd_tbl_t *cmdtp;
	ks_log("\n");
	for (cmdtp = __ks_boot_cmd_start;
		 cmdtp != __ks_boot_cmd_end;
	     cmdtp++) {
		 	if(cmdtp->name != NULL)
		 	ks_log("%s\n",cmdtp->name);
	     }

}
static cmd_tbl_t *find_cmd (const char *cmd)
{
	cmd_tbl_t *cmdtp;
	const char *p;
	unsigned int len;
	
	if( (p = str_find(cmd, '.')) == NULL){
		len = strlen (cmd);
	}
	else{
		len = p - cmd;
		
	}
#if DEBUG_PARSER
	ks_log("command len is %d\n",len);
#endif
	for (cmdtp = __ks_boot_cmd_start;
		 cmdtp != __ks_boot_cmd_end;
	     cmdtp++) {
		if(cmdtp->name == NULL) return NULL;
		if (strncmp (cmd, cmdtp->name, len) == 0) {
            if (len == strlen (cmdtp->name))
			{
#ifdef DEBUG_PARSER
				ks_err("cmd:found it\n");
#endif         
			return cmdtp;  /* full match */
			}
		}
	}
	return NULL;    /* not found or ambiguous command */
}


static int parse_line (char *line, char *argv[],char *point_flag)
{
	int nargs = 0;
	
#ifdef DEBUG_PARSER
	ks_log ("parse_line: \"%s\"\n", line);
#endif
	while (nargs < CONFIG_SYS_MAXARGS) {
        
		/* skip any white space */
		while ((*line == ' ') || (*line == '\t')|| (*line == '.')){
			++line;
		}
		
		if (*line == '\0') {       /* end of line, no more args    */
			argv[nargs] = NULL;
#ifdef DEBUG_PARSER
			ks_log ("parse_line: nargs=%d\n", nargs);
#endif
			return (nargs);
		}
		
		argv[nargs++] = line;        /* begin of argument string    */
		
		/* find end of string */
		while (*line && (*line != ' ') && (*line != '\t')&& (*line != '.') ) {
			++line;
		}
		
		if (*line == '\0') {       /* end of line, no more args    */
			argv[nargs] = NULL;
#ifdef DEBUG_PARSER
			ks_log ("parse_line: nargs=%d\n", nargs);
#endif
			return (nargs);
		}
		if(*line == '.'){
			*point_flag = 1;
		}
		
		*line++ = '\0';            /* terminate current arg          */
	}
	
	ks_err ("** Too many args (max. %d) **\n", CONFIG_SYS_MAXARGS);
	
#ifdef DEBUG_PARSER
	ks_log ("parse_line: nargs=%d\n", nargs);
#endif
	return (nargs);
}

static int cmd_usage(const cmd_tbl_t *cmdtp)
{
	ks_log("%s - %s\n\n", cmdtp->name, cmdtp->usage);
	ks_log("Usage:\n%s ", cmdtp->name);
	
	if (!cmdtp->help) {
		ks_err ("- No additional help available.\n");
		return -1;
	}
	ks_log ("%s\n",cmdtp->help);
	ks_log ("\n");
	return 1;
}

 int run_command (char *cmd)
{
	char point_flag = 0;
	cmd_tbl_t *cmdtp;
	char *argv[CONFIG_SYS_MAXARGS + 1];
	int argc;
#ifdef DEBUG_PARSER
	ks_log ("[RUN_COMMAND] cmd[%p]=\"", cmd);
	ks_log (cmd ? cmd : "NULL");   /* use puts - string may be loooong */
	ks_log ("\"\n");
#endif
	if (!cmd || !*cmd) {
#ifdef DEBUG_PARSER
		ks_err("empty command\n");
#endif
		return -1;   /* empty command */
	}
	
	if (strlen(cmd) >= CFG_CBSIZE) {
		ks_err("## Command too long!\n");
		return -1;
	}
	
#ifdef DEBUG_PARSER
	ks_log ("[PROCESS_SEPARATORS] %s\n", cmd);
#endif
	/* Look up command in command table */
	if ((argc = parse_line (cmd, argv,&point_flag)) == 0) {
		return -1;         /* no command at all */
	}
	if ((cmdtp = find_cmd(argv[0])) == NULL) {
		ks_err("Unknown command %s,try 'help %s'\n",cmd,cmd);
		return -1;
	}
	
	if(cmdtp == NULL) return -1;
	if (argc > cmdtp->maxargs) {
		cmd_usage(cmdtp);
		return -1;
	}
	if ((cmdtp->cmd)(cmdtp, point_flag,argc, argv) != 0) {
		ks_err("error command %s,try 'help %s'\n",cmd,cmd);
	}
	return 0;
}


int do_help (struct cmd_tbl_s *cmdtp, int point_flag, int argc , char * const argv[])
{
	cmd_tbl_t *cmdtp_t;
	int len;
	char flag  = 0;
	if(argv[1] == NULL)
		show_all_cmd();
	else{
		len = 	strlen(argv[1]);
		for (cmdtp_t = __ks_boot_cmd_start;
		 	cmdtp_t != __ks_boot_cmd_end;
	     	cmdtp_t++) {
		 		if (strncmp (argv[1], cmdtp_t->name, len) == 0) {
				flag = 1;
		 		ks_log("\n%s\n",cmdtp_t->help);
	     		}
			}
			if(flag == 0)
				ks_log("Unknow cmd %s\n",argv[1]);
		}
	
	return 0; 
}

extern  int  do_create_file (struct cmd_tbl_s *cmdtp, int point_flag, int argc , char * const argv[]);
extern  int  do_write_file (struct cmd_tbl_s *cmdtp, int point_flag, int argc , char * const argv[]);
extern  int  do_read_file (struct cmd_tbl_s *cmdtp, int point_flag, int argc , char * const argv[]);
extern  int  do_show_directory (struct cmd_tbl_s *cmdtp, int point_flag, int argc , char * const argv[]);
extern  int  do_mk_directory (struct cmd_tbl_s *cmdtp, int point_flag, int argc , char * const argv[]);
extern  int  do_remove_directory (struct cmd_tbl_s *cmdtp, int point_flag, int argc , char * const argv[]);
extern int  do_beep_ctl(struct cmd_tbl_s *cmdtp, int point_flag, int argc , char * const argv[]);
extern int  do_led_ctl(struct cmd_tbl_s *cmdtp, int point_flag, int argc , char * const argv[]);
void add_all_cmd(void)
{
	add_cmd("help",2,0,"usage","help for you",do_help);
	add_cmd("touch",2,0,"usage","create file",do_create_file);
	add_cmd("write",3,0,"usage","write file",do_write_file);
	add_cmd("read",2,0,"usage","read file",do_read_file);
	add_cmd("ls",2,0,"usage","show directory",do_show_directory);
	add_cmd("mkdir",2,0,"usage","create directory",do_mk_directory);
	add_cmd("rm",2,0,"usage","remove directory",do_remove_directory);
	add_cmd("beep",2,0,"usage","beep control",do_beep_ctl);
	add_cmd("led",6,0,"usage","led control",do_led_ctl);
}
#if 0
int main_cmd(void)
{
	char str[20];
	init_cmd();
	add_all_cmd();
	
	while(1)
	{
		printf("%s# ",CMD_NAME);
		memset(str,'\0',20);
		scanf("%[^\n]",str);
		//fflush(stdin);
		if (str == "\0") continue;
		run_command(str);
		
		printf("\n");
	}
	return 0;
}
#endif
#endif /*#if KS_CMD_EN > 0*/
 

