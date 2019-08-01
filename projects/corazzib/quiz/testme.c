#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

#define LENGTH 10

char inputChar(int max, int *range)
{
    int randInt = rand() % max;
    for(int i = 0; i < max; i++) {
      if(i == randInt) {return range[i];}
    }
    return 0;
}

char *inputString()
{
    int range[] = {114, 101, 115, 116, 66};
    char *s = malloc(5 + 1);
    for(int i = 0; i < 5; i++) {
      s[i] = inputChar(4, range);
    }
    return s;
}

void testme()
{
  int tcCount = 0;
  char *s;
  char c;
  int state = 0;
  int range[] = {91, 40, 123, 32, 97, 120, 125, 41, 93};
  while (1)
  {
    tcCount++;
    c = inputChar(9, range);
    s = inputString();
    // printf("Iteration %d: c = %c, s = %s, state = %d\n", tcCount, c, s, state);

    if (c == '[' && state == 0) state = 1;
    if (c == '(' && state == 1) state = 2;
    if (c == '{' && state == 2) state = 3;
    if (c == ' '&& state == 3) state = 4;
    if (c == 'a' && state == 4) state = 5;
    if (c == 'x' && state == 5) state = 6;
    if (c == '}' && state == 6) state = 7;
    if (c == ')' && state == 7) state = 8;
    if (c == ']' && state == 8) state = 9;
    if (s[0] == 'r' && s[1] == 'e'
       && s[2] == 's' && s[3] == 'e'
       && s[4] == 't' && s[5] == '\0'
       && state == 9)
    {
      return;
    }
  }
}


int main(int argc, char *argv[])
{
    srand(time(NULL));
    testme();
    return 0;
}
