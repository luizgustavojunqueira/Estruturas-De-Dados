/**************************************************
*
* Luiz Gustavo S. S. Junqueira; Geffte Luís S. Caetano
* Trabalho 1
* Professor(a): Fábio Martinez
*
*/

#include <iostream>
using namespace std;

class No{
public:
    char* ip;
    int custo;
    No* prox;

    No(char* ip, int custo){
        this->ip = ip;
        this->custo = custo;
        this->prox = nullptr;
    }
};

class ListaLigada{

public:
    No* inicio;

    ListaLigada(){
        inicio = nullptr;
    }

    void apagaLista();
    void insereNo(char* ip, int custo);
    void printarLista();

};

class NoRede{
public:
    char* ip;
    int prior;
    ListaLigada viz;

    NoRede(){

    }

    NoRede(char* ip, int prior){
        this->ip = ip;
        this->prior = prior;
        this->viz = ListaLigada();
    }
};

class Rede{
public:
    NoRede* nos;
    Rede(int n){
        this->nos = new NoRede[n];
    }
};



void ListaLigada::insereNo(char* ip, int custo){
    No* novoNo = new No(ip, custo);

    if(inicio == nullptr){
        inicio = novoNo;
        return;
    }

    //Percorre até o final da lista ligada
    No* p = inicio;
    while(p->prox != nullptr){
        p = p->prox;
    }

    p->prox = novoNo;
}

void ListaLigada::printarLista(){
    No* p = inicio;

    if(inicio == nullptr){
        cout << "Lista Vazia" << endl;
        return;
    }

    while(p != nullptr){
        cout << "IP: " << p->ip << " Custo: " << p->custo << " -> ";
        p = p->prox;
    }
    cout << endl;
}

void ListaLigada::apagaLista(){
    No* temp;
    while(inicio != NULL){
        temp = inicio;
        inicio = inicio->prox;
        free(temp);
    }
}

int compararIp(char *ip1, char *ip2){
    for (int i = 0; i < 16; i++) {
        if(ip1[i] != NULL){
            if(ip2[i] != NULL){
                if (ip1[i] != ip2[i]) {
                    return 0;
                }
            }else{
                return 0;
            }
        }else if(ip2[i] != NULL){
            return 0;
        }
    }
    return 1;
}

int main(){

    int n = 0;
    cin >> n;
    Rede r = Rede(n);

    for(int i = 0; i<n; i++){
        char* ip = new char[16];
        cin >> ip;
        NoRede no = NoRede(ip, 0);
        r.nos[i] = no;
    }

    int m = 0;
    cin >> m;

    for(int i = 0; i < m; i++){
        char *ip1 = new char[16], *ip2 = new char[16];
        int custo = 0;

        cin >> ip1 >> ip2 >> custo;

        for(int j = 0; j < n; j++){
            if(compararIp(ip1, r.nos[j].ip) == 1){
                r.nos[j].viz.insereNo(ip2, custo);
            }
        }
    }

    for(int i = 0; i < n; i++){
        cout << "IP: " << r.nos[i].ip << " Prior: " << r.nos[i].prior << endl;
        cout << "Lista Vizinhos: ";
        r.nos[i].viz.printarLista();
        cout << endl;
    }
    
}