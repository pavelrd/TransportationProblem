#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>

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
    QString calculatePotentials();
    void    calculateFirstSolutionByNorthwestCornerMethod();

    void print_array();
    void print_array_result(int x, int y, int highlightX=-1, int highlightY=-1);

    int array[100][100] = {};

    int array_result[100][100] = {};

private slots:

    void on_action_triggered();
    void on_action_2_triggered();
    void on_action_3_triggered();
    void createTable();
    void calculate();

    void on_action_4_triggered();
};
#endif // MAINWINDOW_H
