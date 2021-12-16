/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include <zephyr.h>
#include <errno.h>
#include <metal/sys.h>
#include <metal/device.h>
#include <metal/alloc.h>
#include <openamp/open_amp.h>
#include <ipc/rpmsg_service.h>
#include <init.h>

#include "nrf_rpc.h"
#include "nrf_rpc_rpmsg.h"

#include <logging/log.h>
LOG_MODULE_REGISTER(NRF_RPC_TR, 3);


/* Asynchronous event notification type */
enum nrf_rpc_event_type {
	NRF_RPC_EVENT_CONNECTED,  /* Handshake was successful */
	NRF_RPC_EVENT_DATA,       /* New packet arrived */
};


/* Semaphore used to do initial handshake and synchronisation.*/
K_SEM_DEFINE(handshake_sem, 0, 1);
K_SEM_DEFINE(remote_init_sem, 0, 1);

/* Upper level callbacks */
static nrf_rpc_tr_receive_handler_t receive_callback;

static int endpoint_id;
static bool is_handshake_done;

/* Translates RPMsg error code to nRF RPC error code. */
static int translate_error(int rpmsg_err)
{
	switch (rpmsg_err) {
	case RPMSG_ERR_BUFF_SIZE:
	case RPMSG_ERR_NO_MEM:
	case RPMSG_ERR_NO_BUFF:
		return -NRF_ENOMEM;
	case RPMSG_ERR_PARAM:
		return -NRF_EINVAL;
	case RPMSG_ERR_DEV_STATE:
	case RPMSG_ERR_INIT:
	case RPMSG_ERR_ADDR:
		return -NRF_EIO;
	default:
		if (rpmsg_err < 0) {
			return -NRF_EIO;
		}
		break;
	}
	return 0;
}

static void event_handler(enum nrf_rpc_event_type event,
				const uint8_t *buf, size_t length)
{
	if (event == NRF_RPC_EVENT_CONNECTED) {
		LOG_INF("nRF RPC Connected");
		k_sem_give(&handshake_sem);
		return;
	} else if (event != NRF_RPC_EVENT_DATA) {
		return;
	}

	NRF_RPC_ASSERT(buf != NULL);

	LOG_HEXDUMP_INF(buf, length, "Received data");

	receive_callback(buf, length);
}

static int endpoint_cb(struct rpmsg_endpoint *ept, void *data, size_t len,
	uint32_t src, void *priv)
{
	if (len == 0) {
		if (!is_handshake_done) {
			if (!IS_ENABLED(CONFIG_RPMSG_SERVICE_MODE_MASTER)) {
				k_sem_take(&remote_init_sem, K_FOREVER);
			}
			rpmsg_service_send(endpoint_id, (uint8_t *)"", 0);
			is_handshake_done = true;
			LOG_INF("Handshake done");
			event_handler(NRF_RPC_EVENT_CONNECTED, NULL, 0);
		}
		return RPMSG_SUCCESS;
	}

	event_handler(NRF_RPC_EVENT_DATA, data, len);

	return RPMSG_SUCCESS;
}

static int nrf_rpc_register_endpoint(const struct device *dev)
{
	ARG_UNUSED(dev);

	int err = rpmsg_service_register_endpoint("nrf_rpc", endpoint_cb);

	endpoint_id = err;

	if (err < 0) {
		LOG_ERR("Registering endpoint failed with %d", err);
		return err;
	}

	return 0;
}

int nrf_rpc_tr_init(nrf_rpc_tr_receive_handler_t callback)
{
	NRF_RPC_ASSERT(callback != NULL);
	receive_callback = callback;

	if (IS_ENABLED(CONFIG_RPMSG_SERVICE_MODE_MASTER)) {
		LOG_INF("RPC Master");
		while (!rpmsg_service_endpoint_is_bound(endpoint_id)) {
			k_sleep(K_MSEC(1));
		}

		rpmsg_service_send(endpoint_id, (uint8_t *)"", 0);
	} else {
		LOG_INF("RPC Remote");
		k_sem_give(&remote_init_sem);
	}

	k_sem_take(&handshake_sem, K_FOREVER);

	if (endpoint_id < 0)	{
		return -NRF_EIO;
	}

	return 0;
}

int nrf_rpc_tr_send(uint8_t *buf, size_t len)
{
	int err;

	NRF_RPC_ASSERT(buf != NULL);
	LOG_INF("Send %u bytes.", len);
	LOG_HEXDUMP_INF(buf, len, "Data:");


	err = rpmsg_service_send(endpoint_id, buf, len);
	if (err > 0) {
		err = 0;
	}

	return translate_error(err);
}

SYS_INIT(nrf_rpc_register_endpoint, POST_KERNEL, CONFIG_RPMSG_SERVICE_EP_REG_PRIORITY);
