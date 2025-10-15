#include "General_typedefs/general_typedefs.h"
#ifndef CASIMPLEBRED
#define CASIMPLEBRED
typedef struct{
 //read & write versions
 size_t* i;
 stdbyte* stack;
 stdbyte* code;
 //fast read versions
 size_t ri;
 size_t rstackptr;
 size_t stackptrloc;
}cmm_args; 
char casimplebred(stdbyte* code, void (*cmm_funcs[])(cmm_args), uint32_t cmm_func_amount);
#endif
