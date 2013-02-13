#include <stdio.h>
#include "bst.h"
#include "assert.h"
#include "structs.h"

/*----------------------------------------------------------------------------
 very similar to the compareTo method in java or the strcmp function in c. it
 returns an integer to tell you if the left value is greater then, less then, or
 equal to the right value. you are comparing the number variable, letter is not
 used in the comparison.

 if left < right return -1
 if left > right return 1
 if left = right return 0
 */

 /*Define this function, type casting the value of void * to the desired type.
  The current definition of TYPE in bst.h is void*, which means that left and
  right are void pointers. To compare left and right, you should first cast
  left and right to the corresponding pointer type (struct data *), and then
  compare the values pointed by the casted pointers.

  DO NOT compare the addresses pointed by left and right, i.e. "if (left < right)",
  which is really wrong.
 */
int compare(TYPE left, TYPE right)
{
    if((((struct data*)left)->number) == ((struct data*)right)->number) return 0;
    if((((struct data*)left)->number) < ((struct data*)right)->number) return -1;
    if((((struct data*)left)->number) > ((struct data*)right)->number) return 1;
}

/*Define this function, type casting the value of void * to the desired type*/
void print_type(TYPE curval)
{
    printf("%d", ((struct data*)curval)->number);

}


