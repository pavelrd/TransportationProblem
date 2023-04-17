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

    void print_array(int x, int y);
    void print_array_result(int x, int y);

private:
    Ui::MainWindow *ui;

public slots:

    void createTable();
    void test();
    void calculate();

signals:

    void testsignal();

private slots:
    void on_action_triggered();
    void on_action_2_triggered();
    void on_action_3_triggered();
};
#endif // MAINWINDOW_H
