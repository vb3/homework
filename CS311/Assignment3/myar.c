#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include <ar.h>
#include <time.h>
#include <utime.h>
#include <string.h>
#include <dirent.h>

#define _BSD_SOURCE
#define DEBUG
#define STR_SIZE sizeof("rwxrwxrwx")
#define FP_SPECIAL 1
#define BLOCKSIZE 8

/* S_ISVTX is a common extension to POSIX.  */
# ifndef S_ISVTX
#  define S_ISVTX 01000
# endif

int fileExists(char *fPath);
int verifyArchive(char *arcName);
void appNamedFiles(char *arcName, char *files[], int fileCount);
void extNamedFiles(char *arcName, char *files[], int fileCount);
void printFiles(char *arcName, int bConsise);
void delNamedFiles(char *arcName, char *files[], int fileCount);
void appAllFiles(char *arcName);
char *filePermStr(mode_t perm, int flags);
char *formatEpoch(int iEpoch, const char *format);
int fchmod(int fd, mode_t mode);
int fchown(int fd, uid_t owner, gid_t group);

int main(int argc, char *argv[])
{
    char archName[16];
    char cCom;
    /* error checking for number of argc */
    if (argc < 2){
        printf("Invalid number of arguments\n");
        exit(EXIT_FAILURE);
    }

    /* error check "key" portion of the arguments should be argv[1] */
    if (strlen(argv[1]) != 2) {
        printf("Invalid option choosen. Make sure only one "
               "option is selected. \nie: myar -q archive.a filename.ext\n");
        exit(EXIT_FAILURE);
    } else {
        cCom = argv[1][1];
    }

    /* check to see if archive exists if command not 'q' */
    if (cCom != 'q' && cCom != 'A') {
        /* if archive doesnt exist, quit with error */
        if(!(fileExists(argv[2]))) {
            printf("Archive was not found\n");
            exit(EXIT_FAILURE);
        }
      
        /* verify archive */
        if (!(verifyArchive(argv[2]))) {
            printf("Archive file format not recognized\n");
            exit(EXIT_FAILURE);
        }
    }

    /* call right function based on option given by user */
    if (cCom == 'q') {
        appNamedFiles(argv[2], &argv[3], argc-3);
    } else if (cCom == 'x') {
        extNamedFiles(argv[2], &argv[3], argc-3);
    } else if (cCom == 't') {
        printFiles(argv[2], 1);
    } else if (cCom == 'v') {
        printFiles(argv[2], 0);
    } else if (cCom == 'd') {
        delNamedFiles(argv[2], &argv[3], argc-3);
    } else if (cCom == 'A') {
        appAllFiles(argv[2]);
    } else {
        printf("Invalid command\n");
        exit(EXIT_FAILURE);
    }
    
    return 0;
}

int fileExists(char * fPath)
{
    int fd;
    /* try to open file */
    fd = open(fPath, O_RDONLY);
    if (fd == -1) { /* if file does not exist */
        return 0;
    } else { /* if file exists */
        close(fd);
        return 1;
    }
}

int verifyArchive(char *arcName)
{
    char buf[SARMAG + 1];
    int arcFD;
    int num_read;
    
    /* open archive */
    arcFD = open(arcName, O_RDONLY);
    if(arcFD == -1){
        perror("Can't open archive file");
        exit(EXIT_FAILURE);
    }
    
    /* get the first SARMAG bytes of archive */
    num_read = read(arcFD, buf, SARMAG);
    if (num_read == -1) {
        perror("Reading error");
        exit(EXIT_FAILURE);
    }
    
    /* make into string */
    buf[SARMAG] = '\0';
    
    /* string compare buf with ARMAG */
    if (strcmp(buf, ARMAG) == 0) {
        close(arcFD);
        return 1;
    }
    
    close(arcFD);
    return 0;
}

void appNamedFiles(char *arcName, char *files[], int fileCount)
{
    int i;
    int in_fd;
    int out_fd;
    char buf[BLOCKSIZE];
    int location;
    int iBufSize;
    int num_read;
    int num_written;
    struct stat sb;
    struct ar_hdr aFileHeader;
    off_t fileSize = 0;
    
    
    /* create archive if it doesnt exist */
    if (!(fileExists(arcName))) {
        out_fd = open(arcName, (O_WRONLY | O_CREAT),
                     (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH));
        if(out_fd == -1){
            perror("Can't create new archive file");
            exit(EXIT_FAILURE);
        }
        
        num_written = write(out_fd, ARMAG, SARMAG);
        if (num_written == -1){
			perror("error writing to archive file");
            exit(EXIT_FAILURE);
		}
    } else {
        /* verify archive */
        if (!(verifyArchive(arcName))) {
            printf("Archive file format not recognized\n");
            exit(EXIT_FAILURE);
        }
        
        out_fd = open(arcName, O_WRONLY);
        if(out_fd == -1){
            perror("Can't open archive file for write");
            exit(EXIT_FAILURE);
        }
    }
    
    /* move file pointer to end of file */
    if (lseek(out_fd, 0, SEEK_END) < 0) {
        perror("Error moving file pointer");
        exit(EXIT_FAILURE);
    }
    
    /* check if all the files listed exists */
    for (i=0; i<fileCount; i++) {
        /* error check if file exists */
        if (!(fileExists(files[i]))) {
            printf("File '%s' was not found.\n", files[i]);
            continue;
        } else if (strlen(files[i]) > 15) {
            printf("File name '%s' more than 15 chars not supported.\n", files[i]);
            continue;
        }
        
        /* get stat, and fill in ar_hdr */
        if (stat(files[i], &sb) == -1) {
            perror("error getting stat for file");
            printf("%s", aFileHeader.ar_name);
            exit(EXIT_FAILURE);
        }
        
        /* ar_name */
        sprintf(aFileHeader.ar_name, "%s%-16c", files[i], '/');
        /* ar_date */
        sprintf(aFileHeader.ar_date, "%-12ld", sb.st_mtime);
        /* ar_uid */
        sprintf(aFileHeader.ar_uid, "%-6ld", (long) sb.st_uid);
        /* ar_gid */
        sprintf(aFileHeader.ar_gid, "%-6ld", (long) sb.st_gid);
        /* ar_mode */
        sprintf(aFileHeader.ar_mode, "%-8lo", (unsigned long) sb.st_mode);
        /* ar_size & set fileSize */
        sprintf(aFileHeader.ar_size, "%-10ld", sb.st_size);
        fileSize = sb.st_size;
        /* ar_fmag */
        strcpy(aFileHeader.ar_fmag, ARFMAG);
        
        /* write ar_hdr to archive */
        num_read = (int)sizeof(struct ar_hdr);
        num_written = write(out_fd, &aFileHeader, num_read);
        location = 0;
        while((num_read = (int)sizeof(struct ar_hdr)) != num_written){
            num_read -= num_written;
            location += num_written;
            num_written = write(out_fd, &aFileHeader + location, num_read);
        }
        
        /* open file being inputted into archive */
        in_fd = open(files[i], O_RDONLY);
        if(in_fd == -1){
            perror("Can't open file for read");
            exit(EXIT_FAILURE);
        }
        
        iBufSize = BLOCKSIZE;
        /* loop until entire file copied */
        while (fileSize > 0) {
            if (iBufSize > fileSize) iBufSize = fileSize;
            num_read = read(in_fd, buf, iBufSize);
            num_written = write(out_fd, buf, num_read);
            
            location = 0;
            while(num_read != num_written){
                num_read -= num_written;
                location += num_written;
                num_written = write(out_fd, buf + location, num_read);
            }
            
            if (num_written == -1){
                perror("error writing - archive might be corrupted");
                exit(EXIT_FAILURE);
            }
            
            /* subtract # of bytes written from total file size */
            fileSize -= iBufSize;
        }

        /* if filesize was odd, add '\n' to end of file. */
        if (iBufSize % 2 == 1) {
            buf[0] = '\n';
            while (num_written = write(out_fd, buf, 1) != 1) {
                if (num_written == -1){
                    perror("error writing - archive might be corrupted");
                    exit(EXIT_FAILURE);
                }
            }
        }
        
        /* close input file */
        close(in_fd);
    }
    
    close(out_fd);
}

void extNamedFiles(char *arcName, char *files[], int fileCount)
{
    int i;
    int iBufSize;
    size_t fileSize;
    int in_fd;
    int out_fd;
    int num_read;
	int num_written = 0;
    char buf[BLOCKSIZE];
    int location;
    struct ar_hdr aFileHeader;
    struct utimbuf utb;
    uid_t uid;
    gid_t gid;
    mode_t mode;
    struct stat sb;
    
    /* open archive for read */
    in_fd = open(arcName, O_RDONLY);
    if(in_fd == -1){
        perror("Can't open archive file for read");
        exit(EXIT_FAILURE);
    }
    
    /* move file pointer past the archive header (ARMAG) */
    if (lseek(in_fd, SARMAG, SEEK_SET) < 0) {
        perror("Error moving file pointer");
        exit(EXIT_FAILURE);
    }
    
    /* read first file header */
    num_read = read(in_fd, &aFileHeader, (int)sizeof(aFileHeader));
    if (num_read == 0){
        printf("No files in archive\n");
        exit(EXIT_SUCCESS);
    }
    
    /* loop through all files in archive */
    while (num_read != 0) {
        if (num_read == -1) {
            perror("Error reading file header from archive");
            exit(EXIT_FAILURE);
        }
        
        /* change aFileHeader.ar_name to string */
        for (i=0; i<16; i++){
            if (aFileHeader.ar_name[i] == '/') {
                aFileHeader.ar_name[i] = '\0';
                break;
            }
        }
        
        /* get filesize */
        fileSize = strtol(aFileHeader.ar_size, NULL, 10);

        /* check if all the files listed exists */
        for (i=0; i<fileCount; i++) {
            if (strcmp(aFileHeader.ar_name, files[i]) == 0) {
            /* file exists */
                strcpy(files[i], "");
                
                /* delete file if exists */
                unlink(files[i]);
                
                /* open file for write */
                out_fd = open(aFileHeader.ar_name, (O_WRONLY | O_CREAT), S_IRWXU);
                if(out_fd == -1){
                    perror("Can't open file to write");
                    exit(EXIT_FAILURE);
                }
                
                iBufSize = BLOCKSIZE;
                /* loop until entire file copied */
                while (fileSize > 0) {
                    if (iBufSize > fileSize) iBufSize = fileSize;
                    num_read = read(in_fd, buf, iBufSize);
                    num_written = write(out_fd, buf, num_read);
                    
                    location = 0;
                    while(num_read != num_written){
                        num_read -= num_written;
                        location += num_written;
                        num_written = write(out_fd, buf + location, num_read);
                    }
                    
                    if (num_written == -1){
                        perror("error writing - deleting output file");
                        unlink(files[i]);
                        exit(EXIT_FAILURE);
                    }
                    
                    /* subtract # of bytes written from total file size */
                    fileSize -= iBufSize;
                }
                
                /* restore mtime and atime */
                utb.modtime = utb.actime = atoi(aFileHeader.ar_date);
                utime(aFileHeader.ar_name, &utb);
                
                /* restore UID and GID */
                uid = atoi(aFileHeader.ar_uid);
                gid = atoi(aFileHeader.ar_gid);
                if (fchown(out_fd, uid, gid) == -1) {
                    perror("fchown");
                    exit(EXIT_FAILURE);
                }
                
                /* restore permissions (mode) */
                mode = strtol(aFileHeader.ar_mode, NULL, 8);
                if (fchmod(out_fd, mode) == -1) {
                    perror("fchmod");
                    exit(EXIT_FAILURE);
                }
                
                /* close written file */
                if (close(out_fd) == -1) {
                    perror("error closing file");
                    exit(EXIT_FAILURE);
                }
                
                /* go to next file in archive */
                break;
            }
        }
        
        /* account for odd filesize */
        if (strtol(aFileHeader.ar_size, NULL, 10) % 2 == 1) {
            fileSize++;
        }
        
        /* move file pointer to next file header */
        if (lseek(in_fd, fileSize, SEEK_CUR) < 0) {
            perror("Error moving file pointer");
            exit(EXIT_FAILURE);
        }
        /* read file header of next file */
        num_read = read(in_fd, &aFileHeader, (int)sizeof(aFileHeader));
    }
    
    /* list files not found in archive */
    for (i=0; i<fileCount; i++) {
        if (strcmp(files[i], "") != 0) printf("no entry %s in archive\n", files[i]);
    }
    
    if (close(in_fd) == -1) {
        perror("error closing file");
        exit(EXIT_FAILURE);
    }
}

void printFiles(char *arcName, int bConsise)
{
    int i;
    int arcFD;
    char buf[60];
    int num_read;
    struct ar_hdr aFileHeader;
    int fileSize;
    
    arcFD = open(arcName, O_RDONLY);
    if(arcFD == -1){
        perror("Can't open archive file for read");
        exit(EXIT_FAILURE);
    }
    
    /* move file pointer past the archive header (ARMAG) */
    if (lseek(arcFD, SARMAG, SEEK_SET) < 0) {
        perror("Error moving file pointer");
        exit(EXIT_FAILURE);
    }
    
    num_read = read(arcFD, &aFileHeader, (int)sizeof(aFileHeader));
    if (num_read == 0){
        printf("No files in archive\n");
        exit(EXIT_SUCCESS);
    }
    
    /* loop through all files in archive */
    while (num_read != 0) {
        if (num_read == -1) {
            perror("Error reading file header from archive");
            exit(EXIT_FAILURE);
        }
        
        /* if not consises, then verbose information */
        if (!(bConsise)) {
            printf("%s ", filePermStr(atoi(aFileHeader.ar_mode), 0));
            printf("%d/%d%7d ", atoi(aFileHeader.ar_uid),
                   atoi(aFileHeader.ar_gid), atoi(aFileHeader.ar_size));
            printf("%s", formatEpoch(atoi(aFileHeader.ar_date), "%b %d %R %Y "));
        }
        
        /* print out of the name of the file */
        for (i=0; i<16; i++){
            if (aFileHeader.ar_name[i] == '/') break;
            printf("%c", aFileHeader.ar_name[i]);
        }
        printf("\n");
        
        /*move file pointer to next file header, account for odd filesize */
        if ((fileSize = atoi(aFileHeader.ar_size)) % 2 == 1) fileSize++;
        
        if (lseek(arcFD, fileSize, SEEK_CUR) < 0) {
            perror("Error moving file pointer");
            exit(EXIT_FAILURE);
        }
        num_read = read(arcFD, &aFileHeader, (int)sizeof(aFileHeader));
    }
    
    close(arcFD);

}

void delNamedFiles(char *arcName, char *files[], int fileCount)
{
    int i;
    int iBufSize;
    size_t fileSize;
    int in_fd;
    int out_fd;
    int num_read;
    int num_written = 0;
    char buf[BLOCKSIZE];
    int location;
    struct ar_hdr aFileHeader;
    int bDelete;
    char tempFileName[16];
    
    /* open archive for read */
    in_fd = open(arcName, O_RDONLY);
    if(in_fd == -1){
        perror("Can't open archive file for read");
        exit(EXIT_FAILURE);
    }
    
    /* unlink old archive */
    if(unlink(arcName) == -1){
        perror("cant unlink");
        exit(EXIT_FAILURE);
    }
    
    /* open new archive for write */
    out_fd = open(arcName, (O_WRONLY | O_CREAT),
                  (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH));
    if(out_fd == -1){
        perror("Can't open new archive file for write");
        exit(EXIT_FAILURE);
    }
    
    /* write ARMAG on new archive */
    num_written = write(out_fd, ARMAG, SARMAG);
    if (num_written == -1){
        perror("error writing to archive file");
        exit(EXIT_FAILURE);
    }
    
    /* move file pointer past the archive header (ARMAG) in_fd */
    if (lseek(in_fd, SARMAG, SEEK_SET) < 0) {
        perror("Error moving file pointer");
        exit(EXIT_FAILURE);
    }
           
    /* read first file header */
    num_read = read(in_fd, &aFileHeader, (int)sizeof(aFileHeader));
    if (num_read == -1){
        perror("read 1st header from old arch");
        exit(EXIT_FAILURE);
    }
    if (num_read == 0){
        printf("No files in archive\n");
        exit(EXIT_SUCCESS);
    }
    
    /* loop through all files in archive */
    while (num_read != 0) {
        if (num_read == -1) {
            perror("Error reading file header from archive");
            exit(EXIT_FAILURE);
        }
        
        /* change aFileHeader.ar_name to string */
        for (i=0; i<16; i++){
            if ((tempFileName[i] = aFileHeader.ar_name[i]) == '/') {
                tempFileName[i] = '\0';
                break;
            }
        }
        
        /* get filesize */
        fileSize = strtol(aFileHeader.ar_size, NULL, 10);
        if (fileSize % 2 == 1) fileSize++;
        
        bDelete = 0;
        /* loop through all file arguments */
        for (i=0; i<fileCount; i++) {
            if (strcmp(tempFileName, files[i]) == 0) {
                /* file needs to be deleted. */
                bDelete = 1;
                strcpy(files[i], "");
                break;
            }
        }
        
        /* file doesnt need to be deleted, so write to new archive */
        if (!(bDelete == 1)) {
            iBufSize = BLOCKSIZE;
            
            /* write ar_hdr to new archive */
            num_read = (int)sizeof(struct ar_hdr);
            num_written = write(out_fd, &aFileHeader, num_read);
            location = 0;
            while((num_read = (int)sizeof(struct ar_hdr)) != num_written){
                num_read -= num_written;
                location += num_written;
                num_written = write(out_fd, &aFileHeader + location, num_read);
            }
            
            /* loop until entire file copied */
            while (fileSize > 0) {
                if (iBufSize > fileSize) iBufSize = fileSize;
                num_read = read(in_fd, buf, iBufSize);
                num_written = write(out_fd, buf, num_read);
                
                location = 0;
                while(num_read != num_written){
                    num_read -= num_written;
                    location += num_written;
                    num_written = write(out_fd, buf + location, num_read);
                }
                
                if (num_written == -1){
                    perror("error writing - deleting output file");
                    unlink(files[i]);
                    exit(EXIT_FAILURE);
                }
                
                /* subtract # of bytes written from total file size */
                fileSize -= iBufSize;
            }
        }
        
        /* move file pointer to next file header */
        if (lseek(in_fd, fileSize, SEEK_CUR) < 0) {
            perror("Error moving file pointer");
            exit(EXIT_FAILURE);
        }
        /* read file header of next file */
        num_read = read(in_fd, &aFileHeader, (int)sizeof(aFileHeader));
    }
    
    /* list files not found in archive */
    for (i=0; i<fileCount; i++) {
        if (strcmp(files[i], "") != 0) printf("no entry %s in archive\n", files[i]);
    }
    
    /* close written file */
    if (close(out_fd) == -1) {
        perror("error closing file");
        exit(EXIT_FAILURE);
    }
    
    /* close read file */
    if (close(in_fd) == -1) {
        perror("error closing file");
        exit(EXIT_FAILURE);
    }
    
}

void appAllFiles(char *arcName)
{
    char buf[1];
    DIR *dirptnr;
    struct dirent *aDirFile;
    char *files[255];
    
    /* open directory */
    if ((dirptnr = opendir(".")) == NULL) {
        perror("Could not open directory");
        exit(EXIT_FAILURE);
    }
    
    /* loop through all the items in dir, to get file count */
    while ((aDirFile=readdir(dirptnr)) != NULL) {
        /* skip if name starts with "." */
        if (strncmp(aDirFile->d_name, ".", 1) == 0) continue;
        /* skip if name is the archive name */
        if (strncmp(aDirFile->d_name, arcName, strlen(arcName)) == 0) continue;
        
        files[0] = aDirFile->d_name;
        appNamedFiles(arcName, &files[0], 1);
    }

    closedir(dirptnr);
}

/*  Return ls(1)-style string for file permissions mask */
char *filePermStr(mode_t perm, int flags)
{
	static char str[STR_SIZE];
	snprintf(str, STR_SIZE, "%c%c%c%c%c%c%c%c%c",
	         (perm & S_IRUSR) ? 'r' : '-', (perm & S_IWUSR) ? 'w' : '-',
	         (perm & S_IXUSR) ?
	         (((perm & S_ISUID) && (flags & FP_SPECIAL)) ? 's' : 'x') :
	         (((perm & S_ISUID) && (flags & FP_SPECIAL)) ? 'S' : '-'),
	         (perm & S_IRGRP) ? 'r' : '-', (perm & S_IWGRP) ? 'w' : '-',
	         (perm & S_IXGRP) ?
	         (((perm & S_ISGID) && (flags & FP_SPECIAL)) ? 's' : 'x') :
	         (((perm & S_ISGID) && (flags & FP_SPECIAL)) ? 'S' : '-'),
	         (perm & S_IROTH) ? 'r' : '-', (perm & S_IWOTH) ? 'w' : '-',
	         (perm & S_IXOTH) ?
	         (((perm & S_ISVTX) && (flags & FP_SPECIAL)) ? 't' : 'x') :
	         (((perm & S_ISVTX) && (flags & FP_SPECIAL)) ? 'T' : '-'));
	return str;
}

/*  Returns a string containing the current time */
char *formatEpoch(int iEpoch, const char *format)
{
    static char buf[1000];
    time_t t;
    size_t s;
    struct tm *tm;
    
    t = iEpoch;
    tm = localtime(&t);
    if (tm == NULL) return NULL;
    
    s = strftime(buf, 1000, (format != NULL) ? format : "%c", tm);
    
    return (s == 0) ? NULL : buf;
}
