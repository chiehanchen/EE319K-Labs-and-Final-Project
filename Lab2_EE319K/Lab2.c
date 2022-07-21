// ****************** Lab2.c ***************
// Program written by: put your names here
// Date Created: 1/18/2017 
// Last Modified: 1/10/2022
#include "Lab2.h"
// Put your name and EID in the next two lines
char Name[] = "Chieh-An Chen";
char EID[] = "cc67994";
// Brief description of the Lab: 
// An embedded system is capturing data from a
// sensor and performing analysis on the captured data.
//   The three analysis subroutines are:
//    1. Calculate the average error
//    2. Perform a linear equation using integer math 
//    3. Check if the captured readings are a monotonic series
// Possibility 1)
//       Return 1 if the readings are in non-increasing order.
//       Examples:
//         10,9,7,7,5,2,-1,-5 is True=1
//         2,2,2,2 is True=1
//         9,7,7,5,6,-1,-5 is False=0 (notice the increase 5 then 6)
//         3,2,1,0,1 is False (notice the increase 0 then 1)
// Possibility 2)
//       Return 1 if the readings are in non-decreasing order.
//       Examples:
//         -5,-1,2,5,7,7,9,10 is True=1
//         2,2,2,2 is True=1
//         -1,6,5,7,7,9,10 is False=0 (notice the decrease 6 then 5)
//         1,2,3,4,3 is False=0 (notice the decrease 4 then 3)
#include <stdint.h>
#include <math.h>
#include <stdlib.h>
// Inputs: Data is an array of 32-bit signed measurements
//         N is the number of elements in the array
// Let x0 be the expected or true value
// Define error as the absolute value of the difference between the data and x0
// Output: Average error
// Notes: you do not need to implement rounding
// The value for your x0 will be displayed in the UART window
int32_t AverageError(const int32_t Readings[], const uint32_t N){
// Replace this following line with your solution
	int32_t x0 = 456;
	int32_t diff = 0;
	int32_t sum = 0;
	for (int i=0; i<N; i++) {
		if (x0 >= Readings[i]){
			diff = x0 - Readings[i];
		} else {
			diff = Readings[i] - x0;
		}
		sum += diff; 
	} 
	return sum/N;
}

// Consider a straight line between two points (x1,y1) and (x2,y2)
// Input: x an integer ranging from x1 to x2 
// Find the closed point (x,y) approximately on the line
// Output: y
// Notes: you do not need to implement rounding
// The values of (x1,y1) and (x2,y2) will be displayed in the UART window
int32_t Linear(int32_t const x){
// Replace this following line with your solution
	int32_t x1 = -100;
	int32_t y1 = -80000;
	int32_t x2 = 100;
	int32_t y2 = 80000;
	
	int32_t result = ((y2-y1)/(x2-x1)) * x;
  return (result);
}

// Return 1 or 0 based on whether the readings are a monotonic series
// Inputs: Readings is an array of 32-bit measurements
//         N is the number of elements in the array
// Output: 1 if monotonic, 0 if nonmonotonic
// Whether you need to implement non-increasing or non-decreasing will be displayed in the UART window
int IsMonotonic(int32_t const Readings[], uint32_t const N){
// Replace this following line with your solution
	uint8_t true = 1;
	uint8_t false = 0;
	
	if (N==false)
		return true;
	
	for (int32_t i = 0; i<N-1; i++) {
		if (Readings[i + 1] < Readings[i])
			return false;
	}
		
		return (true);
}
