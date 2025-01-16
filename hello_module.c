#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>

#define PROC_NAME "my_module"
#define BUFFER_SIZE 256

static struct proc_dir_entry *proc_entry;
static char buffer[BUFFER_SIZE];

// Read function
static ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos){
    int len = sprintf(buffer, "Hello from kernel module!\n");
    return simple_read_from_buffer(usr_buf, count, pos, buffer, len);
}

// Define proc_ops structure
static const struct proc_ops proc_fops = {
    .proc_read = proc_read,
};

// Module initialization
static int __init hello_init(void){
    proc_entry = proc_create(PROC_NAME, 0, NULL, &proc_fops);
    if (proc_entry == NULL){
        printk(KERN_ALERT "Failed to create /proc/%s\n", PROC_NAME);
        return -ENOMEM;
    }
    printk(KERN_INFO "/proc/%s created\n", PROC_NAME);
    return 0;
}

// Module cleanup
static void __exit hello_exit(void){
    proc_remove(proc_entry);
    printk(KERN_INFO "/proc/%s removed\n", PROC_NAME);
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("A simple kernel module using proc_ops");

module_init(hello_init);
module_exit(hello_exit);
