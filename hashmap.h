#ifndef __MAP_H__
#define __MAP_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <pthread.h>

#include "dlist.h"

#define MACADDRSIZE 6

/* -- value_t -- */
typedef struct
{
  dlist_t parent;

  uint64_t key;
  void *real_data;
} value_t;
#define GET_VALUE_INSTANCE(ptr)  ((value_t *)ptr)

static inline value_t *
value_new (void)
{
  value_t *new_value;

  new_value = GET_VALUE_INSTANCE(calloc(1, sizeof(value_t)));
  if (new_value == NULL)
    return NULL;

  return new_value;
}

/* -- Hashmap -- */
typedef struct
{
  size_t elems; /* mapsize */
  size_t mapsize;
  dlist_t *realdata;
  pthread_mutex_t lock;
} hashmap;

static inline void
hashmap_lock(hashmap *map)
{
  if (map)
    pthread_mutex_lock(&(map->lock));
}

static inline void
hashmap_unlock(hashmap *map)
{
  if (map)
    pthread_mutex_unlock(&(map->lock));
}

/* -- Hash functions with macaddr -- */
static inline int
mac2key(const uint8_t *macaddr, uint64_t *out_key)
{
  uint64_t new_key = 0;
  const uint8_t *ptr = macaddr;

  if (macaddr == NULL || out_key == NULL)
    return -1;

  for (int i = 5; i >= 0; ++ptr, i--) {
    new_key |= (uint64_t)*ptr << (8 * i);
  }

  *out_key = new_key;

  return 0;
}

static inline int
rmac2key(const uint8_t *macaddr, uint64_t *out_key)
{
  uint64_t new_key = 0;
  const uint8_t *ptr = macaddr;

  if (macaddr == NULL || out_key == NULL)
    return -1;

  for (int i = 0; i < 6; ++ptr, i--) {
    new_key |= (uint64_t)*ptr << (8 * i);
  }

  *out_key = new_key;

  return 0;
}

#endif
