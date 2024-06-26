# /bin/bash
if [ $# -eq 7 ];then
	export ENABLE_OPENMP=$1
	export TRACE_FILE_HOME=$2
	export FUNCTION_NAME=$3
	export CALL_LOCATION=$4
	export START_LINE=$5
	export END_LINE=$6
	./AutoCheck
	dot -Tpng graph.dot -o result/$7.png
	
else
        export TRACE_FILE_HOME=$2
	export SUB_FILE_HOME=$7
	python3.8 process.py
	export ENABLE_OPENMP=$1
        export TRACE_FILE_HOME=$2
        export FUNCTION_NAME=$3
        export CALL_LOCATION=$4
        export START_LINE=$5
        export END_LINE=$6
        export SUB_FILE_HOME=$7
	echo $7
	./AutoCheck
	dot -Tpng graph.dot -o result/$8.png
fi
#./get_env
