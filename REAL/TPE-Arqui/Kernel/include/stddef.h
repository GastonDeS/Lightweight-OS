
#define NULL (void *)0
#define my_sizeof(type) (long)(char*)(&type+1)-(char*)(&type)
//#define my_sizeof(type) (char )(&type+1)-(char)(&type)