
#include "config.h"
#include "fs_api.h"

FS_FILE *file_id;
char mybuffer[200];

static void ks_create_file(const char *name)
{
	 file_id = FS_FOpen(name,"w+");
	 if (file_id) {
  	 ks_err("OK to create file %s\n",name);
	 }
	  FS_FClose(file_id);
}

 static void ks_write_file(const char *name, const char *txt) {
  int x;
  /* create file */
  file_id = FS_FOpen(name,"w+");
  if (file_id) {
    /* write to file */
    x = FS_FWrite(txt,1,strlen(txt),file_id);
    /* all data written ? */
    if (x!=(int)strlen(txt)) {
      /* check, why not all data was written */
      x = FS_FError(file_id);
      ks_err("Not all bytes written because of error %d.\n\n",x);
    }
    /* close file */
    FS_FClose(file_id);
  }
  else {
   ks_err("Unable to create file %s\n",name);
  }
}

static  void ks_read_file(const char *name) {
  int x;
  /* open file */
  file_id = FS_FOpen(name,"r");
  if (file_id) {
    /* read until EOF has been reached */
    do {
      x = FS_FRead(mybuffer,1,sizeof(mybuffer)-1,file_id);
      mybuffer[x]=0;
      if (x) {
        ks_err("%s\n",mybuffer);
      }
    } while (x);
    /* check, if there is no more data, because of EOF */
    x = FS_FError(file_id);
    if (x!=FS_ERR_EOF) {
      /* there was a problem during read operation */
      ks_err("Error %d during read operation.\n",x);
    }
    /* close file */
    FS_FClose(file_id);
  }
  else {
    ks_err("Unable to open file %s.\n",name);
  }
}

static void ks_create_directory(const char *name)
{
int x;
	x = FS_MkDir(name);
	if(x<0){
		ks_err("mkdir error\n");
	}
}

static void ks_remove_directory(const char *name)
{
	int x;
		x = FS_RmDir(name);
		if(x<0){
			ks_err("mkdir error\n");
		}
}

 static void ks_show_directory(const char *name) {
   FS_DIR *dirp;
   struct FS_DIRENT *direntp;
 
   ks_log("Directory of ");
   ks_log(name);
   ks_log("\n");
   dirp = FS_OpenDir(name);
   if (dirp) {
	 do {
	   direntp = FS_ReadDir(dirp);
	   if (direntp) {
		 sprintf(mybuffer,"%s\n",direntp->d_name);
		 ks_log(mybuffer);
	   }
	 } while (direntp);
	 FS_CloseDir(dirp);
   }
   else {
	 ks_err("Unable to open directory\n");
   }
 }

 void init_ramdisk(void)
{
int x;
	x = FS_IoCtl("ram",FS_CMD_FORMAT_MEDIA,FS_MEDIA_RAM_16KB,0);	
	  if (x!=0) {	 
	  ks_err("##Cannot format RAM disk.\n");   
	} 
}
 int  do_create_file (struct cmd_tbl_s *cmdtp, int point_flag, int argc , char * const argv[])
 {
  if(point_flag == 1) {
  	return -1;
  }
    if(argv[1] == NULL){
		ks_err("please enter file name\n");
		return -1;
    }
 	ks_create_file(argv[1]);
	return 0;
 }

 int  do_write_file (struct cmd_tbl_s *cmdtp, int point_flag, int argc , char * const argv[])
 {
  if(point_flag == 1) {
  	return -1;
  }
    if(argv[1] == NULL){
		ks_err("please enter file name\n");
		return -1;
    }
 	ks_write_file(argv[1],argv[2]);
	return 0;
 }

 int  do_read_file (struct cmd_tbl_s *cmdtp, int point_flag, int argc , char * const argv[])
 {
  if(point_flag == 1) {
  	return -1;
  }
 	if(argv[1] == NULL){
		ks_err("please enter file name\n");
		return -1;
    }
	ks_read_file(argv[1]);
	return 0;
 }

 int  do_show_directory (struct cmd_tbl_s *cmdtp, int point_flag, int argc , char * const argv[])
 {
   if(point_flag == 1) {
  	return -1;
  }
 	
	ks_show_directory("ram:");
	return 0;
 }

 int  do_mk_directory (struct cmd_tbl_s *cmdtp, int point_flag, int argc , char * const argv[])
{
  if(point_flag == 1) {
  	return -1;
  }
	 if(argv[1] == NULL){
			 ks_err("please enter directory name\n");
			 return -1;
	}
	ks_create_directory(argv[1]);
	return 0;
}
 int  do_remove_directory (struct cmd_tbl_s *cmdtp, int point_flag, int argc , char * const argv[])
{
	 if(point_flag == 1) {
		return -1;
	  }
	 if(argv[1] == NULL){
				  ks_err("please enter directory name\n");
				  return -1;
    }
	 ks_remove_directory(argv[1]);
	 return 0;
}

 




