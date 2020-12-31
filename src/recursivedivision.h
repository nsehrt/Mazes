#pragma once

#include "grid.h"
#include "randomizer.h"

namespace RecursiveDivision
{

    static void use(Grid& grid, Randomizer& rand)
    {
        for(Cell& cell : grid.getCells())
        {
            for(Cell* n : cell.getNeighbours())
            {
                if(n != nullptr)
                cell.link(n, false);
            }
        }

        std::function<void(int, int, int, int)> divide;

        auto divideHorizontally = [&](int row, int column, int height, int width)
        {
            int divideSouthOf = rand.nextInt(height - 2);
            int passageAt = rand.nextInt(width - 1);

            for(int x = 0; x < width; x++)
            {
                if(passageAt == x) continue;

                Cell* cell = grid(row + divideSouthOf, column + x);
                if(cell != nullptr && cell->s != nullptr)
                cell->unlink(cell->s);
            }

            divide(row, column, divideSouthOf + 1, width);
            divide(row + divideSouthOf + 1, column, height - divideSouthOf - 1, width);
        };

        auto divideVertically = [&](int row, int column, int height, int width)
        {
            int divideEastOf = rand.nextInt(width - 2);
            int passageAt = rand.nextInt(height - 1);

            for(int x = 0; x < height; x++)
            {
                if(passageAt == x) continue;

                Cell* cell = grid(row + x, column + divideEastOf);
                if(cell != nullptr && cell->e != nullptr)
                cell->unlink(cell->e);
            }

            divide(row, column, height, divideEastOf + 1);
            divide(row, column + divideEastOf + 1, height, width - divideEastOf - 1);
        };

        divide = [&](int row, int column, int height, int width)
        {
            if(height <= 1 || width <= 1) return;

            if(height > width)
                divideHorizontally(row, column, height, width);
            else
                divideVertically(row, column, height, width);
        };


        divide(0, 0, grid.rows(), grid.columns());
    }


};