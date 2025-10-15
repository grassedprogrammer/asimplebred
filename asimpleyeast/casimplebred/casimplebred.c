#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "casimplebred.h"
#define ERRSTR "\033[31mfatal err at %zu, faulty bytecode: 0x%x\n" \
    "in ASCII: %c\n"
char casimplebred(stdbyte* code, void (*cmm_funcs[])(cmm_args), uint32_t cmm_func_amount){
 size_t i = 0;
 stdbyte stack[USHRT_MAX+1] = {0};
 ushrt stackptr = 0;
 start:
  switch(code[i]){
   case 0x00:
    //halt with code(hwc)
    return code[++i];
   case 0x01:
    //push literal(psh)
    if(!stackptr){
     i++;
     stack[stackptr] = code[i];
     break;
    }
    stackptr++;
    i++;
    stack[stackptr] = code[i];
    break;
   case 0x02:
    //call mommy(cmm)
    /*trying to improve this api
     *is from JNI level garbage
     *to good enough is not hard
     *but not easy
    */
    i+=3;
    uint32_t tmp;
    tmp = code[i];
    cmm_args tmp2 = {&i, stack, code, i, stackptr, &stackptr};
    //why the (swear smth smth) can't
    //I use the 0 in 0x???
    tmp+=((uint32_t)code[i-1])*0x100;
    tmp+=((uint32_t)code[i-2])*0x10000;
    if(tmp >= cmm_func_amount ) return (char)-3;
    cmm_funcs[tmp](tmp2);
    break;
   case 0x03:{
    //swap stack vars(ssv)
    tmp = (uint32_t)stack[stackptr];
    stack[stackptr] = stack[stackptr+1];
    stack[stackptr+1] = (stdbyte)tmp;}
    break;
   case 0x04:
    //jmp
    i++;
    i += code[i];
    //printf("jumped to %zu\nvalue: %d\n", i, code[i]);
    goto start;
   case 0x05:{
    //write char literal(wcl)
    putchar((char)code[++i]);
    break;}
//   case 0x06:{
    //input char
   default:
    printf(ERRSTR, i, code[i], code[i]);
    return (char)-2;}
  i++;
  goto start;
 return (char)-1;
}
void basicstrprint(cmm_args a){
 size_t i = (a.ri)-5;
 size_t temp = a.ri-(a.code[(a.ri)-4]);
 for(; i >= temp; --i){
  printf("%c", (char)a.code[i]);
 };
 return;
}
void printstacktop(cmm_args a){
 putchar((char)(a.stack[a.rstackptr]));
 ushrt *b = a.stackptrloc;
 b--;
 }
int main(void){
 stdbyte a[] = {0x01, 'l', 0x01, 'o', 0x01, 'l', 0x02, 0, 0, 1, 2, 0, 0, 1, 2, 0, 0, 1, 0x00, 0x00};
 void (*cmm_funcs[])(cmm_args) = {basicstrprint, printstacktop};
 printf("code: %d\n", casimplebred(a, cmm_funcs, 2));
 return 0;}
