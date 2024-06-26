#ifndef _CHECKPOINT_H
#define _CHECKPOINT_H

#ifdef __cplusplus
extern "C" {
#endif

void write_cp(int rank, int iter, void *data, int size);
void read_cp(int rank, int *iter, int *location, void *data, int size);

#ifdef __cplusplus
}
#endif

#endif