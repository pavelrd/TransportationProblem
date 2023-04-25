#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QAbstractItemDelegate>

#include <vector>

using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:

    Ui::MainWindow *ui;

    bool    isUserInputBad();
    bool    isProblemHasEqualWeight();

    void    clearResultArray();

    void    copyUsersAndSupplyValuesToResultArray();
    void    showAllResultsOnTableWidget();
    void    fillArrayFromTableWidgetData();

    QString calculateDeliveryCost();

    void calculateFirstSolutionByNorthwestCornerMethod();

    void calculateByNorthWestCornerMethod();
    void calculateByRandomMethod();

    void print_array();
    void print_array_result(int x, int y, int highlightX=-1, int highlightY=-1);

    bool isFullBalance();

    int array[100][100] = {};

    int array_result[100][100] = {};

    struct index
    {
        int supply;
        int users;
    };

    vector<index> usedRoute;

    QAbstractItemDelegate* viewDelegate;

    struct Potentials
    {

        Potentials( int sizeU, int sizeV )
        {
            u.resize(sizeU);
            v.resize(sizeV);
        }

        vector<int> u;
        vector<int> v;

        QString calculateString = QString("");

    };

    struct Estimations
    {

        vector<int> est;

        QString calculateString = QString("");

    };

    Potentials calculatePotentials();

    Estimations calculateEstimationOfUnusedRoutes(Potentials potentials);

private slots:

    void on_action_triggered();
    void on_action_2_triggered();
    void on_action_3_triggered();
    void createTable();
    void calculate();

    void on_action_4_triggered();

    void changeView(int state);

};
#endif // MAINWINDOW_H
