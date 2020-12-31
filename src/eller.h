#pragma once

#define NOMINMAX
#include "grid.h"
#include "randomizer.h"
#include <algorithm>

namespace Eller
{
    class RowState
    {
        public:

        explicit RowState(int startingSet = 0) : nextSet(startingSet)
        {
        }

        void record(int set, Cell* cell)
        {
            setForCell[cell->getPosition().first] = set;
            if(cellsInSet.find(set) == cellsInSet.end())
            {
                cellsInSet[set] = {};
            }
            else
            {
                cellsInSet[set].push_back(cell);
            }
        }

        auto setFor(Cell* cell)
        {
            if(setForCell.find(cell->getPosition().first) == setForCell.end())
            {
                record(nextSet, cell);
                nextSet++;
            }

            return setForCell[cell->getPosition().first];
        }

        void merge(int winner, int loser)
        {
            for(const auto& cell : cellsInSet[loser])
            {
                setForCell[cell->getPosition().first] = winner;
                cellsInSet[winner].push_back(cell);
            }

            cellsInSet.erase(cellsInSet.find(loser));
        }

        RowState next()
        {
            return RowState(nextSet);
        }

        auto& eachSet()
        {
            return cellsInSet;
        }

        private:

        int nextSet;
        std::unordered_map<int, std::vector<Cell*>> cellsInSet{};
        std::unordered_map<int, int> setForCell{};
    };

    static void use(Grid& grid, Randomizer& rand)
    {
        auto rng = std::default_random_engine{};
        RowState rowState{};

        for(auto& row : grid.getEachRow())
        {
            for(Cell* cell : row)
            {
                if(cell->w == nullptr) continue;

                int set = rowState.setFor(cell);
                int priorSet = rowState.setFor(cell->w);

                bool shouldLink = (set != priorSet) && (cell->s == nullptr || rand.nextInt(1) == 0);

                if(shouldLink)
                {
                    cell->link(cell->w);
                    rowState.merge(priorSet, set);
                }
            }

            if(row[0]->s != nullptr)
            {
                RowState nextRow = rowState.next();

                for(auto& [set, list] : rowState.eachSet())
                {
                    std::map<int, Cell*> shuffle{};
                    std::vector<int> indices{};

                    for(int i = 0; i < list.size(); i++)
                    {
                        shuffle[i] = list[i];
                        indices.push_back(i);
                    }

                    std::shuffle(indices.begin(), indices.end(), rng);
                    for(int index : indices)
                    {
                        if(index == 0 || rand.nextInt(2) == 0)
                        {
                            shuffle[index]->link(shuffle[index]->s);
                            nextRow.record(rowState.setFor(shuffle[index]), shuffle[index]->s);
                        }
                    }
                }


                rowState = nextRow;
            }
        }

    }
}