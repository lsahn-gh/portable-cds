#include "map.h"

/* --- bloom filter --- */

/* --- hashmap section --- */
static inline void
map_init(dlist_t map[], int mapsize)
{
  int i;

  for (i = 0; i < mapsize; i++) {
    dlist_init_head(&map[i]);
  }
}

hashmap *
hashmap_new(size_t mapsize)
{
  hashmap *new_map;

  if (mapsize < 17)
    mapsize = 17;

  new_map = (hashmap *)malloc(sizeof(hashmap));
  if (!new_map)
    goto out;
  new_map->elems = 0;
  new_map->mapsize = mapsize;
  new_map->realdata = (dlist_t *)malloc(sizeof(dlist_t) * mapsize);
  if (!(new_map->realdata) && new_map) {
    free(new_map);
    new_map = NULL;
    goto out;
  }

  pthread_mutex_init(&(new_map->lock), NULL);
  map_init(new_map->realdata, mapsize);

out:
  return new_map;
}

void
hashmap_destroy(hashmap **map)
{
  hashmap *obj;

  if (map == NULL || *map == NULL)
    return;

  obj = *map;

  pthread_mutex_destroy(&(obj->lock));
  if (obj->realdata)
    free(obj->realdata);
  free(obj);

  *map = NULL;
}

void *
hashmap_put(hashmap *map, const uint64_t key, void *data)
{
  value_t* new_value = NULL;
  size_t index;

  if (map == NULL || data == NULL)
    return NULL;

  new_value = value_new();
  if (!new_value)
    return NULL;

  new_value->key = key;
  new_value->real_data = data;

  index = key % (map->mapsize);

  dlist_append(&(map->realdata[index]), GET_DLIST(new_value));
  map->elems += 1;

  return map;
}

int
hashmap_key_exist(hashmap* map, const uint64_t key)
{
  int ret = 0;
  size_t index;

  if (map == NULL)
    return ret;

  index = key % (map->mapsize);
  dlist_foreach_safe (&(map->realdata[index]))
  {
    value_t *v = GET_VALUE(__ptr);

    if (v->key == key) {
      ret = 1;
      break;
    }
  }

  return ret;
}

value_t *
hashmap_get_value_object_unsafe(hashmap *map, const uint64_t key)
{
  void* ret = NULL;
  size_t index;

  index = key % (map->mapsize);
  dlist_foreach_safe (&(map->realdata[index]))
  {
    value_t *v = GET_VALUE(__ptr);
    if (v->key == key) {
      ret = v;
      break;
    }
  }

  return ret;
}

void *
hashmap_get_value_data_unsafe(const value_t *value)
{
  void *ret = NULL;

  if (value) {
    ret = value->real_data;
  }

  return ret;
}

value_t *
hashmap_remove(hashmap *map, const uint64_t key)
{
  void* ret = NULL;
  size_t index;

  ret =  hashmap_get_value_object_unsafe(map, key);
  if (ret)
    dlist_remove(GET_DLIST(ret));

  return ret;
}
