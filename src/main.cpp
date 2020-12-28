#define OLC_PGE_APPLICATION

#include "extern/olcPixelGameEngine.h"

#include <iostream>
#include "binarytree.h"
#include "sidewinder.h"

class Maze : public olc::PixelGameEngine
{
    public:

    Maze()
    {
        sAppName = "Maze";
    }

    bool OnUserCreate() override;
    bool OnUserUpdate(float fElapsedTime) override;
    void generateMaze();

    Randomizer rand{};
    std::unique_ptr<Grid> grid = nullptr;
};


bool Maze::OnUserCreate()
{
    generateMaze();

    return true;
}


bool Maze::OnUserUpdate(float fElapsedTime)
{
    const int cellBorder = 5;
    const int cellSize = 32;
    const int halfSize = static_cast<int>(static_cast<float>(cellSize) / 2.0f);
    const int halfBorder = static_cast<int>(static_cast<float>(cellBorder) / 2.0f);
    const int offsetX = 164;
    const int offsetY = 48;

    if(GetMouse(0).bReleased)
    {
        generateMaze();
    }

    /*draw*/
    Clear(olc::BLACK);

    /*border*/

    FillRect({ offsetX, offsetY }, { grid->columns() * cellSize + 4 * halfBorder, cellBorder}, olc::RED);
    FillRect({ offsetX, offsetY }, { cellBorder, grid->rows() * cellSize + 4 * halfBorder}, olc::RED);



    int px = 0;
    int py = 0;

    for(const auto& row : grid->getEachRow())
    {
        for(const auto cell : row)
        {
            if(!cell->isLinked(cell->e))
            {
                FillRect({ px * cellSize + cellSize - halfBorder + cellBorder + offsetX, py * cellSize - halfBorder + cellBorder + offsetY },
                         {cellBorder, cellSize + cellBorder}, olc::RED);
            }
            if(!cell->isLinked(cell->s))
            {
                FillRect({ px * cellSize - halfBorder + cellBorder + offsetX, py * cellSize + cellSize - halfBorder + cellBorder + offsetY },
                         { cellSize + cellBorder, cellBorder }, olc::RED);
            }
            px++;
        }
        px = 0;
        py++;
    }


    return true;
}

void Maze::generateMaze()
{
    grid = std::make_unique<Grid>(16, 16, rand);
    //BinaryTree::use(grid, rand);
    SideWinder::use(*grid, rand);

    std::cout << *grid << std::endl;
}


int main()
{

    Maze maze{};

    if(maze.Construct(860, 640, 1, 1))
        maze.Start();

    return 0;

}