/* Copyright (c) 2014-2015, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
#include <linux/gpio.h>
#include <linux/module.h>
#include <linux/of_gpio.h>
#include <linux/platform_device.h>
#include <linux/suspend.h>
#include "smp2p_private.h"

#define SET_DELAY (2 * HZ)
#define PROC_AWAKE_ID 12 /* 12th bit */
int slst_gpio_base_id;

static int smp2p_sleepstate_probe(struct platform_device *pdev)
{
	struct device_node *node = pdev->dev.of_node;

	slst_gpio_base_id = of_get_gpio(node, 0);
	if (slst_gpio_base_id == -EPROBE_DEFER) {
		return slst_gpio_base_id;
	} else if (slst_gpio_base_id < 0) {
		SMP2P_ERR("%s: Error to get gpio %d\n",
				__func__, slst_gpio_base_id);
		return slst_gpio_base_id;
	}


	gpio_set_value(slst_gpio_base_id + PROC_AWAKE_ID, 1);

	return 0;
}

static struct of_device_id msm_smp2p_slst_match_table[] = {
	{.compatible = "qcom,smp2pgpio_sleepstate_3_out"},
	{},
};

static struct platform_driver smp2p_sleepstate_driver = {
	.probe = smp2p_sleepstate_probe,
	.driver = {
		.name = "smp2p_sleepstate",
		.owner = THIS_MODULE,
		.of_match_table = msm_smp2p_slst_match_table,
	},
};

static int __init smp2p_sleepstate_init(void)
{
	int ret;
	ret = platform_driver_register(&smp2p_sleepstate_driver);
	if (ret) {
		SMP2P_ERR("%s: smp2p_sleepstate_driver register failed %d\n",
			 __func__, ret);
		return ret;
	}

	return 0;
}

module_init(smp2p_sleepstate_init);
MODULE_DESCRIPTION("SMP2P SLEEP STATE");
MODULE_LICENSE("GPL v2");
