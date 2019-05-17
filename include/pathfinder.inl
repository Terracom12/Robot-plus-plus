#include <stdarg.h>
#include "containers.h"

#ifndef VEC_2
#define VEC_2
typedef struct
{
  double x;
  double y;
} vec2;
#endif // VEC_2

#ifndef VEC_2_ARRAY
#define VEC_2_ARRAY
typedef struct
{
  vec2 *data;
  size_t size;
} vec2_array;
#endif // VEC_2_ARRAY

bool Robot::drivePathTo(vec2_array points_arr, vec2 start_point, vec2 goal_point,
                        double points_x_distance, double points_y_distance)
{
    vec2 *points = points_arr.data;
    size_t num_points = points_arr.size;

    size_t start = find(points_arr, start_point);
    size_t goal = find(points_arr, goal_point);

    typedef struct { vec2 key; vec2 value; } point_to_point;

    point_to_point *came_from = (point_to_point *)malloc(num_points * sizeof(point_to_point));
    size_t num_came_from = 0;

    vec2_array neighbors(const vec2 &point)
    {
        // The result
        int *neigh = (int *)malloc(num_points * sizeof(int));
        int neigh_num = 0;
        // A `neighbor` is defined as being one unit away
        int i;
        for(i = 0; i < num_points; i++)
        {
            vec2 c = points[i];

            if(c.x == point.x && c.y == point.y)
            {
            // This point
                continue;
            }
            else if(c.x == (point.x+1) && c.y == point.y)
            {
                // Right
                neigh[neigh_num] = i;
                neigh_num++;
            }
            else if(c.x == (point.x-1) && c.y == point.y)
            {
                // Left
                neigh[neigh_num] = i;
                neigh_num++;
            }
            else if(c.x == point.x && c.y == (point.y+1))
            {
                // Above
                neigh[neigh_num] = i;
                neigh_num++;
            }
            else if(c.x == point.x && c.y == (point.y-1))
            {
                // Below
                neigh[neigh_num++] = i;
            }
        }

        vec2_array result = init_array(neigh_num);

        // Go through all the neighbors found
        for(i = 0; i < neigh_num; i++)
            result.data[i] = points[neigh[i]];

        free(neigh);

        return result;
      }
    vec2 find_point(vec2 point)
    {
        int i;
        for(i = 0; i < num_came_from; i++)
        {
            if(came_from[i].key.x == point.x && came_from[i].key.y == point.y)
            {
                return came_from[i].value;
            }
        }
        return point;
    }
    bool visited(vec2 point)
    {
        int i;
        for(i = 0; i < num_came_from; i++)
        {
            if(came_from[i].key.x == point.x && came_from[i].key.y == point.y)
                return true;
            if(came_from[i].value.x == point.x && came_from[i].value.y == point.y)
                return true;
        }

        return false;
    }
    void reverse(vec2_array array_arg)
    {
        int c, d, n;
        // n: number of elements
        n = array_arg.size;
        vec2_array temp = init_array(n);

        // Reverse on to temporary array
        for(c = n - 1, d = 0; c >= 0; c--, d++)
            temp.data[d] = array_arg.data[c];
        // Copy to old array and free the temporary one
        for (c = 0; c < n; c++)
            array_arg.data[c] = temp.data[c];

        free(temp.data);
    }

    if(start < 0 || goal > (num_points - 1))
        return false;

    // Points to visit on next iteration
    vec2_array frontier = init_array(num_points);
    frontier.data[0] = points[start];
    size_t num_frontier = 1;

    void shift(vec2_array &array_arg)
    {
        int i;
        for(i = 0; i < (array_arg.size-1); i++)
        {
            array_arg.data[i] = array_arg.data[i+1];
        }
        array_arg.size--;
    }


    /* Summary of this loop:
    *
    * Get the first point on the frontier, then check if we are at the exit. If so, then exit the
    * loop. Get all of the neighbors to the point `current`. Then loop through all of the
    * neighbors. If the neighbor has not been visited yet, then add it to the end of the frontier
    * and save where it came from.
    */
    while(frontier.size > 0)
    {
        vec2 current = frontier.data[0];
        shift(frontier);
        num_frontier--;
        // Early exit
        if(current.x == points[goal].x && current.y == points[goal].y)
            break;


        vec2_array current_neighbors = neighbors(current);

        int i;
        for(i = 0; i < current_neighbors.size; i++)
        {
            if(!visited(current_neighbors.data[i]))
            {
                frontier.data[num_frontier++] = current_neighbors.data[i];
                point_to_point temporary;
                temporary.key = current_neighbors.data[i];
                temporary.value = current;
                came_from[num_came_from++] = temporary;
            }
        }
        free(current_neighbors.data);
    }

    free(frontier.data);

    int j;
    if(find_point(points[goal]).x == points[goal].x && find_point(points[goal]).y == points[goal].y)
    return false;

    vec2 current = points[goal];
    vec2_array path = init_array(num_points);
    j = 0;
    // While the current point is not the start point
    while(current.x != points[start].x || current.y != points[start].y)
    {
        path.data[j++] = current;
        current = find_point(current);
    }
    path.size = j;

    free(came_from);

    reverse(path);

    vec2 position = points[start];
    static size_t direction; // 0 = 0 degrees, 1 = 90 degrees, 2 = 180 degrees, 3 = 270 degrees
    direction = 3; // Floor the robots rotation to an int
    while(path.size) // != 0
    {
        vec2 next = path.data[0];
        // Check if the point is in front of, behind, to the right, or to the left of the current
        // position, and move/turn accordingly.
        if(position.x == next.x && position.y == (next.y-1))
        {
            // `next` is above the current position
            switch(direction)
            {
            case 0:
                turn(-90);
                break;
            case 1:
                turn(-180);
                break;
            case 2:
                turn(90);
                break;
            // If it is forward nothing has to be done
            }
            direction = 3;

            drive(points_y_distance);
        }
        else if(position.x == next.x && position.y == (next.y+1))
        {
            // `next` is below the current position
            switch(direction)
            {
            case 0:
                turn(90);
                break;
            // If it is backward nothing has to be done
            case 2:
                turn(-90);
                break;
            case 3:
                turn(180);
                break;
            }
            direction = 1;

            drive(points_y_distance);
        }
        else if(position.x == (next.x-1) && position.y == next.y)
        {
            // `next` is to the right of the current position
            switch(direction)
            {
            // If it is facing to the right nothing has to be done
            case 1:
                turn(-90);
                break;
            case 2:
                turn(-180);
                break;
            case 3:
                turn(90);
                break;
            }
            direction = 0;

            drive(points_x_distance);
        }
        else if(position.x == (next.x+1) && position.y == next.y)
        {
            // `next` is to the left of the current position
            switch(direction)
            {
            case 0:
                turn(180);
                break;
            case 1:
                turn(90);
                break;
            // If it is facing to the left nothing has to be done
            case 3:
                turn(-90);
                break;
            }
            direction = 2;

            drive(points_x_distance);
        }
        else
        {
            puts("Error: pathfinding while moving robot failed!");
            return false;
        }
        position = next;
        shift(path);
    }
    // Return to 270 degrees every time it's done
    switch(direction)
    {
    case 0:
        turn(-90);
        break;
    case 1:
        turn(-180);
        break;
    case 2:
        turn(90);
        break;
    // If it is forward nothing has to be done
    }
    direction = 3;

    free(path.data);

    return true;
}

vec2_array create_grid(size_t width, size_t height, bool include_zero, ...)
{
    size_t size = (width * height);
    vec2_array arr = init_array(size);
    size_t num_arr = 0;

    // If 0 is included, then start at 0
    // if not, then start at 1
    size_t start = (include_zero ? 0 : 1);
    // Add 1 to the max of the loop if 0 is not included
    size_t end_add = (include_zero ? 0 : 1);

    vec2_array delete_points;
    size_t num_delete = 0;

    int num_delete_points;

    if(num_delete_points > 0)
    {
        delete_points = init_array(num_delete_points);

        va_list vl;
        va_start(vl, num_delete_points);

        num_delete_points = va_count(vl);

        size -= num_delete_points;


        // Add all of the va_args to the `delete_points` array
        /*while(num_delete_points > 0)
        {
            delete_points.data[num_delete++] = va_arg(vl, vec2);
            num_delete_points--;
        }*/

        va_end(vl);
    }

    bool should_exit = false;

    size_t x, y, i;
    for(x = start; x < (width+end_add); x++)
    {
        for(y = start; y < (height+end_add); y++)
        {
            /*for(i = 0; i < num_delete; i++)
            {
                if(x == delete_points.data[i].x && y == delete_points.data[i].y)
                {
                    should_exit = true;
                    break;
                }
            }*/
            if(should_exit)
            {
                should_exit = false;
                continue;
            }
            vec2 temp = { x, y };
            arr.data[num_arr++] = temp;
        }
    }

    free(delete_points.data);

    printf("Points: (%d)\n", arr.size);
    for(i = 0; i < arr.size; i++)
        printf("\tpoint = (%.0lf, %.0lf)\n", arr.data[i].x, arr.data[i].y);
    return arr;
}
