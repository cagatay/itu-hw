/* 
	to mount the filesystem, give the full path of the directory to be mounted ( with a '/' at the end)
	as the first parameter, and the mountpoint as the second parameter.
	to compile:
	gcc -o mfs -Wall -ansi -W -std=c99 -g -ggdb -D_GNU_SOURCE -D_FILE_OFFSET_BITS=64 -lfuse -lmagic mfs.c
*/

#define FUSE_USE_VERSION 26

#include <sys/types.h>
#include <sys/statvfs.h>
#include <stdio.h>
#include <strings.h>
#include <stdlib.h>
#include <string.h>   
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>
#include <fuse.h>
#include <magic.h>


// Global to store our read-write path
char *rwPath;
// dirs will hold the distinct filetypes of the root directory
char dirs[100];

// Translate an rofs path into it's underlying filesystem path
static char* translate_path(const char* path)
{

    char *rPath= malloc(sizeof(char)*(strlen(path)+strlen(rwPath)+1));
 
    strcpy(rPath,rwPath);
    /*if (rPath[strlen(rPath)-1]=='/') {
        rPath[strlen(rPath)-1]='\0';
    }*/
    strcat(rPath,path);

    return rPath;
}

// returns the filetype of a file with its given filename
static char *fileType(const char* filename){
	magic_t cookie;
	const char *tmp;
	char *filetype;
	unsigned int i;

	cookie = magic_open(MAGIC_NONE);
	magic_load(cookie, NULL);
	tmp = magic_file(cookie, filename);

	// we need only the filetype
	for(i=0; i<strlen(tmp); i++){
		if(tmp[i] == ' ')
			break;
	}
	filetype = malloc(sizeof(char)*(i+1));
	strncpy(filetype, tmp, i);
	filetype[i] = '\0';

	magic_close(cookie);
	return filetype;
}
 
static int callback_getattr(const char *path, struct stat *st_data)
{
	int res;
	path=translate_path("/");
	
	res = lstat(path, st_data);
	free((char*)path);
	if(res == -1) {
		return -errno;
	}
	return 0;
}

static int callback_readdir(const char *path, void *buf, fuse_fill_dir_t filler,off_t offset, struct fuse_file_info *fi)
{	
    DIR *dp;
    struct dirent *de;
    int res;
	char *ft;
	const char *tmp;

    (void) offset;
    (void) fi;

	memset(dirs, '\0', 100);
	
	/*
	 *	if we are in the root directory
	 */
	if(strcmp(path, "/") == 0){

		path=translate_path(path);

		dp = opendir(path);
		free((char*)path);
		if(dp == NULL) {
			res = -errno;
			return res;
		}

		while((de = readdir(dp)) != NULL) {
			struct stat st;
			memset(&st, 0, sizeof(st));
			st.st_ino = de->d_ino;
			st.st_mode = de->d_type << 12;
			if(strcmp(de->d_name, ".") !=0 && strcmp(de->d_name, "..") !=0){
				ft = fileType(translate_path(de->d_name));
				if(strstr(dirs, ft) == NULL){ // don't write same type again
					strcat(dirs, ft);		// store this type in the dirs, so we know it is written once
					st.st_mode = S_IFDIR;
					if (filler(buf, fileType(translate_path(de->d_name)), &st, 0))
						break;
				}
			}
        
		}
	}
	// list all the files types of which match the path.
	else{
		tmp = &path[1];
		path = translate_path("/");
		dp = opendir(path);
		free((char*)path);
		if(dp == NULL) {
			res = -errno;
			return res;
		}

		while((de = readdir(dp)) != NULL) {
			struct stat st;
			memset(&st, 0, sizeof(st));
			st.st_ino = de->d_ino;
			st.st_mode = de->d_type << 12;
			if(strcmp(de->d_name, ".") !=0 && strcmp(de->d_name, "..") !=0){
				ft = fileType(translate_path(de->d_name));
				if(strcmp(ft, tmp) == 0){
					st.st_mode = S_IFREG;
					if (filler(buf, de->d_name, &st, 0))
						break;
				}
			}
        
		}
	}
	closedir(dp);
	return 0;
}
	 
static int callback_access(const char *path, int mode)
{
	int res;
  	path=translate_path("/");
  	
  	/* Don't pretend that we allow writing
  	 * Chris AtLee <chris@atlee.ca>
  	 */
    if (mode & W_OK)
        return -EROFS;
        
  	res = access(path, mode);
	free((char*)path);
  	if (res == -1) {
    	return -errno;
  	}
  	return res;
}

struct fuse_operations callback_oper = {
    .getattr	= callback_getattr,
    .readdir	= callback_readdir,
    .access		= callback_access,
};

enum {
    KEY_HELP,
};

static void usage(const char* progname)
{
    fprintf(stdout,
"usage: %s readwritepath mountpoint [options]\n"
"\n"
"   Mounts readwritepath as a read-only mount at mountpoint\n"
"\n"
"general options:\n"
"   -o opt,[opt...]     mount options\n"
"   -h  --help          print help\n"
"\n", progname);
}

static int rofs_parse_opt(void *data, const char *arg, int key,
        struct fuse_args *outargs)
{
    (void) data;

    switch (key)
    {
        case FUSE_OPT_KEY_NONOPT:
            if (rwPath == 0)
            {
                rwPath = strdup(arg);
                return 0;
            }
            else
            {
                return 1;
            }
        case FUSE_OPT_KEY_OPT:
            return 1;
        case KEY_HELP:
            usage(outargs->argv[0]);
            exit(0);
        default:
            fprintf(stderr, "see `%s -h' for usage\n", outargs->argv[0]);
            exit(1);
    }
    return 1;
}

static struct fuse_opt rofs_opts[] = {
    FUSE_OPT_KEY("-h",          KEY_HELP),
    FUSE_OPT_KEY("--help",      KEY_HELP),
    FUSE_OPT_END
};

int main(int argc, char *argv[])
{
    struct fuse_args args = FUSE_ARGS_INIT(argc, argv);
    int res;

    res = fuse_opt_parse(&args, &rwPath, rofs_opts, rofs_parse_opt);
    if (res != 0)
    {
        fprintf(stderr, "Invalid arguments\n");
        fprintf(stderr, "see `%s -h' for usage\n", argv[0]);
        exit(1);
    }
    if (rwPath == 0)
    {
        fprintf(stderr, "Missing readwritepath\n");
        fprintf(stderr, "see `%s -h' for usage\n", argv[0]);
        exit(1);
    }

    fuse_main(args.argc, args.argv, &callback_oper, NULL);

    return 0;
}
