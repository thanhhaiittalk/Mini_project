#include <stdio.h>
#include <unistd.h>  // For sleep function

int main() {
  while (1) {
    printf("sensor!\n");
    sleep(1);  // Sleep for 1 second
  }
  return 0;
}