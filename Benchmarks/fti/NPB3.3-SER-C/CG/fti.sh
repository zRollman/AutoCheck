make clean
make CLASS=S

cp ../bin/cg.S.x ./cg

mpirun --allow-run-as-root -np 4 ./cg
