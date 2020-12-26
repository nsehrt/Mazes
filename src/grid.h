#pragma once
#include "cell.h"

class Grid
{
    public:

    explicit Grid(int _width, int _height) : width(_width), height(_height)
    {
        prepareGrid();
        configureCells();
    }

    virtual void prepareGrid()
    {
        for(int i = 0; i < height; i++)
        {
            for(int j = 0; j < width; j++)
            {
                cells.emplace_back(j, i);
            }
        }
    }

    virtual void configureCells()
    {
        for(auto& cell : cells)
        {
            const auto[row, column] = cell.getPosition();

            cell.n = (*this)(row - 1, column);
            cell.s = (*this)(row + 1, column);
            cell.w = (*this)(row, column - 1);
            cell.e = (*this)(row, column + 1);
        }
    }

    virtual Cell* operator()(int x, int y)
    {
        if(x < 0 || x > width - 1 || y < 0 || y > height - 1)
            return nullptr;

        int index = y * width + x;
        return &cells[index];
    }

    int getColumns() const
    {
        return width;
    }

    int getRows() const
    {
        return height;
    }

    int size() const
    {
        return width * height;
    }

    std::vector<std::vector<Cell*>> getEachRow()
    {
        std::vector<std::vector<Cell*>> result{};

        for(int i = 0; i < getRows(); i++)
        {
            std::vector<Cell*> row{};

            for(int j = 0; j < getColumns(); j++)
            {
                row.push_back(&cells[i * getColumns() + j]);
            }
            result.push_back(std::move(row));
        }

        return result;
    }

    std::vector<Cell>& getCells()
    {
        return cells;
    }

    friend std::ostream& operator<<(std::ostream& os, Grid& grid)
    {
        const std::string body = "   ";
        const std::string corner = "+";

        std::cout << "+" << Grid::repeat("---+", grid.getColumns()) << "\n";

        for(const auto& row : grid.getEachRow())
        {
            std::string top = "|";
            std::string bottom = "+";

            for(const auto cell : row)
            {
                const std::string east_boundary = cell->isLinked(cell->e) ? " " : "|";
                const std::string south_boundary = cell->isLinked(cell->s) ? "   " : "---";

                top += "   " + east_boundary;
                bottom += south_boundary + "+";
            }

            std::cout << top << "\n" << bottom << "\n";
        }


        return os;
    }

    static std::string repeat(const std::string& input, int n)
    {
        std::string ret;
        ret.reserve(input.size() * n);
        while(n--)
            ret += input;
        return ret;
    }

    private:

    int width, height;
    std::vector<Cell> cells;
};