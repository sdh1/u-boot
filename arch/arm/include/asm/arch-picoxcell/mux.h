/*
 * linux/arch/arm/mach-picoxcell/mux.h
 *
 * Copyright (c) 2010 Picochip Ltd., Jamie Iles
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * All enquiries to support@picochip.com
 */
#ifndef __MUX_H__
#define __MUX_H__

#include <linux/list.h>

enum mux_setting {
	MUX_PERIPHERAL_RSVD	= -1,
	MUX_UNMUXED,
	MUX_ARM,
	MUX_SD,
	MUX_PERIPHERAL_FRACN,
	MUX_PERIPHERAL_EBI,
	MUX_PERIPHERAL_PAI,
	MUX_PERIPHERAL_DECODE,
	MUX_PERIPHERAL_SSI,
	MUX_PERIPHERAL_MII,
	MUX_PERIPHERAL_MAXIM,
	MUX_PERIPHERAL_USIM,
	NR_MUX_SETTINGS,
};

enum mux_flags {
	MUX_RO			= (1 << 0),
	MUX_INVERT_PERIPH	= (1 << 1),
	MUX_CONFIG_BUS		= (1 << 2),
};

struct mux_def {
	const char		*name;
	int			armgpio;
	int			sdgpio;
	int			periph;
	int			periph_b;
	s16			gpio_reg_offs;
	s16			gpio_reg_bit;
	s16			periph_reg;
	s16			periph_bit;
	u16			caeid;
	u16			caddr;
	u16			mask;
	unsigned		flags;
	struct list_head	head;
	enum mux_setting	(*get_setting)(const struct mux_def *def);
};

struct mux_cfg {
	const char		*name;
	enum mux_setting	setting;
};

#define MUXCFG(__name, __setting) \
	{ .name = __name, .setting = __setting }

extern int mux_configure_one(const char *name, enum mux_setting setting);
extern int mux_configure_table(const struct mux_cfg *cfg,
			       unsigned int nr_cfgs);

#define MUXGPIO(__name, __arm, __sd, __periph, __gpio_reg, __gpio_bit, \
		__periph_reg, __periph_bit, __flags) { \
	.name		= #__name, \
	.armgpio	= __arm, \
	.sdgpio		= __sd, \
	.periph		= MUX_PERIPHERAL_ ## __periph, \
	.gpio_reg_offs	= __gpio_reg, \
	.gpio_reg_bit	= __gpio_bit, \
	.periph_reg	= __periph_reg, \
	.periph_bit	= __periph_bit, \
	.periph_b	= -1, \
	.flags		= __flags, \
}

#define MUX2PERIPH(__name, __periph, __periph_b, __periph_reg, \
		   __periph_bit) { \
	.name		= #__name, \
	.armgpio	= -1, \
	.sdgpio		= -1, \
	.periph		= MUX_PERIPHERAL_ ## __periph, \
	.periph_b	= MUX_PERIPHERAL_ ## __periph_b, \
	.periph_reg	= __periph_reg, \
	.periph_bit	= __periph_bit, \
}

#define MUXCFGBUS(__name, __arm, __sd, __periph, __caeid, __caddr, __mask) { \
	.name		= #__name, \
	.armgpio	= __arm, \
	.sdgpio		= __sd, \
	.periph		= MUX_PERIPHERAL_ ## __periph, \
	.caeid		= __caeid, \
	.caddr		= __caddr, \
	.flags		= MUX_CONFIG_BUS, \
	.mask		= __mask, \
	.periph_b	= -1, \
}

#define MUXGPIOFUNC(__name, __arm, __sd, __periph, __get_func) { \
	.name		= #__name, \
	.armgpio	= __arm, \
	.sdgpio		= __sd, \
	.periph		= MUX_PERIPHERAL_ ## __periph, \
	.get_setting	= __get_func, \
	.flags		= MUX_RO, \
}

extern void picoxcell_mux_register(struct mux_def *defs, int nr_defs);

#endif /* __MUX_H__ */