#Overview
Kernel module to list tasks

# Execution
Things required to execute the modlule are wrapped in a script. Execution
step is as follows

1. sh pslist.sh
2. Output will be dumped into pslist_out.txt

# Sample outputs
1. pslist_out.txt is the sample output from the kernel module
2. Additionally some profilng information is collected by modifying the script
     a. Execution time of system command ps for 50 iterations present at exec_time/pstime.txt
     b. Execution time of pslist module for 50 iterations present at exec_time/pslist_out.txt

