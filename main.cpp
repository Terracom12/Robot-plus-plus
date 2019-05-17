#include "include/robot"
#include "include/containers.h"

int main(int argc, char const *argv[])
{
  Robot *robot = new Robot();

  #include "competitions/comp9.h"

  delete robot;

  return 0;
}
