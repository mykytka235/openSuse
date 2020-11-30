#include <iostream>
#include <fstream>
#include <vector>

void InsertionSort(std::vector<int>& array)
{
    int i, j, key;

    for (i = 1; i < array.size(); i++)
    {
        key = array[i];
        j = i - 1;
        while (j >= 0 && array[j] > key)
        {
            array[j + 1] = array[j];
            j = j - 1;
        }
        array[j + 1] = key;
    }
}
int main(int argc, char *argv[])
{
    std::ofstream out;
    std::string line, file1Path, file2Path;

    if(argc > 1){
        file1Path = argv[0];
        file2Path = argv[1];
    }
    else{
        file1Path = "/home/nekit/SPZ/Lr4/test.txt";
        file2Path = "/home/nekit/SPZ/Lr4/testSorted.txt";
    }
    out.clear();
    out.open(file1Path);

    if (!out.is_open())
        std::cout << "Unable to open file";

    int counter = 0;

    srand((unsigned) time(0));

    while (counter < 100) {
        out << rand() % 100 << std::endl;
        counter++;
    }

    out.close();


    std::vector<int> array;

    std::ifstream in(file1Path);

    if (in.is_open())
        while (getline(in, line))
            array.push_back(stoi(line));

    in.close();

    InsertionSort(array);

    out.open(file2Path);

    uint i = 0;

    while (i < array.size()) {

        out << array[i] << std::endl;
        i++;
    }

    out.close();

    in.open(file2Path);
    int sum = 0;

    if (in.is_open())
        while (getline(in, line))
            sum^=stoi(line);

    in.close();

    out.open(file2Path, std::ofstream::out | std::ofstream::app);
    out << sum << std::endl;
    out.close();

    return 0;
}
