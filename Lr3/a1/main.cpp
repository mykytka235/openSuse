#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char *argv[])
{
    ofstream out;

    out.open("/home/nekit/SPZ/Lr3/a1.txt");

    int counter = 0;

    srand((unsigned) time(0));
    while (counter < 100) {

        out << + rand() % 100 << endl;

        counter++;
    }

    out.close();

    //cout << "This program is executed with these command line arguments: "
    //     << argv[1]
    //     << atoi(argv[2]) << endl;


    return 0;
}
