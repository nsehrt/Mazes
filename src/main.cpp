#include <iostream>

#include "binarytree.h"
#include "sidewinder.h"

int main()
{
    Randomizer rand{};
    int out = 10;

    while(out == 10)
    {
        Grid grid(16, 16, rand);
        //BinaryTree::use(grid, rand);
        SideWinder::use(grid, rand);

        std::cout << grid << std::endl;

        out = std::getchar();
    }



    return 0;
}