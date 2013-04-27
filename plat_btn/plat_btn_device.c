/*
  基于platform的ok6410按键中断实验

  http://blog.csdn.net/gotosola/article/details/7548807

  http://blog.csdn.net/gotosola/article/details/7554732


  plat_btn_device.c
*/

#include <linux/module.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/interrupt.h>
#include <linux/device.h>
#include <linux/io.h>


#define DEVICE_NAME "ok6410_plat_btn"


/*平台资源的定义,按键中断*/
static struct resource s3c_buttons_resource[] = {
	[0]={
		.start = IRQ_EINT(0),
		.end   = IRQ_EINT(0),
		.flags = IORESOURCE_IRQ,
	},
	[1]={
		.start = IRQ_EINT(1),
		.end   = IRQ_EINT(1),
		.flags = IORESOURCE_IRQ,
	},
	[2]={
		.start = IRQ_EINT(2),
		.end   = IRQ_EINT(2),
		.flags = IORESOURCE_IRQ,
	},
	[3]={
		.start = IRQ_EINT(3),
		.end   = IRQ_EINT(3),
		.flags = IORESOURCE_IRQ,
	},
	[4]={
		.start = IRQ_EINT(4),
		.end   = IRQ_EINT(4),
		.flags = IORESOURCE_IRQ,
	},
	[5]={
		.start = IRQ_EINT(5),
		.end   = IRQ_EINT(5),
		.flags = IORESOURCE_IRQ,
	}
};

static struct platform_device *s3c_buttons;


static int __init platform_init(void)
{

	s3c_buttons = platform_device_alloc(DEVICE_NAME,-1);

	//为平台设备s3c_buttons添加平台资源
	platform_device_add_resources(s3c_buttons,&s3c_buttons_resource,6);

	/*平台设备的注册*/
	platform_device_add(s3c_buttons);


}

static void __exit platform_exit(void)
{
	platform_device_unregister(s3c_buttons);
}

module_init(platform_init);
module_exit(platform_exit);

MODULE_AUTHOR("Sola");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:ok6410_buttons");
