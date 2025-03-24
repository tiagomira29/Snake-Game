#include <stdlib.h>
#include <stdio.h>

typedef struct s1{
    int a;
    int b;
    int c;
} s1;

typedef struct s2{
    int d;
    s1 a1;
} s2;

int main(){

    s1 x = {1,2,3};
    s2 y;
    y.a1.a = 4;
    y.a1.b = 5;
    y.a1.c = 6;

    printf("%d\n", ((s1 *) &y)->c);
    printf("%d\n", y.a1.a);

    return 0;
}