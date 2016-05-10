/*
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 */
#ifndef __LWIPOPTS_H__
#define __LWIPOPTS_H__


#define NO_SYS                      1
#define MEMP_MEM_MALLOC             1
#define MEM_ALIGNMENT               4  //很重要，平台相关
#define MEM_SIZE                    (10*1024)
#define MEMP_NUM_PBUF               10
#define MEMP_NUM_UDP_PCB            6
#define MEMP_NUM_TCP_PCB            10
#define MEMP_NUM_TCP_PCB_LISTEN     6
#define MEMP_NUM_TCP_SEG            12
#define MEMP_NUM_REASSDATA          16
#define MEMP_NUM_ARP_QUEUE          10
#define PBUF_POOL_SIZE              12
#define LWIP_ARP                    0
#define IP_REASS_MAX_PBUFS          4
#define IP_FRAG_USES_STATIC_BUF     0
#define IP_DEFAULT_TTL              128
#define IP_SOF_BROADCAST            0
#define IP_SOF_BROADCAST_RECV       0
#define LWIP_ICMP                   0
#define LWIP_BROADCAST_PING         0
#define LWIP_MULTICAST_PING         0
#define LWIP_RAW                    1
#define LWIP_TCP                    1
#define TCP_WND                     (2 * TCP_MSS)
#define TCP_MSS                     1460
#define TCP_SND_BUF                 (2 * TCP_MSS)
#define TCP_LISTEN_BACKLOG          1
#define TCP_QUEUE_OOSEQ             0
#define LWIP_DHCP                   0
#define LWIP_NETIF_STATUS_CALLBACK  1
#define LWIP_NETIF_LINK_CALLBACK    1
#define LWIP_NETIF_HWADDRHINT       1
#define LWIP_NETCONN                0
#define LWIP_SOCKET                 0
#define LWIP_STATS_DISPLAY          0
#define MEM_STATS                   0
#define SYS_STATS                   0
#define MEMP_STATS                  0
#define LINK_STATS                  0
#define ETHARP_TRUST_IP_MAC         0
#define ETH_PAD_SIZE                2
#define LWIP_CHKSUM_ALGORITHM       2
#define LWIP_TCP_KEEPALIVE          1

// Keepalive values, compliant with RFC 1122. Don't change this unless you know what you're doing
#define TCP_KEEPIDLE_DEFAULT        10000UL // Default KEEPALIVE timer in milliseconds
#define TCP_KEEPINTVL_DEFAULT       2000UL  // Default Time between KEEPALIVE probes in milliseconds
#define TCP_KEEPCNT_DEFAULT         9U      // Default Counter for KEEPALIVE probes

//// Use hardware checksum offload
//#define CHECKSUM_GEN_IP             0
//#define CHECKSUM_GEN_UDP            0
//#define CHECKSUM_GEN_TCP            0
//#define CHECKSUM_GEN_ICMP           0
//#define CHECKSUM_CHECK_IP           0
//#define CHECKSUM_CHECK_UDP          0
//#define CHECKSUM_CHECK_TCP          0
//#define CHECKSUM_CHECK_ICMP         0

///* RTE based configuration */
//#define ETH0_PHY                    ST802RT1 // or DP83848C

///* MAC address */
//#define MAC_ADDR0                   2
//#define MAC_ADDR1                   0
//#define MAC_ADDR2                   0
//#define MAC_ADDR3                   0
//#define MAC_ADDR4                   0
//#define MAC_ADDR5                   0
// 
///* Static IP address */
//#define IP_ADDR0                    192
//#define IP_ADDR1                    168
//#define IP_ADDR2                    0
//#define IP_ADDR3                    10
//   
///* NET mask*/
//#define NETMASK_ADDR0               255
//#define NETMASK_ADDR1               255
//#define NETMASK_ADDR2               255
//#define NETMASK_ADDR3               0

///* Gateway address */
//#define GW_ADDR0                    192
//#define GW_ADDR1                    168
//#define GW_ADDR2                    0
//#define GW_ADDR3                    1

/*disable ipcp_up() HISADDR check 
  because some ISP will not provide this field. */
#define PPP_HISADDR_WARNING 

#endif /* __LWIPOPTS_H__ */
