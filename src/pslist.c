#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/time.h>
#include <linux/rcupdate.h>
#include <linux/rculist.h>

extern struct task_struct init_task;
static int process_count = 0;

/* Utility function for pretty printing */
void printk_repeatc (char input , int count )
{
    int i = 0;
    for (i=0; i != count; i++ )
    {
        printk(KERN_CONT "%c", input);
    }
}

/* Get time in Microseconds */
unsigned long long get_time_us(void)
{
   static struct timeval _t;  
   do_gettimeofday(&_t);  
   return _t.tv_usec + (1000000 * _t.tv_sec);
}

/* Main function for the pslist module.*/
void print_process_tree(struct task_struct *pTask)
{
    struct task_struct *pChild = NULL;
    //struct list_head *pListHeadEle = NULL;
    
    if(NULL == pTask)
    {
        printk(KERN_ERR "Task is NULL\n"); 
    }
    else
    {

        printk(KERN_ERR "%10.10s\t%10.3d\t%10.3li\t%10.3d \n",  
                pTask->comm, pTask->pid, pTask->state, pTask->real_parent->pid);
        ++process_count;
        printk_repeatc('-',50);
        list_for_each_entry_rcu(pChild, &pTask->children, sibling) 
        {
            print_process_tree(pChild);
        }
    }
}

/* This function is called when the module is loaded. */
int pslist_init(void)
{
    unsigned long long start_time = 0;
    unsigned long long  end_time = 0;

    printk(KERN_DEBUG "Loading Module\n");
    
    /* Print table title */
    printk_repeatc('=',50);
    printk(KERN_ERR "%10s\t%10s\t%10s\t%10s\t\n","NAME","PID","STATE","PARENT");
    printk_repeatc('=',50);
    
    start_time = get_time_us();
    rcu_read_lock();
    print_process_tree(&init_task);
    rcu_read_unlock();
    end_time = get_time_us();

    printk("\n\n");
    printk_repeatc('=',20);
    printk(KERN_ERR "PSLIST EXEC STATS\n");
    printk_repeatc('=',20);
    printk(KERN_ERR "Number of processes: %d\n", process_count);
    printk(KERN_ERR "Execution time: %llu us\n", (end_time - start_time));
    return 0;
}

/* This function is called when the module is removed. */
void pslist_exit(void)
{
    printk(KERN_INFO "Removing Module\n");
}

/* Macros for registering module entry and exit points. */
module_init(pslist_init);
module_exit(pslist_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("pslist");
MODULE_AUTHOR("Vivek Maran");
