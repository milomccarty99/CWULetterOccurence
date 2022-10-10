#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <fstream>
using namespace std;


class OccurrenceHandler { 
	public:
		// aAbBcCdDeEfFgGhHiIjJkKlLmMnNoOpPqQrRsStTuUvVwWxXyYzZ,!?-.01234567890
		int* singleLetter(string filename) {
			string line;
			ifstream ReadFile(filename);
			while(getline(ReadFile, line)) {
				cout << line;
			}
			ReadFile.close();
			return 0;
		}
};

int main(int argc, char** argv)
{
	// arguments
	
	// -d double letters (seperate process)
	// -p letter pairs (seperate process)
	// -v visual
	string filename = "blah.txt";
	for(int i = 1; i < argc; i++)
	{
		if(argv[i][0] != '-')
		{
			filename = argv[i];
		}
		else
		{
			cout << "cli argument" << argv[i] << endl;
		}
	}
	cout << filename << endl;
	OccurrenceHandler myObj;
	myObj.singleLetter(filename);
	return 0;
}
