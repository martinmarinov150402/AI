#include <iostream>
#include <cmath>
#include <vector>
#include <climits>
#include <set>
#include <chrono>
#include <algorithm>

using namespace std::chrono;

const int FOUND = -58855;

void printMatrix(int** matrix, int dim)
{
    for(int i = 0; i < dim; i++)
    {
        for(int j = 0; j < dim; j++)
        {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

int heuristic(int size, int position, int** matrix)
{
    if(position < 0)
    {
        position = size + 1 + position;
    }
    int result = 0;
    int dim = sqrt(size+1);
    for(int i = 0; i < dim; i++)
    {
        for(int j = 0; j < dim; j++)
        {
            if(matrix[i][j] == 0)
            {
                continue;
            }
            int xCorrect = matrix[i][j] <= position ? (matrix[i][j] - 1) / dim : matrix[i][j] / dim;
            int yCorrect = matrix[i][j] <= position ? (matrix[i][j] - 1) % dim : matrix[i][j] % dim;
            result += abs(i - xCorrect) + abs(j - yCorrect);
        }
    }
    return result;
}
bool isGoal(int size, int position, int** matrix)
{
    return heuristic(size, position, matrix) == 0;
}
int** makeCopy(int** matrix, int dim)
{
    int** result = new int*[dim];   
    for(int i = 0; i < dim; i++)
    {
        result[i] = new int[dim];
    }

    for(int i = 0; i < dim; i++)
    {
        for(int j = 0; j < dim; j++)
        {
            result[i][j] = matrix[i][j];
        }
    }

    return result;
}
std::vector<int** > successors(int size, int position, int** matrix)
{
    std::vector<int**> successorsSet;
    int dim = sqrt(size + 1);
    if(position < 0)
    {
        position = size + 1 + position;
    }
    else
    {
        position;
    }
    int posX = position / dim;
    int posY = position % dim;
    int xOfEmpty, yOfEmpty;
    for(int i = 0; i < dim; i++)
    {
        for(int j = 0; j < dim; j++)
        {
            if(matrix[i][j] == 0)
            {
                xOfEmpty = i;
                yOfEmpty = j;
            }
        }
    }
    if(xOfEmpty > 0)
    {
        int **tmp = makeCopy(matrix, dim);
        std::swap(tmp[xOfEmpty][yOfEmpty], tmp[xOfEmpty-1][yOfEmpty]);
        successorsSet.push_back(tmp);
    }
    if(xOfEmpty < dim-1)
    {
        int **tmp = makeCopy(matrix, dim);
        std::swap(tmp[xOfEmpty][yOfEmpty], tmp[xOfEmpty+1][yOfEmpty]);
        successorsSet.push_back(tmp);
    }
    if(yOfEmpty > 0)
    {
        int **tmp = makeCopy(matrix, dim);
        std::swap(tmp[xOfEmpty][yOfEmpty], tmp[xOfEmpty][yOfEmpty - 1]);
        successorsSet.push_back(tmp);
    }
    if(yOfEmpty < dim - 1)
    {
        int **tmp = makeCopy(matrix, dim);
        std::swap(tmp[xOfEmpty][yOfEmpty], tmp[xOfEmpty][yOfEmpty+1]);
        successorsSet.push_back(tmp);
    }
    return successorsSet;
}
bool areEqual(int **a, int **b, int dim)
{
    bool answer = true;
    for(int i = 0; (i < dim) && answer; i++)
    {
        for(int j = 0; (j < dim) && answer; j++)
        {
            if(a[i][j] != b[i][j])
            {
                answer = false;
            }
        }
    }   
    return answer;
}
int search(std::vector<int**>& path, int g, int bound, int size, int position, int** node)
{
    int dim = sqrt(size+1);
    int f = g + heuristic(size, position, node);
    if(f > bound) return f;
    if(isGoal(size, position, node))
    {
        path.push_back(node);
        return FOUND;        
    }
    int minimum = INT_MAX;
    auto successorsSet = successors(size, position, node);
    for(int** suc : successorsSet)
    {
        bool isInPath = false;
        for(int **element : path)
        {
            if(areEqual(element, suc, dim))
            {
                isInPath = true;
            }
        }
        if(!isInPath)
        {
            int t = search(path, g + 1, bound, size, position, suc);
            if(t == FOUND)
            {
                path.push_back(node);
                return FOUND;
            } 
            if(t < minimum) minimum = t;
        }
    }
    return minimum;


}

std::vector<int**> ida_star(int** &matrix, int size, int position)
{
    int bound = heuristic(size, position, matrix);
    std::vector<int**> path;
    int t = 0;

    while(t != FOUND && t != INT_MAX)
    {
        path.clear();
        t = search(path, 0, bound, size, position, matrix);
        
        if(t == FOUND) return path;
        bound = t;
    }
    return path;
}
void init(int &size, int &position, int** &matrix)
{
    std::cin >> size;
    std::cin >> position;

    int dim = sqrt(size+1);
    matrix = new int*[dim];
    for(int i = 0; i < dim; i++)
    {
        matrix[i] = new int[dim];
    }

    for(int i = 0; i < dim; i++)
    {
        for(int j = 0; j < dim; j++)
        {
            std::cin >> matrix[i][j];
        }
    }
}

std::pair<int,int> findZero(int** matrix, int dim)
{
    for(int i = 0; i < dim; i++)
    {
        for(int j = 0; j < dim; j++)
        {
            if(matrix[i][j] == 0)
            {
                return std::make_pair(i,j);
            }
        }
    }
    return std::make_pair(-1,-1);
}
int main(int argc, const char** argv)
{
    int size, position;
    int **matrix;
    init(size, position, matrix);
    auto start = high_resolution_clock::now();
    int dim = sqrt(size + 1);
    std::vector<int**> path = ida_star(matrix,size,position);
    std::cout << path.size() - 1 << std::endl;
    reverse(path.begin(), path.end());
    for(int i = 1; i < path.size(); i++)
    {
        std::pair<int,int> zero1 = findZero(path[i-1],dim);
        std::pair<int,int> zero2 = findZero(path[i], dim);
        if(zero1.second < zero2.second)
        {
            std::cout << "left" << std::endl;
        }
        else if(zero1.second > zero2.second)
        {
            std::cout << "right" << std::endl;
        }
        else if(zero1.first < zero2.first)
        {
            std::cout << "up" << std::endl;
        }
        else
        {
            std::cout << "down" << std::endl;
        }
    }
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    double out = duration.count();
    out /= 1000000;
    if(argc > 1)
    {
        std::cout << out << " seconds!" << std::endl;
    }
}