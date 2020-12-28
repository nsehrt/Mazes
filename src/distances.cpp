#include "distances.h"
#include "cell.h"

Distances Distances::path(Cell* goal)
{
    Cell* current = goal;

    Distances breadcrumbs{ root };
    breadcrumbs.set(current, distances[goal]);

    while(current != root)
    {
        for(Cell* neighbour : current->getLinks())
        {
            if(distances[neighbour] < distances[current])
            {
                breadcrumbs.set(neighbour, distances[neighbour]);
                current = neighbour;
            }
        }
    }

    return breadcrumbs;
}
