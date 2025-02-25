#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE  
#define _CRT_NONSTDC_NO_DEPRECATE

#include <iostream>
#include <vector>
#include <queue>

//constexpr int N_ROWSin = 0; // numero di righe ottenuto con wc
//constexpr int N_ROWSout = 0; // numero di righe ottenuto con wc
//constexpr int N_ROWS = 1053211; // numero di righe ottenuto con wc
constexpr int N_TX = 10572826; //numero di transazione totali

constexpr int N_ROWS= 10532115; // numero di righe ottenuto con wc
constexpr int N_ROWSin = 21378770; // numero di righe ottenuto con wc
constexpr int N_ROWSout = 24573071; // numero di righe ottenuto con wc


using namespace std;





struct nodo {
    long timestamp_nodo;
    long blockId_nodo;
    long txId_nodo;
    long isCoinbase_nodo;
    long fee_nodo;
    vector<pair<long long, long long>> vicini;



};

vector <nodo> adj;
int index[N_TX];


int N, M, infinito = 0;

void DFS(int u, int raggiunto[]) {
    raggiunto[u] = 1;
    
        for (auto x : adj[u].vicini) {
            int v=x.first;
                if (raggiunto[v] == 0) DFS(v,raggiunto); 
        }
    

}
int somma(int a[], int size) {
    int somma = 0;
    for (int i= 0; i < size; i++) {
        somma = somma + a[i];


    }
    return somma;
}

void myDFS(int s, int raggiunto[]) {
    
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
            }
        }
    }

}
void check() {
    int raggiunto[N_ROWS];
    for (long s = 0; s < N_ROWS; s++) {
        raggiunto[s] = 0;
    }
    int dimensione = 0, sumtot = 0;
    for (long s = 0; s < N_ROWS; s++) {
        if (raggiunto[s] == 0) {
            myDFS(s,raggiunto);
            dimensione = somma(raggiunto, N_ROWS) - sumtot;
            sumtot = sumtot + dimensione;
            cout << s << ' ' << "dim: " << dimensione << "\n";

        }
        
    }


}
void elimina() {
    int raggiunto[N_ROWS];
    for (long s = 0; s < N_ROWS; s++) {
        raggiunto[s] = 0;
    }
    
    for (long s = 0; s < N_ROWS; s++) {
        if ((raggiunto[s] == 0) && (s == 0)) {
            myDFS(s, raggiunto);
        }
        else if((raggiunto[s] == 0) && (s != 0)){
            cancella(s, raggiunto);
        }
        

    }



}
void cancella(int s, int raggiunto[]) {
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
            }
            adj.erase(adj.begin()+s);
        }
    }
}
void crea() {
    freopen("transactions.txt", "r", stdin);
    for (long i = 1; i <= N_ROWS; i++) {
        long long timestamp, blockId, txId, isCoinbase, fee;
        cin >> timestamp >> blockId >> txId >> isCoinbase >> fee;
        nodo mynodo;
        mynodo.timestamp_nodo = timestamp;
        mynodo.txId_nodo = txId;
        mynodo.blockId_nodo = blockId;
        mynodo.isCoinbase_nodo = isCoinbase;
        mynodo.fee_nodo = fee;

        
        adj.push_back(mynodo);
        index[txId] = i-1;

      
    }
     
    freopen("outputs.txt", "r", stdin);
    long long j = 0;
    for (long i = 1; i <= 0; i++) {
        
        
        long long txId, txPos, addressId, amount, scriptType;
        cin >> txId >> txPos >> addressId >> amount >> scriptType;
        
        while (adj[j].txId_nodo < txId) {
            j++;
            
        }
        
        if (adj[j].txId_nodo == txId) {
            
            adj[j].vicini.push_back({ 0, amount });

        }
    }
    
freopen("inputs.txt", "r", stdin);
    for (long i = 1; i <= N_ROWSin; i++) {
        
        long long txId, prevTxId, prevTxPos;
        cin >> txId >> prevTxId >> prevTxPos;
        adj[(index[prevTxId])].vicini.push_back({ index[txId],1 });
        adj[(index[txId])].vicini.push_back({ index[prevTxId],1 });
        
        //adj[(index[prevTxId])].vicini[prevTxPos].first = index[txId];

        

    }
}
int main() {
    int raggiunto2[N_ROWS];
    for (long s = 0; s < N_ROWS; s++) {
        raggiunto2[s] = 0;
    }
    
    crea();
    cout << "hello"<<"\n";
    //check();
    myDFS(0,raggiunto2);
    elimina();
    

    return 0;
}