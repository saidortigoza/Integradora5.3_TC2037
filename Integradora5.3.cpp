#include <iostream>
#include <fstream>
#include <iomanip>
#include <regex>
#include <string>
#include <vector>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "utils.h"



const int threads = 8;

using namespace std;

// Resaltador Secuencial
void resaltadorSeq(string a, int size) {

}

typedef struct {
	int start, end;
	string* arr;
} Block;

//Resaltador Secuencial
void* resaltadorConc(void* args) {
	Block *block;
	block = (Block*)args;
	for (int i = block->start; i < block->end; i++) {
		string line;
		ifstream inputFile(block->arr[i]);
		if (inputFile.is_open()) {
			while (getline(inputFile, line)) {
				//cout << line << endl;
				//Manipulacion del archivo
			}
		}
		inputFile.close();
		
		//cout << block->arr[i] << endl;
	}
}

int main(int argc, char* argv[]) {
	pthread_t th[threads];
	double ms;
	Block blocks[threads];
	int block_size, i, j;
	string *fileRoutes;

	const int size_Arr = argc;

	fileRoutes = new string[size_Arr];

	for (i = 0; i < argc; i++) {
		fileRoutes[i] = argv[i];
	}

	block_size = size_Arr / threads;

	for (i = 0; i < threads; i++) {
		blocks[i].arr = fileRoutes;
		blocks[i].start = i * block_size;
		blocks[i].end = i != (threads - 1) ? (i + 1) * block_size : size_Arr;
	}

	ms = 0;
	for (j = 0; j < N; j++) {
		start_timer();

		for (i = 0; i < threads; i++) {
			if (pthread_create(&th[i], NULL, resaltadorConc, (void*)&blocks[i]) != 0) {
				perror("Fallo al crear el hilo");
			}
		}

		for (i = 0; i < threads; i++) {
			if (pthread_join(th[i], NULL) != 0) {
				perror("Fallo al unir hilo");
			}
		}
		ms += stop_timer();
	}
	
	cout << "------------MultiThread--------------\n";
	cout << "avg time = " << setprecision(5) << (ms / N) << "\n";


	return 0;
}