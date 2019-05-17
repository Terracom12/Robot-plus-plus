#ifndef CONTAINERS_H
#define CONTAINERS_H

#ifndef _SIZE_T_DEFINED
#define _SIZE_T_DEFINED
typedef unsigned long long size_t;
#endif _SIZE_T_DEFINED

/********************TEMPORARY********************/
  #ifndef VEC_2
  #define VEC_2
  typedef struct 
  { 
    double x;
    double y;
  } vec2;
  #endif

  #ifndef VEC_2_ARRAY
  #define VEC_2_ARRAY
  typedef struct
    {
      vec2 *data;
      size_t size;
    } vec2_array;
  #endif // VEC_2_ARRAY

  size_t find(vec2_array array_arg, vec2 what)
  {
      // If found, the index is returned
      // If not found, 1 more than the size is returned
      size_t i;
      for(i = 0; i < array_arg.size; i++)
      {
          if(array_arg.data[i].x == what.x && array_arg.data[i].y == what.y)
              return i;
      }
      return (array_arg.size+1);
  }
  bool in_range(int num, int min, int max)
  {
      return (num >= min && num <= max);
  }
/********************TEMPORARY********************/

/********************ARRAY********************
#ifndef ARRAY_
#define ARRAY_

// TODO: decide whether to call it `vector` or not

#define Array(...) _Generic()

class Array_i // Array of ints
{

}
#endif // ARRAY_
********************ARRAY********************/

#endif // CONTAINERS_H