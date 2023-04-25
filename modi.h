#ifndef MODI_H
#define MODI_H


#include <vector>
#include <bits/stdc++.h>
#include <QString>

using namespace std;

typedef vector<vector<float>>  mymatrix;
typedef vector<vector<string>> mystringmatrix;

class MODI
{
private:

    struct node {
        string a;
        int value;
        int i;
        int j;
        node* left;
        node* right;
        node* top;
        node* bottom;
        node(string z, int i, int j) : a(z), value(0), i(i), j(j), left(NULL), right(NULL), top(NULL), bottom(NULL) {}
    };



public:

    float calculate(mymatrix a,mystringmatrix &b,vector<float>&u,vector<float>&v,vector<vector<node*>>&nd);

    // void print_solution(mystringmatrix b);

    float optimality_check( mymatrix a, mystringmatrix &b, vector<float>&u, vector<float>&v, vector<vector<node*>>&nd );

    bool  is_matrix_balanced( mymatrix matrix );

    QString calc(mymatrix inputMatrix , mymatrix resultMatrix);

private:

    int maximise;
    int flag=1;

    vector<node*> loop;

// Методы решения:

    float lcm(mymatrix a,mystringmatrix &b);
    float nwcr(mymatrix a,mystringmatrix &b);
    float vam( mymatrix a, mystringmatrix  &b );

//

    int   loop_it(node* curr, int dir);
    int   loop_form(mystringmatrix &b,vector<vector<node*>>nd, int i, int j, int yes);
    void  unv(vector<float>&u, vector<float>&v, vector<vector<node*>>nd, mystringmatrix b, mymatrix a);
    int   choose_new(mystringmatrix &b,vector<vector<node*>>&nd);
    void  print_matrix( mymatrix matrix);
    void  print_string_matrix( mystringmatrix b );
    mymatrix get_matrix_from_user_input();

};

#endif // MODI_H
