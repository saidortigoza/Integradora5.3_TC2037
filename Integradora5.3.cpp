#include <iostream>
#include <fstream>
#include <iomanip>
#include <math.h>
#include <regex>
#include <string>
#include <string.h>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include "utils.h"

const int threads = 8;

using namespace std;

string addTag(string type, string content) {
	return "<span class='" + type + "'>" + content + "</span>";
}

bool isVariable (string str) {
  regex expression("[a-zA-Z_]+[a-zA-Z_0-9]*");

  /*
  regex reg1("[0-9]+[a-zA-Z]+[0-9]*");
  regex reg2("[/][/][ a-zA-Z0-9_]*");
  string filterNumStr;
  regex_replace(back_inserter(filterNumStr), str.begin(), str.end(), reg1, "");
  string filterComStr;
  regex_replace(back_inserter(filterComStr), filterNumStr.begin(), filterNumStr.end(), reg2,"");
  sregex_iterator currentMatch(filterComStr.begin(), filterComStr.end(), reg);
  sregex_iterator lastMatch;
  */

	if (regex_match(str, expression)) {
		return true;
	} else {
		return false;
	}

	/*
  while (currentMatch != lastMatch) {
    smatch match = *currentMatch;
    //cout << match.str() << " : VARIABLE" << "\n";
    currentMatch++;
  }
  return true;
  */
}

bool isInteger (string str) {
  regex reg("[0-9]+");
  regex reg1("[0-9]+[.][0-9]+");
  regex reg2("[a-zA-Z]+[+-]*[0-9]+");
  string filterDecimal;
  regex_replace(back_inserter(filterDecimal), str.begin(), str.end(), reg1, "");
  string filterExp;
  regex_replace(back_inserter(filterExp), filterDecimal.begin(), filterDecimal.end(), reg2, "");
  sregex_iterator currentMatch(filterExp.begin(), filterExp.end(), reg);
  sregex_iterator lastMatch;
  while (currentMatch != lastMatch) {
    smatch match = *currentMatch;
    //cout << match.str() << " : INTEGER" << "\n";
    currentMatch++;
  }
  return true;
}

bool isReal (string str) {
  regex reg("[+-]*[0-9]+[.][0-9]+([eE][+-]*[0-9]+)?");
  sregex_iterator currentMatch(str.begin(), str.end(), reg);
  sregex_iterator lastMatch;
  while (currentMatch != lastMatch) {
    smatch match = *currentMatch;
    //cout << match.str() << " : REAL" << "\n";
    currentMatch++;
  }
  return true;
}

bool isComment(string str) {
  regex reg("[/][/][ a-zA-Z0-9]*");
  sregex_iterator currentMatch(str.begin(), str.end(), reg);
  sregex_iterator lastMatch;
  while (currentMatch != lastMatch) {
    smatch match = *currentMatch;
    //cout << match.str() << " : COMMENT" << "\n";
    currentMatch++;
  }
  return true;
}

// Implementación del Resaltador Secuencial
void resaltadorSec(string *rutas, int size) {
	for (int i = 1; i <= size; i++) {
		string line;
		ifstream inputFile(rutas[i]);
		if (inputFile.is_open()) {
			while (getline(inputFile, line, ' ')) {
				cout << line << endl;

				/*if(cont >= start && cont <= limit){
						writeFile << "<br>";
						for (int i = 0; i < str.length(); i++) {
							c = str[i];
							substring = str.substr(i);

							if (c == " ") {
								writeFile<< " ";
							}

							else if (isVariable(substring)) {
								writeFile<< addTag("variable", substring);
							}

							else if (isInteger(substring)) {
								writeFile<< addTag("number", substring);
							}

							else if (isReal(substring)) {
								writeFile<< addTag("number", substring);
							}

							else if (isComment(substring)) {
								writeFile<< addTag("comment", substring);
							}

							else if (c == "=") {
								writeFile<< addTag("operator",c);
							}

							else if (c == "+"){
								writeFile<< addTag("operator",c);
							}

							else if (c == "*") {
								writeFile<< addTag("operator",c);
							}
							
							else if (c == "/"){
								writeFile<< addTag("operator",c);
							}
							
							else if (c == "^") {
								writeFile<< addTag("operator",c);
							}
							
							else if ( c == "-" && str[i+1]==' ' ){
								writeFile<< addTag("operadores", c);
							}
							
							else if ( c == "(" ){
								writeFile<< addTag("operator",c);
							}
							
							else if ( c == ")" ){
								writeFile<< addTag("operator",c);
							}
							
							else if (c == "'")
							{
								writeFile<< addTag("operator", c);
							}
							
							else if ( c == "," ){
								writeFile<< addTag("operator",c);
							}
							
							else if ( c == "#" ){
								writeFile<< addTag("operator",c);
							}
							
							else if ( c == ";" ){
								writeFile<< addTag("operator",c);
							}
							
							else if (c == "{"){
								writeFile<< addTag("operator", c);
							}
							
							else if ( c == "}" ){
								writeFile<< addTag("operator",c);
							}
							
							else if (c == "["){
								writeFile<< addTag("operator", c);
							}
							
							else if ( c == "]" ){
								writeFile<< addTag("operator",c);
							}
							
							else if ( c == "<" ){
								writeFile<< addTag("operator",c);
							}
							
							else if (c == ">"){
								writeFile<< addTag("operator", c);
							}

							else continue;
						}
					}
					cont++;
				}
				return (void*) 0; */
			}
		}
	}
}


void* lexer(string archivo, int start, int limit, char type, int num) {
	string c, str, substring, textoHTML;

	ofstream writeFile;

	cout << "Lexer iniciado..." << endl;

	writeFile << "<!DOCTYPE html> <html lang='en'> <head> <meta charset='UTF-8'> <meta http-equiv='X-UA-Compatible' content='IE=edge'> <meta name='viewport' content='width=device-width, initial-scale=1.0'> <title>Actividad 5.3</title> <link rel='stylesheet' href='styles.css'> </head> <body>";

	ifstream readFile;

	readFile.open(archivo);
	int cont = 0;

	while (getline(readFile, str)) {
		if(cont >= start && cont <= limit){
			writeFile << "<br>";
			for (int i = 0; i < str.length(); i++)
			{
				c = str[i];
				substring = str.substr(i);

				if (c == " ") {
					writeFile<< " ";
				}

				else if (isVariable(substring)) {
					writeFile<< addTag("variable", substring);
				}

				else if (isInteger(substring)) {
					writeFile<< addTag("number", substring);
				}

				else if (isReal(substring)) {
					writeFile<< addTag("number", substring);
				}

				else if (isComment(substring)) {
					writeFile<< addTag("comment", substring);
				}

				else if (c == "=") {
					writeFile<< addTag("operator",c);
				}

				else if (c == "+"){
					writeFile<< addTag("operator",c);
				}

				else if (c == "*") {
					writeFile<< addTag("operator",c);
				}
				
				else if (c == "/"){
					writeFile<< addTag("operator",c);
				}
				
				else if (c == "^") {
					writeFile<< addTag("operator",c);
				}
				
				else if ( c == "-" && str[i+1]==' ' ){
					writeFile<< addTag("operadores", c);
				}
				
				else if ( c == "(" ){
					writeFile<< addTag("operator",c);
				}
				
				else if ( c == ")" ){
					writeFile<< addTag("operator",c);
				}
				
				else if (c == "'")
				{
					writeFile<< addTag("operator", c);
				}
				
				else if ( c == "," ){
					writeFile<< addTag("operator",c);
				}
				
				else if ( c == "#" ){
					writeFile<< addTag("operator",c);
				}
				
				else if ( c == ";" ){
					writeFile<< addTag("operator",c);
				}
				
				else if (c == "{"){
					writeFile<< addTag("operator", c);
				}
				
				else if ( c == "}" ){
					writeFile<< addTag("operator",c);
				}
				
				else if (c == "["){
					writeFile<< addTag("operator", c);
				}
				
				else if ( c == "]" ){
					writeFile<< addTag("operator",c);
				}
				
				else if ( c == "<" ){
					writeFile<< addTag("operator",c);
				}
				
				else if (c == ">"){
					writeFile<< addTag("operator", c);
				}

				else continue;
			}
		}
		cont++;
	}
	return (void*) 0;
}

// Implementación del Resaltador Concurrente

typedef struct {
	int start, end;
	string* arr;
} Block;

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
	}
}

/*
typedef struct{
	int start, end, id;
	string file;
} Block;


void* task(void* param){
    Block *block;
    block = (Block *) param;
    return ((void*)lexer(block->file, block->start, block->end, 'p', block->id));
}
*/

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

	// Ejecución secuencial
	cout << "Ejecutando forma secuencial..." << endl;

	msSeq = 0;
	start_timer();
	resaltadorSec(rutasArchivos, size_Arr);
	msSeq = stop_timer();

	// Ejecución concurrente
	cout << "Ejecutando forma concurrente...";
	
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