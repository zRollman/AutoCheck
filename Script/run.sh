#!/bin/bash

AUTOCHECK_ROOT="/workspace/AutoCheck/"
TRACE_FILE_ROOT=${AUTOCHECK_ROOT}"tracefile/"
TRACE_FOR_SERIAL=${TRACE_FILE_ROOT}"serial/"
TRACE_FOR_PARALLEL=${TRACE_FILE_ROOT}"parallel/"

read -p "Please enter the file name of the trace for the benchmark that needs to identify critical variables: " file_name

#Check if the trace file exists
path=$TRACE_FOR_SERIAL$file_name

echo $path
if [ -f ${path} ];then
        echo -e "\033[34m ${path} exists ! \033[0m"
        read -p "Enable or disable OpenMP acceleration (on or off): " enable_OpenMP
	
        if [ $enable_OpenMP = "on" ]; then
                read -p "Enter the name of the function where the main computation loop is located: " function_name
                read -p "Enter the location where the function for which the main calculation loop exists is called: " call_location
                read -p "Enter the line number at the beginning of the main calculation loop: " start_line_num
                read -p "Enter the line number of the end of the main calculation loop: " end_line_num

                expr $call_location + 1 >/dev/null 2>&1
                if [ $? -ne 0 ]; then
                        echo -e " $call_location is not a number \033[0m "
                        exit 1
                fi
                
                expr $start_line_num + 1 >/dev/null 2>&1
                if [ $? -ne 0 ]; then 
                        echo $start_line_num is not a number
                        exit 1
                fi

                expr $end_line_num + 1 >/dev/null 2>&1
                if [ $? -ne 0 ]; then 
                        echo $end_line_num is not a number
                        exit 1
                fi
		
		trace_file_home=$TRACE_FOR_PARALLEL$file_name
	
                if [ -d $trace_file_home ];then
                        rm -rf $trace_file_home
			mkdir -p $trace_file_home 
                        echo "OK"
                else
                        mkdir -p $trace_file_home
                fi
		# Add environment variable
        	./set_env.sh $enable_OpenMP $path $function_name $call_location $start_line_num $end_line_num $trace_file_home $file_name
	elif [ $enable_OpenMP = "off" ]; then
	
		read -p "Enter the name of the function where the main computation loop is located: " function_name
                read -p "Enter the location where the function for which the main calculation loop exists is called: " call_location
                read -p "Enter the line number at the beginning of the main calculation loop: " start_line_num
                read -p "Enter the line number of the end of the main calculation loop: " end_line_num

                expr $call_location + 1 >/dev/null 2>&1
                if [ $? -ne 0 ]; then
                        echo $call_location is not a number
                        exit 1
                fi
                
                expr $start_line_num + 1 >/dev/null 2>&1
                if [ $? -ne 0 ]; then 
                        echo $start_line_num is not a number
                        exit 1
                fi

                expr $end_line_num + 1 >/dev/null 2>&1
                if [ $? -ne 0 ]; then 
                        echo $end_line_num is not a number
                        exit 1
                fi
		# Add environment variable
		./set_env.sh $enable_OpenMP $path $function_name $call_location $start_line_num $end_line_num $file_name
	
        else
                echo "Please check your input if you need to enable OpenMP (on and off)."
                exit 1
        fi
else
        echo ${path} not exists!
        exit 1
fi

