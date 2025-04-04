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

vector<vector<string>> roundRobin(vector<vector<string>> process, int time, int flag)
{
    int n = process.size();
    vector<bool> firstExecution(n, true);
    vector<int> indices;

    for (int i = 0; i < n; ++i)
    {
        if (stoi(process[i][3]) == flag && stoi(process[i][1]) != 0)
            indices.push_back(i);
    }

    // Ordenar por prioridad
    sort(indices.begin(), indices.end(), [&](int a, int b)
         { return stoi(process[a][4]) < stoi(process[b][4]); });

    for (int i : indices)
    {
        if (firstExecution[i])
        {
            rt[i] = countW - stoi(process[i][2]);
            firstExecution[i] = false;
        }

        wt[i] += countW;
        int remaining = stoi(process[i][1]);

        if (time <= remaining)
        {
            process[i][1] = to_string(remaining - time);
            countW += time;
        }
        else
        {
            int lesst = min(remaining, time);
            process[i][1] = to_string(remaining - lesst);
            countW += lesst;
        }

        if (process[i][1] == "0")
        {
            ct[i] = countW;
            tat[i] = ct[i] - stoi(process[i][2]);
        }

        process[i][3] = to_string(stoi(process[i][3]) + 1);
    }

    return process;
}

vector<vector<string>> firstComeFirstServer(vector<vector<string>> process, int flag)
{
    int n = process.size();
    vector<bool> firstExecution(n, true);
    vector<int> indices;

    for (int i = 0; i < n; ++i)
    {
        if (stoi(process[i][3]) == flag && stoi(process[i][1]) != 0)
            indices.push_back(i);
    }

    sort(indices.begin(), indices.end(), [&](int a, int b)
         {
        int at_a = stoi(process[a][2]);
        int at_b = stoi(process[b][2]);
        if (at_a == at_b) {
            return stoi(process[a][4]) < stoi(process[b][4]);
        }
        return at_a < at_b; });

    for (int i : indices)
    {
        if (firstExecution[i])
        {
            rt[i] = countW - stoi(process[i][2]);
            firstExecution[i] = false;
        }

        wt[i] += countW;
        countW += stoi(process[i][1]);
        ct[i] = countW;
        tat[i] = ct[i] - stoi(process[i][2]);
        process[i][1] = "0";
        process[i][3] = to_string(stoi(process[i][3]) + 1);
    }

    return process;
}

vector<vector<string>> shortestJobFirst(vector<vector<string>> process, int flag)
{
    int n = process.size();
    vector<int> indices;
    vector<bool> firstExecution(n, true);

    for (int i = 0; i < n; ++i)
    {
        if (stoi(process[i][3]) == flag && stoi(process[i][1]) != 0)
            indices.push_back(i);
    }

    sort(indices.begin(), indices.end(), [&](int a, int b)
         {
        int bt_a = stoi(process[a][1]);
        int bt_b = stoi(process[b][1]);
        if (bt_a == bt_b) {
            return stoi(process[a][4]) < stoi(process[b][4]);
        }
        return bt_a < bt_b; });

    for (int i : indices)
    {
        if (firstExecution[i])
        {
            rt[i] = countW - stoi(process[i][2]);
            firstExecution[i] = false;
        }

        wt[i] += countW;
        countW += stoi(process[i][1]);
        ct[i] = countW;
        tat[i] = ct[i] - stoi(process[i][2]);
        process[i][1] = "0";
        process[i][3] = to_string(stoi(process[i][3]) + 1);
    }

    return process;
}

vector<vector<string>> shortestTimeToCompletionFirst(vector<vector<string>> process, int flag)
{
    int n = process.size();
    int currentTime = 0;
    vector<bool> firstExecution(n, true);

    while (true)
    {
        int shortestIndex = -1;
        int shortestTime = INT_MAX;

        for (int i = 0; i < n; ++i)
        {
            if (stoi(process[i][3]) == flag && stoi(process[i][1]) != 0)
            {
                int remainingTime = stoi(process[i][1]);
                if (remainingTime < shortestTime ||
                    (remainingTime == shortestTime && stoi(process[i][4]) < stoi(process[shortestIndex][4])))
                {
                    shortestTime = remainingTime;
                    shortestIndex = i;
                }
            }
        }

        if (shortestIndex == -1)
            break;

        if (firstExecution[shortestIndex])
        {
            rt[shortestIndex] = currentTime - stoi(process[shortestIndex][2]);
            firstExecution[shortestIndex] = false;
        }

        process[shortestIndex][1] = to_string(stoi(process[shortestIndex][1]) - 1);
        currentTime++;

        if (process[shortestIndex][1] == "0")
        {
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
    ifstream archivo("parcial.txt");
    if (!archivo)
    {
        cerr << "Error al abrir el archivo.\n";
        return 1;
    }

    vector<vector<string>> lineas_elementos;
    string linea;

    while (getline(archivo, linea))
    {
        vector<string> elementos;
        stringstream ss(linea);
        string elemento;

        while (getline(ss, elemento, ';'))
        {
            elemento.erase(0, elemento.find_first_not_of(" \t\r"));
            elemento.erase(elemento.find_last_not_of(" \t\r") + 1);
            if (!elemento.empty())
                elementos.push_back(elemento);
        }

        if (!elementos.empty())
            lineas_elementos.push_back(elementos);
    }

    archivo.close();

    sort(lineas_elementos.begin(), lineas_elementos.end(), [](const vector<string> &a, const vector<string> &b)
         { return a[2] < b[2]; });

    vector<string> esquema = {"RR(1)", "RR(3)", "SJF"};
    wt.resize(lineas_elementos.size(), 0);
    tat.resize(lineas_elementos.size(), 0);
    ct.resize(lineas_elementos.size(), 0);
    rt.resize(lineas_elementos.size(), 0);

    vector<vector<string>> q1, q2, q3;

    for (auto &v : lineas_elementos)
    {
        int id = stoi(v[3]);

        if (id == 1)
        {
            int flag = 1;
            if (esquema[0][0] == 'R')
            {
                int quantum = stoi(esquema[0].substr(3));
                q1 = roundRobin(lineas_elementos, quantum, flag);
            }
            else if (esquema[0] == "FCFS")
            {
                q1 = firstComeFirstServer(lineas_elementos, flag);
            }
            else if (esquema[0] == "SJF")
            {
                q1 = shortestJobFirst(lineas_elementos, flag);
            }
            else if (esquema[0] == "STCF")
            {
                q1 = shortestTimeToCompletionFirst(lineas_elementos, flag);
            }
        }
        else if (id == 2)
        {
            int flag = 2;
            if (esquema[1][0] == 'R')
            {
                int quantum = stoi(esquema[1].substr(3));
                q2 = roundRobin(q1, quantum, flag);
            }
            else if (esquema[1] == "FCFS")
            {
                q2 = firstComeFirstServer(q1, flag);
            }
            else if (esquema[1] == "SJF")
            {
                q2 = shortestJobFirst(q1, flag);
            }
            else if (esquema[1] == "STCF")
            {
                q2 = shortestTimeToCompletionFirst(q1, flag);
            }
        }
        else if (id == 3)
        {
            int flag = 3;
            if (esquema[2][0] == 'R')
            {
                int quantum = stoi(esquema[2].substr(3));
                q3 = roundRobin(q2, quantum, flag);
            }
            else if (esquema[2] == "FCFS")
            {
                q3 = firstComeFirstServer(q2, flag);
            }
            else if (esquema[2] == "SJF")
            {
                q3 = shortestJobFirst(q2, flag);
            }
            else if (esquema[2] == "STCF")
            {
                q3 = shortestTimeToCompletionFirst(q2, flag);
            }
        }
    }

    std::ofstream salida("salida.txt");

    // Validar que se abrió bien
    if (!salida.is_open())
    {
        cerr << "Error al abrir el archivo de salida.\n";
        return 1;
    }

    // Encabezado
    salida << "# etiqueta; BT; AT; Q; Pr; WT; CT; RT; TAT\n";

    // Datos de cada proceso
    for (size_t i = 0; i < lineas_elementos.size(); i++)
    {
        salida << lineas_elementos[i][0] << "; "
               << lineas_elementos[i][1] << "; "
               << lineas_elementos[i][2] << "; "
               << lineas_elementos[i][3] << "; "
               << lineas_elementos[i][4] << "; "
               << wt[i] << "; "
               << ct[i] << "; "
               << rt[i] << "; "
               << tat[i] << "\n";
    }

    // Cálculo de promedios
    double sum_wt = 0, sum_ct = 0, sum_tat = 0, sum_rt = 0;
    for (size_t i = 0; i < lineas_elementos.size(); ++i)
    {
        sum_wt += wt[i];
        sum_ct += ct[i];
        sum_tat += tat[i];
        sum_rt += rt[i];
    }

    // Escribir promedios
    salida << "\nPromedio WT: " << sum_wt / lineas_elementos.size() << endl;
    salida << "Promedio CT: " << sum_ct / lineas_elementos.size() << endl;
    salida << "Promedio TAT: " << sum_tat / lineas_elementos.size() << endl;
    salida << "Promedio RT: " << sum_rt / lineas_elementos.size() << endl;

    salida.close(); // Cerramos el archivo

    cout << "Resultado guardado en 'salida.txt' exitosamente.\n";
    return 0;
}