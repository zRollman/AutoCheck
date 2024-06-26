mpicc -o Himeno himenoBMT.c -I/usr/bin/fti/include -L/usr/bin/fti/lib -lfti
mpirun --allow-run-as-root -np 4 ./Himeno

