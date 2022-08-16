#!/bin/bash
# author: Armando Rivero
# date:   2020.03.09
# modif.: 2020.03.09
# ver.:   1.00

echo "erasing compiled files"
rm test_thread_1
rm test_thread_2
rm test_thread_3
rm test_threadSingle
rm test_threads
rm test_threadsArguments

echo "compiling [filename].c"
gcc thread_1.c -lpthread -o test_thread_1
gcc thread_2.c -lpthread -o test_thread_2
gcc thread_3.c -lpthread -o test_thread_3
gcc thread_single.c -lpthread -o test_threadSingle
gcc threads.c -lpthread -o test_threads
gcc threads_arguments.c -lpthread -o test_threadsArguments

