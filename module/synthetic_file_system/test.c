#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/rwlock.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/init.h>
#include <linux/string.h>
#include <linux/proc_fs.h>

static rwlock_t lock;
static struct proc_dir_entry *test = NULL;
static struct kobject *example_kobj = NULL;
static int major = 0;
static char test_string[10] = "Hello\n\0";

ssize_t test_read(struct file *fd, char __user *buff, size_t size, loff_t *off) {
	size_t rc = 0;
	read_lock(&lock);
	rc = simple_read_from_buffer(buff, size, off, test_string, 10);
	read_unlock(&lock);

	return rc;
}

ssize_t test_write(struct file *fd,const char __user *buff, size_t size, loff_t *off) {
	size_t rc = 0;

	write_lock(&lock);
	rc = simple_write_to_buffer(test_string, 10, off, buff, size);
	write_unlock(&lock);

	rc = strlen(test_string);

	return rc;
}

ssize_t test_proc_read(struct file *fd, char __user *buff, size_t size, loff_t *off) {
	size_t rc = 0;
	
	rc = simple_read_from_buffer(buff, size, off, test_string, 10);
	
	return rc;
}

ssize_t test_proc_write(struct file *fd,const char __user *buff, size_t size, loff_t *off) {
	size_t rc = 0;

	rc = simple_write_to_buffer(test_string, 10, off, buff, size);

	return rc;
}

static ssize_t b_show(struct kobject *kobj, struct kobj_attribute *attr,
		      char *buf)
{
	size_t rc = 0;

	memcpy(buf, test_string, 10);
	rc = strlen(test_string);
	
	return rc;
}

static ssize_t b_store(struct kobject *kobj, struct kobj_attribute *attr,
		       const char *buf, size_t count)
{
	size_t rc = 0;

	memcpy(test_string, buf, count);
	rc = strlen(test_string);
	
	return rc;
}

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.write = test_write,
	.read = test_read	
};

static const struct file_operations pops = {
	.read = test_proc_read,
	.write = test_proc_write
};

static struct kobj_attribute string_attribute =
	__ATTR(test_string, 0664, b_show, b_store);

static struct attribute *attrs[] = {
	&string_attribute.attr,
	NULL,	/* need to NULL terminate the list of attributes */
};

static struct attribute_group attr_group = {
	.attrs = attrs,
};


int init_module(void) {
	int retval;
	
	pr_info("LOADED\n");
	rwlock_init(&lock);
	major = register_chrdev(major, "test", &fops);

	if(major < 0) {
		return major;
	}

	test = proc_create("test", 0666, NULL, &pops);
	example_kobj = kobject_create_and_add("kobject_example", kernel_kobj);

	if (!example_kobj)
		return -ENOMEM;

	retval = sysfs_create_group(example_kobj, &attr_group);

	if (retval)
		kobject_put(example_kobj);
	
	pr_info("Major = %d\n", major);

	return retval;
}

void cleanup_module(void) {

	unregister_chrdev(major, "test");
	proc_remove(test);
	kobject_put(example_kobj);
	pr_info("UNLOAD\n");
}

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("Eltex task \"module 3\"\n");
