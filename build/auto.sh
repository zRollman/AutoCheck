#!/bin/bash 

workdir=$(cd $(dirname $0); pwd)
testPath=${workdir}"/result/"
#echo "$path"

if [[ ! -d $testPath ]]; then
	mkdir -p "$testPath"
fi

#Himeno
export ENABLE_OPENMP="off"
export TRACE_FILE_HOME="/workspace/AutoCheck/tracefile/serial/himeno"
export FUNCTION_NAME="jacobi"
export CALL_LOCATION="116"
export START_LINE="186"
export END_LINE="217"
./AutoCheck
dot -Tpng graph.dot -o result/hi.png

#HPCCG
export ENABLE_OPENMP="off"
export TRACE_FILE_HOME="/workspace/AutoCheck/tracefile/serial/HPCCG"
export FUNCTION_NAME="HPCCG"
export CALL_LOCATION="138"
export START_LINE="118"
export END_LINE="146"
./AutoCheck
dot -Tpng graph.dot -o result/hpccg.png

#CG
export ENABLE_OPENMP="off"
export TRACE_FILE_HOME="/workspace/AutoCheck/tracefile/serial/CG"
export FUNCTION_NAME="main"
export CALL_LOCATION="0"
export START_LINE="296"
export END_LINE="330"
./AutoCheck
dot -Tpng graph.dot -o result/cg.png

#MG
export ENABLE_OPENMP="off"
export TRACE_FILE_HOME="/workspace/AutoCheck/tracefile/serial/MG"
export FUNCTION_NAME="main"
export CALL_LOCATION="0"
export START_LINE="266"
export END_LINE="276"
./AutoCheck
dot -Tpng graph.dot -o result/mg.png

#FT
export ENABLE_OPENMP="off"
export TRACE_FILE_HOME="/workspace/AutoCheck/tracefile/serial/FT"
export FUNCTION_NAME="appft"
export CALL_LOCATION="74"
export START_LINE="109"
export END_LINE="119"
./AutoCheck
dot -Tpng graph.dot -o result/ft.png

#SP
export ENABLE_OPENMP="off"
export TRACE_FILE_HOME="/workspace/AutoCheck/tracefile/serial/SP"
export FUNCTION_NAME="main"
export CALL_LOCATION="0"
export START_LINE="184"
export END_LINE="190"
./AutoCheck
dot -Tpng graph.dot -o result/sp.png

#EP
export ENABLE_OPENMP="off"
export TRACE_FILE_HOME="/workspace/AutoCheck/tracefile/serial/EP"
export FUNCTION_NAME="main"
export CALL_LOCATION="0"
export START_LINE="168"
export END_LINE="213"
./AutoCheck
dot -Tpng graph.dot -o result/ep.png

#IS
export ENABLE_OPENMP="off"
export TRACE_FILE_HOME="/workspace/AutoCheck/tracefile/serial/IS"
export FUNCTION_NAME="main"
export CALL_LOCATION="0"
export START_LINE="732"
export END_LINE="736"
./AutoCheck
dot -Tpng graph.dot -o result/is.png

#BT
export ENABLE_OPENMP="off"
export TRACE_FILE_HOME="/workspace/AutoCheck/tracefile/serial/BT"
export FUNCTION_NAME="main"
export CALL_LOCATION="0"
export START_LINE="180"
export END_LINE="186"
./AutoCheck
dot -Tpng graph.dot -o result/bt.png

#LU
export ENABLE_OPENMP="off"
export TRACE_FILE_HOME="/workspace/AutoCheck/tracefile/serial/LU"
export FUNCTION_NAME="ssor"
export CALL_LOCATION="205"
export START_LINE="115"
export END_LINE="267"
./AutoCheck
dot -Tpng graph.dot -o result/lu.png
