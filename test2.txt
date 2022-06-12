#include <iostream>    //Para imprimir
 #include <cstring>     //Para usar funciones avanzadas de strings
 #include "Hospital.h" 
 #include "Usuario.h"
 #include "Cita.h"
 
 using namespace std;
 
 //Procedimiento menu
 void menu(){
	//Este es el menu principal de opciones que el usuario puede elegir
	cout << " \nMenu\n";
	cout << " 1. Registrar nuevo usuario\n";
	cout << " 2. Buscar usuario\n";
	cout << " 3. Mostrar registro de usuarios\n";
	cout << " 4. Mostrar informacion del hospital.\n";
	cout << " 5. Salir del programa\n";
 }
 
 //Programa principal
 int main(){
	int opc,dia,hora;
	string nombreUsuario,mes;
	
	//Se crea el objeto de tipo hospital privado
	HospitalPrivado starMedica;
	
	//Se asigna la informacion del hospital
	starMedica.setNombre("Star Medica");
	starMedica.setDireccion("Blvd. Bernardo Quintana Arrioja 4060, Col. San Pablo, QRO.");
	starMedica.setPrecioCita(1500);
	starMedica.creaEjemploUsario();
	
	//Inicio del programa
	cout<<" Bienvenido al asistente de "<<starMedica.getNombre()<<". Por favor seleccione una opcion del menu que se muestra a continuacion."<<endl;
	
	//Ciclo para que el sistema siga corriendo mientras no elija la opcion salir
	while(opc < 5 && opc > -1){
		//Impresion de menu
		menu();
		cin>>opc;
        cin.ignore();
		
		//Switch donde dependiendo la eleccion efectua diversos procedimientos
		switch(opc){
			
			//Caso 1 que agrega un nuevo usuario a la base de datos
			case 1:
			 cout<<"\n Por favor ingrese su nombre: ";
	         getline(cin, nombreUsuario);
		     cout<<"\n Por favor ingrese el mes en el que desea agendar su cita: ";
		     getline(cin, mes);
		     cout<<"\n Ingrese el dia: ";
		     cin>>dia;
		     cout<<"\n Ingrese la hora: ";
		     cin>>hora;
		     cout<<"\n Registro exitoso. Volviendo al menu principal..."<<endl;
			 starMedica.agregaUsuario(nombreUsuario,mes,dia,hora);
			break;
		
			//Caso 2 que busca un usuario en la base de datos
			case 2:
			 cout<<"\n Ingresa el nombre del usuario: ";
			 getline(cin, nombreUsuario);
			 starMedica.buscarUsuario(nombreUsuario);
			break;
			
			//Caso 3 que muestra todos los usuarios registrados
			case 3:
			 starMedica.muestraUsuario();
			break;
			
			//Caso 4 que muestra la informacion del hospital
			case 4:
		 	 starMedica.muestraInfo();
			 starMedica.muestraIngresos();
			break;
		}
	}
 }
