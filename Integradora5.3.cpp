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

// Funcion addTag
// La funcion addTag tiene una complejidad temporal de O(1) o constante para todos los casos,
// ya que unicamente trabaja con los parámetros que recibe para devolver un string.
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

// Funcion isVariable
// La funcion isVariable tiene una complejidad temporal de O(1) o constante para todos los casos,
// ya que unicamente trabaja con el string que recibe para devolver un valor booleano si el string
// hace match con la expresion regular declarada. En este caso si el string corresponde a una
// variable.
bool isVariable (string str) {
	regex expression("[a-zA-Z_]+[a-zA-Z_0-9]*");
	if (regex_match(str, expression)) {
		return true;
	}
	return false;
}

// Funcion isNumber
// La funcion isNumber tiene una complejidad temporal de O(1) o constante para todos los casos,
// ya que unicamente trabaja con el string que recibe para devolver un valor booleano si el string
// hace match con la expresion regular declarada. En este caso si el string corresponde a un
// numero, entero o flotante.
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

// Funcion isComment
// La funcion isVariable tiene una complejidad temporal de O(1) o constante para todos los casos,
// ya que unicamente trabaja con el string que recibe para devolver un valor booleano si el string
// hace match con la expresion regular declarada. En este caso si el string corresponde a un
// comentario.
bool isComment(string str) {
	regex expression("[/][/][ a-zA-Z0-9]*[;_.,]*");
	if (regex_match(str, expression)) {
		return true;
	}
	return false;
}

// Funcion isOperator
// La funcion isOperator tiene una complejidad temporal de O(1) o constante para todos los casos,
// ya que unicamente trabaja con el string que recibe para devolver un valor booleano si el string
// hace match con la expresion regular declarada. En este caso si el string corresponde a un
// operador.
bool isOperator(string str) {
	regex expression("[+]|[-]|[*]|[%]|[/]|[!]|[=]|[<]|[>]|");
	if (regex_match(str, expression)) {
		return true;
	}
	return false;
}

// Funcion isPunctuation
// La funcion isPunctuation tiene una complejidad temporal de O(1) o constante para todos los casos,
// ya que unicamente trabaja con el string que recibe para devolver un valor booleano si el string
// hace match con la expresion regular declarada. En este caso si el string corresponde a un
// signo de puntuacion.
bool isPunctuation(string str) {
	regex expression("\\[|\\]|\\{|\\}|\\(|\\)|;|,|:");
	if (regex_match(str, expression)) {
		return true;
	}
	return false;
}

// Funcion isString
// La funcion isString tiene una complejidad temporal de O(1) o constante para todos los casos,
// ya que unicamente trabaja con el string que recibe para devolver un valor booleano si el string
// hace match con la expresion regular declarada. En este caso si el string corresponde a un
// string.
bool isString(string str) {
	regex expression("[\"][ a-zA-Z0-9_;,.]*[\"]");
	if (regex_match(str, expression)) {
		return true;
	}
	return false;
}

// Funcion isReserved
// La funcion isReserved tiene una complejidad temporal de O(1) o constante para todos los casos,
// ya que unicamente trabaja con el string que recibe para devolver un valor booleano si el string
// hace match con la expresion regular declarada. En este caso si el string corresponde a una
// palabra reservada.
bool isReserved(string str) {
	regex expression("|push_back|front|back|pop|stack|queue|dequeu|iterator|if|else|while|for|do|const|int|float|string|char|void|return|continue|using|namespace|break|bool|static|new|null|false|switch|this|throw|case|true|catch|try|class|public|virtual|double|cout|cin|long|printf|scanf|unsigned|typedef|delete");
	if (regex_match(str, expression)) {
		return true;
	}
	return false;
}

// Funcion isSpace
// La funcion isSpace tiene una complejidad temporal de O(1) o constante para todos los casos,
// ya que unicamente trabaja con el string que recibe para devolver un valor booleano si el string
// hace match con un espacio en blanco.
bool isSpace(string str) {
	if (str == " ") {
		return true;
	}
	return false;
}

// Funcion isBreakLine
// La funcion isBreakLine tiene una complejidad temporal de O(1) o constante para todos los casos,
// ya que unicamente trabaja con el string que recibe para devolver un valor booleano si el string
// hace match con un salto de linea.
bool isBreakLine(string str) {
	if (str == "\n") {
		return true;
	}
	return false;
}

// Funcion isTab
// La funcion isTab tiene una complejidad temporal de O(1) o constante para todos los casos,
// ya que unicamente trabaja con el string que recibe para devolver un valor booleano si el string
// hace match con un tabulador.
bool isTab(string str) {
	if (str == "\t") {
		return true;
	}
	return false;
}

// Funcion isInclude
// La funcion isInclude tiene una complejidad temporal de O(1) o constante para todos los casos,
// ya que unicamente trabaja con el string que recibe para devolver un valor booleano si el string
// hace match con la expresion regular declarada. En este caso si el string corresponde a un
// #include.
bool isInclude(string str) {
	regex expression("#include.*");
	if (regex_match(str, expression)) {
		return true;
	}
	return false;
}

// Funcion isDelimiter
// La funcion isDelimiter tiene una complejidad temporal de O(1) o constante para todos los casos,
// ya que unicamente trabaja con el char que recibe para devolver un valor booleano si el char
// hace match con un delimitador.
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

// Funcion crearArregloPalabras
// La funcion crearArregloPalabras trabaja con el string line que recibe, el cual corresponde al codigo
// completo del archivo que se desea transformar. En este caso, devuelve un vector de strings que contiene
// el codigo separado de manera que el resaltador pueda trabajar con el de manera sencilla.
// El algoritmo recorre n*n pasos para el peor de los casos, por lo que su complejidad temporal es de O(n^2).
vector<string> crearArregloPalabras(string line) {
	string palabra = "";
	vector<string> vectorPalabras;
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

// Funcion resaltadorSec
// La funcion resaltadorSec recorre n pasos 4 veces, por lo que tiene una complejidad temporal de O(n^4)
// para el peor de los casos.
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

			int lengthArchivo = rutas[i].length();
			string nombreArchivo = "secuencial_" + rutas[i].substr(lengthArchivo - 6, lengthArchivo) + ".html";
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
		inputFile.close();
	}
}

// Implementación del Resaltador Concurrente
typedef struct {
	int start, end;
	string* arr;
} Block;

// Funcion resaltadorConc
// La funcion resaltadorConc recorre n pasos 4 veces, por lo que tiene una complejidad temporal de O(n^4)
// para el peor de los casos.
void* resaltadorConc(void* args) {
	Block *block;
	block = (Block*)args;
	vector<string> vectorPalabras;
	for (int i = block->start; i < block->end; i++) {
		string line;
		string aux;
		ifstream inputFile(block->arr[i]);
		if (inputFile.is_open()) {
			while (getline(inputFile, line)) {
				aux += line;
				aux += "\n";
			}
			vectorPalabras = crearArregloPalabras(aux);
			int lengthArchivo = block->arr[i].length();
			string nombreArchivo = "concurrente_" + block->arr[i].substr(lengthArchivo - 6, lengthArchivo) + ".html";
			ofstream html(nombreArchivo);

			html << "<!DOCTYPE html> \n";
			html << "<html lang = 'es'> \n";
			html << "<head> \n\t";
			html << "<meta charset = 'UTF-8'> \n\t";
			html << "<meta http - equiv = 'X-UA-Compatible' content = 'IE=edge'>\n\t";
			html << "<meta name = 'viewport' content = 'width=device-width, initial-scale=1.0'>\n\t";
			html << "<title> Actividad 5.3 </title> <link rel = 'stylesheet' href = 'styles.css'>";
			html << "</head>\n<body>\n";

			for (int j = 0; j < vectorPalabras.size(); j++) {
				string tagWord;
				if (isInclude(vectorPalabras[j])) {
					tagWord = addTag("includes", vectorPalabras[j]);
					html << tagWord;
				}
				else if (isReserved(vectorPalabras[j])) {
					tagWord = addTag("reserved", vectorPalabras[j]);
					html << tagWord;
				}
				else if (isVariable(vectorPalabras[j])) {
					tagWord = addTag("variable", vectorPalabras[j]);
					html << tagWord;
				}
				else if (isNumber(vectorPalabras[j])) {
					tagWord = addTag("number", vectorPalabras[j]);
					html << tagWord;
				}
				else if (isOperator(vectorPalabras[j])) {
					tagWord = addTag("operator", vectorPalabras[j]);
					html << tagWord;
				}
				else if (isPunctuation(vectorPalabras[j])) {
					tagWord = addTag("punctuation", vectorPalabras[j]);
					html << tagWord;
				}
				else if (isComment(vectorPalabras[j])) {
					tagWord = addTag("comment", vectorPalabras[j]);
					html << tagWord;
				}
				else if (isString(vectorPalabras[j])) {
					tagWord = addTag("string", vectorPalabras[j]);
					html << tagWord;
				}
				else if (isSpace(vectorPalabras[j])) {
					html << " ";
				}
				else if (isBreakLine(vectorPalabras[j])) {
					tagWord = "<br>";
					html << "\n";
					html << tagWord;
				}
				else if (isTab(vectorPalabras[j])) {
					tagWord = "&emsp;";
					html << "\t";
					html << tagWord;
				}

			}

			html << "\n</body>\n</html>";
			html.close();
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

	for (i = 1; i < argc; i++) {
		rutasArchivos[i] = argv[i];
	}

	// Ejecución secuencial
	cout << "Ejecutando forma secuencial..." << endl;

	msSeq = 0;
	start_timer();
	resaltadorSec(rutasArchivos, size_Arr);
	msSeq = stop_timer();
	//sleep(5);

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

	double n = ((size_Arr - 1) / threads) < 1 ? (size_Arr - 1) % threads : threads;
	cout << "------------Secuencial--------------\n";
	cout << "Tiempo = " << setprecision(5) << (msSeq) << "\n";
	cout << "------------Multihilo--------------\n";
	cout << "Tiempo Promedio = " << setprecision(5) << (msConc / n) << "\n";

	return 0;
}