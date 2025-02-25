#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE  
#define _CRT_NONSTDC_NO_DEPRECATE

#include <iostream>
#include <vector>
#include <queue>

//constexpr int N_ROWSin = 0; // numero di righe ottenuto con wc
//constexpr int N_ROWSout = 0; // numero di righe ottenuto con wc
//constexpr int N_ROWS = 1053211; // numero di righe ottenuto con wc
constexpr long long N_TX = 10572826; //numero di transazione totali

constexpr long long N_ROWS = 10532115; // numero di righe ottenuto con wc
constexpr long long N_ROWSin = 21378770; // numero di righe ottenuto con wc
constexpr long long N_ROWSout = 24573071; // numero di righe ottenuto con wc
constexpr long long N_ROWSconn = 21378770; // numero di righe ottenuto con wc


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



void myDFS(int s, int raggiunto[]) {
    
        raggiunto[s] = 1;
        for (auto x : adj[s].vicini) {
            auto v = x.first;
            if (raggiunto[index[v]] == 0) {
                myDFS(v, raggiunto);
        }
    }
}
void myDFS2(long long s, int raggiunto[]) {

    vector<long long> P;

    P.push_back(s);
    while (!P.empty()) {
        int u = P.back();
        P.pop_back();
        if (raggiunto[u] == 0) {
            raggiunto[u] = 1;
            for (auto x : adj[u].vicini) {
                long long v = x.first;
                P.push_back(v);
            }
        }
    }

}

arco* cerca(long long radice, long long pos, vector<arco>& vet, long long sx, long long dx) {

    if (sx == dx) {
        return &vet[sx];
    }

    long long m = (sx + dx) / 2;
    if (radice == vet[m].partenza) {
        if (pos <= vet[m].posizione) {


            return cerca(radice, pos, vet, sx, m);


        }
        else {
            return cerca(radice, pos, vet, m + 1, dx);
        }

    }
    else if (radice < vet[m].partenza) {


        return cerca(radice, pos, vet, sx, m);


    }
    else {
        return cerca(radice, pos, vet, m + 1, dx);
    }



}
void crea_grafo_completo() {
    freopen("transactions.txt", "r", stdin);
    for (long long i = 1; i <= N_ROWS; i++) {
        long long timestamp, blockId, txId, isCoinbase, fee;
        cin >> timestamp >> blockId >> txId >> isCoinbase >> fee;
        nodo mynodo;
        mynodo.timestamp_nodo = timestamp;
        mynodo.txId_nodo = txId;
        mynodo.blockId_nodo = blockId;
        mynodo.isCoinbase_nodo = isCoinbase;
        mynodo.fee_nodo = fee;


        adj.push_back(mynodo); // anche qui sfrutto l'ordine del file per txId
        index[txId] = i - 1; //salvo l'indice nella lista di adiacenza del txId, così da avere una immediata corrispondenza tra txId e nodo
        if (N_ROWS - i < 32000) {
            cout << "Trans:" << N_ROWS - i << "\n";
        } //stampa per il debug


    }

    freopen("connesso.txt", "r", stdin);
    for (long long i = 1; i <= N_ROWSconn; i++) {
        long long txId, posizione, destinazione, valore;
        cin >> txId >> posizione >> destinazione >> valore;

        adj[index[txId]].vicini.push_back({ index[destinazione],valore }); //arco vero
        adj[index[destinazione]].vicini.push_back({ index[txId], -1 }); //arco finto marcato con -1 per trovare la componente connessa ignorando la direzione degli archi
        if (i % 1000000 == 0) {
            cout << "Conn:" << N_ROWSconn - i << "\n";
        } //stampa per debug

    }
}
void crea_archi() {




    freopen("outputs.txt", "r", stdin);
    for (long long i = 1; i <= N_ROWSout; i++) {
        long long txId, txPos, addressId, amount, scriptType;
        cin >> txId >> txPos >> addressId >> amount >> scriptType;
        arco myarco;
        myarco.partenza = txId;
        myarco.posizione = txPos;
        myarco.valore = amount;
        myarco.arrivo = -1;
        arclis.push_back(myarco);
        if (i % 1000000 == 0) {
            cout << "Output:" << N_ROWSout - i << "\n";
        }


    }
    // arclis è ordinato per txId perché ho verificato che outputs.txt è ordinato per txId e txPos quindi
    //posso fare ricerca binaria
    freopen("inputs.txt", "r", stdin);
    for (long long i = 1; i <= N_ROWSin; i++) {

        long long txId, prevTxId, prevTxPos;
        cin >> txId >> prevTxId >> prevTxPos;
        arco* u = cerca(prevTxId, prevTxPos, arclis, 0, arclis.size() - 1);
        u->arrivo = txId;

        if (i % 1000000 == 0) {
            cout << "Intput:" << N_ROWSin - i << "\n";
        }

    }
}
long long somma(int a[], long long n) {
    long long sum = 0;
    for(long long i=0;i<n;i++){
        sum += a[i];
    }
    return sum;
}
void scrivimi() {
    freopen("connesso.txt", "w", stdout);
    int  m = arclis.size();
    for (int i = 0; i < m; i++) {
        if (arclis[i].arrivo != -1) {
            cout << arclis[i].partenza << ' ' << arclis[i].posizione << ' ' << arclis[i].arrivo << ' ' << arclis[i].valore << "\n";
        }
    }

}
long long componenti( ){
    int raggiunti[N_ROWS];
    long long mass = 0;
    long long indice,maxindex = 0;
    for (long long i = 0; i < N_ROWS; i++) {
        raggiunti[i] = 0;
    }
    for (long long i = 0; i < N_ROWS; i++) {
        if (raggiunti[i]==0) {
            indice = i;
            
            myDFS2(i, raggiunti);
        }
        long long s = somma(raggiunti, N_ROWS);
        if (s > mass) {
            mass = s;
            maxindex = indice;

        }


    }
    return maxindex;
}

void elimina(int raggiunto[]) {
    long long count = 0;
    for (long long i = 0; i < N_ROWS; i++) {
        if (raggiunto[i] == 0) {
            adj.erase(adj.begin() + i - count);
            count++;
        }
    }

}
void scrivimi2() {
    freopen("connesso2.txt", "w", stdout);

    for (auto x : adj) {

        cout << x.txId_nodo << ' ';
        for (auto y : x.vicini) {
            cout << y.first << ' ' << y.second << ' ';
        }
        cout << "\n";

    }

}
//nt main() {


    //crea_archi();
    //scrivimi(); funzioni usate nella prima parte. Con wc ho trovato il numero di righe di connesso.txt che e N_ROWSconn magari fai una funzione count
   // crea_grafo_completo();
   // int raggiunto[N_ROWS];
   // for (long long i = 0; i < N_ROWS; i++) {
    //    raggiunto[i] = 0;
   // }
    //cout<<componenti(adj); questa funzione mi stampa il nodo ''inziale'' della cc più grande
   // myDFS2(0, raggiunto); //sfrutto che so che la cc più grande contiene il nodo 0
    //elimina(raggiunto); elimina dal vector di adiacenza i nodi non raggiunti, ma non l'ho usata alla fine
    //mi elimini tutte gli j-esimi righe di connesso.txt dove j è tale che raggiunto[j]=0
   // freopen("connesso.txt", "r", stdin);
   // freopen("connesso2.txt", "w", stdout);
   // for (int i = 1; i <= N_ROWSconn; i++) {
       // long long txId, posizione, destinazione, valore;
       // cin >> txId >> posizione >> destinazione >> valore;
       // if (raggiunto[index[txId]] == 1) {
        //    cout << txId << ' ' << posizione << ' ' << destinazione << ' ' << valore << "\n";
       // }

   // }




    return 0;
}

