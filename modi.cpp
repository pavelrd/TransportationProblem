#include "modi.h"

#define DEBUG

/**
 *
 * @brief MODI::loop_it
 *
 * @param curr
 * @param dir
 *
 * @return
 *
 */

int MODI::loop_it(node* curr, int dir)
{
#ifdef DEBUG
    printf("call loop_it()\n");
#endif
    if(curr==NULL || curr->value==flag) return 0;
    else if(curr->value==flag+1) {
        flag++;
        return 1;
    }
    else {
        curr->value=flag;
        if(dir==0) {
            if(loop_it(curr->bottom,0)+loop_it(curr->left,3)+loop_it(curr->right,2)) {
                curr->value=flag;
                loop.insert(loop.begin(),curr);
                return 1;
            }
            return 0;
        }
        else if(dir==1) {
            if(loop_it(curr->top,1)+loop_it(curr->left,3)+loop_it(curr->right,2)) {
                curr->value=flag;
                loop.insert(loop.begin(),curr);
                return 1;
            }
            return 0;
        }
        else if(dir==2) {
            if(loop_it(curr->top,1)+loop_it(curr->bottom,0)+loop_it(curr->right,2)) {
                curr->value=flag;
                loop.insert(loop.begin(),curr);
                return 1;
            }
            return 0;
        }
        else {
            if(loop_it(curr->top,1)+loop_it(curr->bottom,0)+loop_it(curr->left,3)) {
                curr->value=flag;
                loop.insert(loop.begin(),curr);
                return 1;
            }
            return 0;
        }
    }
}


/**
 *
 * @brief MODI::loop_form
 *
 * @param b
 * @param nd
 * @param i
 * @param j
 * @param yes
 *
 * @return
 *
 */

int MODI::loop_form(mystringmatrix &b,vector<vector<node*>>nd, int i, int j, int yes)
{
#ifdef DEBUG
    printf("call loop_form()\n");
#endif
    flag++;
    loop.clear();
    node* curr=new node(b[i][j],i,j);
    node* curr1=NULL;
    node* curr2=NULL;
    node* curr3=NULL;
    node* curr4=NULL;
    int k1, k2, k3, k4;
    for(int l=0; l<i; l++) {
        if(nd[l][j]) {
            curr1=nd[l][j];
            k1=l;
        }
    }
    for(int l=0; l<j; l++) {
        if(nd[i][l]) {
            curr2=nd[i][l];
            k2=l;
        }
    }
    for(int l=b.size()-2; l>i; l--) {
        if(nd[l][j]) {
            curr3=nd[l][j];
            k3=l;
        }
    }
    for(int l=b[0].size()-1; l>j; l--) {
        if(nd[i][l]) {
            curr4=nd[i][l];
            k4=l;
        }
    }
    curr->left=curr2;
    if(curr2) curr2->right=curr;
    curr->top=curr1;
    if(curr1) curr1->bottom=curr;
    curr->right=curr4;
    if(curr4) curr4->left=curr;
    curr->bottom=curr3;
    if(curr3) curr3->top=curr;
    curr->value=flag+1;
    int ans=loop_it(curr->top,1)+loop_it(curr->bottom,0)+loop_it(curr->left,3)+loop_it(curr->right,2);
    if(yes) {
        if(curr1) curr1->bottom=curr3;
        if(curr3) curr3->top=curr1;
        if(curr2) curr2->right=curr4;
    }
    loop.insert(loop.begin(),curr);
    if(loop.size()!=4) {
        for(int i=2; i<loop.size(); i++) {
            if(loop[i]->i!=loop[i-1]->i && loop[i-1]->i!=loop[i-2]->i) {
                loop.erase(loop.begin()+i-1);
                i--;
            }
            if(loop[i]->j!=loop[i-1]->j && loop[i-1]->j!=loop[i-2]->j) {
                loop.erase(loop.begin()+i-1);
                i--;
            }
        }
        if(loop[0]->i!=loop[loop.size()-1]->i && loop[loop.size()-1]->i!=loop[loop.size()-2]->i) loop.erase(loop.begin()+loop.size()-1);
        if(loop[0]->j!=loop[loop.size()-1]->j && loop[loop.size()-1]->j!=loop[loop.size()-2]->j) loop.erase(loop.begin()+loop.size()-1);
    }
    return ans;
}

/**
 *
 * @brief MODI::unv
 *
 * @param u
 * @param v
 * @param nd
 * @param b
 * @param a
 *
 */

void MODI::unv(vector<float>&u, vector<float>&v, vector<vector<node*>>nd, mystringmatrix b, mymatrix a)
{
#ifdef DEBUG
    printf("call unv()\n");
#endif
    vector<int>w(v.size(),0);
    for(int j=0; j<b[0].size()-1; j++) {
        if(nd[0][j]) {
            v[j]=a[0][j];
            w[j]++;
        }
    }
    vector<int>next;
    for(int i=1; i<b.size()-1; i++) {
        int j=0;
        for(; j<b[i].size()-1; j++) {
            if(nd[i][j] && w[j]) {
                u[i]=a[i][j]-v[j];
                break;
            }
            else if(j==b[i].size()-2) next.push_back(i);
        }
        if(j!=b[i].size()-1) {
            for(int j=0; j<b[i].size()-1; j++) {
                if(nd[i][j] && w[j]==0) {
                    v[j]=a[i][j]-u[i];
                    w[j]++;
                }
            }
        }
    }
    while(next.size()) {
        for(int i=0; i<next.size(); i++) {
            int j=0;
            for(; j<b[next[i]].size()-1; j++) {
                if(nd[next[i]][j] && w[j]) {
                    u[next[i]]=a[next[i]][j]-v[j];
                    break;
                }
                else if(j==b[next[i]].size()-2) next.push_back(next[i]);
            }
            if(j!=b[next[i]].size()-1) {
                for(int j=0; j<b[next[i]].size()-1; j++) {
                    if(nd[next[i]][j] && w[j]==0) {
                        v[j]=a[next[i]][j]-u[next[i]];
                        w[j]++;
                    }
                }
                next.erase(next.begin()+i);
                i--;
            }
        }
    }
}

int MODI::choose_new(mystringmatrix &b,vector<vector<node*>>&nd)
{
#ifdef DEBUG
    printf("call choose_new()\n");
#endif
    for(int i=0; i<b.size(); i++) {
        for(int j=0; j<b[0].size(); j++) {
            if(nd[i][j]==NULL && !loop_form(b,nd,i,j,1)) {
                b[i][j].insert(b[i][j].begin(),'0');
                node* curr=new node(b[i][j],i,j);
                node* curr1=NULL;
                node* curr2=NULL;
                node* curr3=NULL;
                node* curr4=NULL;
                int k1, k2, k3, k4;
                for(int l=0; l<i; l++) {
                    if(nd[l][j]) {
                        curr1=nd[l][j];
                        k1=l;
                    }
                }
                for(int l=0; l<j; l++) {
                    if(nd[i][l]) {
                        curr2=nd[i][l];
                        k2=l;
                    }
                }
                for(int l=b.size()-2; l>i; l--) {
                    if(nd[l][j]) {
                        curr3=nd[l][j];
                        k3=l;
                    }
                }
                for(int l=b[0].size()-1; l>j; l--) {
                    if(nd[i][l]) {
                        curr4=nd[i][l];
                        k4=l;
                    }
                }
                curr->left=curr2;
                if(curr2) curr2->right=curr;
                curr->top=curr1;
                if(curr1) curr1->bottom=curr;
                curr->right=curr4;
                if(curr4) curr4->left=curr;
                curr->bottom=curr3;
                if(curr3) curr3->top=curr;
                nd[i][j]=curr;
                flag++;
                return 1;
            }
        }
    }
    return 0;
}

/**
 *
 * @brief MODI::optimality_check
 *
 * @param a
 * @param b
 * @param u
 * @param v
 * @param nd
 *
 * @return
 *
 */

float MODI::optimality_check( mymatrix a, mystringmatrix &b, vector<float>&u, vector<float>&v, vector<vector<node*>>&nd )
{
#ifdef DEBUG
    printf("call optimality_check()\n");
#endif
    int k=0;
    for(int i=0; i<b.size()-1; i++) {
        for(int j=0; j<b[i].size()-1; j++) {
            if(b[i][j][0]!='0' || b[i][j][1]!='*') {
                k++;
                node* curr=new node(b[i][j],i,j);
                for(int l=0; l<i; l++) {
                    node* curr1=nd[l][j];
                    if(curr1 && curr1->bottom==NULL) {
                        curr->top=curr1;
                        curr1->bottom=curr;
                        nd[l][j]=curr1;
                    }
                }
                for(int l=0; l<j; l++) {
                    node* curr1=nd[i][l];
                    if(curr1 && curr1->right==NULL) {
                        curr->left=curr1;
                        curr1->right=curr;
                        nd[i][l]=curr1;
                    }
                }
                nd[i][j]=curr;
            }
        }
    }
    if(b.size()+b[0].size()-3!=k) {
        for(int i=0; i<(b.size()+b[0].size()-3-k); i++) {
            if(!choose_new(b,nd)) return 2;
        }
    }
    unv(u,v,nd,b,a);
    for(int i=0; i<b.size()-1; i++) {
        for(int j=0; j<b[0].size()-1; j++) {
            if(nd[i][j]==NULL && (u[i]+v[j]-a[i][j])>0) return 0;
        }
    }
    return 1;
}

/**
 *
 * @brief MODI::calculate
 *
 * @param a
 * @param b
 * @param u
 * @param v
 * @param nd
 *
 * @return
 *
 */

float MODI::calculate(mymatrix a,mystringmatrix &b,vector<float>&u,vector<float>&v,vector<vector<node*>>&nd) {
#ifdef DEBUG
    printf("call calculate()\n");
#endif
    int max=0, maxi=-1, maxj=-1;
    for(int i=0; i<b.size()-1; i++) {
        for(int j=0; j<b[0].size()-1; j++) {
            if(nd[i][j]==NULL && (u[i]+v[j]-a[i][j])>max) {
                max=u[i]+v[j]-a[i][j];
                maxi=i;
                maxj=j;
            }
        }
    }
    if(max==0) {
        int z=0;
        for(int i=0; i<b.size()-1; i++) {
            for(int j=0; j<b[0].size()-1; j++) {
                int k=b[i][j].find("*");
                float k1;
                stringstream ss4;
                ss4 << b[i][j].substr(0,k+1);
                ss4 >> k1;
                z+=(k1*a[i][j]);
            }
        }
        return z;
    }
    if(loop_form(b,nd,maxi,maxj,0)) {
        int mini=0, minj=0;
        float min=10000000000;
        for(int i=1; i<loop.size(); i+=2) {
            string s1=loop[i]->a;
            int j=s1.find("*");
            float k;
            stringstream ss1;
            ss1 << s1.substr(0,j+1);
            ss1 >> k;
            if(min>k) {
                min=k;
                mini=loop[i]->i;
                minj=loop[i]->j;
            }
        }
        for(int i=0; i<loop.size(); i++) {
            stringstream ss2;
            string s1=loop[i]->a, s2;
            int j=s1.find("*");
            float k;
            stringstream ss1;
            ss2 << s1.substr(0,j+1);
            ss2 >> k;
            if(i%2) k-=min;
            else k+=min;
            stringstream ss3;
            ss3 << k;
            ss3 >> s2;
            s2+=s1.substr(j);
            loop[i]->a=s2;
            b[loop[i]->i][loop[i]->j]=s2;
        }
        nd[maxi][maxj]=loop[0];
        if(nd[mini][minj]->left) nd[mini][minj]->left->right=nd[mini][minj]->right;
        if(nd[mini][minj]->right) nd[mini][minj]->right->left=nd[mini][minj]->left;
        if(nd[mini][minj]->top) nd[mini][minj]->top->bottom=nd[mini][minj]->bottom;
        if(nd[mini][minj]->bottom) nd[mini][minj]->bottom->top=nd[mini][minj]->top;
        nd[mini][minj]=NULL;
        unv(u,v,nd,b,a);
        for(int i=0, j=0; i<b.size()-1; i++) {
            for(j=0; j<b[0].size()-1; j++) {
                if(nd[i][j]==NULL && (u[i]+v[j]-a[i][j])>0) {
                    return calculate(a,b,u,v,nd);
                    break;
                }
            }
            if(j!=b[0].size()-1) break;
        }
        int z=0;
        for(int i=0; i<b.size()-1; i++) {
            for(int j=0; j<b[0].size()-1; j++) {
                int k=b[i][j].find("*");
                float k1;
                stringstream ss4;
                ss4 << b[i][j].substr(0,k+1);
                ss4 >> k1;
                z+=(k1*a[i][j]);
            }
        }
        return z;
    }
    return -1;
}



/**
 *
 * @brief print_matrix
 * @param matrix
 *
 */

void MODI::print_matrix( mymatrix matrix)
{

    cout << "Matrix \r\n";

    for( auto item : matrix )
    {
        for( auto subitem : item )
        {
            cout << setw(4) << subitem << " ";
        }
        cout << "\r\n";
    }

}

/**
 *
 * @brief print_string_matrix
 *
 * @param b
 *
 */

void MODI::print_string_matrix( mystringmatrix b )
{

    cout << "In string mode: " << endl << endl;

    for( auto item : b )
    {
        for( auto subitem : item )
        {
            cout << setw(4) << subitem << " ";
        }
        cout << endl;
    }

    cout << endl;

}

QString MODI::calc( mymatrix inputMatrix, mymatrix resultMatrix )
{

    QString result;

    printf("INPUT MATRIX\n");

 //   mymatrix fixed_matrix_4x4 = {
 //                                 {  5,  3,  1, 10 },
 //                                 {  3,  2,  4, 20 },
 //                                 {  4,  1,  2, 30 },
 //                                 { 15, 20, 25,  0 }
 //                               };

    // inputMatrix = fixed_matrix_4x4;

    print_matrix(inputMatrix);

    printf("RESULT MATRIX\r\n");

    print_matrix(resultMatrix);

    // vector<string>b1(inputMatrix[0].size(),"");

    mystringmatrix b; // (inputMatrix.size(),b1);

    // int z = nwcr( inputMatrix, b ); // vam / nwcr
    //     cout << "NWCR SOLUTION : " << endl;
    //     print_solution(b);

// 10*5  0*3  0*1   10
//  5*3 15*2  0*4   20
//  0*4  5*1 25*2   30
//  15   20   25    0

    for( int i = 0 ; i < inputMatrix.size(); i++ )
    {
        vector<string> b1;
        for( int j = 0 ;  j < inputMatrix[0].size(); j++ )
        {

            string mystr;

            if( ( j == (inputMatrix.size()-1) ) || (  i == (inputMatrix.size()-1) ) )
            {

                mystr = std::to_string((int)inputMatrix[i][j]);

            }
            else
            {
                 mystr = std::to_string((int)resultMatrix[i][j]) + string("*") + std::to_string((int)inputMatrix[i][j]);
            }

            b1.push_back( mystr );

        }

        b.push_back(b1);

    }

    // print_string_matrix(b);


    vector<float>u(b.size()-1,0);
    vector<float>v(b[0].size()-1,0);

    node* curr=NULL;

    vector<node*>nd1(b[0].size(),curr);
    vector<vector<node*>>nd(b.size(),nd1);

    int chec=optimality_check(inputMatrix,b,u,v,nd);

    result += "Решение неоптимальное, оптимизация...\n\n";

    printf("U vectors: ");

    for( auto item : u ) { printf( "%f\n", item); }

            printf("V vectors: ");

    for( auto item : v ) { printf( "%f\n", item); }

    print_string_matrix(b);

    int z = calculate( inputMatrix, b, u, v, nd );

    result += "Оптимизация выполнена, результат: \n\n";

    for( auto item : b )
    {
        for( auto subitem : item )
        {
            result += QString("%1").arg( QString::fromUtf8(subitem.c_str()), 7 );
            result += " ";
        }
        result += "\n";
    }

    // print_solution(b);

    cout << "Z=" << z << endl << endl;

    result += QString("\nСтоимость доставки продукции: ") + QString::number(z) + QString(" ден. ед\n");

    return result;

}


    // http://reshmat.ru/transport.html?c11=5&c12=3&c13=1&z1=10&c21=3&c22=2&c23=4&z2=20&c31=4&c32=1&c33=2&z3=30&b1=15&b2=20&b3=25&method=SZCorner&step=2&sizeA=3&sizeB=3#b

    // http://reshmat.ru/transport.html?c11=5&c12=3&c13=1&c14=1&z1=10&c21=3&c22=2&c23=4&c24=4&z2=20&c31=4&c32=1&c33=2&c34=2&z3=30&c41=4&c42=1&c43=2&c44=2&z4=10&b1=15&b2=20&b3=25&b4=10&method=SZCorner&step=2&sizeA=4&sizeB=4#b
