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
#include <linux/lcd_notify.h>

static struct notifier_block apq_hotplug_lcd_notif;

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

static int __cpuinit apq_hotplug_lcd_notifier_callback(
					struct notifier_block *this,
					unsigned long event, void *data)
{
	switch (event) {
	case LCD_EVENT_ON_END:
	case LCD_EVENT_OFF_START:
		break;
	case LCD_EVENT_ON_START:
		__apq_hotplug_resume();
		break;
	case LCD_EVENT_OFF_END:
		__apq_hotplug_suspend();
		break;
	default:
		break;
	}

	return NOTIFY_OK;
}

static int __init apq_hotplug_init(void)
{
	apq_hotplug_lcd_notif.notifier_call = apq_hotplug_lcd_notifier_callback;

	pr_info("initialized!\n");

	return 0;
}

static void __exit apq_hotplug_exit(void)
{
	lcd_unregister_client(&apq_hotplug_lcd_notif);
	apq_hotplug_lcd_notif.notifier_call = NULL;
}

late_initcall(apq_hotplug_init);
module_exit(apq_hotplug_exit);
