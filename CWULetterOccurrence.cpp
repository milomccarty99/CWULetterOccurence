#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <fstream>
#include <string>
#include <tuple> // for sorting functionality
#include <sys/wait.h> // wait functionality
#include <sys/mman.h> // mmap functionality
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

#define NUMINTS (256) // we will examine all ascii chars
#define LOWERCASE_OFFSET (97)
#define UPPERCASE_OFFSET (65)
#define ALPHABET_LEN (26)
#define MMAP_FILESIZE (NUMINTS * sizeof(int))
using namespace std;


class OccurrenceHandler {
	public:
	//iterates through the file provided (filename) to find number of char c in file
		int singleLetter(string filename, char c) {
			fstream ReadFile(filename);
			string line;
			int count = 0;
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
//helper method for quickSort
int partition(tuple<char,int>* data, int low, int high)
{
	tuple<char, int> pivot = data[high];

	int i = low-1;

	for(int j = low; j< high; j++)
	{
		if(get<1>(data[j]) > get<1>(pivot)) // sort high to low
		{
			i++;
			tuple<char, int> temp_swap = data[i];
			data[i] = data[j];
			data[j] = temp_swap;
		}
	}
	tuple<char,int> temp = data[i+1];
	data[i+1] = data[high];
	data[high]= temp;
	
	return i+1;

}
// quickSort method for sorting characters
tuple<char, int>* quickSort(tuple<char,int>* data,int low, int high)
{
	if(low < high)
	{
		int pi = partition(data, low, high);
		quickSort(data, low, pi -1);
		quickSort(data, pi+1, high);
	}
	return data;
}

void outputData(tuple<char, int> * data, int size)
{
	for(int i = 0; i < size; i++)
	{
		cout << get<0>(data[i]) << ": " << get<1>(data[i]) << endl;
	}
}

//given a sorted list of alphabet characters
void displayBarGraph(tuple<char, int> * data, int size)
{
	cout<<"bar graph of letter occurrence: " << endl;
	if(size == 0)
	{
		return;
	}
	int height = 10;
	int largest = get<1>(data[0]);
	int bar = largest / height;
	if(bar<=0)
	{
		bar = 1;
	}
	int *vis = (int*) malloc(size * sizeof(int));
	for(int i = 0; i < size; i++)
	{
		vis[i] = ((int)(get<1>(data[i])/ bar));
	}

	for(int i = height -1; i >= 0; i--)
	{
		for(int j = 0; j < size; j++)
		{
			if(vis[j]>= i)
			{
				cout<< "█ ";
			}
			else
			{
				cout << "  ";
			}
		}
		cout << endl;
	}
	for(int i = 0; i < size; i ++ )
	{
		cout << get<0>(data[i]) << " " ;
	}
	cout << endl;
}
//
int main(int argc, char** argv)
{
	// arguments

	// -i ignore bar graph
	bool showBarGraph = true;
	string filename = "file not provided";
	for(int i = 1; i < argc; i++)
	{
		if(argv[i][0] != '-')
		{
			filename = argv[i];
		}
		else
		{
			cout << "cli argument" << argv[i] << endl;
			int j = 0; 
			while(argv[i][j] != '\0')
			{
				if(argv[i][j] == 'i')
				{
					showBarGraph = false;
				}
				j++;
			}
		}
	}

	cout << "The most common letters in " << filename << " are as follows:" << endl;
	//having the map anonymized makes it so we do not have to worry about file management (score!)
	int *map = (int*) mmap(NULL, MMAP_FILESIZE,PROT_READ|PROT_WRITE,MAP_ANON| MAP_SHARED,-1,0);
	OccurrenceHandler myObj;

	//iterate through all ascii characters to fulfill tracking requirements
	for(int i =  0; i < NUMINTS ; i++)
	{
		int pid = fork(); //fork returns the pid number
		if(pid ==0) // child process
		{
			map[i] += myObj.singleLetter(filename, i);
			
			exit(EXIT_SUCCESS);
		}
		
	}
	// wait until all child processes terminate
	while(wait(NULL) >= 0);
	// create array of tuples for our data
	tuple<char,int>* alphabet = (tuple<char,int>*)malloc(ALPHABET_LEN * sizeof(tuple<char,int>));
	for(int i = 0; i < 26; i++)
	{
		// combine lower and upper case letter counts as sum
		int sum = map[LOWERCASE_OFFSET + i] + map[UPPERCASE_OFFSET + i];
		alphabet[i] = make_tuple((char)(UPPERCASE_OFFSET+i), sum );
	}
	//print of sorted characters

	quickSort(alphabet, 0, ALPHABET_LEN -1);

	outputData(alphabet,ALPHABET_LEN);
	if(showBarGraph) 
		displayBarGraph(alphabet, ALPHABET_LEN);
	cout << "exiting program" << endl;
	//unmap allocated shared memory
	munmap(map,MMAP_FILESIZE);
	return 0;

}
