////////////////////////////////////////////////////////////////////////////////
// file: binary_cmp.c
// Author : Tushar Shrivastav
// Date: November 15, 2021
////////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>

#define BUF_SIZE 8192

int usage(char* pname){
    printf("Usage is: %s <filename1 filename2>\n", pname);
    exit(1);
}

int main(int argc, char *argv[])
{
    struct timeval start, end;
    unsigned char buffer1[BUF_SIZE];
    unsigned char buffer2[BUF_SIZE];
    int fd1, fd2, read_bytes1, read_bytes2, compare_bytes;
    int count = 0;
    char *fname1 = argv[1];
    char *fname2 = argv[2];
    int different = 0 ;

    if(argc < 3){
        usage(argv[0]);
    }

    // try and open first file
    if ((fd1 = open (fname1, O_RDONLY) ) < 0 ){
        perror(argv[0]);
        exit(1);
    }

    // try and open second file, if fail close first file before exit
    if ((fd2 = open (fname2, O_RDONLY) ) < 0 ){
        perror(argv[0]);
        close(fd1);
        exit(1);
    }

    // get the time before start 
    gettimeofday(&start, NULL);
    while (1) {
        compare_bytes = BUF_SIZE;
        read_bytes1 = read(fd1, buffer1, BUF_SIZE);
        read_bytes2 = read(fd2, buffer2, BUF_SIZE);

        if ((read_bytes1 < BUF_SIZE) || (read_bytes2 < BUF_SIZE) ){
            if(read_bytes1 < read_bytes2)
                compare_bytes = read_bytes1;
	    else
                compare_bytes = read_bytes2;
        }   

        for (int i = 0; i < compare_bytes; i++){
            count++;
            if (buffer1[i] != buffer2[i]){
		different = 1 ;
                break;	// found the difference so break out of for loop
            }
        }

	// break out of while loop if difference is detected or end of file
        if (different || (compare_bytes < BUF_SIZE))
            break;

    }

    if (different){
    	printf("Difference found at: 0x%x\n", count);
        lseek(fd1, count-1, SEEK_SET);
        lseek(fd2, count-1, SEEK_SET);
        read_bytes1 = read(fd1, buffer1, 16);
        read_bytes2 = read(fd2, buffer2, 16);
        printf("file1->");
        for(int i = 0; i < read_bytes1; i++){
            printf("0x%x ", buffer1[i]);
        }
        printf("\n");
        printf("file2->");
        for(int i = 0; i < read_bytes2; i++){
            printf("0x%x ", buffer2[i]);
        }
        printf("\n");
    }
    else
    	printf("Files are identical!\n");

    gettimeofday(&end, NULL);
    printf("Time taken %ld ms\n", ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec))/1000);
    //close finehandles before exit
    close(fd1);
    close(fd2);

    exit(0);
}

