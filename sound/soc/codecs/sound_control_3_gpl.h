/*
 * Author: Paul Reioux aka Faux123 <reioux@gmail.com>
 *
 * WCD93xx sound control module header
 * Copyright 2013 Paul Reioux
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

#ifndef _SOUND_CONTROL_3_GPL_H
#define _SOUND_CONTROL_3_GPL_H

extern struct snd_soc_codec *fauxsound_codec_ptr;
extern unsigned int wcd9xxx_hw_revision;

int snd_hax_reg_access(unsigned int reg);
unsigned int snd_hax_cache_read(unsigned int reg);
void snd_hax_cache_write(unsigned int reg, unsigned int value);

#endif
