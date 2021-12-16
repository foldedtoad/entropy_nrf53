/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include <zephyr.h>

#include <nrf_rpc.h>

#include "entropy_ser.h"

#include <logging/log.h>
LOG_MODULE_REGISTER(app_main, 3);

#define BUFFER_LENGTH 10

static uint8_t buffer[BUFFER_LENGTH];

static void result_callback(int result, uint8_t *buffer, size_t length)
{
	if (result) {
		LOG_ERR("Entropy remote get failed: %d", result);
		return;
	}

	LOG_HEXDUMP_INF(buffer, sizeof(buffer), "");
}

void main(void)
{
	int err;

	LOG_INF("Entropy sample started [APP Core].");

	err = entropy_remote_init();
	if (err) {
		LOG_ERR("Remote entropy driver initialization failed.");
		return;
	}

	LOG_INF("Remote init send.");

	while (true) {
		k_sleep(K_MSEC(2000));

		LOG_INF("entropy_remote_get");
		err = entropy_remote_get(buffer, sizeof(buffer));
		if (err) {
			LOG_ERR("Entropy remote get failed: %d.", err);
			continue;
		}

		LOG_HEXDUMP_INF(buffer, sizeof(buffer), "");

		k_sleep(K_MSEC(2000));

		LOG_INF("entropy_remote_get_inline");
		err = entropy_remote_get_inline(buffer, sizeof(buffer));
		if (err) {
			LOG_ERR("Entropy remote get failed: %d.", err);
			continue;
		}

		LOG_HEXDUMP_INF(buffer, sizeof(buffer), "");

		k_sleep(K_MSEC(2000));

		LOG_INF("entropy_remote_get_async");
		err = entropy_remote_get_async(sizeof(buffer), result_callback);
		if (err) {
			LOG_ERR("Entropy remote get async failed: %d.", err);
			continue;
		}

		k_sleep(K_MSEC(2000));

		LOG_INF("entropy_remote_get_cbk");
		err = entropy_remote_get_cbk(sizeof(buffer), result_callback);
		if (err) {
			LOG_ERR("Entropy remote get callback failed: %d.", err);
			continue;
		}
	}
}
