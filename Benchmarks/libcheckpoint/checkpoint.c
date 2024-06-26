#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <limits.h>
#include"checkpoint.h"

void write_cp(int rank, int iter, void *data, int size){
    char filename[64];

    sprintf( filename, "cpk_directory/check_%d_%d", rank, iter);
    printf("write check_%d_%d \n", rank, iter);
    FILE *fp = fopen( filename, "ab" );
    assert( NULL != fp );
    fwrite( &size, sizeof(int), 1, fp );
    fwrite( data, size, 1, fp );
    fclose( fp );

    sprintf( filename, "cpk_directory/tmp_%d", rank );
    fp = fopen( filename, "wb" );
    assert( NULL != fp );
    fwrite( (char *)&iter, sizeof(int), 1, fp);
    printf("Write tmp_%d: the iter value is: %d \n", rank, iter);
    fclose( fp );
}

void read_cp(int rank, int *iter, int *location, void *data, int size){
    int local_iter;
    char filename[64];
    sprintf( filename, "cpk_directory/tmp_%d", rank);
    FILE *fp = fopen( filename, "rb" );
    assert( NULL != fp );
    fseek(fp, 0, SEEK_SET);
    fread( (char *)&local_iter, sizeof(int), 1, fp );
    printf("local_iter: %d\n",local_iter);
    printf("Read tmp_%d: the iter value is: %d \n", rank, local_iter);
    fclose( fp );
    sprintf( filename, "cpk_directory/check_%d_%d", rank, local_iter);
//// testing ////
    printf("read check_%d_%d \n", rank, local_iter);
    fp = fopen( filename, "rb" );
    assert( NULL != fp );
    fseek(fp, *location, SEEK_SET);
    fread( &size, sizeof(int), 1, fp );
    *location += sizeof(int);
    fread( data, size, 1, fp );
    *location += size;
    fclose( fp );
    printf("Rank %d READCP file\n", rank); 
    *iter = local_iter + 1;
}
