bin/test1_aos_scalar_cpp_debug.exe >result_test1_aos_scalar_cpp_debug.txt
perl compare.pl test1_reference.txt result_test1_aos_scalar_cpp_debug.txt >diff_test1_aos_scalar_cpp_debug.txt

bin/test2_aos_scalar_cpp_debug.exe >result_test2_aos_scalar_cpp_debug.txt
perl compare.pl test2_reference.txt result_test2_aos_scalar_cpp_debug.txt >diff_test2_aos_scalar_cpp_debug.txt

bin/test3_aos_scalar_cpp_debug.exe >result_test3_aos_scalar_cpp_debug.txt
perl compare.pl test3_reference.txt result_test3_aos_scalar_cpp_debug.txt >diff_test3_aos_scalar_cpp_debug.txt

bin/test4_aos_scalar_cpp_debug.exe >result_test4_aos_scalar_cpp_debug.txt
perl compare.pl test4_reference.txt result_test4_aos_scalar_cpp_debug.txt >diff_test4_aos_scalar_cpp_debug.txt

bin/test1_aos_SSE_cpp_debug.exe >result_test1_aos_SSE_cpp_debug.txt
perl compare.pl test1_reference.txt result_test1_aos_SSE_cpp_debug.txt >diff_test1_aos_SSE_cpp_debug.txt


