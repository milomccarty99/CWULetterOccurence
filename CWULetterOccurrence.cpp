#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <fstream>
#include <string>
#include <sys/wait.h> // wait functionality
#include <sys/mman.h> // mmap functionality
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

#define NUMINTS (256) // we will examine all ascii chars
#define LOWERCASE_OFFSET (97)
#define UPPERCASE_OFFSET (65)
#define FILESIZE (NUMINTS *sizeof(int))
using namespace std;


class OccurrenceHandler {
	public:
	//iterates through the file provided (filename) to find number of char c in file
		int singleLetter(string filename, char c) {
			string line;
			int count = 0;
			ifstream ReadFile(filename);
			while(getline(ReadFile, line))
			{
				for (int i = 0; i < line.length(); i++)
				{
					if(line[i] == c)
					{
						count++;
					}
				}
			}
			ReadFile.close();
			return count;
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
	int c = 5;
	int *cptr = &c;

	int *map;
	

	//having the map anonymized makes it so we do not have to worry about file management (score!)
	map = (int*) mmap(NULL, FILESIZE,PROT_READ|PROT_WRITE,MAP_ANON| MAP_SHARED,-1,0);
	
	OccurrenceHandler myObj;

	//iterate through all ascii characters to fulfill tracking requirements
	for(int i =  0; i < NUMINTS ; i++)
	{
		int pid = fork(); //fork returns the pid number
		if(pid ==0)
		{
			map[i] += myObj.singleLetter(filename, i);
			
			exit(EXIT_SUCCESS);
		}
		else{
			// cout << "hello from parent" << endl;
		}
		
	}
	// wait until all child processes terminate
	while(wait(NULL) >= 0);
	/*
	//printing statement for all ascii chars
	for(int i = 0; i < NUMINTS; i++)
	{
		cout <<((char)(i))<<":: "<< (map[i]) << endl;
	}
	*/
	int* alphabet = (int*)calloc(0,26 * sizeof(int));
	for(int i = 0; i < 26; i++)
	{
		alphabet[i] = map[LOWERCASE_OFFSET + i] + map[UPPERCASE_OFFSET + i];
	}

	for(int i = 0; i < 26; i++)
	{
		cout<< ((char)(i+LOWERCASE_OFFSET)) << ": " << alphabet[i] << endl;
	}
	cout << "exiting program" << endl;
	return 0;

}
