# coding=utf-8

import os
import subprocess


def file_repair(file_num, sub_file_path):
    zf = len(str(file_num))
    for i in range(file_num):
        file_name = sub_file_path  + str(i).zfill(zf)
        with open(file_name, 'r+') as fp:
            my_str = fp.readline()
            if my_str == "\n" or my_str == r"b'\n'":
                fp.close()
                continue
            str_list = my_str.split(",")
            if str_list[0] == "0":
                fp.close()
                continue
            else:
                file_name =  sub_file_path  + str(i - 1).zfill(zf)
                file_name =  sub_file_path  + str(i).zfill(zf)
                file_copy = ""
                with open(file_name, 'rb+') as fp:
                    while 1:
                        str_temp = fp.readline()
                        # print(str_temp)
                        # str_temp_0 = str(str_temp)
                        str_temp_0 = str_temp.decode('utf-8')
                        # print(str_temp_0[0])
                        if str_temp_0[0] == '0':
                            break

                        file_copy += str_temp_0

                file_name_p = sub_file_path  + str(i - 1).zfill(zf)

                with open(file_name_p, 'a') as fpw:
                    fpw.write(str(file_copy))


def get_threads_num():
    cmd = 'nproc --all'
    result = subprocess.Popen(['nproc', '--all'], stdout=subprocess.PIPE)
    stdout, stderr = result.communicate()
    res = stdout.split()[0]
    num_of_threads = int(res.decode('utf-8'))
    
    # print(threads_num)
    return num_of_threads


# get the path of trace file
trace_file_path = os.getenv('TRACE_FILE_HOME')

# get the file name
file_name = os.path.basename(trace_file_path)

sub_file_home = os.getenv('SUB_FILE_HOME')
sub_file_home += "/file" 
#sub_file_home = os.path(sub_file_home)
#print(sub_file_home)
#print(trace_file_path)
#get line number of the trace file
with open(trace_file_path, 'r+') as trace_fp:
    line_num = len(trace_fp.readlines())

# get the number of threads
threads_num = get_threads_num()

# get the line number of each file
lines_each = int(line_num / threads_num) + 1
#print(lines_each)
# split the trace file
cmd = 'split -d -a ' + str(len(str(threads_num))) + ' ' + '-l ' + str(lines_each) + ' ' + str(trace_file_path) + ' ' + str(sub_file_home)
os.system(cmd)
#print(cmd)
file_repair(threads_num, sub_file_home)



