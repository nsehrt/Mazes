#pragma once

#include "distances.h"
#include <vector>

class Cell
{
    public:

    explicit Cell(int x, int y) : xPos(x), yPos(y),
        n(nullptr), e(nullptr), s(nullptr), w(nullptr)
    {
    }

    void link(Cell* cell, bool bidirectional = true)
    {
        if(std::find(links.begin(), links.end(), cell) == links.end())
        {
            links.push_back(cell);
        }

        if(bidirectional)
        {
            cell->link(this, false);
        }
    }

    void unlink(Cell* cell, bool bidirectional = true)
    {
        const auto position = std::find(links.begin(), links.end(), cell);

        if(position != links.end())
        {
            links.erase(position);
        }

        if(bidirectional)
        {
            cell->unlink(this, false);
        }
    }

    const auto& getLinks() const
    {
        return links;
    }

    bool isLinked(const Cell* cell) const
    {
       return std::find(links.begin(), links.end(), cell) != links.end();
    }

    std::pair<int, int> getPosition() const
    {
        return { xPos, yPos };
    }

    std::vector<Cell*> getNeighbours() const
    {
        std::vector<Cell*> neighbours{};

        if(n != nullptr) neighbours.push_back(n);
        if(e != nullptr) neighbours.push_back(e);
        if(s != nullptr) neighbours.push_back(s);
        if(w != nullptr) neighbours.push_back(w);

        return neighbours;
    }

    Distances distances()
    {
        Distances dist(this);
        std::vector<Cell*> frontiers{ this };

        while(!frontiers.empty())
        {
            std::vector<Cell*> newFrontiers{};

            for(Cell* cell : frontiers)
            {
                for(Cell* link : cell->links)
                {
                    if(dist.exist(link))
                        continue;

                    dist.set(link, dist.get(cell) + 1);
                    newFrontiers.push_back(link);
                }
            }

            frontiers = newFrontiers;
        }

        return dist;
    }

    public:

    Cell* n, * e, * s, * w;

    private:

    int xPos, yPos;
    std::vector<Cell*> links{};
};