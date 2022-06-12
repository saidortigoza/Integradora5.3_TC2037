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
void resaltadorSeq(string *rutas, int size) {
	for (int i = 0; i < size; i++) {
		string line;
		ifstream inputFile(rutas[i]);
		if (inputFile.is_open()) {
			while (getline(inputFile, line)) {
				cout << line << endl;
				//Manipulacion del archivo
			}
		}
	}
}

typedef struct {
	int start, end;
	string* arr;
} Block;

//Resaltador Concurrente
void* resaltadorConc(void* args) {
	Block *block;
	block = (Block*)args;
	for (int i = block->start; i < block->end; i++) {
		string line;
		ifstream inputFile(block->arr[i]);
		if (inputFile.is_open()) {
			while (getline(inputFile, line)) {
				cout << line << endl;
				//Manipulacion del archivo
			}
		}
		inputFile.close();
	}
}

int main(int argc, char* argv[]) {
	pthread_t th[threads];
	double msSeq, msConc;
	Block blocks[threads];
	int block_size, i, j;
	string *rutasArchivos;

	const int size_Arr = argc;

	rutasArchivos = new string[size_Arr];

	for (i = 0; i < argc; i++) {
		rutasArchivos[i] = argv[i];
	}
	//Manera secuencial

	msSeq = 0;
	start_timer();
	resaltadorSeq(rutasArchivos, size_Arr);
	msSeq = stop_timer();

	//Manera Concurrente
	block_size = size_Arr / threads;

	for (i = 0; i < threads; i++) {
		blocks[i].arr = rutasArchivos;
		blocks[i].start = i * block_size;
		blocks[i].end = i != (threads - 1) ? (i + 1) * block_size : size_Arr;
	}

	msConc = 0;
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
		msConc += stop_timer();
	}

	cout << "------------Secuencial--------------\n";
	cout << "Tiempo = " << setprecision(5) << (msSeq / N) << "\n";
	cout << "------------Multihilo--------------\n";
	cout << "Tiempo Promedio = " << setprecision(5) << (msConc / N) << "\n";


	return 0;
}