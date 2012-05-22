#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kprobes.h>



int my_inet_bind(struct socket *sock, struct sockaddr *uaddr, int addr_len)
{
        printk("%d %lx\n", current->pid, sock);

        dump_stack();

	jprobe_return();
	return 0;
}

static struct jprobe my_jprobe = {
	.entry			= my_inet_bind,
	.kp = {
		.symbol_name	= "inet_bind",
	},
};

static int __init jprobe_init(void)
{
	int ret;

	ret = register_jprobe(&my_jprobe);
	if (ret < 0) {
		printk(KERN_INFO "register_jprobe failed, returned %d\n", ret);
		return -1;
	}
	printk(KERN_INFO "Planted jprobe at %p, handler addr %p\n",
	       my_jprobe.kp.addr, my_jprobe.entry);
	return 0;
}

static void __exit jprobe_exit(void)
{
	unregister_jprobe(&my_jprobe);
	printk(KERN_INFO "jprobe at %p unregistered\n", my_jprobe.kp.addr);
}

module_init(jprobe_init)
module_exit(jprobe_exit)
MODULE_LICENSE("GPL");
