#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

void SelectionSort(vector<int>& array)
{
    uint jMin;

    for (uint i = 0; i < array.size()-1; i++)
    {
        jMin = i;
        for (uint j = i+1; j < array.size(); j++)
        {
            if (array[j] < array[jMin])
                jMin = j;
        }
        if (jMin != i)
            swap(array[i], array[jMin]);
    }
}

int main()
{
    string line;
    vector<int> array;

    ifstream in("/home/nekit/SPZ/Lr3/a1.txt"); // окрываем файл для чтения

    if (in.is_open())
        while (getline(in, line))
            array.push_back(stoi(line));

    in.close();     // закрываем файл

    SelectionSort(array);
    /*
    for(auto &num : array)
        cout << num << endl;

    cout << "End of program" << endl;
    */

    ofstream out;
    out.open("/home/nekit/SPZ/Lr3/a1.txt");

    uint i = 0;

    while (i < array.size()) {

        out << array[i] << endl;
        i++;
    }

    out.close();
}
