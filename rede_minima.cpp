/**************************************************
*
* Luiz Gustavo S. S. Junqueira; Geffte Luís S. Caetano
* Trabalho 1
* Professor(a): Fábio Martinez
*
*/

#include <iostream>
#include <climits>
using namespace std;

int compararIp(char *ip1, char *ip2);

class No{
public:
    char* ip;
    int custo;
    No* prox;

    No(){}

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

class IP{
public:
    char* ip;
    int prior;
    ListaLigada viz;

    IP(){

    }

    IP(char* ip, int prior){
        this->ip = ip;
        this->prior = prior;
        this->viz = ListaLigada();
    }
};

class Rede{
public:
    IP* ips;
    int tam;

    Rede(int n){
        this->tam = n;
        this->ips = new IP[n];

        for(int i = 0; i<n; i++){
            char* ip = new char[16];
            cin >> ip;
            IP no = IP(ip, INT_MAX);
            ips[i] = no;
        }

        int m = 0;
        cin >> m;

        for(int i = 0; i < m; i++){
            char *ip1 = new char[16], *ip2 = new char[16];
            int custo = 0;

            cin >> ip1 >> ip2 >> custo;

            for(int j = 0; j < n; j++){
                if(compararIp(ip1, ips[j].ip) == 1){
                    ips[j].viz.insereNo(ip2, custo);
                }
            }
        }
    }

    IP extrai_min();
    void diminui_prioridade(IP j, int c);
    void constroiMinHeap();
    IP buscarIP(char *ip);

private:
    int pai(int i);
    int esquerdo(int i);
    int direito(int i);
    void troca(int i, int j);
    void desce(int i);
    void sobe(int i); 
};

int compararIp(char *ip1, char *ip2){
    for (int i = 0; i < 16; i++) {
        if(ip1[i] != '\0'){
            if(ip2[i] != '\0'){
                if (ip1[i] != ip2[i]) {
                    return 0;
                }
            }else{
                return 0;
            }
        }else if(ip2[i] != '\0'){
            return 0;
        }
    }
    return 1;
}

void Rede::constroiMinHeap(){
    for(int i = tam/2-1; i >= 0; i--){
        desce(i);
    }
}

int redeCustoMinimo(Rede R){
    int custoTotal = 0;
    R.ips[0].prior = 0;
    R.constroiMinHeap();

    while(R.tam != 0){
        IP i = R.extrai_min();
        custoTotal += i.prior;

        No *p = i.viz.inicio;
        while(p != NULL){
            IP j = R.buscarIP(p->ip);
            if(j.ip != NULL && p->custo < j.prior){
                R.diminui_prioridade(j, p->custo);
            }
            p = p->prox;
        }


        cout << "CUSTO: " << custoTotal << endl << endl;

    }

    return custoTotal;
}

int main(){

    int n = 0;
    cin >> n;
    Rede r = Rede(n);

    // for(int i = 0; i < n; i++){
    //     cout << "IP: " << r.ips[i].ip << " Prior: " << r.ips[i].prior << endl;
    //     cout << "Lista Vizinhos: ";
    //     r.ips[i].viz.printarLista();
    //     cout << endl;
    // }

    cout << "CUSTO: " << redeCustoMinimo(r) << endl;

}

IP Rede::extrai_min(){
    IP menor;
    if(tam > 0){
        menor = ips[0];
        ips[0] = ips[tam-1];
        tam -= 1;
        desce(0);
    }
    return menor;
}

IP Rede::buscarIP(char *ip){
    for(int i = 0; i < tam; i++){
        if(compararIp(ip, ips[i].ip) == 1){
            return ips[i];
        }
    }
    IP a;
    return a;
}

int Rede::pai(int i){
    return (i - 1) / 2;
}

int Rede::esquerdo(int i){
    return 2 * (i + 1) - 1;
}

int Rede::direito(int i){
    return 2 * (i + 1);
}

void Rede::troca(int i, int j){
    IP aux = ips[i];
    ips[i] = ips[j];
    ips[j] = aux;
}

void Rede::sobe(int i){
    while(ips[pai(i)].prior > ips[i].prior){
        troca(i, pai(i));
        i = pai(i);
    }
}

void Rede::desce(int i){
    int e, d, maior;
    e = esquerdo(i);
    d = direito(i);

    if(e < tam && ips[e].prior < ips[i].prior){
        maior = e;
    }else{
        maior = i;
    }

    if(d < tam && ips[d].prior < ips[maior].prior){
        maior = d;
    }

    if(maior != i){
        troca(i, maior);
        desce(maior);
    }
}

void Rede::diminui_prioridade(IP j, int c){
    int i = 0;
    for(i = 0; i < tam; i++){
        if(compararIp(ips[i].ip, j.ip) == 1){
            break;
        }
    }
    if(c < ips[i].prior){
        ips[i].prior = c;
        desce(i);
    }
}