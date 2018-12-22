#include <linux/init.h>
#include <linux/module.h>
#include <linux/reboot.h>
#include <linux/slab.h>

static int snakeoil_wipe(struct notifier_block *nb, unsigned long action, void *data)
{
	void *ptr = NULL;
	void *prev = NULL;

	while ((ptr = kmalloc(4096, GFP_KERNEL|__GFP_NORETRY)) != NULL) {
		memset(ptr, 0, 4096);
		*(void **)ptr = prev;
		prev = ptr;
	}

	while (prev != NULL) {
		ptr = *(void **)prev;
		kfree(prev);
		prev = ptr;
	}

	return 0;
}

static struct notifier_block snakeoil_reboot_notifier = {
	.notifier_call = snakeoil_wipe,
	.next          = NULL,
	.priority      = 0
};

static int __init snakeoil_init(void)
{
	int ret = register_reboot_notifier(&snakeoil_reboot_notifier);

	if (ret != 0) {
		// error handling
		return 1;
	}

	printk(KERN_INFO "snakeoil loaded\n");

	return 0;
}

static void __exit snakeoil_exit(void)
{
	unregister_reboot_notifier(&snakeoil_reboot_notifier);
	printk(KERN_INFO "snakeoil unloaded\n");
}

module_init(snakeoil_init);
module_exit(snakeoil_exit);
