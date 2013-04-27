#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <asm/uaccess.h>	/* copy_to_user,copy_from_user */
#define MY_MAJOR 240

int my_open(struct inode *inode, struct file *filp)
{
	printk("#########open######\n");
	return 0;
}

ssize_t my_read(struct file * filp, char __user * buf, size_t count,
		loff_t * f_pos)
{
	printk("#########read######\n");
	return count;
}

ssize_t my_write(struct file * filp, const char __user * buf, size_t count,
		 loff_t * f_pos)
{
	printk("#########write######\n");
	return count;
}

int my_release(struct inode *inode, struct file *filp)
{
	printk("#########release######\n");
	return 0;
}

struct file_operations my_fops = {
	.owner = THIS_MODULE,
	.open = my_open,
	.read = my_read,
	.write = my_write,
	.release = my_release,
};

int __init my_init(void)
{
	int rc;
	printk("Test char dev\n");
	rc = register_chrdev(MY_MAJOR, "my", &my_fops);
	if (rc < 0) {
		printk("register %s char dev error\n", "my");
		return -1;
	}

	printk("ok!\n");
	return 0;
}

void __exit my_exit(void)
{
	unregister_chrdev(MY_MAJOR, "my");
	printk("module exit\n");
}

MODULE_LICENSE("GPL");
module_init(my_init);
module_exit(my_exit);
