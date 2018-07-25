#!/bin/sh

MODULE="pslist"
MODULE_PATH=`pwd`
OUTPUT_PATH=`pwd`
OUTPUT_FILE_NAME="pslist_out.txt"

#############################################
#Remove pslist module if already loaded
#############################################
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

#############################################
# Launching pslist module
#############################################
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

echo "Clearing old logs"
echo "$MODULE_PATH/$OUTPUT_FILE_NAME" > /dev/null

echo "Copying dmesg logs to output file.."
dmesg > $MODULE_PATH/$OUTPUT_FILE_NAME

echo "=============================================================="
echo "PS LIST OUTPUT PRESENT AT:"
echo "$MODULE_PATH/$OUTPUT_FILE_NAME"
echo "=============================================================="
