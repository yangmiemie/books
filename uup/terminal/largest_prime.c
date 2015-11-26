#include <stdio.h>
#include <signal.h>

long current;
long largest_prime;
long primes_found;

void status(int n);

int main(int argc, char *argv[])
{
  long test;
  long temp_prime;
  current = 2;

  signal(SIGINT, status);

  while(1)
  {
    for (test = 2; test < current; ++test)
    {
      if (current % test == 0)
      {
        break;
      }
      else
      {
        if (test == current - 1)
        {
          ++primes_found;
          largest_prime = current;
        }
      }
    }
    
    ++current;  
  }
  
  return 0;
}

void status(int n)
{
  printf("Status of prime number finder:\n");
  printf("Currently trying:     %8d\n", current);
  printf("Largest prime found:  %8d\n", largest_prime);
  printf("Primes found:         %8d\n", primes_found);
  printf("Average density:      %8f\n", (float)primes_found / current);
}
