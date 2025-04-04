#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <limits.h>

using namespace std;
int countW = 0;
vector<int> wt;  // Waiting Time
vector<int> tat; // Turnaround Time
vector<int> ct;  // Completion Time
vector<int> rt;  // Response Time

vector<vector<string>> roundRobin(vector<vector<string>> process, int time, int flag) // Round Robin

{
    int n = process.size();
    vector<bool> firstExecution(n, true);
    
    for (int i = 0; i < n; ++i) {
        if (stoi(process[i][3]) == flag && stoi(process[i][1]) != 0){ //condiciones para que se ejecute el proceso por medio del algoritmo
            if (firstExecution[i]) {
                rt[i] = countW - stoi(process[i][2]);
                firstExecution[i] = false;
            }
            wt[i] += countW; // Suma el tiempo de espera
            if (time <= stoi(process[i][1])){
                process[i][1] = to_string(stoi(process[i][1]) - time);  // Resta el tiempo de ejecucion
                countW = countW + time; // Suma el tiempo de espera
            } else {
                int lesst = min(stoi(process[i][1]), time); // Obtiene el menor valor entre el tiempo de ejecucion y el tiempo de quantum
                process[i][1] = to_string(stoi(process[i][1]) - lesst); 
                countW = countW + lesst;
            }
            if (process[i][1] == "0"){
                ct[i] = countW; // tiempo en el que se completa el proceso
                tat[i] = ct[i] - stoi(process[i][2]);// Calcula el tiempo de respuesta
            }
            process[i][3] = to_string(stoi(process[i][3]) + 1);   //el proceso cambia de cola en el mlf
        } 
    }

    return process;
}

//funcion first come first serve
vector<vector<string>> firstComeFirstServer(vector<vector<string>> process, int flag) {
    int n = process.size();
    vector<bool> firstExecution(n, true);
    
    for (int i = 0; i < n; ++i) {
        if (stoi(process[i][3]) == flag && stoi(process[i][1]) != 0) {
            if (firstExecution[i]) {
                rt[i] = countW - stoi(process[i][2]);
                firstExecution[i] = false;
            }
            wt[i] = wt[i] + countW;
            countW += stoi(process[i][1]); // Ejecuta todo el proceso sin interrupción
            ct[i] = countW;
            tat[i] = ct[i] - stoi(process[i][2]);
            process[i][1] = "0"; // Marca el proceso como completado
            process[i][3] = to_string(stoi(process[i][3]) + 1);  //el proceso cambia de cola en el mlf
        }
    }
    return process;
}

//funcion shorstestjobfirst
vector<vector<string>> shortestJobFirst(vector<vector<string>> process, int flag) {
    int n = process.size();
    vector<int> indices;
    vector<bool> firstExecution(n, true);

    // Filtrar los procesos que están en el nivel `flag` y aún no han terminado
    for (int i = 0; i < n; ++i) {
        if (stoi(process[i][3]) == flag && stoi(process[i][1]) != 0) {
            indices.push_back(i);
        }
    }

    // Ordenar los procesos filtrados por tiempo de ráfaga (SJF)
    sort(indices.begin(), indices.end(), [&](int a, int b) {
        return stoi(process[a][1]) < stoi(process[b][1]);
    });

    // Ejecutar los procesos en orden de menor a mayor tiempo de ráfaga
    for (int i : indices) {
        if (firstExecution[i]) {
            rt[i] = countW - stoi(process[i][2]);
            firstExecution[i] = false;
        }
        wt[i] = wt[i] + countW;
        countW += stoi(process[i][1]); // Ejecuta todo el proceso sin interrupción
        ct[i] = countW;
        tat[i] = ct[i] - stoi(process[i][2]);
        process[i][1] = "0"; // Marca el proceso como completado
        process[i][3] = to_string(stoi(process[i][3]) + 1);
    }

    return process;
}

vector<vector<string>> shortestTimeToCompletionFirst(vector<vector<string>> process, int flag) {
    int n = process.size();
    int currentTime = 0;
    vector<bool> firstExecution(n, true);

    while (true) {
        int shortestIndex = -1;
        int shortestTime = INT_MAX;

        // Buscar el proceso con el menor tiempo restante que esté en el nivel `flag`
        for (int i = 0; i < n; ++i) {
            if (stoi(process[i][3]) == flag && stoi(process[i][1]) != 0) {
                int remainingTime = stoi(process[i][1]);
                if (remainingTime < shortestTime) {
                    shortestTime = remainingTime;
                    shortestIndex = i;
                }
            }
        }

        // Si no hay más procesos en este nivel, salir del bucle
        if (shortestIndex == -1) break;

        if (firstExecution[shortestIndex]) {
            rt[shortestIndex] = currentTime - stoi(process[shortestIndex][2]);
            firstExecution[shortestIndex] = false;
        }
        // Ejecutar el proceso por 1 unidad de tiempo
        process[shortestIndex][1] = to_string(stoi(process[shortestIndex][1]) - 1);
        currentTime++;

        // Si el proceso terminó, actualizar tiempos
        if (process[shortestIndex][1] == "0") {
            ct[shortestIndex] = currentTime;
            tat[shortestIndex] = ct[shortestIndex] - stoi(process[shortestIndex][2]);
            wt[shortestIndex] = tat[shortestIndex] - stoi(process[shortestIndex][1]);
            process[shortestIndex][3] = to_string(stoi(process[shortestIndex][3]) + 1);
        }
    }

    return process;
}
int main(int argc, char *argv[])
{

    ifstream archivo("parcial.txt"); // Abre el archivo
    if (!archivo)
    {
        cerr << "Error al abrir el archivo.\n";
        return 1;
    }

    vector<vector<string>> lineas_elementos;
    string linea;

    while (getline(archivo, linea)) // Lee cada línea del archivo
    {
    vector<string> elementos;
        stringstream ss(linea);
        string elemento;

        while (getline(ss, elemento, ';')) 
        {  
            elemento.erase(0, elemento.find_first_not_of(" \t\r"));
            elemento.erase(elemento.find_last_not_of(" \t\r") + 1);

            if (!elemento.empty()) 
            {
                elementos.push_back(elemento);
            }
        }

        if (!elementos.empty()) 
        {
            lineas_elementos.push_back(elementos);
        }
    }

    archivo.close();

    sort(lineas_elementos.begin(), lineas_elementos.end(), [](const std::vector<std::string>& a, const std::vector<std::string>& b) {
        return a[2] < b[2];
    }); // Ordena las líneas por tiempo de llegada

    
    /*
    std::cout << "Vector ordenado:\n{\n";
    for (const auto& fila : lineas_elementos) {
        std::cout << "  { ";
        for (const auto& elem : fila) {
            std::cout << "'" << elem << "', ";
        }
        std::cout << "},\n";
    }
    std::cout << "}\n";
    */
    vector<string> esquema = {"RR(1)", "RR(3)", "SJF"};;
    wt.resize(lineas_elementos.size(), 0);
    tat.resize(lineas_elementos.size(), 0);
    ct.resize(lineas_elementos.size(), 0);
    rt.resize(lineas_elementos.size(), 0);

    vector<vector<string>> q1;
    vector<vector<string>> q2;
    vector<vector<string>> q3;
    //esquema 1 - roundRobin
    for (auto& v : lineas_elementos) {
        int id = stoi(v[3]);
        
        if (id == 1){ //dependiendo la cola en la que se encuentre el proceso se le asigna un esquema
            int flag = 1;
            
            if (esquema[0][0] == 'R'){
                int quantum =  stoi(esquema[0].substr(3));
                q1 = roundRobin(lineas_elementos, quantum, flag);
            }
            else if (esquema[0] == "FCFS"){
                q1 = firstComeFirstServer(lineas_elementos, flag);
            }
            else if (esquema[0] == "SJF"){
                q1 = shortestJobFirst(lineas_elementos, flag);
                
            }
            else if (esquema[0] == "STCF"){
                q1 = shortestTimeToCompletionFirst(lineas_elementos, flag);
            }
        }
        else if (id == 2 ){
            int flag = 2;
            if (esquema[1][0] == 'R'){
                int quantum =  stoi(esquema[1].substr(3));
                q2 = roundRobin(q1, quantum, flag);
            }
            else if (esquema[1] == "FCFS"){
                q2 = firstComeFirstServer(q1, flag);
            }
            else if (esquema[1] == "SJF"){
                q2 = shortestJobFirst(q1, flag);
            }
            else if (esquema[1] == "STCF"){
                q2 = shortestTimeToCompletionFirst(q1, flag);
            }
        }
        else if (id == 3){
            int flag = 3;
            if (esquema[2][0] == 'R'){
                int quantum =  stoi(esquema[2].substr(3));
                q3 = roundRobin(q2, quantum, flag);
            }
            else if (esquema[2] == "FCFS"){
                q3 = firstComeFirstServer(q2, flag);
            }
            else if (esquema[2] == "SJF"){
                q3 = shortestJobFirst(q2, flag);
            }
            else if (esquema[2] == "STCF"){
                q3 = shortestTimeToCompletionFirst(q2, flag);
            }
        }
    }
    // Imprimir resultados en el formato requerido
    cout << "# etiqueta; BT; AT; Q; Pr; WT; CT; RT; TAT\n";
    for (size_t i = 0; i < lineas_elementos.size(); i++) {
        cout << lineas_elementos[i][0] << "; "  // Etiqueta del proceso
             << lineas_elementos[i][1] << "; "  // BT
             << lineas_elementos[i][2] << "; "  // AT
             << lineas_elementos[i][3] << "; "  // Q
             << lineas_elementos[i][4] << "; "  // Pr
             << wt[i] << "; "                   // WT
             << ct[i] << "; "                   // CT
             << rt[i] << "; "                   // RT
             << tat[i] << "\n";                 // TAT
    }

    double sum_wt = 0, sum_ct = 0, sum_tat = 0, sum_rt = 0;

    for (int i = 0; i < lineas_elementos.size(); ++i) {
        sum_wt += wt[i];
        sum_ct += ct[i];
        sum_tat += tat[i];
        sum_rt += rt[i];
    }

    cout << "\nPromedio WT: " << sum_wt / lineas_elementos.size() << endl;
    cout << "Promedio CT: " << sum_ct / lineas_elementos.size() << endl;
    cout << "Promedio TAT: " << sum_tat / lineas_elementos.size() << endl;
    cout << "Promedio RT: " << sum_rt / lineas_elementos.size() << endl;
    return 0;
}