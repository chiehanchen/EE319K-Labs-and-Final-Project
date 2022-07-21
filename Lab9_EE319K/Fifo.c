// FiFo.c
// Runs on LM4F120/TM4C123
// Provide functions that implement the Software FiFo Buffer
// Last Modified: 11/11/2021
// Student names: change this to your names or look very silly
#include <stdint.h>

// Declare state variables for FiFo
// size, buffer, put and get indexes

char arr[9];
char* getPtr;
char* putPtr;


// *********** FiFo_Init**********
// Initializes a software FIFO of a
// fixed size and sets up indexes for
// put and get operations
void Fifo_Init() {
//Complete this
putPtr=arr;
getPtr=arr;

}

// *********** FiFo_Put**********
// Adds an element to the FIFO
// Input: Character to be inserted
// Output: 1 for success and 0 for failure
//         failure is when the buffer is full
uint32_t Fifo_Put(char data){
  //Complete this routine
if((putPtr+1)==(getPtr)){
return 0;
}
*putPtr=data;
putPtr++;
if(putPtr==&arr[9]){
putPtr=arr;
}
  return(1);
}

// *********** Fifo_Get**********
// Gets an element from the FIFO
// Input: Pointer to a character that will get the character read from the buffer
// Output: 1 for success and 0 for failure
//         failure is when the buffer is empty
uint32_t Fifo_Get(char *datapt){
  //Complete this routine
if((putPtr)==(getPtr)){
return 0;
}
*datapt=*getPtr;
getPtr++;
if(getPtr==&arr[9]){
getPtr=arr;
}
  return(1);
}
