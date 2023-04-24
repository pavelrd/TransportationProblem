#include "modi.h"

#define DEBUG

mymatrix get_matrix_from_user_input();

/**
 *
 * @brief MODI::print_solution
 *
 * @param b
 *
 */

void MODI::print_solution(mystringmatrix b)
{
    int max=0;
    for(int i=0; i<b.size(); i++) {
        for(int j=0; j<b[i].size(); j++) {
            if(max<b[i][j].size()) max=b[i][j].size();
        }
    }
    if(max<6) max=6;
    string s1(max+2,'-');
    s1.push_back('+');
    string s4=s1;
    for(int i=0; i<b[0].size(); i++) s1+=s4;
    s1="+"+s1;
    cout << s1 << endl << "|";
    for(int i=0; i<=b[0].size(); i++) {
        string s3;
        if(i==0) {
            string s2(max,' ');
            cout << " " << s2 << " |";
        }
        else if(i==b[0].size()) {
            string s2(max-6,' ');
            cout << " " << "Supply" << s2 << " |";
        }
        else {
            string s3;
            stringstream ss1;
            ss1 << i;
            ss1 >> s3;
            s3="D"+s3;
            string s2(max-s3.size(),' ');
            cout << " " << s3 << s2 << " |";
        }
    }
    cout << endl << s1 << endl;
    for(int i=0; i<b.size(); i++) {
        cout << "|";
        for(int j=0; j<=b[0].size(); j++) {
            if(j==0 && i<b.size()-1) {
                string s3;
                stringstream ss2;
                ss2 << i+1;
                ss2 >> s3;
                s3="S"+s3;
                string s2(max-s3.size(),' ');
                cout << " " << s3 << s2 << " |";
            }
            else if(j==0) {
                string s2(max-6,' ');
                cout << " " << "Demand" << s2 << " |";
            }
            else {
                string s2(max-b[i][j-1].size(),' ');
                cout << " " << b[i][j-1] << s2 << " |";
            }
        }
        cout << endl << s1 << endl;
    }
}

/**
 *
 * @brief MODI::lcm
 *
 * @param a
 * @param b
 *
 * @return
 *
 */

float MODI::lcm(mymatrix a,mystringmatrix &b)
{
#ifdef DEBUG
    printf("call lcm()\n");
#endif
    vector<int>ki(b.size(),0);
    vector<int>kj(b[0].size(),0);
    float z=0;
    for(int i=0; i<a.size(); i++) {
        for(int j=0; j<a[i].size(); j++) {
            stringstream ss1;
            ss1 << a[i][j];
            ss1 >> b[i][j];
            if(i<a.size()-1 && j<a[i].size()-1) b[i][j]="0*"+b[i][j];
        }
    }
    while(a.size()!=1) {
        string s1;
        stringstream ss1;
        int mini=0, minj=0;
        float min=a[0][0];
        for(int i=0; i<a.size()-1; i++) {
            for(int j=0; j<a[i].size()-1; j++) {
                if(a[i][j]<min) {
                    min=a[i][j];
                    mini=i;
                    minj=j;
                }
            }
        }
        if(a[mini][a[mini].size()-1]<a[a.size()-1][minj]) {
            ss1 << a[mini][a[mini].size()-1];
            ss1 >> s1;
            z+=(a[mini][minj]*a[mini][a[mini].size()-1]);
            a[a.size()-1][minj]-=a[mini][a[mini].size()-1];
            a.erase(a.begin()+mini);
            for(int i=0; i<=mini; i++) {
                if(ki[i]) mini++;
            }
            for(int i=0; i<=minj; i++) {
                if(kj[i]) minj++;
            }
            ki[mini]=1;
            b[mini][minj]=s1+b[mini][minj].substr(1);
        }
        else if(a[mini][a[mini].size()-1]>a[a.size()-1][minj]) {
            ss1 << a[a.size()-1][minj];
            ss1 >> s1;
            z+=(a[mini][minj]*a[a.size()-1][minj]);
            a[mini][a[mini].size()-1]-=a[a.size()-1][minj];
            for(int i=0; i<a.size(); i++) a[i].erase(a[i].begin()+minj);
            for(int i=0; i<=mini; i++) {
                if(ki[i]) mini++;
            }
            for(int i=0; i<=minj; i++) {
                if(kj[i]) minj++;
            }
            kj[minj]=1;
            b[mini][minj]=s1+b[mini][minj].substr(1);
        }
        else {
            ss1 << a[a.size()-1][minj];
            ss1 >> s1;
            z+=(a[mini][minj]*a[a.size()-1][minj]);
            for(int i=0; i<a.size(); i++) a[i].erase(a[i].begin()+minj);
            a.erase(a.begin()+mini);
            for(int i=0; i<=mini; i++) {
                if(ki[i]) mini++;
            }
            for(int i=0; i<=minj; i++) {
                if(kj[i]) minj++;
            }
            ki[mini]=1;
            kj[minj]=1;
            b[mini][minj]=s1+b[mini][minj].substr(1);
        }
    }
    return z;
}

/**
 *
 * @brief nwcr NORTH WEST CORNOR SOLUTION
 *
 * @param a
 * @param b
 *
 * @return
 *
 */

float MODI::nwcr(mymatrix a,mystringmatrix &b)
{
#ifdef DEBUG
    printf("call nwcr()\n");
#endif
    int i=0, j=0;
    float z=0;
    for(int i=0; i<a.size(); i++) {
        for(int j=0; j<a[i].size(); j++) {
            stringstream ss1;
            ss1 << a[i][j];
            ss1 >> b[i][j];
            if(i<a.size()-1 && j<a[i].size()-1) b[i][j]="0*"+b[i][j];
        }
    }
    while(i!=(a.size()-1) || j!=(a[0].size()-1)) {
        string s1;
        stringstream ss1;
        if(a[i][a[i].size()-1]<a[a.size()-1][j]) {
            ss1 << a[i][a[i].size()-1];
            ss1 >> s1;
            b[i][j]=s1+b[i][j].substr(1);
            z+=(a[i][j]*a[i][a[i].size()-1]);
            a[a.size()-1][j]-=a[i][a[i].size()-1];
            a[i][a[i].size()-1]=0;
            i++;
        }
        else if(a[i][a[i].size()-1]>a[a.size()-1][j]){
            ss1 << a[a.size()-1][j];
            ss1 >> s1;
            b[i][j]=s1+b[i][j].substr(1);
            z+=(a[i][j]*a[a.size()-1][j]);
            a[i][a[i].size()-1]-=a[a.size()-1][j];
            a[a.size()-1][j]=0;
            j++;
        }
        else {
            ss1 << a[i][a[i].size()-1];
            ss1 >> s1;
            b[i][j]=s1+b[i][j].substr(1);
            z+=(a[i][j]*a[a.size()-1][j]);
            a[i][a[i].size()-1]=0;
            a[a.size()-1][j]=0;
            i++,j++;
        }
    }
    return z;
}


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
 * @brief MODI::is_matrix_balanced
 *
 * @param matrix
 *
 * @return
 *
 */

bool MODI::is_matrix_balanced( mymatrix matrix )
{
    int sum_column = 0;
    int sum_row = 0;

    for(int i=0; i < matrix.size()-1; i++)
    {
        sum_column += matrix[i][matrix[0].size()-1];
    }

    for(int i=0; i<matrix[0].size()-1; i++)
    {
        sum_row += matrix[matrix.size()-1][i];
    }

    if( sum_column != sum_row )
    {
        return false;
    }

    return true;

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

/**
 *
 * @brief Запрашивает данные для матрицы от пользователя
 *
 * @return заполненная пользователем матрица
 *
 */

mymatrix MODI::get_matrix_from_user_input()
{
    int n,m;
    float k;
    maximise=0;
    cout << "Enter the number of sources : ";
    cin >> n;
    cout << "Enter the number of demands : ";
    cin >> m;
    vector<float>a1(m+1,0);
    mymatrix a(n+1,a1);
    cout << "Enter the input table of the transportation problem, entering " << m << " costs for " << m << " demands followed by supply value of each source and then the demand value " << endl;
    for(int i=0; i<n; i++) {
        cout << "    S" << i+1 << " : ";
        for(int j=0; j<m; j++) cin >> a[i][j];
        cin >> a[i][m];
    }
    cout << "Demand" << " : ";
    for(int j=0; j<m; j++) cin >> a[n][j];
    return a;
}

QString MODI::calc( mymatrix inputMatrix )
{

    QString result;

    cout << "Input matrix is: \r\n";

    mymatrix fixed_matrix_4x4 = {
                                  {  5,  3,  1, 10 },
                                  {  3,  2,  4, 20 },
                                  {  4,  1,  2, 30 },
                                  { 15, 20, 25,  0 }
                                };

    // inputMatrix = fixed_matrix_4x4;

    print_matrix(inputMatrix);

    if( is_matrix_balanced(inputMatrix) )
    {

        vector<string>b1(inputMatrix[0].size(),"");

        mystringmatrix b(inputMatrix.size(),b1);

        int z = nwcr( inputMatrix, b ); // vam / nwcr

         cout << "NWCR SOLUTION : " << endl;

         print_solution(b);

         cout << "Z=" << z << endl << endl;

        print_string_matrix(b);

        vector<float>u(b.size()-1,0);
        vector<float>v(b[0].size()-1,0);

        node* curr=NULL;

        vector<node*>nd1(b[0].size(),curr);
        vector<vector<node*>>nd(b.size(),nd1);

        int chec=optimality_check(inputMatrix,b,u,v,nd);

        if(chec==1)
        {
            result += "Решение оптимальное\n";
        }
        else if(chec==2)
        {
            result += "---\n";

        }
        else
        {

            result += "Решение неоптимальное, оптимизация...\n\n";

            int z = calculate(inputMatrix,b,u,v,nd);

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

            print_solution(b);

            cout << "Z=" << z << endl << endl;

            result += QString("\nСтоимость доставки продукции: ") + QString::number(z) + QString(" ден. ед\n");

            //print_string_matrix(b);

        }

    }
    else
    {
        cout << "Given input is of an unbalanced transportation problem" << endl;
    }

    return result;

}

float MODI::vam( mymatrix a, mystringmatrix  &b )
{
#ifdef DEBUG
    printf("call vam()\n");
#endif
    float z=0;

    vector<int> ki(b.size(),0);
    vector<int> kj(b[0].size(),0);

    for(int i=0; i<a.size(); i++)
    {
        for(int j=0; j<a[i].size(); j++)
        {
            stringstream ss1;
            ss1 << a[i][j];
            ss1 >> b[i][j];
            if(i<a.size()-1 && j<a[i].size()-1) b[i][j]="0*"+b[i][j];
        }
    }

    while(a.size()!=1 && a[0].size()!=1) {

        vector<float>penalties(a.size()+a[0].size()-2,0);
        for(int i=0; i<a.size()-1; i++) {
            vector<float>a1=a[i];
            a1.erase(a1.begin()+a1.size()-1);
            if(a1.size()>1) {
                nth_element(a1.begin(),a1.begin()+1,a1.end());
                penalties[i]=a1[1];
                nth_element(a1.begin(),a1.begin(),a1.end());
                penalties[i]-=a1[0];
            }
        }
        for(int i=0; i<a[0].size()-1; i++) {
            vector<float>a1;
            for(int j=0; j<a.size()-1; j++) a1.push_back(a[j][i]);
            if(a1.size()>1) {
                nth_element(a1.begin(),a1.begin()+1,a1.end());
                penalties[i+a.size()-1]=a1[1];
                nth_element(a1.begin(),a1.begin(),a1.end());
                penalties[i+a.size()-1]-=a1[0];
            }
        }
        float max=penalties[0];
        int maxi=0;
        for(int i=1; i<penalties.size(); i++) {
            if(max<penalties[i]) {
                max=penalties[i];
                maxi=i;
            }
        }
        float min;
        int minj;
        if(maxi<(a.size()-1)) {
            min=a[maxi][0];
            minj=0;
            for(int j=1; j<a[0].size()-1; j++) {
                if(min>a[maxi][j]) {
                    min=a[maxi][j];
                    minj=j;
                }
            }
        }
        else {
            maxi-=(a.size()-1);
            min=a[0][maxi];
            minj=0;
            for(int j=1; j<a.size()-1; j++) {
                if(min>a[j][maxi]) {
                    min=a[j][maxi];
                    minj=j;
                }
            }
            int random=maxi;
            maxi=minj;
            minj=random;
        }
        string s;
        if(a[maxi][a[maxi].size()-1]<a[a.size()-1][minj]) {
            stringstream ss1;
            z+=(a[maxi][minj]*a[maxi][a[maxi].size()-1]);
            ss1 << a[maxi][a[maxi].size()-1];
            ss1 >> s;
            a[a.size()-1][minj]-=a[maxi][a[maxi].size()-1];
            a.erase(a.begin()+maxi);
            for(int i=0; i<=maxi; i++) {
                if(ki[i]) maxi++;
            }
            for(int i=0; i<=minj; i++) {
                if(kj[i]) minj++;
            }
            b[maxi][minj]=b[maxi][minj].substr(1);
            b[maxi][minj]=s+b[maxi][minj];
            ki[maxi]++;
        }
        else if(a[maxi][a[maxi].size()-1]>a[a.size()-1][minj]) {
            stringstream ss1;
            z+=(a[maxi][minj]*a[a.size()-1][minj]);
            ss1 << a[a.size()-1][minj];
            ss1 >> s;
            a[maxi][a[maxi].size()-1]-=a[a.size()-1][minj];
            for(int i=0; i<a.size(); i++) a[i].erase(a[i].begin()+minj);
            for(int i=0; i<=maxi; i++) {
                if(ki[i]) maxi++;
            }
            for(int i=0; i<=minj; i++) {
                if(kj[i]) minj++;
            }
            b[maxi][minj]=b[maxi][minj].substr(1);
            b[maxi][minj]=s+b[maxi][minj];
            kj[minj]++;
        }
        else {
            stringstream ss1;
            z+=(a[maxi][minj]*a[a.size()-1][minj]);
            ss1 << a[a.size()-1][minj];
            ss1 >> s;
            a.erase(a.begin()+maxi);
            for(int i=0; i<a.size(); i++) a[i].erase(a[i].begin()+minj);
            for(int i=0; i<=maxi; i++) {
                if(ki[i]) maxi++;
            }
            for(int i=0; i<=minj; i++) {
                if(kj[i]) minj++;
            }
            b[maxi][minj]=b[maxi][minj].substr(1);
            b[maxi][minj]=s+b[maxi][minj];
            ki[maxi]++;
            kj[minj]++;
        }
    }
    return z;
}

/*
5 3 1 10
3 2 4 20
4 1 2 30
15 20 25 0
*/

/*
int main()
{

    // Получение матрицы от пользователя
    //  Если не нужно это делать просто закомментировать нижележащую строку кода

    // mymatrix user_input_matrix = get_matrix_from_user_input();

    // Заранее инициализированные матрицы


    // http://reshmat.ru/transport.html?c11=5&c12=3&c13=1&z1=10&c21=3&c22=2&c23=4&z2=20&c31=4&c32=1&c33=2&z3=30&b1=15&b2=20&b3=25&method=SZCorner&step=2&sizeA=3&sizeB=3#b

    mymatrix fixed_matrix_4x4 = {
                                  {  5,  3,  1, 10 },
                                  {  3,  2,  4, 20 },
                                  {  4,  1,  2, 30 },
                                  { 15, 20, 25,  0 }
                                };

    // http://reshmat.ru/transport.html?c11=5&c12=3&c13=1&c14=1&z1=10&c21=3&c22=2&c23=4&c24=4&z2=20&c31=4&c32=1&c33=2&c34=2&z3=30&c41=4&c42=1&c43=2&c44=2&z4=10&b1=15&b2=20&b3=25&b4=10&method=SZCorner&step=2&sizeA=4&sizeB=4#b

    mymatrix fixed_matrix_5x5 = {
                                  {  5,  3,  1,  1, 10 },
                                  {  3,  2,  4,  4, 20 },
                                  {  4,  1,  2,  2, 30 },
                                  {  4,  1,  2,  2, 10 },
                                  { 15, 20, 25, 10,  0 }
                                };

    //
    // Расчет будет проводиться над матрицей input_matrix
    //  ниже можно задать её значение, например user_input_matrix или fixed_matrix_4x4, fixed_matrix_5x5
    //

    mymatrix inputMatrix = fixed_matrix_5x5;

    cout << "Input matrix is: \r\n";

    print_matrix(inputMatrix);

    if( is_matrix_balanced(inputMatrix) )
    {

        vector<string>b1(inputMatrix[0].size(),"");

        mystringmatrix b(inputMatrix.size(),b1);

        int z = vam( inputMatrix, b );

        cout << "VAM SOLUTION : " << endl;

        print_solution(b);

        cout << "Z=" << z << endl << endl;

        print_string_matrix(b);

        vector<float>u(b.size()-1,0);
        vector<float>v(b[0].size()-1,0);

        node* curr=NULL;

        vector<node*>nd1(b[0].size(),curr);
        vector<vector<node*>>nd(b.size(),nd1);

        int chec=optimality_check(inputMatrix,b,u,v,nd);

        if(chec==1)
        {
            cout << "The solution is optimal as checked by U-V Method/ MODI Method" << endl;
        }
        else if(chec==2)
        {
            cout << "The solution obtained is degenerate as it doesn't obey 'm+n-1=number of chosen costs' rule" << endl;
        }
        else
        {

            cout << "The solution obtained by VAM isn't optimal. Now, we find the optimal solution by using Modified distribution method" << endl;

            z = MODI(inputMatrix,b,u,v,nd);

            cout << "Phase 2 solution by MODI Method : " << endl;

            print_solution(b);

            cout << "Z=" << z << endl << endl;

            print_string_matrix(b);

        }

    }
    else
    {
        cout << "Given input is of an unbalanced transportation problem" << endl;
    }

    return 0;

}

*/
