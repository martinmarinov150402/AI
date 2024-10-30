#include <iostream>

void init(int* placesInCol, int n)
{
    placesInCol[0] = rand() % n;
    for(int i = 1; i < n; i++)
    {
        int choices[] = {placesInCol[i-1] + 2, placesInCol[i-1] - 2};
        int choice = rand() % 2;
        placesInCol[i] = (choices[choice] < 0 || choices[choice] >= n  ? choices[!choice] : choices[choice]);
    }
}
void printMatrix(int* placesInCol, int n)
{
    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < n; j++)
        {
            std::cout << ((placesInCol[j] == i) ? "*" : "-");
           
        }
        std::cout << std::endl;
    }
}
int main()
{
    int n;
    std::cin >> n;
    srand(time(nullptr));
    bool *row, *col, *diagonal;
    int* placesInCol;
    row = new bool[n];
    col = new bool[n];
    diagonal = new bool[n];
    placesInCol = new int[n];

    init(placesInCol, n);
    printMatrix(placesInCol, n);

    
    
}
