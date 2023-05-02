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

//Declaração da função compararIP 
int compararIp(char *ip1, char *ip2);

//Declaração da classe No e seus construtores 
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

//Declaração da classe ListaLigada e seu construtor
//Declaração das funções apagaLista, InsereNo e printarLista
class ListaLigada{
public:
    No* inicio;

    ListaLigada(){
        inicio = nullptr;
    }


    void insereNo(char* ip, int custo);
    void printarLista();
    void apagarLista();

};

//A função lista ligada recebe um IP do tipo char e um custo do tipo inteiro, cria um
// novo No e caso a lista esteja vazia, ele será o primeiro elemento, caso haja outros IPs ele será 
// posto em ultimo
void ListaLigada::insereNo(char* ip, int custo){
    No* novoNo = new No(ip, custo);

    if(inicio == nullptr){
        inicio = novoNo;
        return;
    }

    //Percorre até o final da lista ligada e insere
    No* p = inicio;
    while(p->prox != nullptr){
        p = p->prox;
    }

    p->prox = novoNo;
}

//A função abaixo imprime todos os IPs e seus custos, 
// ou imprime que a lista está vazia, caso esteja
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

void ListaLigada::apagarLista(){
    printarLista();
    No* atual = inicio;
    while (atual != nullptr) {
        cout << atual->ip << endl;
        No* proximo = atual->prox;
        free(atual);
        atual = proximo;
    }
}

//Definição da classe IP e seus construtores, onde o IP receberá o ip propriamente dito, uma prioridade e uma lista ligada
class IP{
public:
    char* ip;
    int prior;
    ListaLigada viz;

    IP(){}

    IP(char* ip, int prior){
        this->ip = ip;
        this->prior = prior;
        this->viz = ListaLigada();
    }

};

//Na classe rede, há o vetor de IPs, que será o n dado na entrada, e no construtor será lido os IPs da rede e inseridos no vetor de IPs da classe IP.
//Também é feita a leitura dos IP's que serão conectados e seus custos, já os conectando
//Há declarações de funções do MinHeap
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

            for(int k = 0; k < n; k++){
                if(compararIp(ip2, ips[k].ip) == 1){
                    ips[k].viz.insereNo(ip1, custo);
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


//A função compararIP recebe dois IPs do tipo char e os compara
//retornando 0 de forem diferentes ou 1 se forem iguais 
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

//Construção do MinHeap pela Rede
void Rede::constroiMinHeap(){
    for(int i = tam/2-1; i >= 0; i--){
        desce(i);
    }
}

//
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
            R.constroiMinHeap();
        }

    }

    return custoTotal;
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


int main(){

    int n = 0;
    cin >> n;
    Rede r = Rede(n);

    cout << redeCustoMinimo(r) << endl;

}