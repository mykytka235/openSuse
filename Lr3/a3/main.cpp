#include<iostream>
#include<fstream>
#include <string>

using namespace std;

int main()
{
    string line, path = "/home/nekit/SPZ/Lr3/a1.txt";
    int sum = 0;

    ifstream in(path);

    if (in.is_open())
        while (getline(in, line))
            sum^=stoi(line);

    in.close();

    ofstream out;
    out.open (path, ofstream::out | ofstream::app);

    out << sum << endl;

    out.close();

}
