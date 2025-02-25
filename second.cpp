#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE  
#define _CRT_NONSTDC_NO_DEPRECATE

#include <iostream>
#include <vector>
#include <queue>

constexpr long long N_TX = 10572826; //numero di transazione totali

constexpr long long N_ROWS = 10532115; // numero di righe ottenuto con wc
constexpr long long N_ROWSin = 21378770; // numero di righe ottenuto con wc
constexpr long long N_ROWSout = 24573071; // numero di righe ottenuto con wc
constexpr long long N_ROWSconn = 21378770; // numero di righe ottenuto con wc
constexpr long long N_ROWS2= 21373121; // numero di righe ottenuto con wc di connesso2.txt
constexpr long long N_ROWSnodi = 10525103; // numero di righe ottenuto con wc di nodi.txt


using namespace std;



struct arco {
    long long partenza;
    long long arrivo;
    long long valore;
    long long posizione;

};

struct nodo {
    long long timestamp_nodo;
    long long blockId_nodo;
    long long txId_nodo;
    long long isCoinbase_nodo;
    long long fee_nodo;
    vector<pair<long long, long long>> vicini;



};

vector <nodo> adj;
vector<arco> arclis;
vector<long long> index(N_TX + 1);

vector<vector<arco>> flusso;
int myDFS(long long s, int raggiunto[]) {

    vector<long long> P;
    long long padre[N_ROWSnodi];
   padre[s] = s;
   

   P.push_back(s);
   while (!P.empty()) {
       int u = P.back();
       P.pop_back();
       if (raggiunto[u] == 0) {
           raggiunto[u] = 1;
           for (auto x : adj[u].vicini) {
               long long v = x.first;
               P.push_back(v);
               if (raggiunto[v] == 0) {
                   padre[v] = u;
               }
           }
       }
   }
    for (long long i=0;i<N_ROWSnodi;i++) {
        for (auto y : adj[i].vicini) {
            auto ancestor=i;
            do{ 
                
                if (ancestor == y.first) { return 1; }
                ancestor = padre[ancestor];
            } while (ancestor!=s );
        }
    }

    return 0;

}

int myDFS_check(int s, int raggiunto[]) {

    vector<int> P;

    P.push_back(s);
    while (!P.empty()) {
        int u = P.back();
        P.pop_back();
        if (raggiunto[u] == 0) {

            raggiunto[u] = 1;
            for (auto x : adj[u].vicini) {

                int v = x.first;
                P.push_back(v);
                if (raggiunto[v] == 0) {
                    for (auto y : adj[v].vicini) {
                        if (y.first == u) {
                            return 1; //è ciclico
                        }
                    }

                }
            }
        }
    }
    return 0;

} 
// non l'ho più usata. Mi verifica che non esistono cicli "banali"


void costruisci() {

    freopen("nodisord.txt", "r", stdin);
    for (long long i = 1; i <= N_ROWSnodi; i++)
    {
        long long txId;
        cin >> txId;
        


        nodo mynodo;

        mynodo.txId_nodo = txId;



        adj.push_back(mynodo);
        index[txId] = i - 1;
        if (i % 1000000 == 0) {
            cout << "Nodi:" << N_ROWSnodi - i << "\n";
        }
    }

            
           
            

        

    freopen("connesso2.txt", "r", stdin);
    for (long long i = 1; i <= N_ROWS2; i++){
        long long txId, posizione, destinazione, valore;
        cin >> txId >> posizione >> destinazione >> valore;


        adj[index[txId]].vicini.push_back({ index[destinazione],valore });

        if (i % 1000000 == 0) {
            cout << "Archi:" << N_ROWSconn - i << "\n";
        }

    }


    
}

void nodi() {
    int raggiunto[N_TX+1];
    for (long long i = 0; i <= N_TX; i++) {
        raggiunto[i] = 0;
    }

    freopen("nodi.txt", "w", stdout);
    freopen("connesso2.txt", "r", stdin);
    for (long long i = 1; i <= N_ROWS2; i++) {
        long long txId, posizione, destinazione, valore;
        cin >> txId >> posizione >> destinazione >> valore;
        if (raggiunto[txId] == 0) {
            cout << txId << "\n";
            raggiunto[txId] = 1;
        }
        if (raggiunto[destinazione] == 0) {
            cout << destinazione << "\n";
            raggiunto[destinazione] = 1;
        }
        

    }
}
void newtrans() {
    
    freopen("nodisord.txt","r", stdin);
    freopen("newtransactions.txt", "r", stdin);
    for (long long i = 1; i <= N_ROWSnodi; i++) {
        long long txId;
        cin >> txId;
        


    }



}

void flusso_percorso(long long u, long long soglia) {
    vector<long long> pila;
    vector<long long> cammino;
    pila.push_back(u);
    long long somma = 0;
    long long ultimo = 0;
    while (!pila.empty()) {
        long long v = pila.back();
        pila.pop_back();
        cammino.push_back(v); 

        if (!adj[v].vicini.empty()) {
            for (auto x : adj[v].vicini) {
                int y = x.first;
                pila.push_back(y);
            }
            somma = somma + adj[v].vicini.back().second;
            ultimo = adj[v].vicini.back().second;
        }
        else { //è un DAG, quindi prima o poi c'è un nodo senza vicini
            if (somma >= soglia) {

                cout << "Cammino: ";
                for (auto x : cammino) {
                    cout << x << ' '; 



                }
                cout << "somma: " << somma << "\n";
                somma = somma - ultimo;;

            }
            else {
                cout << "NotEn"<<"\n";
            }
            cammino.pop_back();
        }





    }
}


int main() {
   // nodi(); //fai un file con ogni riga il numero della transaione nel grafo e poi lo ordino con la bash di linux;
    
    costruisci();
   // int raggiunto[N_ROWS];
  // for (long long i = 0; i < N_ROWS; i++) {
    //   raggiunto[i] = 0;
  //  }
   // if (myDFS(0, raggiunto) == 1) {
   //    cout << "Il grafo è ciclico";


   // }
 // else cout << "Il grafo è ACICLICO";
    // ho verificato che il grafo è aciclico, quindi non c'è bisogno di levare gli archi dei cicli

    flusso_percorso(0, 33131000000);

	return 0;
}