#pragma once

#include "grid.h"
#include "randomizer.h"

namespace Wilson
{

    static void use(Grid& grid, Randomizer& rand)
    {

        std::vector<Cell*> unvisited{};

        for(Cell& cell : grid.getCells())
        {
            unvisited.push_back(&cell);
        }

        Cell* first = &grid.getRandomCell();
        unvisited.erase(std::remove(unvisited.begin(), unvisited.end(), first), unvisited.end());

        while(!unvisited.empty())
        {
            int randomIndex = rand.nextInt(static_cast<int>(unvisited.size()) - 1);
            Cell* cell = unvisited[randomIndex];
            std::vector<Cell*> path{ cell };

            while(std::find(unvisited.begin(), unvisited.end(), cell) != unvisited.end())
            {
                const auto neighbours = cell->getNeighbours();
                int randomIndex = rand.nextInt(static_cast<int>(neighbours.size()) - 1);
                cell = neighbours[randomIndex];
                auto position = std::find(path.begin(), path.end(), cell);

                if(position != path.end())
                {
                    path = { path.begin(), position };
                }
                else
                {
                    path.push_back(cell);
                }
                
            }

            for(int i = 0; i < path.size() - 2; i++)
            {
                int next = i + 1;
                path[i]->link(path[next]);
                unvisited.erase(std::remove(unvisited.begin(), unvisited.end(), path[i]), unvisited.end());
            }

        }
    }

};