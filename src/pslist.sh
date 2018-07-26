#!/bin/sh

MODULE="pslist"
MODULE_PATH=`pwd`
OUTPUT_PATH=`pwd`
OUTPUT_FILE_NAME="pslist_out.txt"

#############################################
#Remove pslist module if already loaded
#############################################
echo ""
echo "--------------------------"
echo "CLEAN UP                 |"
echo "--------------------------"
echo "Removing pslist module if already loaded.."
if lsmod | grep "$MODULE" &> /dev/null ; then
  sudo rmmod $MODULE > /dev/null
  echo "Removed the module $MODULE"
else
  echo "pslist module is not already loaded"
fi


#############################################
# Flush demsg logs
#############################################
echo "Flushing dmesg logs.."
dmesg -c > /dev/null
echo "Removing existing logs"
rm $OUTPUT_PATH/$OUTPUT_FILE_NAME

#############################################
# PS Data
#############################################
echo ""
echo "--------------------------"
echo "PS COMMAND               |"
echo "--------------------------"
process_count=`ps --no-headers -aux | wc -l`
process_count=$((process_count-3))

echo "Number of processes: $process_count"
`/usr/bin/time --format='Execution time is %e seconds' ps -aux --no-headers > /dev/null`

#############################################
# Launching pslist module
#############################################
echo ""
echo "--------------------------"
echo "PSLIST LKM               |"
echo "--------------------------"
echo "Launching pslist module.."
sudo insmod $MODULE_PATH/pslist.ko
if [ $? -eq 0 ]; then
    echo "Successfully launched pslist module"
else
    echo "Oops, something bad happened :("
    exit 1;
fi

#############################################
# Writing dmesg to output file
#############################################
echo ""
echo "--------------------------"
echo "LOG BACKUP               |"
echo "--------------------------"
echo "Clearing old logs"
echo "$MODULE_PATH/$OUTPUT_FILE_NAME" > /dev/null
echo "Copying dmesg logs to output file.."
dmesg > $MODULE_PATH/$OUTPUT_FILE_NAME
echo "OutputPath: $MODULE_PATH"
echo "OutputFile: $OUTPUT_FILE_NAME"

echo ""
echo "--------------------------"
echo "GUI DISPLAY              |"
echo "--------------------------"
echo "Opening outputfile in graphical text editor"
echo ""

gedit $MODULE_PATH/$OUTPUT_FILE_NAME &
