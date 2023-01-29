#include <linux/module.h>
#include <linux/kernel.h>

int __init hello_init(void) {
	pr_info("LOADED\n");
	return 0;
}

void __exit hello_cleanup(void) {
	pr_info("UNLOAD\n");
}

module_init(hello_init);
module_exit(hello_cleanup);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("!!!\n");
