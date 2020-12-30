#pragma once

#include "grid.h"
#include "randomizer.h"
#include <stack>

namespace RecursiveBacktracker
{

    static void use(Grid& grid, Randomizer& rand)
    {
        std::stack<Cell*> stack{};
        Cell* start = &grid.getRandomCell();
        stack.push(start);

        while(!stack.empty())
        {
            Cell* current = stack.top();
            const auto neighbours = current->getNeighbours();
            std::vector<Cell*> unvisitedNeighbours{};

            for(auto cell : neighbours)
            {
                if(cell->getLinks().empty())
                {
                    unvisitedNeighbours.push_back(cell);
                }
            }

            if(unvisitedNeighbours.empty())
            {
                stack.pop();
            }
            else
            {
                int randIndex = rand.nextInt(static_cast<int>(unvisitedNeighbours.size()) - 1);
                Cell* neighbour = unvisitedNeighbours[randIndex];
                current->link(neighbour);
                stack.push(neighbour);
            }

        }

    }
}