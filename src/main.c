/*
 * Copyright (c) 2022 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/** @file
 * @brief WiFi scan sample
 */
#include <zephyr/kernel.h>
#include <stdio.h>
#include <stdlib.h>

#include <zephyr/drivers/gpio.h>
#include <zephyr/dt-bindings/gpio/nordic-nrf-gpio.h>


#define FW_VERSION			"1.4.0"

#define SLEEP_TIME_MS 		10

static const struct gpio_dt_spec iovdd_ctrl_spec = GPIO_DT_SPEC_GET(DT_NODELABEL(iovdd), gpios);
static const struct gpio_dt_spec bucken_spec = GPIO_DT_SPEC_GET(DT_NODELABEL(bucken), gpios);




int wifi_gpio_config(void)
{
	int ret;

	if (!device_is_ready(iovdd_ctrl_spec.port)) {
		return -ENODEV;
	}

	if (!device_is_ready(bucken_spec.port)) {
		return -ENODEV;
	}

	ret = gpio_pin_configure_dt(&bucken_spec, (GPIO_OUTPUT | NRF_GPIO_DRIVE_H0H1));

	ret = gpio_pin_configure_dt(&iovdd_ctrl_spec, GPIO_OUTPUT);

	printf("GPIO configuration done...\n\n");

	return 0;
}


static int wifi_pwron(void)
{
	gpio_pin_set_dt(&bucken_spec, 1);
	k_msleep(SLEEP_TIME_MS);
	gpio_pin_set_dt(&iovdd_ctrl_spec, 1);
	k_msleep(SLEEP_TIME_MS);
	printf("BUCKEN=1, IOVDD=1...\n");

	return 0;
}



void main(void)
{
	wifi_gpio_config();

	// k_sleep(K_SECONDS(1));

	wifi_pwron();

	printf("Starting %s with with version: %s\r\n", CONFIG_BOARD, FW_VERSION);
}
