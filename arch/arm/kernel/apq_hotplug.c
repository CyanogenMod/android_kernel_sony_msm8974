/*
 * apq_hotplug - a multicore hotplug driver
 *
 * Copyright (C) 2015 Tom G. <roboter972@gmail.com>
 *     
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#define pr_fmt(fmt) "apq_hotplug: " fmt

#include <linux/cpu.h>
#include <linux/cpumask.h>
#include <linux/init.h>
#include <linux/pm.h>
#include <linux/suspend.h>

static struct notifier_block pm_notifier_block;

static inline void offline_all(void)
{
	unsigned int cpu;

	for_each_possible_cpu(cpu) {
		if (cpu_online(cpu) && (cpu))
			cpu_down(cpu);
	}
}

static inline void online_all(void)
{
	unsigned int cpu;

	for_each_possible_cpu(cpu) {
		if (!cpu_online(cpu))
			cpu_up(cpu);
	}
}

static void __apq_hotplug_suspend(void)
{
	offline_all();
}

static void __apq_hotplug_resume(void)
{
	online_all();
}

int __cpuinit apq_hotplug_pm_notify(struct notifier_block *notify_block,
					unsigned long mode, void *unused)
{
	switch (mode) {
	case PM_HIBERNATION_PREPARE:
	case PM_SUSPEND_PREPARE:
		__apq_hotplug_suspend();
		break;
	case PM_POST_SUSPEND:
	case PM_POST_HIBERNATION:
	case PM_POST_RESTORE:
		__apq_hotplug_resume();
		break;
	}

	return NOTIFY_OK;
}

static int __init apq_hotplug_init(void)
{
	int ret = 0;

	pm_notifier_block.notifier_call = apq_hotplug_pm_notify;
	register_pm_notifier(&pm_notifier_block);

	pr_info("initialized!\n");

	return ret;
}

static void __exit apq_hotplug_exit(void)
{
	unregister_pm_notifier(&pm_notifier_block);
}

late_initcall(apq_hotplug_init);
module_exit(apq_hotplug_exit);
