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

#define FILEPATH "./temp.tmp"
#define NUMINTS (26) // 26 for now, but we will want to keep stats of non-alphabet chars
#define FILESIZE (NUMINTS *sizeof(int))
using namespace std;


class OccurrenceHandler {
	public:
		// aAbBcCdDeEfFgGhHiIjJkKlLmMnNoOpPqQrRsStTuUvVwWxXyYzZ,!?-.01234567890
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

	//int pid = fork();

	for(int i =  0; i < 26; i++)
	{
		int pid = fork(); //fork returns the pid number
		if(pid ==0)
		{
			//ChildProcess();
			map[i] += myObj.singleLetter(filename, i+65);
			//cout << ((char)(i+65)) << ": " << counts[i] << endl;
			//*cptr+=1;
			//cout<<*cptr << endl;
			
			exit(EXIT_SUCCESS);
		}
		else{
			//ParentProcess();
		}
		
	}
	for(int i = 0; i <26; i++)
	{
		//counts2[i] = 0;
		int pid = fork();
		if(pid ==0)
		{
			map[i]+= myObj.singleLetter(filename,i+97);
			//cout << ((char)(i+97)) << ":: " << *counts2[i]<<endl;
			return 0;
		}
	}
	while(wait(NULL) >= 0);

	//printing statement
	for(int i = 0; i < 26; i++){
		cout <<((char)(97+i))<<":: "<< (map[i]) << endl;
	}

	while(wait(NULL) >= 0);
	//cout << map[20] <<endl ;
	cout << "exiting program" << endl;
	return 0;

}
