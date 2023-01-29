#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/rwlock.h>

static rwlock_t lock;
static int major = 0;
static char test_string[10] = "Hello\n\0";

ssize_t file_read(struct file *fd, char __user *buff, size_t size, loff_t *off) {
	size_t rc = 0;
	read_lock(&lock);
	rc = simple_read_from_buffer(buff, size, off, test_string, 10);
	read_unlock(&lock);

	return rc;
}

ssize_t file_write(struct file *fd,const char __user *buff, size_t size, loff_t *off) {
	size_t rc = 0;

	write_lock(&lock);
	rc = simple_write_to_buffer(test_string, 10, off, buff, size);
	write_unlock(&lock);

	return rc;
}

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.write = file_write,
	.read = file_read	
};

int init_module(void) {
	pr_info("LOADED\n");
	rwlock_init(&lock);
	major = register_chrdev(major, "test", &fops);

	if(major < 0) {
		return major;
	}
	
	pr_info("Major = %d\n", major);

	return 0;
}

void cleanup_module(void) {

	unregister_chrdev(major, "test");
	pr_info("UNLOAD\n");
}

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("Eltex task \"module 2\"\n");
