#ifndef __ARPCACHE_H__
#define __ARPCACHE_H__

#include "base.h"
#include "types.h"
#include "list.h"

#include <pthread.h>

#define MAX_ARP_SIZE 32
#define ARP_ENTRY_TIMEOUT 15
#define ARP_REQUEST_MAX_RETRIES	5

struct cached_pkt {  // 缓存的包结构
	struct list_head list;
	char *packet;
	int len;			// todo: !important: 这里带上length是因为packet实际上就是一个首地址的作用，通过这个首地址+数据长度获取到相应的结构体数据
};

struct arp_req {  // 等待ARP回复的包缓存，二维链表结构
	struct list_head list;  // 指向前一个和后一个arp_req的指针结构体
	iface_info_t *iface;
	u32 ip4;
	time_t sent;					  // 发送时间
	int retries;					  // 重试次数
	struct list_head cached_packets;  // 自身维护的链表
};

struct arp_cache_entry {  // ARP表中的一个条目
	u32 ip4; 	// stored in host byte order IP地址，本地字节序
	u8 mac[ETH_ALEN];
	time_t added;
	int valid; 		// 是否继续有效，如果无效置为0，维护线程会自动清理掉
};

typedef struct {  // ARP表
	struct arp_cache_entry entries[MAX_ARP_SIZE]; // table行数组
	struct list_head req_list;  // 等待ARP回复的包结构
	pthread_mutex_t lock;  // ARP表维护线程
	pthread_t thread; // ARP表维护线程
} arpcache_t;

void arpcache_init();
void arpcache_destroy();
void *arpcache_sweep(void *);

int arpcache_lookup(u32 ip4, u8 mac[]);
void arpcache_insert(u32 ip4, u8 mac[]);
void arpcache_append_packet(iface_info_t *iface, u32 ip4, char *packet, int len);
void print_arp_cache_list();

#endif
