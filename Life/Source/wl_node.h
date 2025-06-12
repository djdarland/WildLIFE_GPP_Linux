typedef unsigned long long *GENERIC;

class wl_node {
public:
  char *key;
  ptr_node left;
  ptr_node right;
  GENERIC data;
};
