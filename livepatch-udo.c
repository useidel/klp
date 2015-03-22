/*
 * livepatch-udo.c - Udos Kernel Live Patching Sample Module
 * based on
 * livepatch-sample.c - Kernel Live Patching Sample Module
 * Copyright (C) 2014 Seth Jennings <sjenning@redhat.com>
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/livepatch.h>

/*
 * This (dumb) live patch overrides the function that prints the
 * kernel boot uptime when /proc/uptime is read.
 *
 * Example:
 * $ cat /proc/uptime
 * <your uptime>
 * $ insmod livepatch-udo.ko
 * $ cat /proc/uptime
 * Happy Linux Kernel Live Patching!!
 * $ echo 0 > /sys/kernel/livepatch/livepatch-udo/enabled
 * cat /proc/uptime
 * <your uptime>
 */

#include <linux/seq_file.h>
static int livepatch_uptime_proc_show(struct seq_file *m, void *v)
{
	seq_printf(m, "%s\n", "Happy Linux Kernel Live Patching!!");
	return 0;
}

static struct klp_func funcs[] = {
	{
		.old_name = "uptime_proc_show",
		.new_func = livepatch_uptime_proc_show,
	}, { }
};

static struct klp_object objs[] = {
	{
		/* name being NULL means vmlinux */
		.funcs = funcs,
	}, { }
};

static struct klp_patch patch = {
	.mod = THIS_MODULE,
	.objs = objs,
};

static int livepatch_init(void)
{
	int ret;

	ret = klp_register_patch(&patch);
	if (ret)
		return ret;
	ret = klp_enable_patch(&patch);
	if (ret) {
		WARN_ON(klp_unregister_patch(&patch));
		return ret;
	}
	return 0;
}

static void livepatch_exit(void)
{
	WARN_ON(klp_disable_patch(&patch));
	WARN_ON(klp_unregister_patch(&patch));
}

module_init(livepatch_init);
module_exit(livepatch_exit);
MODULE_LICENSE("GPL");
