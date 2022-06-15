#include <iostream>
#include <fstream>
#include <algorithm>
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
	regex lt("<");
	regex gt(">");
	string tag;
	if (type == "includes") {
		content = regex_replace(content, lt, "&lt;");
		content = regex_replace(content, gt, "&gt;");
		tag = "<span class='" + type + "'>" + content + "</span>";
	}
	else {
		tag = "<span class='" + type + "'>" + content + "</span>";
	}
	return tag;
}

bool isVariable (string str) {
	regex expression("[a-zA-Z_]+[a-zA-Z_0-9]*");
	if (regex_match(str, expression)) {
		return true;
	}
	return false;
}

bool isNumber(string str) {
	regex expressionInt("[0-9]+");
	regex expressionFloat("[+-]*[0-9]+[.][0-9]+([eE][+-]*[0-9]+)?");
	if (regex_match(str, expressionInt)) {
		return true;
	}
	if (regex_match(str, expressionFloat)) {
		return true;
	}
	return false;
}

bool isComment(string str) {
	regex expression("[/][/][ a-zA-Z0-9]*[;_.,]*");
	if (regex_match(str, expression)) {
		return true;
	}
	return false;
}

bool isOperator(string str) {
	regex expression("[+]|[-]|[*]|[%]|[/]|[!]|[=]|[<]|[>]|");
	if (regex_match(str, expression)) {
		return true;
	}
	return false;
}

bool isPunctuation(string str) {
	regex expression("\\[|\\]|\\{|\\}|\\(|\\)|;|,");
	if (regex_match(str, expression)) {
		return true;
	}
	return false;
}

bool isString(string str) {
	regex expression("[\"][ a-zA-Z0-9_;,.]*[\"]");
	if (regex_match(str, expression)) {
		return true;
	}
	return false;
}

bool isReserved(string str) {
	regex expression("if|else|while|for|do|const|int|float|string|char|void|return|continue|using|namespace|break|bool|static|new|null|false|switch|this|throw|case|true|catch|try|class|public|virtual|double|cout|cin|long");
	if (regex_match(str, expression)) {
		return true;
	}
	return false;
}

bool isSpace(string str) {
	if (str == " ") {
		return true;
	}
	return false;
}

bool isBreakLine(string str) {
	if (str == "\n") {
		return true;
	}
	return false;
}

bool isTab(string str) {
	if (str == "\t") {
		return true;
	}
	return false;
}

bool isInclude(string str) {
	regex expression("#include.*");
	if (regex_match(str, expression)) {
		return true;
	}
	return false;
}

bool isDelimiter (char letter) {
	if (letter == '\n') {
		return true;
	}
	if (letter == ' ') {
		return true;
	}
	if (letter == '\t') {
		return true;
	}
	if (letter == '{' || letter == '}') {
		return true;
	}
	if (letter == '(' || letter == ')') {
		return true;
	}
	if (letter == '[' || letter == ']') {
		return true;
	}
	if (letter == '<' || letter == '>') {
		return true;
	} 
	if (letter == '+' || letter == '*' || letter == '-' || letter == '/' || letter == '%') {
		return true;
	}
	if (letter == ',') {
		return true;
	}
	if (letter == ';') {
		return true;
	}
	return false;
}

vector<string> crearArregloPalabras(string line) {
	string palabra = "";
	vector<string> vectorPalabras;
	//cout << line << endl;
	for (int i = 0; i < line.length(); i++) {
		if (line[i] == '#') {
			for (int p = i; p < line.length(); p++) {
				if (line[p + 1] == '>') {
					palabra += line[p];
					palabra += line[p + 1];
					vectorPalabras.push_back(palabra);
					palabra = "";
					i = p + 2;
					break;
				}
				else {
					palabra += line[p];
				}
			}
		}
		if (line[i] == '/' && line[i + 1] == '/') {
			for (int k = i; k < line.length(); k++) {
				if (line[k + 1] == '\n') {
					palabra += line[k];
					vectorPalabras.push_back(palabra);
					palabra = "";
					i = k + 1;
					break;
				}
				else {
					palabra += line[k];
				}
			}
		}
		else if (isDelimiter(line[i])) {
			palabra = line[i];
			vectorPalabras.push_back(palabra);
			palabra = "";
		} 
		else if (line[i] == '"') {
			for (int j = i; j < line.length(); j++) {
				if (line[j + 1] == '"') {
					palabra += line[j];
					palabra += line[j + 1];
					vectorPalabras.push_back(palabra);
					palabra = "";
					i = j + 1;
					break;
				}
				else {
					palabra += line[j];
				}
			}
		}
		else if (!isDelimiter(line[i + 1])) {
			palabra += line[i];
		} else {
			palabra += line[i];
			vectorPalabras.push_back(palabra);
			palabra = "";
		}
	}
	return vectorPalabras;
}




// Implementación del Resaltador Secuencial
void resaltadorSec(string *rutas, int size) {
	vector<string> vectorPalabras;
	for (int i = 0; i < size; i++) {
		string line;
		string aux;
		ifstream inputFile(rutas[i]);
		if (inputFile.is_open()) {
			while (getline(inputFile, line)) {
				aux += line;
				aux += "\n";
			}
			vectorPalabras = crearArregloPalabras(aux);
			int counter = 0;
			cout << "Vector de palabras\n";
			for (int j = 0; j < vectorPalabras.size(); j++) {
				if (vectorPalabras[j] == "<") {
					counter++;
				}
				cout << vectorPalabras[j] << ",";
			}
			cout << endl << counter << endl;
			
			int lengthArchivo = rutas[i].length();
			string nombreArchivo = "secuencial_" + rutas[i].substr(0, lengthArchivo) + ".html";
			ofstream html(nombreArchivo);
			
			html << "<!DOCTYPE html> \n";
			html << "<html lang = 'es'> \n";
			html << "<head> \n\t";
			html << "<meta charset = 'UTF-8'> \n\t";
			html << "<meta http - equiv = 'X-UA-Compatible' content = 'IE=edge'>\n\t";
			html << "<meta name = 'viewport' content = 'width=device-width, initial-scale=1.0'>\n\t";
			html << "<title> Actividad 5.3 </title> <link rel = 'stylesheet' href = 'styles.css'>";
			html << "</head>\n<body>\n";
			
			for(int j = 0; j < vectorPalabras.size(); j++) {
				string tagWord;
				if (isInclude(vectorPalabras[j])) {
					tagWord = addTag("includes", vectorPalabras[j]);
					html << tagWord;
				} else if (isReserved(vectorPalabras[j])) {
					tagWord = addTag("reserved", vectorPalabras[j]);
					html << tagWord;
				} else if (isVariable(vectorPalabras[j])) {
					tagWord = addTag("variable", vectorPalabras[j]);
					html << tagWord;
				} else if (isNumber(vectorPalabras[j])) {
					tagWord = addTag("number", vectorPalabras[j]);
					html << tagWord;
				} else if (isOperator(vectorPalabras[j])) {
					tagWord = addTag("operator", vectorPalabras[j]);
					html << tagWord;
				} else if (isPunctuation(vectorPalabras[j])) {
					tagWord = addTag("punctuation", vectorPalabras[j]);
					html << tagWord;
				} else if (isComment(vectorPalabras[j])) {
					tagWord = addTag("comment", vectorPalabras[j]);
					html << tagWord;
				} else if (isString(vectorPalabras[j])) {
					tagWord = addTag("string", vectorPalabras[j]);
					html << tagWord;
				} else if (isSpace(vectorPalabras[j])) {
					html << " ";
				} else if (isBreakLine(vectorPalabras[j])) {
					tagWord = "<br>";
					html << "\n";
					html << tagWord;
				} else if (isTab(vectorPalabras[j])) {
					tagWord = "&emsp;";
					html << "\t";
					html << tagWord;
				}

			}
			
			html << "\n</body>\n</html>";
			html.close();
		}
	}
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
	sleep(5);

	// Ejecución concurrente
	cout << "Ejecutando forma concurrente..." << endl;
	
	block_size = size_Arr / threads;

	for (i = 0; i < threads; i++) {
		blocks[i].arr = rutasArchivos;
		blocks[i].start = i * block_size;
		blocks[i].end = i != (threads - 1) ? (i + 1) * block_size : size_Arr;
	}

	msConc = 0;
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


	cout << "------------Secuencial--------------\n";
	cout << "Tiempo = " << setprecision(5) << (msSeq) << "\n";
	cout << "------------Multihilo--------------\n";
	cout << "Tiempo Promedio = " << setprecision(5) << (msConc / threads) << "\n";

	return 0;
}