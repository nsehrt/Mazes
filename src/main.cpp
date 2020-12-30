#define OLC_PGE_APPLICATION

#include "extern/olcPixelGameEngine.h"

#include <iostream>
#include "binarytree.h"
#include "sidewinder.h"
#include "aldousbroder.h"
#include "wilson.h"
#include "huntkill.h"
#include "recursivebacktracker.h"

enum class MazeAlgorithm
{
    BinaryTree,
    SideWinder,
    AldousBroder,
    Wilson,
    HuntAndKill,
    RecursiveBacktracker,
    COUNT
};

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
    void solveMaze();

    Randomizer rand{};
    std::unique_ptr<Grid> grid = nullptr;
    MazeAlgorithm algorithm = MazeAlgorithm::BinaryTree;
    std::pair<int, int> startPos{ 0,0 };
    std::pair<int, int> goalPos{ 0,15 };
    bool drawDistances = false;
    bool showOnlyPath = false;
    bool drawPolarGrid = false;
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

    //input

    if(GetKey(olc::Key::ESCAPE).bPressed)
    {
        return false;
    }

    if(GetKey(olc::Key::SPACE).bPressed)
    {
        generateMaze();
    }

    if(GetKey(olc::Key::W).bPressed)
    {
        algorithm = static_cast<MazeAlgorithm>((static_cast<int>(algorithm) + 1) % static_cast<int>(MazeAlgorithm::COUNT));
        generateMaze();
    }
    else if(GetKey(olc::Key::S).bPressed)
    {
        int t = static_cast<int>(algorithm) - 1;

        if(t < 0)
        {
            algorithm = static_cast<MazeAlgorithm>(static_cast<int>(MazeAlgorithm::COUNT) - 1);
        }
        else
        {
            algorithm = static_cast<MazeAlgorithm>(t);
        }

        generateMaze();
    }

    //set start, goal to maximal length
    if(GetKey(olc::Key::E).bPressed)
    {
        Cell* startCell = (*grid)(0, 0);
        Distances d = startCell->distances();
        auto [newStart, maxDistance] = d.maxPath();
        Distances newD = newStart->distances();
        auto [goalCell, maxDistanceGoal] = newD.maxPath();

        startPos = newStart->getPosition();
        goalPos = goalCell->getPosition();
        grid->setDistances(newD.path(goalCell));
        solveMaze();
    }

    if(GetKey(olc::Key::Q).bPressed)
    {
        drawDistances = !drawDistances;
    }

    if(GetKey(olc::Key::D).bPressed)
    {
        drawPolarGrid = !drawPolarGrid;
    }

    if(GetKey(olc::Key::A).bPressed)
    {
        showOnlyPath = !showOnlyPath;
        solveMaze();
    }

    const int selectedX = (GetMouseX() - offsetX) / cellSize;
    const int selectedY = (GetMouseY() - offsetY) / cellSize;

    if(GetMouse(0).bReleased)
    {
        if(selectedX >= 0 && selectedX < grid->columns() && selectedY >= 0 && selectedY < grid->rows())
        {
            if(!GetKey(olc::SHIFT).bHeld)
            {
                if(startPos != std::make_pair(selectedX, selectedY))
                {
                    goalPos = std::make_pair(selectedX, selectedY);
                    solveMaze();
                }

            }
            else
            {
                if(goalPos != std::make_pair(selectedX, selectedY))
                {
                    startPos = std::make_pair(selectedX, selectedY);
                    solveMaze();
                }
            }
        }

    }

    /*draw*/
    Clear(olc::BLACK);


    //draw normal grid
    if(!drawPolarGrid)
    {
        /*border*/

        FillRect({ offsetX, offsetY }, { grid->columns() * cellSize + 4 * halfBorder, cellBorder }, olc::RED);
        FillRect({ offsetX, offsetY }, { cellBorder, grid->rows() * cellSize + 4 * halfBorder }, olc::RED);

        /*background*/
        int px = 0;
        int py = 0;

        Distances& cDistances = grid->getDistances();
        const int maxDistValue = cDistances.maxValue();

        for(const auto& row : grid->getEachRow())
        {
            for(const auto cell : row)
            {
                float intensity = static_cast<float>(maxDistValue - cDistances.get(cell)) / static_cast<float>(maxDistValue);
                int dark = static_cast<int>(255.0f * (std::clamp<float>(intensity - 0.15f, 0.0f, 1.0f)));
                int bright = 128 + static_cast<int>(127 * (std::clamp<float>(intensity - 0.15f, 0.0f, 1.0f)));

                FillRect({ px * cellSize + cellBorder + offsetX, py * cellSize + cellBorder + offsetY }, { cellSize, cellSize },
                     {
                        static_cast<std::uint8_t>(dark),
                        static_cast<std::uint8_t>(bright),
                        static_cast<std::uint8_t>(dark)
                     });
                px++;
            }
            px = 0;
            py++;
        }

        /*start/goal*/

        FillRect({ startPos.first * cellSize + cellBorder + offsetX, startPos.second * cellSize + cellBorder + offsetY },
                 { cellSize, cellSize }, olc::GREEN);

        FillRect({ goalPos.first * cellSize + cellBorder + offsetX, goalPos.second * cellSize + cellBorder + offsetY },
                 { cellSize, cellSize }, olc::DARK_RED);

        /*maze*/

        px = 0;
        py = 0;

        for(const auto& row : grid->getEachRow())
        {
            for(const auto cell : row)
            {
                if(!cell->isLinked(cell->e))
                {
                    FillRect({ px * cellSize + cellSize - halfBorder + cellBorder + offsetX, py * cellSize - halfBorder + cellBorder + offsetY },
                             { cellBorder, cellSize + cellBorder }, olc::RED);
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

        //solver output
        if(drawDistances)
        {
            auto& dist = grid->getDistances();

            for(int i = 0; i < grid->rows(); i++)
            {
                for(int j = 0; j < grid->columns(); j++)
                {
                    std::string t = std::to_string(dist.get((*grid)(j, i)));
                    DrawString({ j * cellSize + halfSize + offsetX, i * cellSize + halfSize + offsetY }, t, olc::WHITE, 1);
                }
            }
        }

    }
    //draw polar grid
    else
    {

    }


    // text output
    switch(algorithm)
    {
        case MazeAlgorithm::BinaryTree: DrawString({ 10,10 }, "BinaryTree", olc::WHITE, 2); break;
        case MazeAlgorithm::SideWinder: DrawString({ 10,10 }, "SideWinder", olc::WHITE, 2); break;
        case MazeAlgorithm::AldousBroder: DrawString({ 10,10 }, "Aldous-Broder", olc::WHITE, 2); break;
        case MazeAlgorithm::Wilson: DrawString({ 10,10 }, "Wilson", olc::WHITE, 2); break;
        case MazeAlgorithm::HuntAndKill: DrawString({ 10,10 }, "Hunt&Kill", olc::WHITE, 2); break;
        case MazeAlgorithm::RecursiveBacktracker: DrawString({ 10,10 }, "RecursiveBacktracker", olc::WHITE, 2); break;
    }
 
    int deadends = static_cast<int>(grid->deadends().size());
    int dPercent = static_cast<int>(deadends / static_cast<float>(grid->size()) * 100.0f);
    std::string deadEndText = "Deadends: " + std::to_string(deadends) + " (" + std::to_string(dPercent) + "%)";

    DrawString({ 10,30 }, deadEndText, olc::WHITE, 2);

    return true;
}

void Maze::generateMaze()
{
    grid = std::make_unique<Grid>(16, 16, rand);

    switch(algorithm)
    {
        case MazeAlgorithm::BinaryTree: BinaryTree::use(*grid, rand); break;
        case MazeAlgorithm::SideWinder: SideWinder::use(*grid, rand); break;
        case MazeAlgorithm::AldousBroder: AldousBroder::use(*grid, rand); break;
        case MazeAlgorithm::Wilson: AldousBroder::use(*grid, rand); break; //!!!
        case MazeAlgorithm::HuntAndKill: HuntKill::use(*grid, rand); break;
        case MazeAlgorithm::RecursiveBacktracker: RecursiveBacktracker::use(*grid, rand); break;
    }

    solveMaze();
    std::cout << *grid << std::endl;
}

void Maze::solveMaze()
{

    //solve maze
    Cell* start = (*grid)(startPos.first, startPos.second);
    Cell* goal = (*grid)(goalPos.first, goalPos.second);
    Distances distances = start->distances();
    if(showOnlyPath)
    {
        Distances path = distances.path(goal);
        grid->setDistances(std::move(path));
    }
    else
    {
        grid->setDistances(std::move(distances));
    }

}

int main()
{

    Maze maze{};

    if(maze.Construct(860, 640, 1, 1))
        maze.Start();

    return 0;

}