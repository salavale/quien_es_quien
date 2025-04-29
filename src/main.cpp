#include <fstream>
#include <iostream>
#include <string>
#include <cmath>
#include <QApplication>
#include "quienesquien.h"

using namespace std;

int main(int argc, char * argv[]){

	bool jugar = false;
	bool limpiar = false;
	QuienEsQuien quienEsQuien;

    QApplication a(argc, argv);

	
   if(argc == 1){
        cout<<"Los parametro son:"<<endl;
        cout <<"1.- el fichero con los datos del juego" << endl;
        cout <<"2.- (Opcional) El fichero en disco de la imagen que oculta los personajes"<<endl;
        return 1;

    }else if(argc== 2 ){

               string parametroAleatorio = "aleatorio";
		if(argv[1]== parametroAleatorio){
			cout << "Creando un QuienEsQuien aleatorio"<< endl;
			int numero_de_personajes;
		   	int numero_de_atributos;

		   	do{
		   		cout << "¿Número de personajes? ";
		   		cin >>numero_de_personajes;
		   	}while(numero_de_personajes<=0);
                       quienEsQuien.setModoGraph(false);
		   	quienEsQuien.tablero_aleatorio(numero_de_personajes);

		   	string nombreFicheroSalida = string("datos/tablero") +
		   		"-num-per="+to_string(numero_de_personajes)+".csv";

		   	cout << "Guardando tablero aleatorio en "<<nombreFicheroSalida<<endl;
		   	ofstream archivoDeSalida(nombreFicheroSalida);
		   	archivoDeSalida << quienEsQuien;

		   	cout << "Guardado"<<endl;
		   	return 0;

		}else{
			cout << "Cargando fichero para jugar'"<< argv[1] <<"'"<< endl;
			ifstream f (argv[1]);
			if (!f){
				cout<<"No puedo abrir el fichero "<<argv[1]<<endl;
				return 1;
			}
                       quienEsQuien.setModoGraph(false);
			f>> quienEsQuien;
			jugar = true;
		}
	} else if(argc == 3 ){
		string parametroLimpiar = "limpiar";
		if(parametroLimpiar== argv[2]){
			cout << "Cargando fichero para limpiar (sin jugar) '"<< argv[1] <<"'"<< endl;
			ifstream f (argv[1]);
			if (!f){
				cout<<"No puedo abrir el fichero "<<argv[1]<<endl;
				return 1;
			}
			quienEsQuien.setModoGraph(false);
			f>> quienEsQuien;
			jugar = false;
		} else{
            cout<<"Modo Grafico......"<<endl;
            cout << "Cargando fichero para jugar'"<< argv[1] <<"'"<< endl;
            ifstream f (argv[1]);
            if (!f){
                cout<<"No puedo abrir el fichero "<<argv[1]<<endl;
                return 1;
            }
            quienEsQuien.setModoGraph(true);
            f>> quienEsQuien;
            
            quienEsQuien.setImagenOcultar(argv[2]);
            jugar = true;
		}

	} else {
		cout << "No se reconocen los argumentos. Ejemplos de uso:" << endl;
		cout << "\tJugar:               ./bin/quienesquien RUTA_FICHERO"<< endl;
		cout << "\tLimpiar sin jugar:   ./bin/quienesquien RUTA_FICHERO limpiar"<< endl;
		cout << "\tGenerar aleatorio:   ./bin/quienesquien aleatorio"<< endl;
        cout << "\t Jugar en Modo Grafico : ./bin/quiensquien ./datos/personajes_graficos.csv ./datos/Tapada.png"<<endl;
		return 1;
	}
	
	cout << "=========== Arbol en crudo ===========" << endl;
	quienEsQuien.escribir_arbol_completo();
	
	cout << "Profundidad promedio de las hojas del arbol: ";
	cout << quienEsQuien.profundidad_promedio_hojas() << endl; 
	cout << "======================================" << endl << endl << endl;

	quienEsQuien.eliminar_nodos_redundantes();

	cout << "=========== Arbol ===================="<<endl;
	quienEsQuien.escribir_arbol_completo();
	cout << "Profundidad promedio de las hojas del arbol: ";
	cout << quienEsQuien.profundidad_promedio_hojas()<<endl; 
	cout << "======================================" << endl << endl << endl;
	int r=0;
	if(jugar){

		quienEsQuien.iniciar_juego();
		
		r=a.exec();
	}	
        return r;
}
