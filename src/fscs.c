#include "fscs.h"
#include <stdlib.h>

// auxiliary function to see if a node has already been visited by another citizen
int is_already_visited(Coordinate coordinate, Path *paths, int num_people)
{
    for (int i = 0; i < num_people; i++)
    {
        for (int j = 0; j < paths[i].length; j++)
        {
            if (coordinate.avenue == paths[i].path[j].avenue && coordinate.street == paths[i].path[j].street)
            {
                return 1;
            }
        }
    }
    return 0;
}

int find_safe_citizen(const Map *map, Path *paths, int num_people) {
    // for each person, calculate a safe path to a supermarket
    for (int person = 0; person < num_people; person ++) {
        int num_solutions = 0;

        Coordinate curr_pos = map->people[person];
        paths[person].length = 0;

        // while the person is not at a supermarket, search for the nearest safe node
        while (1)
        {
            // checks if the current position is a supermarket
            for (int i = 0; i < num_people; i++)
            {
                if (curr_pos.avenue == map->supermarket[i].avenue && curr_pos.street == map->supermarket[i].street)
                {
                    paths[person].path[paths[person].length++] = curr_pos;
                    num_solutions++;
                    break;
                }
            }

            // logic to find the next node
            Coordinate next_pos = {curr_pos.avenue, curr_pos.street + 1};

            // verify if node has been visited
            if (is_already_visited(next_pos, paths, num_people)){
                break;
            }
            
            // add node to path and go
            paths[person].path[paths[person].length++] = next_pos;
            curr_pos = next_pos;
        }

        return num_solutions;
        
    }
}