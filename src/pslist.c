#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/delay.h>

extern struct task_struct init_task;

//#define update() printk("\033[H\033[J")
//#define gotoxy(x, y) printk("\033[%d;%dH", x, y)

void printk_repeatc (char input , int count )
{
    int i = 0;
    for (i=0; i != count; i++ )
    {
        printk(KERN_CONT "%c", input);
    }
}

/* Main function for the pslist module.*/
void print_process_tree(struct task_struct *pTask)
{
    struct task_struct *pChild = NULL;
    struct list_head *pListHeadEle = NULL;
    
    //printk(KERN_INFO "print_process_tree++\n");
    if(NULL == pTask)
    {
        printk(KERN_ERR "Task is NULL\n"); 
    }
    else
    {
               
        printk(KERN_ERR "%10.10s\t%10.3d\t%10.3li\t%10.3d \n",  
                pTask->comm, pTask->pid, pTask->state, pTask->real_parent->pid);
        printk_repeatc('-',50);
        
        list_for_each(pListHeadEle, &pTask->children) 
        {
            pChild = list_entry(pListHeadEle, struct task_struct, sibling);
            print_process_tree(pChild);
        }
    }
    //printk(KERN_INFO "print_process_tree--\n");
}

/* This function is called when the module is loaded. */
int pslist_init(void)
{
    printk(KERN_INFO "Loading Module\n");
    printk_repeatc('=',50);
    printk(KERN_ERR "%10s\t%10s\t%10s\t%10s\t\n","NAME","PID","STATE","PARENT");
    printk_repeatc('=',50);
//  update();
    print_process_tree(&init_task);
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
