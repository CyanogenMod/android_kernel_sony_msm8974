/* drivers/misc/fsync_control.h
 *
 * Copyright 2012  Ezekeel
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef _LINUX_FSYNC_CONTROL_H
#define _LINUX_FSYNC_CONTROL_H

#define FSYNCCONTROL_VERSION 1

static bool fsync_enabled = true;

bool fsynccontrol_fsync_enabled(void);

#endif
