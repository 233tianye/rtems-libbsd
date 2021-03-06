/* Copyright 2008-2015 Freescale Semiconductor, Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *	 notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *	 notice, this list of conditions and the following disclaimer in the
 *	 documentation and/or other materials provided with the distribution.
 *     * Neither the name of Freescale Semiconductor nor the
 *	 names of its contributors may be used to endorse or promote products
 *	 derived from this software without specific prior written permission.
 *
 *
 * ALTERNATIVELY, this software may be distributed under the terms of the
 * GNU General Public License ("GPL") as published by the Free Software
 * Foundation, either version 2 of that License or (at your option) any
 * later version.
 *
 * THIS SOFTWARE IS PROVIDED BY Freescale Semiconductor ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL Freescale Semiconductor BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __MAC_H
#define __MAC_H

#include <linux/device.h>
#include <linux/if_ether.h>
#include <linux/phy.h>
#include <linux/list.h>
#ifdef __rtems__
#include <linux/netdevice.h>
#endif /* __rtems__ */

#include "fman_port.h"
#include "fman.h"
#include "fman_mac.h"

struct fman_mac;
struct mac_priv_s;

#ifdef __rtems__
struct dpaa_eth_data {
	struct device_node *mac_node;
	struct mac_device *mac_dev;
	int mac_hw_id;
	int fman_hw_id;
};
int dpaa_eth_priv_probe(struct platform_device *pdev,
    struct mac_device *mac_dev);
int dpa_eth_priv_start(struct net_device *net_dev);
int dpa_eth_priv_stop(struct net_device *net_dev);
#endif /* __rtems__ */
struct mac_device {
#ifndef __rtems__
	struct resource		*res;
#endif /* __rtems__ */
	u8			 addr[ETH_ALEN];
	struct fman_port	*port[2];
	u32			 if_support;
	struct phy_device	*phy_dev;
#ifdef __rtems__
	struct fman_ivars	 ivars[2];
	struct platform_device	 pdev;
	struct dpaa_eth_data	 data;
	struct net_device	 net_dev;
#endif /* __rtems__ */

	bool autoneg_pause;
	bool rx_pause_req;
	bool tx_pause_req;
	bool rx_pause_active;
	bool tx_pause_active;
	bool promisc;

#ifndef __rtems__
	struct phy_device *(*init_phy)(struct net_device *net_dev,
				       struct mac_device *mac_dev);
#else /* __rtems__ */
	bool use_dedicated_portal;
	struct qman_portal *portal;
	void (*adjust_link)(struct mac_device *mac_dev, u16 speed);
#endif /* __rtems__ */
	int (*init)(struct mac_device *mac_dev);
	int (*start)(struct mac_device *mac_dev);
	int (*stop)(struct mac_device *mac_dev);
	int (*set_promisc)(struct fman_mac *mac_dev, bool enable);
	int (*change_addr)(struct fman_mac *mac_dev, enet_addr_t *enet_addr);
	int (*set_multi)(struct net_device *net_dev,
			 struct mac_device *mac_dev);
	int (*set_rx_pause)(struct fman_mac *mac_dev, bool en);
	int (*set_tx_pause)(struct fman_mac *mac_dev, u8 priority,
			    u16 pause_time, u16 thresh_time);
	int (*set_exception)(struct fman_mac *mac_dev,
			     enum fman_mac_exceptions exception, bool enable);
	int (*add_hash_mac_addr)(struct fman_mac *mac_dev,
				 enet_addr_t *eth_addr);
	int (*remove_hash_mac_addr)(struct fman_mac *mac_dev,
				    enet_addr_t *eth_addr);

	struct fman_mac		*fman_mac;
	struct mac_priv_s	*priv;
};

#ifndef __rtems__
struct dpaa_eth_data {
	struct device_node *mac_node;
	struct mac_device *mac_dev;
	int mac_hw_id;
	int fman_hw_id;
};
#endif /* __rtems__ */

extern const char	*mac_driver_description;

int fman_set_mac_active_pause(struct mac_device *mac_dev, bool rx, bool tx);

void fman_get_pause_cfg(struct mac_device *mac_dev, bool *rx_pause,
			bool *tx_pause);

#endif	/* __MAC_H */
