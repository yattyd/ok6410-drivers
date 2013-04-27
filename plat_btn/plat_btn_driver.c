/*
  plat_btn_driver.c

*/

#include <linux/module.h>
#include <linux/types.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/interrupt.h>
#include <linux/clk.h>
#include <linux/uaccess.h>
#include <linux/io.h>
#include <mach/map.h>
#include <linux/poll.h>
#include <linux/irq.h>
#include <asm/unistd.h>
#include <linux/device.h>
#include <linux/sched.h>	/* TASK_INTERRUPTIBLE */

#define DRIVER_NAME "ok6410_plat_btn"


//定义并初始化等待队列头
static DECLARE_WAIT_QUEUE_HEAD(button_waitq);

static volatile int ev_press = 0;

static int key_value;
static struct device	 *buttons_dev;	/* platform device attached to */
static struct resource	 *buttons_irq;


static int button_irqs[6];//中断号

/*按键中断处理函数*/
static irqreturn_t buttons_interrupt(int irq, void *dev_id)
{
	int i;
	for(i=0; i<6; i++){
		if(irq == button_irqs[i]){
			//printk("==>interrput number:%d\n",irq);
			key_value = i;
			ev_press =1;
			wake_up_interruptible(&button_waitq);
		}
	}

	return IRQ_RETVAL(IRQ_HANDLED);

}

static int s3c6410_buttons_open(struct inode *inode, struct file *file)
{
	int i;
	int err = 0;
	/*注册中断*/
	for(i=0; i<6; i++){
		if (button_irqs[i] < 0)
			continue;

		/*中断触发方式：下降沿触发，中断接口函数*/
		err = request_irq(button_irqs[i],buttons_interrupt,IRQF_TRIGGER_FALLING,NULL,NULL);
		if(err)
			break;
	}

	if (err) {
		i--;
		for (; i >= 0; i--) {
			if (button_irqs[i] < 0) {
				continue;
			}
			disable_irq(button_irqs[i]);
			free_irq(button_irqs[i], NULL);
		}
		return -EBUSY;
	}

	ev_press = 0;
	return 0;
}

static int s3c6410_buttons_close(struct inode *inode, struct file *file)
{
	int i;
	for (i=0; i<6; i++) {
		if (button_irqs[i] < 0) {
			continue;
		}
		free_irq(button_irqs[i],NULL);
	}
	return 0;
}

static int s3c6410_buttons_read(struct file *filp, char __user *buff, size_t count, loff_t *offp)
{
	unsigned long err;
	if (!ev_press) {//如果键没有被按下
		if (filp->f_flags & O_NONBLOCK)//如果是非阻塞模式，就直接返回
			return -EAGAIN;
		else
			//阻塞，直到按键按下
			wait_event_interruptible(button_waitq, ev_press);
	}
	ev_press = 0;

	//将键值送回到用户空间
	err = copy_to_user(buff, &key_value, sizeof(key_value));
	return sizeof(key_value);
}

static unsigned int s3c6410_buttons_poll( struct file *file, struct poll_table_struct *wait)
{
	unsigned int mask = 0;
	poll_wait(file, &button_waitq, wait);//添加等待队列头
	if (ev_press){
		mask |= POLLIN | POLLRDNORM;
	}

	//返回掩码
	return mask;
}

static struct file_operations ok6410_buttons_fops = {
	.owner	 =   THIS_MODULE,
	.open	 =   s3c6410_buttons_open,
	.release =   s3c6410_buttons_close,
	.read	 =   s3c6410_buttons_read,
	.poll	 =   s3c6410_buttons_poll,
};

//声明一个混杂设备,设备名称为"buttons"
static struct miscdevice ok6410_miscdev = {

	.major = MISC_DYNAMIC_MAJOR,
	.minor = MISC_DYNAMIC_MINOR,
	.name ="buttons",
	.fops = &ok6410_buttons_fops,//操作集
};

/* device interface ，当发现匹配设备时会调用此函数*/
static int ok6410_buttons_probe(struct platform_device *pdev)
{
	struct resource *res;
	struct device *dev;
	int ret;
	int size;
	int i;

	printk("probe:%s\n", __func__);
	dev = &pdev->dev;
	buttons_dev = &pdev->dev;

	/*平台资源获取*/
	/*get irq number*/
	for(i=0; i<6; i++){
		//获取中断号
		buttons_irq = platform_get_resource(pdev,IORESOURCE_IRQ,i);
		if(buttons_irq == NULL){
			dev_err(dev,"no irq resource specified\n");
			ret = -ENOENT;
			return ret;
		}
		button_irqs[i] = buttons_irq->start;
		//printk("button_irqs[%d]=%d\n",i,button_irqs[i]);
	}

	//注册混杂设备
	ret = misc_register(&ok6410_miscdev);

	return ret;
}

static int ok6410_buttons_remove(struct platform_device *dev)
{
	misc_deregister(&ok6410_miscdev);
	return 0;
}

/*平台驱动定义*/
static struct platform_driver ok6410_buttons_driver = {
	.probe		= ok6410_buttons_probe,//探针函数
	.remove		= ok6410_buttons_remove,
	.driver		= {
		.owner	= THIS_MODULE,
		.name	= DRIVER_NAME,
	},
};

static char banner[] __initdata = "ok6410 Buttons Driver\n";

static int __init buttons_init(void)
{
	printk(banner);
	/*平台驱动注册*/
	platform_driver_register(&ok6410_buttons_driver);
	return 0;
}

static void __exit buttons_exit(void)
{
	//平台驱动的注销
	platform_driver_unregister(&ok6410_buttons_driver);
}

module_init(buttons_init);
module_exit(buttons_exit);

MODULE_AUTHOR("David Xie");
MODULE_DESCRIPTION("ok6410 Buttons Driver");
MODULE_LICENSE("GPL");
