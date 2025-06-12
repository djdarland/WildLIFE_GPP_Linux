typedef unsigned long long *GENERIC;

struct wl_node {
  char *key;
  ptr_node left;
  ptr_node right;
  GENERIC data;
};
