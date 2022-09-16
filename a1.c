#include <stdio.h>
int my_func( int x, int y)
{
  if (x<y)
    if (2*x<y)
      return y-2*x;
    else 
      return y-x;
  else 
   return y+x;
}
void main(){
   int c, a,b;
  
   printf("input a: ");
   scanf( "%d", &a);
   printf("input b: ");
   scanf("%d", &b);
   c=my_func(a,b);
   printf(" output = %d",c);
   return;
} 
