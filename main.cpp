#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdio.h>
#include <string>
using namespace std;

int exact_algorithm(int **, int **, int, int);
int algorithm3(int **, int **, int, int);
int algorithm2(int **, int **, int, int);
int algorithm1(int **, int **, int, int, int);
int *prufercode_generate(int);
int **printTreeEdges(int[], int);

void printMatrix(int **matrix, int rows, int cols) {
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      cout << matrix[i][j] << " ";
    }
    cout << endl;
  }
}

void printGraphInfo(int **M, int **L, int numberofvertex, int numberofcolors,
                    int coloring_exist, int coloring_found_alg1,
                    int coloring_found_alg2, int coloring_found_alg3,
                    int graph_number) {
  cout << "GRAPH NUMBER: " << graph_number << endl;
  cout << "vertex number: " << numberofvertex << endl;
  cout << "colors number: " << numberofcolors << endl;
  cout << "coloring exist: " << coloring_exist << endl;
  cout << "coloring found alg1: " << coloring_found_alg1 << endl;
  cout << "coloring found alg2: " << coloring_found_alg2 << endl;
  // cout << "coloring found alg3: " << coloring_found_alg3 << endl;

  // cout << "Matrix M:" << endl;
  // printMatrix(M, numberofvertex, numberofvertex);

  // cout << "Matrix L:" << endl;
  // printMatrix(L, numberofvertex, numberofcolors);

  cout << endl;
}

void saveGraphAsEdgeList(int **AdjacencyMatrix, int numVertices,
                         const std::string &filename) {
  std::ofstream outputFile(filename);
  if (!outputFile.is_open()) {
    std::cerr << "Error opening file: " << filename << std::endl;
    return;
  }

  outputFile << "<?xml version='1.0' encoding='utf-8'?>" << std::endl;
  outputFile << "<graphml xmlns=\"http://graphml.graphdrawing.org/xmlns\" "
                "xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" "
                "xsi:schemaLocation=\"http://graphml.graphdrawing.org/xmlns "
                "http://graphml.graphdrawing.org/xmlns/1.0/graphml.xsd\">"
             << std::endl;
  outputFile << "  <graph edgedefault=\"undirected\">" << std::endl;

  // Writing nodes
  for (int i = 0; i < numVertices; ++i) {
    outputFile << "    <node id=\"" << i << "\" />" << std::endl;
  }

  // Writing edges
  for (int i = 0; i < numVertices; ++i) {
    for (int j = i + 1; j < numVertices; ++j) {
      if (AdjacencyMatrix[i][j] != 0) {
        outputFile << "    <edge source=\"" << i << "\" target=\"" << j
                   << "\" />" << std::endl;
      }
    }
  }

  outputFile << "  </graph>" << std::endl;
  outputFile << "</graphml>" << std::endl;

  outputFile.close();
}

void saveColoringInFile(int *Coloring, int vertex_colored, int numberofvertex,
                        string &str) {
  ofstream outfile(str);
  if (outfile.is_open()) {
    outfile << numberofvertex << "\n";
    for (int i = 0; i < vertex_colored; ++i) {
      outfile << Coloring[i] << " ";
    }
    outfile << "\n";
    outfile.close();
  } else {
    cerr << "Unable to open file";
  }
}

void saveColorLists(int **L, int numberofvertex, int numberofcolors,
                    string &str) {
  ofstream outfile(str);
  if (outfile.is_open()) {
    outfile << numberofvertex << " " << numberofcolors << "\n";
    for (int i = 0; i < numberofvertex; ++i) {
      for (int j = 0; j < numberofcolors; ++j) {
        outfile << L[i][j] << " ";
      }
      outfile << "\n";
    }
    outfile.close();
  } else {
    cerr << "Unable to open file";
  }
}

int main() {
  srand(time(0));
  unsigned int start_time = clock();

  const int r = 4, numberofgraphs = 10000;
  int numberofvertex, numberofcolors, x;
  int **M, **L, **M0, **M1, **M2, **M3, **L0, **L1, **L2, **L3;
  int *s, result[r] = {0};

  ofstream outfile("not_solved_graphs_by_alg1");

  ofstream outfile1("not_solved_graphs_by_alg2");

  if (!outfile || !outfile1) {
    std::cerr << "Unable to open file for writing" << std::endl;
    return 1;
  }

  for (int u = 0; u < numberofgraphs; u++) {

    int min = 10;
    int max = 20;
    int prufersize = rand() % (max - min + 1) + min;

    int numberofvertex = prufersize + 2;
    int *prufercode = prufercode_generate(prufersize);

    min = numberofvertex * 0.2;
    max = numberofvertex * 0.5;
    numberofcolors = rand() % (max - min + 1) + min;

    s = new int[numberofvertex];
    for (int i = 0; i < numberofvertex; i++) {
      s[i] = 0;
    }
    M = new int *[numberofvertex];
    L = new int *[numberofvertex];
    M0 = new int *[numberofvertex];
    L0 = new int *[numberofvertex];
    M1 = new int *[numberofvertex];
    L1 = new int *[numberofvertex];
    M2 = new int *[numberofvertex];
    L2 = new int *[numberofvertex];
    M3 = new int *[numberofvertex];
    L3 = new int *[numberofvertex];

    for (int i = 0; i < numberofvertex; i++) {
      M[i] = new int[numberofvertex];
      L[i] = new int[numberofcolors];
      L0[i] = new int[numberofcolors];
      M0[i] = new int[numberofvertex];
      M1[i] = new int[numberofvertex];
      L1[i] = new int[numberofcolors];
      M2[i] = new int[numberofvertex];
      L2[i] = new int[numberofcolors];
      M3[i] = new int[numberofvertex];
      L3[i] = new int[numberofcolors];
    }
    M = printTreeEdges(prufercode, prufersize);
    for (int i = 0; i < numberofvertex; i++)
      for (int j = 0; j < numberofcolors; j++)
        L[i][j] = rand() % 2;
    for (int i = 0; i < numberofvertex; i++) {
      for (int j = 0; j < numberofcolors; j++)
        s[i] += L[i][j];
      if (s[i] == 0) {
        x = rand() % numberofcolors;
        L[i][x] = 1;
      }
    }
    for (int i = 0; i < numberofvertex; i++)
      for (int j = 0; j < numberofvertex; j++) {
        M0[i][j] = M[i][j];
        M1[i][j] = M[i][j];
        M2[i][j] = M[i][j];
        M3[i][j] = M[i][j];
      }
    for (int i = 0; i < numberofvertex; i++)
      for (int j = 0; j < numberofcolors; j++) {
        L0[i][j] = L[i][j];
        L1[i][j] = L[i][j];
        L2[i][j] = L[i][j];
        L3[i][j] = L[i][j];
      }

    int coloring_exist, coloring_found_alg1, coloring_found_alg2,
        coloring_found_alg3 = 0;

    coloring_exist = exact_algorithm(M0, L0, numberofvertex, numberofcolors);
    coloring_found_alg1 = algorithm1(M1, L1, numberofvertex, numberofcolors, u);
    coloring_found_alg2 = algorithm2(M2, L2, numberofvertex, numberofcolors);
    coloring_found_alg3 = algorithm3(M3, L3, numberofvertex, numberofcolors);

    printGraphInfo(M, L, numberofvertex, numberofcolors, coloring_exist,
                   coloring_found_alg1, coloring_found_alg2,
                   coloring_found_alg3, u);

    string str = "./graphs/graphml";
    str = str + to_string(u);
    saveGraphAsEdgeList(M, numberofvertex, str);

    str = "./lists/list";
    str = str + to_string(u);
    saveColorLists(L, numberofvertex, numberofcolors, str);

    if (coloring_exist == 1 && coloring_found_alg1 == 0) {
      outfile << u << " ";
    }

    if (coloring_exist == 1 && coloring_found_alg2 == 0) {
      outfile1 << u << " ";
    }

    result[0] += coloring_exist;
    result[1] += coloring_found_alg1;
    result[2] += coloring_found_alg2;
    result[3] += coloring_found_alg3;

    for (int i = 0; i < numberofvertex; i++) {
      delete[] M[i];
      delete[] L[i];
      delete[] M0[i];
      delete[] L0[i];
      delete[] M1[i];
      delete[] L1[i];
      delete[] M2[i];
      delete[] L2[i];
      delete[] M3[i];
      delete[] L3[i];
    }
    delete[] s;
  }

  outfile.close();
  outfile1.close();

  cout << "\nNumber of randomly generated colored graphs from "
       << numberofgraphs << endl
       << "(Algorithm 3) by exhaustive search algorithm: " << result[0] << endl
       << "(Algorithm 1) by simplest heuristic algorithm: " << setprecision(3)
       << "" << result[1] << "  " << (float)result[1] / (float)result[0] * 100
       << " %" << endl
       << "(Algorithm 2) by modified heuristic algorithm: "
       << "" << result[2] << "  " << (float)result[2] / (float)result[0] * 100
       << " % " << endl
       << "(Algorithm 2.1) without condition for selecting vertices with "
          "minimum degree: "
       << "" << (float)result[3] / (float)result[0] * 100 << " %" << endl
       << endl;

  ofstream outfile2("results");
  outfile2 << numberofgraphs << " " << result[0] << " " << result[1] << " "
           << (float)result[1] / (float)result[0] * 100 << " " << result[2]
           << " " << (float)result[2] / (float)result[0] * 100;
  outfile2.close();

  unsigned int end_time = clock();
  unsigned int search_time = end_time - start_time;

  cout << "\nThe program ran for " << search_time / 1000.0 << " seconds"
       << endl;
  return 0;
}

int exact_algorithm(int **M, int **L, int numberofvertex, int numberofcolors) {
  int *l, *l1, **L1, **L2;
  int i = 0, a = 0, t = 0, p = 0, NofFC = 0;
  l = new int[numberofvertex];
  l1 = new int[numberofvertex];
  L1 = new int *[numberofvertex];
  L2 = new int *[numberofvertex];
  for (int j = 0; j < numberofvertex; j++) {
    L1[j] = new int[numberofcolors];
    L2[j] = new int[numberofcolors];
  }
  for (int u = 0; u < numberofvertex; u++) {
    l[u] = 0;
    l1[u] = 0;
    M[u][u] = 0;
    for (int j = 0; j < numberofcolors; j++) {
      L1[u][j] = 0;
      L2[u][j] = L[u][j];
    }
  }
  while (i < numberofvertex) {
    if (i == 0) {
      p = 0;
      if (a == 1) {
        for (int b = i + 1; b < numberofvertex; b++)
          for (int j = 0; j < numberofcolors; j++)
            L1[b][j] = 0;
        t = l1[i] - 1;
        L1[i][t] = 1;
        for (int j = 0; j < numberofvertex; j++)
          for (int u = 0; u < numberofcolors; u++)
            L2[j][u] = L[j][u] - L1[j][u];
        for (int j = 0; j < numberofcolors; j++)
          if (L2[i][j] == 0)
            p += 1;
        if (numberofcolors == p) {
          for (int i = 0; i < numberofvertex; i++) {
            delete[] L1[i];
            delete[] L2[i];
          }
          delete[] l;
          delete[] l1;
          return 0;
        }
      }
      a = 0;
      for (int j = 0; j < numberofcolors; j++)
        if (L2[i][j] == 1) {
          NofFC = j;
          break;
        }
      l[i] = NofFC + 1;
      l1[i] = 0;
      for (int j = i + 1; j < numberofvertex; j++)
        if (M[i][j] == 1)
          L1[j][NofFC] = 1;
      i++;
    } else {
      NofFC = -1;
      if (a == 1) {
        for (int b = i + 1; b < numberofvertex; b++)
          for (int j = 0; j < numberofcolors; j++)
            L1[b][j] = 0;
        for (int b = i + 1; b < numberofvertex; b++)
          for (int j = 0; j < i; j++)
            if (M[b][j] == 1)
              L1[b][l[j] - 1] = 1;
        t = l1[i] - 1;
        L1[i][t] = 1;
      }
      a = 0;
      for (int j = 0; j < numberofvertex; j++)
        for (int u = 0; u < numberofcolors; u++)
          L2[j][u] = L[j][u] - L1[j][u];
      for (int j = 0; j < numberofvertex; j++)
        for (int u = 0; u < numberofcolors; u++)
          if (L2[j][u] < 0)
            L2[j][u] = 0;
      for (int j = 0; j < numberofcolors; j++)
        if (L2[i][j] == 1) {
          NofFC = j;
          break;
        }
      if (NofFC >= 0) {
        l[i] = NofFC + 1;
        a = 0;
        l1[i] = 0;
        for (int j = i + 1; j < numberofvertex; j++)
          if (M[i][j] == 1)
            L1[j][NofFC] = 1;
        i++;
      } else {
        l1[i - 1] = l[i - 1];
        l[i] = 0;
        l1[i] = 0;
        i -= 1;
        a = 1;
      }
    }
  }
  for (int i = 0; i < numberofvertex; i++) {
    delete[] L1[i];
    delete[] L2[i];
  }
  delete[] l;
  delete[] l1;
  return 1;
}

int algorithm1(int **M, int **L, int numberofvertex, int numberofcolors,
               int graph_number) {
  string str = "./colorings/coloring";
  str = str + to_string(graph_number);

  int *l = new int[numberofvertex];
  int vertex_colored = 0;
  for (int i = 0; i < numberofvertex; i++) {
    l[i] = 0;
  }
  int q, p = 0;
  for (int i = 0; i < numberofvertex; i++) {
    vertex_colored++;
    for (int j = 0; j < numberofcolors; j++) {
      p += L[i][j];
    }
    if (p == 0) {
      saveColoringInFile(l, vertex_colored, numberofvertex, str);
      delete[] l;
      return 0;
    } else
      do {
        q = rand() % numberofcolors;
        if (L[i][q] == 1) {
          l[i] = q;
        }
      } while (L[i][q] != 1);
    for (int j = 0; j < numberofvertex; j++) {
      if (M[i][j] == 1 && j != i) {
        L[j][q] = 0;
      }
    }
    p = 0;
  }

  saveColoringInFile(l, vertex_colored, numberofvertex, str);

  delete[] l;
  return 1;
}

int algorithm2(int **M, int **L, int numberofvertex, int numberofcolors) {
  int *l = new int[numberofvertex];
  for (int i = 0; i < numberofvertex; i++)
    l[i] = 0;
  int minPofL = numberofcolors + 1, maxDeg = 0, minP = numberofvertex + 1,
      w = 0, t, t1, t2, x = 0, y, z = 0, s;
  int *PofL, *Deg, *P, *a, *b, *c;
  for (int i = 0; i < numberofvertex; i++) {
    PofL = new int[numberofvertex];
    Deg = new int[numberofvertex];
    P = new int[numberofcolors];
    a = new int[numberofvertex];
    b = new int[numberofvertex];
    c = new int[numberofcolors];
    for (int i = 0; i < numberofvertex; i++) {
      PofL[i] = 0;
      Deg[i] = 0;
      a[i] = 0;
      b[i] = 0;
    }
    for (int i = 0; i < numberofcolors; i++) {
      P[i] = 0;
      c[i] = 0;
    }
    for (int j = 0; j < numberofvertex; j++) {
      for (int u = 0; u < numberofcolors; u++) {
        PofL[j] = PofL[j] + L[j][u];
      }
    }
    for (int j = 0; j < numberofvertex; j++) {
      for (int m = 0; m < numberofvertex; m++)
        Deg[j] = Deg[j] + M[j][m];
      if (Deg[j] != 0)
        Deg[j] = Deg[j] - 1;
    }
    for (int u = 0; u < numberofcolors; u++)
      for (int j = 0; j < numberofvertex; j++)
        P[u] = P[u] + L[j][u];
    for (int j = 0; j < numberofvertex; j++)
      if (minPofL > PofL[j]) {
        if (PofL[j] != 0)
          minPofL = PofL[j];
        else if (M[j][j] != 0) {
          delete[] PofL;
          delete[] Deg;
          delete[] P;
          delete[] a;
          delete[] b;
          delete[] c;
          delete[] l;
          return 0;
        }
      }
    for (int j = 0; j < numberofvertex; j++)
      if (minPofL == PofL[j]) {
        a[w] = j;
        w++;
      }
    for (int j = 0; j < w; j++) {
      t = a[j];
      if (Deg[t] > maxDeg)
        maxDeg = Deg[t];
    }
    for (int j = 0; j < w; j++) {
      t = a[j];
      if (maxDeg == Deg[t]) {
        b[x] = t;
        x++;
      }
    }
    if (x > 1)
      y = rand() % x;
    else
      y = 0;
    t1 = b[y];
    for (int j = 0; j < numberofcolors; j++) {
      if (L[t1][j] == 1)
        if (minP > P[j] && P[j] != 0)
          minP = P[j];
    }
    for (int j = 0; j < numberofcolors; j++)
      if (L[t1][j] == 1)
        if (minP == P[j]) {
          c[z] = j;
          z++;
        }
    if (z > 1)
      s = rand() % z;
    else
      s = 0;
    t2 = c[s];
    l[t1] = t2 + 1;
    for (int j = 0; j < numberofvertex; j++)
      if (M[t1][j] == 1)
        L[j][t2] = 0;
    for (int j = 0; j < numberofcolors; j++)
      L[t1][j] = 0;
    for (int j = 0; j < numberofvertex; j++)
      if (M[t1][j] == 1) {
        M[t1][j] = 0;
        M[j][t1] = 0;
      }
    w = 0;
    minPofL = numberofcolors + 1;
    maxDeg = 0;
    minP = numberofvertex + 1;
    x = 0;
    z = 0;
    delete[] PofL;
    delete[] Deg;
    delete[] P;
    delete[] a;
    delete[] b;
    delete[] c;
  }
  delete[] l;
  return 1;
}

int algorithm3(int **M, int **L, int numberofvertex, int numberofcolors) {
  int minPofL = numberofcolors + 1, maxDeg = 0, minP = numberofvertex + 1,
      w = 0, t, t1, t2, x = 0, y, z = 0, s;
  int *PofL, *Deg, *P, *a, *b, *c;
  for (int i = 0; i < numberofvertex; i++) {
    PofL = new int[numberofvertex];
    Deg = new int[numberofvertex];
    P = new int[numberofcolors];
    a = new int[numberofvertex];
    b = new int[numberofvertex];
    c = new int[numberofcolors];
    for (int i = 0; i < numberofvertex; i++) {
      PofL[i] = 0;
      Deg[i] = 0;
      a[i] = 0;
      b[i] = 0;
    }
    for (int i = 0; i < numberofcolors; i++) {
      P[i] = 0;
      c[i] = 0;
    }
    for (int j = 0; j < numberofvertex; j++) {
      for (int u = 0; u < numberofcolors; u++) {
        PofL[j] = PofL[j] + L[j][u];
      }
    }
    for (int j = 0; j < numberofvertex; j++) {
      for (int m = 0; m < numberofvertex; m++)
        Deg[j] = Deg[j] + M[j][m];
      if (Deg[j] != 0)
        Deg[j] = Deg[j] - 1;
    }
    for (int u = 0; u < numberofcolors; u++)
      for (int j = 0; j < numberofvertex; j++)
        P[u] = P[u] + L[j][u];
    for (int j = 0; j < numberofvertex; j++)
      if (minPofL > PofL[j]) {
        if (PofL[j] != 0)
          minPofL = PofL[j];
        else if (M[j][j] != 0) {
          delete[] PofL;
          delete[] Deg;
          delete[] P;
          delete[] a;
          delete[] b;
          delete[] c;
          return 0;
        }
      }
    for (int j = 0; j < numberofvertex; j++) {
      a[w] = j;
      w++;
    }
    for (int j = 0; j < w; j++) {
      t = a[j];
      if (Deg[t] > maxDeg)
        maxDeg = Deg[t];
    }
    for (int j = 0; j < w; j++) {
      t = a[j];
      if (maxDeg == Deg[t]) {
        b[x] = t;
        x++;
      }
    }
    if (x > 1)
      y = rand() % x;
    else
      y = 0;
    t1 = b[y];
    for (int j = 0; j < numberofcolors; j++) {
      if (L[t1][j] == 1)
        if (minP > P[j] && P[j] != 0)
          minP = P[j];
    }
    for (int j = 0; j < numberofcolors; j++)
      if (L[t1][j] == 1)
        if (minP == P[j]) {
          c[z] = j;
          z++;
        }
    if (z > 1)
      s = rand() % z;
    else
      s = 0;
    t2 = c[s];
    for (int j = 0; j < numberofvertex; j++)
      if (M[t1][j] == 1)
        L[j][t2] = 0;
    for (int j = 0; j < numberofcolors; j++)
      L[t1][j] = 0;
    for (int j = 0; j < numberofvertex; j++)
      if (M[t1][j] == 1) {
        M[t1][j] = 0;
        M[j][t1] = 0;
      }
    w = 0;
    minPofL = numberofcolors + 1;
    maxDeg = 0;
    minP = numberofvertex + 1;
    x = 0;
    z = 0;
    delete[] PofL;
    delete[] Deg;
    delete[] P;
    delete[] a;
    delete[] b;
    delete[] c;
  }
  return 1;
}

int *prufercode_generate(int size) {
  int *prufer = new int[size];
  for (int i = 0; i < size; i++) {
    prufer[i] = (rand() % size) + 1;
  }
  return prufer;
}

int **printTreeEdges(int prufer[], int prufersize) {
  int vertices = prufersize + 2;
  int *vertex_set = new int[vertices];
  for (int i = 0; i < vertices; i++)
    vertex_set[i] = 0;
  for (int i = 0; i < vertices - 2; i++)
    vertex_set[prufer[i] - 1] += 1;
  int **M;
  M = new int *[vertices];
  for (int i = 0; i < vertices; i++) {
    M[i] = new int[vertices];
  }
  for (int i = 0; i < vertices; i++) {
    for (int j = 0; j < vertices; j++) {
      if (i == j) {
        M[i][j] = 1;
      } else if (i < j) {
        M[i][j] = 0;
      } else if (i > j) {
        M[i][j] = M[j][i];
      }
    }
  }
  int j = 0;
  for (int i = 0; i < vertices - 2; i++) {
    for (j = 0; j < vertices; j++) {
      if (vertex_set[j] == 0) {
        vertex_set[j] = -1;
        M[j][prufer[i] - 1] = 1;
        M[prufer[i] - 1][j] = 1;
        vertex_set[prufer[i] - 1]--;
        break;
      }
    }
  }
  j = 0;
  int p = 0;
  for (int i = 0; i < vertices; i++) {
    if (vertex_set[i] == 0 && j == 0) {
      j++;
      p = i;
    } else if (vertex_set[i] == 0 && j == 1) {
      M[p][i] = 1;
      M[i][p] = 1;
    }
  }
  for (int i = 0; i < vertices; i++) {
    for (int j = 0; j < vertices; j++) {
      if (i == j) {
        M[i][j] = 1;
      } else if (i < j) {
        if (M[i][j] == 0) {
          M[i][j] = rand() % 2;
        }
      } else if (i > j) {
        M[i][j] = M[j][i];
      }
    }
  }
  return M;
}
