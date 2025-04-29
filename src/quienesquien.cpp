#include "../include/quienesquien.h"
#include <sstream>
#include <iostream>
#include <iterator>
#include <math.h>
#include <algorithm>

QuienEsQuien::QuienEsQuien(){
}

QuienEsQuien::QuienEsQuien(const QuienEsQuien &quienEsQuien){
}

QuienEsQuien& QuienEsQuien::operator= (const QuienEsQuien &quienEsQuien){
     return *this;
}

QuienEsQuien::~QuienEsQuien(){
     this->limpiar() ;
}

void QuienEsQuien::limpiar(){
     this->personajes.clear();
     this->atributos.clear();
     this->personajes_images.clear();
     for(vector<vector<bool>>::iterator it_tablero = this->tablero.begin();
         it_tablero != this->tablero.end();
         it_tablero++){
          (*it_tablero).clear();
     }
     this->tablero.clear();
     this->arbol.clear() ;
}

template <typename T>
std::ostream& operator<<  (std::ostream& out, const std::vector<T>& v) {
     if ( !v.empty() ) {
          out <<  '[' ;
          std::copy (v.begin(), v.end(), std::ostream_iterator<T>(out, ", "));
          out <<  "\b\b]";
     }

     return out;
}

void QuienEsQuien::mostrar_estructuras_leidas(){
     cout <<  "personajes: " <<  (this->personajes) <<  endl;
     cout <<  "atributos: " <<  (this->atributos) <<  endl;
     cout <<  "tablero:    " <<  endl;
     // Escribe la cabecera del tablero
     for(vector<string>::iterator it_atributos = this->atributos.begin() ;
         it_atributos != this->atributos.end() ;
         it_atributos++){
          cout <<  *it_atributos << "\t";
     }
     cout << endl;

     int indice_personaje = 0;
     for(vector<vector<bool>>::iterator it_tablero_atributos = tablero.begin();
         it_tablero_atributos!= tablero.end();
         it_tablero_atributos++){
          string personaje = this->personajes[indice_personaje];
          int indice_atributo = 0;
          for(vector<bool>::iterator it_atributos_personaje = (*it_tablero_atributos).begin();
              it_atributos_personaje != (*it_tablero_atributos).end() ;
              it_atributos_personaje++){
               cout <<  *it_atributos_personaje<< "\t";
               indice_atributo++;
          }
          cout <<  personaje <<  endl;
          indice_personaje++;
     }
}

/**
  * @brief Devuelve una copia de la cadena original sin las subcadenas no deseadas.
  *
  * @param cadena_original Cadena de la que se eliminan las subcadenas no deseadas.
  * @param cadena_a_eliminar Subcadena que se busca y se elimina.
  *
  * @return Copia de la cadena original sin las subcadenas no deseadas.
  */


string limpiar_string(string cadena_original,string cadena_a_eliminar){
     string linea(cadena_original);
     while(linea.find_first_of(cadena_a_eliminar) != std::string::npos){
          linea.erase(linea.find_first_of(cadena_a_eliminar),cadena_a_eliminar.length());
     }

     return linea;
}

istream& operator >>  (istream& is, QuienEsQuien &quienEsQuien) {
     quienEsQuien.limpiar();
     if(is.good()){ //Lee la linea con los nombres de las columnas
          string linea;
          getline(is, linea, '\n');
          linea = limpiar_string(linea,"\r");
          bool salir=false;
          while(!salir && linea.find('\t') != string::npos ){
               string atributo = linea.substr(0,linea.find('\t'));
               quienEsQuien.atributos.push_back(atributo);
               linea = linea.erase(0,linea.find('\t')+1);
               if (linea.substr(0,linea.find('\t'))=="Nombre personaje")
                   salir=true;
          }
     }
     //LEE los atributos para cada personaje
     while( is.good()) {
          string linea;
          getline(is, linea, '\n');
          linea = limpiar_string(linea,"\r");

          //Si la linea contiene algo extrae el personaje.
          //Si no la ignora.
          if(linea != ""){;
               vector<bool> atributos_personaje;
               int indice_atributo=0;
               bool salir=false;
               while(!salir && linea.find('\t') != string::npos){
                   string valor = linea.substr(0,linea.find('\t'));
                   if (valor == "0" || valor == "1"){
                    bool valor_atributo = valor == "1";
                    atributos_personaje.push_back(valor_atributo);
                    linea = linea.erase(0,linea.find('\t')+1) ;
                    indice_atributo++;
                   }
                   else{
                       salir=true;
                   }
               }
               string nombre_personaje;
               string imagen_personaje;
               if (linea.find('\t') == string::npos){
                nombre_personaje = linea;
                imagen_personaje=""; //no tiene imagen
               }
               else{
                nombre_personaje=linea.substr(0,linea.find('\t'));
                linea = linea.erase(0,linea.find('\t')+1) ;
                imagen_personaje=linea;

               }
               quienEsQuien.personajes.push_back(nombre_personaje);
               quienEsQuien.tablero.push_back(atributos_personaje);
               quienEsQuien.personajes_images.push_back(imagen_personaje);
          }
     }

     quienEsQuien.mostrar_estructuras_leidas();
     quienEsQuien.arbol = quienEsQuien.crear_arbol();
     quienEsQuien.tg=nullptr; //puntero al tablero grafico
     quienEsQuien.con=nullptr; //puntero a la consola
     if (quienEsQuien.modo_graph){
        int nper = quienEsQuien.personajes.size();
        int nrows,ncols=3;
        if (nper<8)
           nrows = nper/ncols;
        else{
            ncols=4;
            nrows = nper/ncols;
        }
        //se crea un tablero grafico
        quienEsQuien.tg = new TableroGrafico(400,400,nrows,ncols);
        //ponemos en el tablero grafico las im�genes
        for (int i=0;i<nper;i++){

            quienEsQuien.tg->putImagen(i/ncols,i%ncols,quienEsQuien.personajes_images[i].c_str());
        }
        //creamos la consola
        quienEsQuien.con=new Consola();
     }
     return is;
}

ostream& operator << (ostream& os, const QuienEsQuien &quienEsQuien){
     //Escribimos la cabecera, que contiene los atributos y al final una columna para el nombre

     cout<<"NUmero de Atributos "<<quienEsQuien.atributos.size()<<endl;
     for(vector<string>::const_iterator it_atributos = quienEsQuien.atributos.begin();
         it_atributos != quienEsQuien.atributos.end();
         it_atributos++){
          os << *it_atributos << "\t";
     }

     os << "Nombre personaje" << endl;
     
     //Rellenamos con ceros y unos cada linea y al final ponemos el nombre del personaje.
     for(int indice_personaje=0;indice_personaje<quienEsQuien.personajes.size();indice_personaje++){
          for(int indice_atributo=0;indice_atributo<quienEsQuien.atributos.size();indice_atributo++){
               os << quienEsQuien.tablero[indice_personaje][indice_atributo] << "\t";
          }
          os << quienEsQuien.personajes[indice_personaje] << endl;
     }
     
     return os;
}

/**
* @brief Convierte un numero a un vector de bool que corresponde
*        con su representadon en binario con un numero de digitos
*              f i j o.
*
* @param n Numero a convertir en binario.
* @param digitos Numero de digitos de la representaddn binaria.
*
* @return Vector de booleanos con la representadon en binario de @e n
*      con el numero de elementos especificado por @e digitos.
*/
vector<bool> convertir_a_vector_bool(int n, int digitos) {
     vector<bool> ret;
     while(n) {
          if (n&1)
               ret.push_back(true);
          else
               ret.push_back(false);

          n>>=1;
     }

     while(ret.size()<digitos)
          ret.push_back(false);

     reverse(ret.begin(),ret.end());
     return ret;
}

/**
 * @brief Calcula la entropía de un atributo específico.
 *
 * Esta función calcula la entropía de un atributo en el tablero de personajes, 
 * usando la fórmula de la entropía.
 *
 * @param atributo Índice del atributo para el cual se calcula la entropía.
 *                 Representa la columna de un atributo en el tablero.
 * 
 * @return La entropía calculada para el atributo especificado.
 *         
*/

double QuienEsQuien::calcular_entropia(int atributo) const {
    double positivos = 0, negativos = 0;
    for (size_t i = 0; i < tablero.size(); ++i) {
        const vector<bool>& fila = tablero[i];  // Obtenemos la fila en cada iteración
        if (fila[atributo]) {
            positivos++;
        } else {
            negativos++;
        }
    }

    int total = positivos + negativos;
    double ratioPositivo = positivos / total;
    double ratioNegativo = negativos / total;

    if (ratioPositivo != 0) {
    	ratioPositivo = -1 * (ratioPositivo) * log(ratioPositivo);
    }
    if (ratioNegativo != 0) {
    	ratioNegativo = -1 * (ratioNegativo) * log(ratioNegativo);
    }

    double entropia = ratioPositivo + ratioNegativo;
    
    return entropia;
}



/**
* @brief Esta es una propuesta de cabecera de la funcion para crear el arbol.
*/
bintree<Pregunta> QuienEsQuien::crear_arbol( vector<string> atributos,
                                             int indice_atributo,
                                             vector<string> personajes,
                                             vector<bool> personajes_restantes,
                                             vector<vector<bool>> tablero){

     //TODO :D:D
     bintree<Pregunta> arbol;

     int num_personajes = count(personajes_restantes.begin(),personajes_restantes.end(),true);
     if (num_personajes == 1) {
       string personaje = personajes[find(personajes_restantes.begin(),personajes_restantes.end(), true)-personajes_restantes.begin()];
       Pregunta pregunta(personaje, num_personajes);
       return bintree<Pregunta>(pregunta);
     }
     else if (num_personajes == 0 or indice_atributo >= atributos.size()) {
       return bintree<Pregunta>();
     }
	 else {

	  double min_entropia = 1000.0;  // Un valor arbitrariamente grande
	  int mejor_atributo = -1;

	  // Calcular la entropía para cada atributo y seleccionar el mejor.
	  for (int i = indice_atributo; i < atributos.size(); ++i) {
	  	double entropia_actual = calcular_entropia(i);  // Calcular entropía para el atributo i.
		if (entropia_actual < min_entropia) {
		    min_entropia = entropia_actual;
		    mejor_atributo = i;
		}
	  }

	  // Crear la pregunta asociada al mejor atributo.
	  Pregunta pregunta(atributos[mejor_atributo], num_personajes);

      vector<bool> personajes_si(personajes_restantes);
      vector<bool> personajes_no(personajes_restantes);

      for (int i=0; i<personajes_restantes.size(); i++) {
      	if (personajes_restantes[i]) {
        	if(tablero[i][indice_atributo]){
            	personajes_no[i] = false;
            } else {
                personajes_si[i] = false;
            }
        }
      }

          bintree<Pregunta> arbol_si = crear_arbol(atributos, indice_atributo+1, personajes, personajes_si, tablero);
          bintree<Pregunta> arbol_no = crear_arbol(atributos, indice_atributo+1, personajes, personajes_no, tablero);

          bintree<Pregunta> arbol(pregunta);
          arbol.insert_left(arbol.root(), arbol_si);
          arbol.insert_right(arbol.root(), arbol_no);

          return arbol;
	}
}

bintree<Pregunta> QuienEsQuien::crear_arbol(){
     int indice_atributo = 0;
     vector<bool> personajes_restantes;
     for(vector<string>::iterator it = personajes.begin();it!=personajes.end();it++){
          personajes_restantes.push_back(true);
     }

     return crear_arbol( this->atributos, indice_atributo, this->personajes,
                         personajes_restantes, this->tablero);
}

void QuienEsQuien::usar_arbol(bintree<Pregunta> arbol_nuevo){
     arbol = arbol_nuevo;
}

void QuienEsQuien::iniciar_juego(){
     Ventana v(tg,con,"WisW");
     if (modo_graph){
          v.show();
     }
     jugada_actual = arbol.root();
     while ((*jugada_actual).es_pregunta()) {
          cout << *jugada_actual << " (si/no): ";
          string respuesta;
          cin >> respuesta;
          while (respuesta != "si" && respuesta != "no") {
               cout << "Por favor, responde con 'si' o 'no': ";
               cin >> respuesta;
          }
          if (respuesta == "si") {
               jugada_actual = jugada_actual.left();
          } else {
               jugada_actual = jugada_actual.right();
          }
     }
     if ((*jugada_actual).es_personaje()) {
          string personaje = (*jugada_actual).obtener_personaje();
          cout << "Ya lo se! Tu personaje es: " << personaje << endl;
     }
     if (modo_graph){
          con->WriteText("Cuando completes QuienEsQuien, este mensaje lo podr?s quitar");
          char c;
          do{
               con->WriteText("Pulsa 0 para salir");
               c = con->ReadChar();

          }while (c!='0');

     }
     v.cerrar();
}	

set<string> QuienEsQuien::informacion_jugada(bintree<Pregunta>::node jugada_actual){
     //TODO :)
     set<string> personajes_levantados;
     bintree<Pregunta> subarbol;
     bintree<Pregunta>::level_iterator it;

     // Determinar si la jugada actual es la raíz
     bool esRaiz = (jugada_actual == arbol.root());
     bintree<Pregunta>::node nodoPadre;
     if (esRaiz) {
        nodoPadre = bintree<Pregunta>::node();
     } else {
        nodoPadre = jugada_actual.parent();
     }

     bool esHijoIzquierdo = false;
     if (!esRaiz) {
        esHijoIzquierdo = (jugada_actual == nodoPadre.left());
     }

     if (esRaiz) {
        subarbol = arbol;
     } else {
        if (esHijoIzquierdo) {
            arbol.prune_left(nodoPadre, subarbol);
        } else {
            arbol.prune_right(nodoPadre, subarbol);
        }
     }

     for (it = subarbol.begin_level(); it != subarbol.end_level(); ++it) {
        if ((*it).es_personaje()) {
            personajes_levantados.insert((*it).obtener_personaje());
        }
     }

     if (esRaiz) { //si la jugada es la raiz borrarla
        subarbol.clear();
     } else {
        if (esHijoIzquierdo) {
            arbol.insert_left(nodoPadre, subarbol);
        } else {
            arbol.insert_right(nodoPadre, subarbol);
        }
     }
     return personajes_levantados;
}

void escribir_esquema_arbol(ostream& ss, const bintree <Pregunta>& a,
                            bintree<Pregunta>::node n, string& pre){
     if (n.null())
          ss <<  pre <<  "-- x" <<  endl;
     else {
          ss <<  pre <<  "-- " <<  (*n) <<  endl;
          if ( !n.right().null() || !n.left().null()) {// Si no es una hoja
               pre += "   |";
               escribir_esquema_arbol(ss,a, n.right(), pre);
               pre.replace(pre.size()-4, 4, "    ");
               escribir_esquema_arbol (ss,a, n.left(), pre);
               pre.erase(pre.size()-4, 4);
          }
     }
}

void QuienEsQuien::escribir_arbol_completo() const{
     string pre = "";
     escribir_esquema_arbol(cout,this->arbol,this->arbol.root(),pre);
}

bintree<Pregunta> QuienEsQuien::eliminar_nodos_redundantes(bintree<Pregunta> &arbol, const bintree<Pregunta>::node &nodo) {
     if ((nodo.right().null() && nodo.left().null())) {
          return bintree<Pregunta>(*nodo);
     }

     bintree<Pregunta>::node left = nodo.left();
     bintree<Pregunta>::node right = nodo.right();
     bintree<Pregunta> nuevoArbol;

     if (!(!left.null() && (*left).obtener_num_personajes() > 0)) {
          bintree<Pregunta> rightTree = eliminar_nodos_redundantes(arbol, nodo.right());
          nuevoArbol.assign_subtree(rightTree, rightTree.root());
     } else if (!(!right.null() && (*right).obtener_num_personajes() > 0)) {
          bintree<Pregunta> leftTree = eliminar_nodos_redundantes(arbol, nodo.left());
          nuevoArbol.assign_subtree(leftTree, leftTree.root());
     } else {
          bintree<Pregunta> leftTree = eliminar_nodos_redundantes(nuevoArbol, nodo.left());
          bintree<Pregunta> rightTree = eliminar_nodos_redundantes(nuevoArbol, nodo.right());

          nuevoArbol = bintree<Pregunta>(*nodo);
          nuevoArbol.insert_left(nuevoArbol.root(), leftTree);
          nuevoArbol.insert_right(nuevoArbol.root(), rightTree);
     }

     return nuevoArbol;
}

void QuienEsQuien::eliminar_nodos_redundantes() {
     if (arbol.empty()) {
          return; // Si el árbol está vacío, no hay nada que hacer
     }
     arbol = eliminar_nodos_redundantes(arbol, arbol.root());
}



float QuienEsQuien::profundidad_promedio_hojas(){
	float total=0;
    int hijos=0;
    //recorremos todos los nodos del arbol y cuando el nodo es una hoja, calculamos su profundidad
    for(auto it=arbol.begin_level(); it != arbol.end_level(); ++it){
      if((*it).es_personaje()){
        ++hijos;
        int level=0;
        bintree<Pregunta>::node nodo_nivel = *it;
        while(!nodo_nivel.parent().null()){
          level++;
          nodo_nivel=nodo_nivel.parent();
        }
		total+=level;
     }
   }
   return total/hijos;
}

/**
* @brief Genera numero enteros positivos aleatorios en el rango [min,max].
**/
int generaEntero(int min, int max){
     int tam= max-min;
     return ((rand( )%tam)+min) ;
}

void QuienEsQuien::tablero_aleatorio(int numero_de_personajes){
     srand(0);
     this->limpiar();
     float log_2_numero_de_personajes = log(numero_de_personajes)/log(2);
     int numero_de_atributos = ceil(log_2_numero_de_personajes);

     cout <<  "Peticion para generar " <<  numero_de_personajes << " personajes ";
     cout <<  "con " << numero_de_atributos <<  " atributos" << endl;
     cout <<  "Paso 1: generar " <<  pow(2, numero_de_atributos) <<  " personajes" << endl;

     //Fase 1: completar el tablero con todos los personajes posibles
     //Complete el tablero y los nombres de personajes a la vez
     for(int indice_personaje=0;indice_personaje< pow(2,numero_de_atributos);indice_personaje++){
          vector<bool> atributos_personaje =
                                    convertir_a_vector_bool(indice_personaje,numero_de_atributos);
          string nombre_personaje = "Personaje_"+to_string(indice_personaje);

          this->personajes.push_back(nombre_personaje);
          this->tablero.push_back(atributos_personaje);
     }

     // Completo los nombres de los atributos.
     for( int indice_atributo=0;indice_atributo<numero_de_atributos;indice_atributo++){
          string nombre_atributo = "Atributo_"+to_string(indice_atributo);
          this->atributos.push_back(nombre_atributo);
     }
     cout <<  "Paso 2: eliminar " << (pow(2,numero_de_atributos)-numero_de_personajes) <<  "personajes" << endl;

     //Fase 2. Borrar personajes aleatoriamente hasta que quedan solo los que hemos pedido.
     while(personajes.size()>numero_de_personajes){
          int personaje_a_eliminar = generaEntero(0,personajes.size());
          personajes.erase(personajes.begin()+personaje_a_eliminar);
          tablero.erase(tablero.begin()+personaje_a_eliminar);
     }
}
void QuienEsQuien::ocultar_personajes_graph(const set<string> &personajes_activos){
    //ocultamos los personajes
    int idx=0;
    int ncols=tg->getNcols();
    for (auto it=personajes.begin();it!=personajes.end();++it,idx++){
        if ( personajes_activos.find(*it)==personajes_activos.end())
            tg->putImagen(idx/ncols,idx%ncols,imagen_ocultar.c_str());
    }
}


void QuienEsQuien::setImagenOcultar(const char * n){
    imagen_ocultar=n;
}

void QuienEsQuien::setModoGraph(bool m){
    modo_graph=m;
}

string QuienEsQuien::preguntas_formuladas(bintree<Pregunta>::node jugada){
  string preguntas;
  preguntas+="El personaje oculto tiene las siguientes características:\n";
  if(!jugada.parent().null()){
		preguntas+=(*(jugada.parent())).obtener_pregunta();
		if(jugada==jugada.parent().left()){
			preguntas+=" - si\n";
		}else{ // jugada es el hijo derecha
  			 preguntas+=" - no\n";
		}
  }
  preguntas+="pero aún no sé cual es";
  return preguntas;
}

void QuienEsQuien::elimina_personaje (const string nombre){
     bool found=false;
     int indice=0;
     while((!found)&&(indice<personajes.size())){
       if(personajes[indice]==nombre){
         found=true;
       }else ++indice;
     }
     if(found){
          //Si encuentra el personaje a borrar, lo eliminamos de los personajes, los atributos, el tablero,
          //y del arbol
          personajes.erase(personajes.begin()+indice);
          personajes_images.erase(personajes_images.begin()+indice);
          tablero.erase(tablero.begin()+indice);
          auto it=arbol.begin_preorder();
          bool quitado=false;
          while((!quitado)&&(it!=arbol.end_preorder())){
            if((*it).es_personaje()){
              if((*it).obtener_personaje()==nombre){
                bintree<Pregunta>::node hijo = *it;
                quitado=true;
                //Si el personaje es hijo izquierda, el padre se convierte directamente en el hijo derecha
                if(hijo==hijo.parent().left()){
                    hijo.parent()=hijo.parent().right();
                }else{ //el personaje es el hijo derecha
                     hijo.parent()=hijo.parent().left();
                }
              }
            }
            ++it;
          }
     }
}











