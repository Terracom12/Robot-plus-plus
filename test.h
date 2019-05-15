#include <linkbot.h>

typedef struct
{
    int x;
    int y;
} vec2;

void vec2_func(vec2 &vec)
{
    printf("Vec2: (%d, %d)\n", vec.x, vec.y);
}

int main()
{
    int *remove[12];
    remove = { 1, 2, 3, 4 };
    //vec2_func();

    return 0;
}