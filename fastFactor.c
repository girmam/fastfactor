
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>
#include<sys/wait.h>
#include <math.h>
#include <stdbool.h>


/*
 Script done on 2019-02-12 23:59:24-08:00
 girma@girma-VirtualBox:~/Desktop$ clear
 girma@girma-VirtualBox:~/Desktop$ gcc -o fastfactor fastfactor.c -lm
 girma@girma-VirtualBox:~/Desktop$ ./fastfactor 234121324120

 234121324120 :1 2 4 5 8 10 20 40 5853033103 11706066206 23412132412 29265165515 46824264824 58530331030 117060662060 234121324120 girma@girma-VirtualBox:~/Desktop$

 */
long numberOfFactors(long long number)
{
   long size = 0;
   for(long long factor = 1; factor <= sqrt(number); factor++ ) {
      if(number % factor == 0) {
         if(number/factor == factor) {
            size += 1;
         } else {
            size += 2;
         }
      }
   }
   return size;
}

long long * fastFactor(long long number, long *size)
{
   *size = numberOfFactors(number);
   long long *factors = malloc(*size * sizeof(long long));
   long left = 0;
   long right = *size - 1;
   for(long long factor = 1; factor <= sqrt(number); factor++ ) {
      if(number % factor == 0) {
         if(number/factor == factor) {
            factors[left++] = factor;
         } else {
            factors[left++] = factor;
            factors[right--] = number / factor;
         }
      }
   }
   return factors;
}



int main(int argc, char **argv) {
   int p[2];
   if(pipe(p) < 0) {
      exit(1);
   }

   long long colon = -1;
   long long newNumber = -2;

   for (int i = 1; i < argc; ++i) {
      long long number = atoll(argv[i]);
      if(number <= 0) {
         printf("\nInvalid input\n");
         exit(2);
      }

      int result = fork();
      if(result < 0) {
         exit(1);
      } else if(result == 0) {
         long size = 0;
         long long *factors = fastFactor(number, &size);
         write(p[1], &newNumber, sizeof(long long));
         write(p[1], &number, sizeof(long long));
         write(p[1], &colon, sizeof(long long));
         write(p[1], factors, size * sizeof(long long));

         exit(0);
      }
   }
   close(p[1]);
   long long number;
   while (read(p[0], &number, sizeof(long long)) > 0) {
      if(newNumber == number) {
         printf("\n");
      } else if (number == colon) {
         printf(":");
      } else {
         printf("%lli ", number);
      }
   }
   return 0;
}
