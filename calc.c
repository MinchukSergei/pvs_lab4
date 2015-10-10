#include <linux/cdev.h> 
#include <linux/fs.h> 
#include <linux/init.h> 
#include <linux/module.h> 
#include <linux/device.h> 
#include <asm/uaccess.h> 

MODULE_LICENSE("GPL"); 
MODULE_AUTHOR("Minchuk SG"); 

#define DEVICE_FIRST  0
#define DEVICE_COUNT  4 
#define DEVICE_NAME "calc" 
#define DIVISION_BY_ZERO 1
#define WRONG_SIGN -1



static int size_of_message;


static long var1;
static long var2;
static char sign;
static long res;

static ssize_t dev_read(struct file * file, char * buf, size_t count, loff_t *ppos);
static ssize_t dev_write(struct file *file, const char __user * buffer, size_t length, loff_t * offset);
static int dev_open(struct inode *inode, struct file *file);
static int dev_release(struct inode *inode, struct file *file);
static int dev_init(void);
static void dev_exit(void);
static int calculate(void);

struct file_operations my_fops = {
	.owner   = THIS_MODULE,
	.read    = dev_read,
	.write   = dev_write,
	.open    = dev_open,
	.release = dev_release,
};

static int calculate(void) 
{
	switch(sign) {
		case '+':
		res = var1 + var2;
		break;

		case '-':
		res = var1 - var2;
		break;		

		case '*':
		res = var1 * var2;
		break;

		case '/':
		if (var2 == 0) {
			return DIVISION_BY_ZERO;
		} else {
			res = var1 / var2;
		}
		break;
		default:
			return WRONG_SIGN;
	}
	return 0;
}

static ssize_t dev_read(struct file * file, char * buf, 
                         size_t len, loff_t *ppos) 
{ 
	int error = 0;
	int major, minor;
	char message[256];
	memset(message, 0, sizeof(message));
	major = MAJOR(file_inode(file)->i_rdev);
	minor = MINOR(file_inode(file)->i_rdev);
	error = calculate();
	switch(minor) {
		case 3:
		if (error == 0) {
			snprintf(message, sizeof(message), "Result is %ld.\n", res);
		} else if (error == DIVISION_BY_ZERO) {
			snprintf(message, sizeof(message), "Division by zero.\n");
		} else if (error == WRONG_SIGN) {
			snprintf(message, sizeof(message), "Bad sign.\n");
		}
		break;
	}

	
	size_of_message = strlen(message);

	if (message[*ppos] == '\0') {
        printk(KERN_INFO "End of string, returning zero.\n");
        return 0;
    }

	copy_to_user(buf, &message[*ppos], 1);
	*ppos += 1;
	return 1;
} 

static ssize_t dev_write(struct file *file, 
	const char __user * buf, size_t length, loff_t * offset) 
{
   int major, minor;
   char buffer[256];
   memset(buffer, 0, sizeof(buffer));

   major = MAJOR(file_inode(file)->i_rdev);
   minor = MINOR(file_inode(file)->i_rdev);
   
   copy_from_user(buffer, buf, sizeof(buf));
   switch(minor) {
   	case 0:
   		copy_from_user(buffer, buf, sizeof(buf));
   		kstrtol(buffer, 10, &var1);
   	break;

   	case 1:
   		copy_from_user(buffer, buf, sizeof(buf));
   		kstrtol(buffer, 10, &var2);
   	break;

    case 2:
    	copy_from_user(buffer, buf, sizeof(buf));
    	sign = buffer[0];
   	break;
   }
   size_of_message = strlen(buffer);
   
   return size_of_message;
} 

static int dev_open(struct inode *inode,struct file *filp)
{
    return 0;
}

static int dev_release(struct inode *inode,struct file *filp)
{
    return 0;
}

static int dev_init(void)
{
	int err = 0;
	// first = MKDEV(300, 0);
 //    register_chrdev_region(first, 4, "calc");
 // 	size_of_message = 0;
 //    cdev_init(&c_dev, &my_fops);
 //    c_dev.owner = THIS_MODULE;
 //    if (cdev_add(&c_dev, first, 4)) {
 //    	printk("Device not added\n");
 //    	return -1;
 //    }
    
    err = register_chrdev(300, "calc", &my_fops); 
	if(err < 0) { 
		printk(KERN_ERR "Can not register char device\n"); 
		return err;
	}
	return 0;
}
 
/* remove the module
 */
static void dev_exit(void)
{
	// cdev_del(&c_dev);
 //    device_destroy(cl, first);
 //    class_destroy(cl);
 //    unregister_chrdev_region(first, 4);
 	unregister_chrdev(300, "calc");
    printk(KERN_ALERT "Device unregistered\n");
}

module_init(dev_init);
module_exit(dev_exit);