#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QDebug"
#include "QLineEdit"
#include "QRegExp"
#include "QMessageBox"
#include <stdlib.h>
#include "ItemDelegate.hpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    // ui->tableWidget->setItemDelegate(new ItemDelegate());

    connect(ui->pushButton_create, SIGNAL(clicked()), this, SLOT(createTable()) );

    connect(ui->pushButton_calculate, SIGNAL(clicked()), this, SLOT(calculate()) );

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createTable()
{

    int users = ui->spinBox_users->value();

    int supply = ui->spinBox_supply->value();

    int currentUsers  = ui->tableWidget->columnCount();
    int currentSupply = ui->tableWidget->rowCount();

    if( (users == (currentUsers-1)) && (supply == (currentSupply-1)) ) {
        return;
    }

    //qDebug() << currentSupply;
    //qDebug() << currentUsers;

    QTableWidgetItem *unblockedItem = ( ui->tableWidget->item(currentSupply-1, currentUsers-1) );

    unblockedItem->setFlags( unblockedItem->flags() | Qt::ItemIsEnabled );

    ui->tableWidget->setColumnCount(users+1);
    ui->tableWidget->setRowCount(supply+1);

    for(int i = currentSupply; i<(supply+1); i++){
        for(int j = 0; j<(users+1); j++){
            QTableWidgetItem* Item = new QTableWidgetItem("");
            ui->tableWidget->setItem(i,j,Item);
        }
    }

    for(int j = currentUsers; j<(users+1); j++){
        for(int i = 0; i<(supply+1); i++){
            QTableWidgetItem* Item = new QTableWidgetItem("");
            ui->tableWidget->setItem(i,j,Item);
        }
    }

    QTableWidgetItem *blockedItem = ( ui->tableWidget->item(supply, users) );

    blockedItem->setFlags( blockedItem->flags() & (~Qt::ItemIsEnabled) );

    QStringList usersHeaders;

    for( int i = 1 ; i <= users; i++ )
    {
        QString test = QString("B") + QString::number(i); //;QString(i);

        usersHeaders.append( test );

    }

    QStringList supplyHeaders;

    for( int i = 1 ; i <= supply; i++ )
    {
        QString test = QString("A") + QString::number(i); //;QString(i);

        supplyHeaders.append( test );

    }

    usersHeaders.append("Запас");
    supplyHeaders.append("Потребность");

    ui->tableWidget->setHorizontalHeaderLabels(usersHeaders);
    ui->tableWidget->setVerticalHeaderLabels(supplyHeaders);

    qDebug() << "users: " << users << " supply: " << supply;

   // emit testsignal();

}

void MainWindow::test()
{
    qDebug() << "test";
    qDebug() << ui->tableWidget->columnCount();
    qDebug() << ui->tableWidget->rowCount();

    //QTableWidgetItem *item = ( ui->tableWidget->item(4, 4) );

   // item->setFlags( item->flags() & (~Qt::ItemIsEnabled) );
}

int array[100][100];

int array_result[100][100];

void MainWindow::print_array(int x, int y)
{

    for (int i = 0; i<x; i++) {
        QString rowText = "";
        for(int j = 0; j<y;j++){
            rowText += QString::number(array[i][j]);
            rowText += " ";
        }
        qDebug() << rowText;
    }

}

void MainWindow::print_array_result(int x, int y)
{

    for (int i = 0; i<x; i++) {
        QString rowText = "";
        for(int j = 0; j<y;j++){
            rowText += QString::number(array_result[i][j]);
            rowText += " ";
        }
        qDebug() << rowText;
    }

}

void MainWindow::calculate()
{

    int currentUsers  = ui->tableWidget->columnCount();
    int currentSupply = ui->tableWidget->rowCount();

    for(int i = 0; i < currentSupply; i++)
    {
        for( int j = 0 ; j < currentUsers; j++)
        {
            if( ( i == currentSupply-1 ) && (j == currentUsers-1) )
            {
                // Крайняя, неактивированная ячейка
                continue;
            }
            QTableWidgetItem* item =  ui->tableWidget->item(i,j);

            if( item == 0 )
            {
                // Вывести окно с ошибкой
                qDebug() << QString("Обращение по недопустимому индексу массива - A")+QString::number(i+1)+QString(" B")+QString::number(j+1);

            }
            else
            {
                QString text = item->text();
                QRegExp re("\\d*");

                if( ( text.isEmpty() ) || ( text.isNull() ) || ( !re.exactMatch(text) ))
                {
                    // Вывести окно с ошибкой
                    QMessageBox msgBox;

                    QString supplyText, usersText;

                    if( i == (currentSupply-1) )
                    {
                        supplyText = QString("Потребность");
                    }
                    else
                    {
                        supplyText = QString("А") + QString::number(i+1);
                    }

                    if( j == (currentUsers-1) )
                    {
                        usersText = QString("Запас");
                    }
                    else
                    {
                        usersText = QString("B") + QString::number(j+1);
                    }

                    msgBox.setText( QString("Не заполнен или заполнен неправильно элемент в строке: ") + supplyText + QString(" столбце: ") + usersText );

                    msgBox.exec();
                    return;
                }
                else
                {
                    array[i][j] = text.toInt();
                }
            }

        }
    }

    // int array[currentSupply][currentUsers];

    print_array( currentSupply, currentUsers );

    int sum_supply = 0;
    int sum_users = 0;

    for (int j = 0; j < currentUsers - 1; j++) {
        sum_supply += array[currentSupply - 1][j];
    }

    for (int i = 0; i < currentSupply - 1; i++) {
        sum_users += array[i][currentUsers - 1];
    }

    qDebug()<<sum_users<<" "<<sum_supply;

    if(sum_supply == sum_users){
        ui->plainTextEdit_result->appendPlainText(QString("Задача равновесная"));
    }
    else{
        ui->plainTextEdit_result->appendPlainText(QString("Задача неравновесная"));
    }

    //qDebug()<<supply_calc<<" "<<users_calc;


    for(int i = 0; i < currentSupply; i++ )
    {
        for( int j = 0; j < currentUsers; j++ )
        {
            array_result[i][j] = 0;
        }
    }

    // --------

    for( int i = 0; i < currentUsers-1 ; i++)
    {
        array_result[currentSupply-1][i] = array[currentSupply-1][i];
    }

    for( int i = 0; i < currentSupply-1 ; i++)
    {
        array_result[i][currentUsers-1] = array[i][currentUsers-1];
    }

    // -------

    qDebug() << "Empty result";

    //print_array_result(currentSupply, currentUsers);

    int x_supply = 0;
    int y_users = 0;

    for(int i = 0; i<(currentUsers+currentSupply-3); i++) // так как нам нужна размерность, которую задает пользовтель - 1
    {

        int supply_calc = array_result[currentSupply-1][x_supply];
        int users_calc  = array_result[y_users][currentUsers-1];

        if(supply_calc == users_calc)
        {

            array_result[y_users][x_supply] = supply_calc;
            array_result[y_users][currentUsers-1] = -1;

            if( ((currentSupply-2) == x_supply) && ((currentUsers-2) == y_users) )
            {
                array_result[currentSupply-1][x_supply] = -1;
            }
            else
            {
                array_result[currentSupply-1][x_supply] = 0;
            }

            y_users += 1;

            qDebug() << "DOWN\r\n";
        }
        else if( users_calc > supply_calc )
        {

           int minvalue = std::min(supply_calc, users_calc);

           array_result[y_users][x_supply] = supply_calc;

           qDebug() << "Supply calc " << supply_calc;

           array_result[y_users][currentUsers-1]   = users_calc - minvalue;
           array_result[currentSupply-1][x_supply] = -1;

           x_supply += 1;

           qDebug() << "RIGHT\r\n";

        }
        else if( supply_calc > users_calc )
        {

            int minvalue = std::min(supply_calc, users_calc);

            array_result[y_users][x_supply] = users_calc;

            array_result[y_users][currentUsers-1] = -1;
            array_result[currentSupply-1][x_supply] = supply_calc - minvalue;

            y_users += 1;

            qDebug() << "DOWN\r\n";

        }

        qDebug() << "Result array, iteration: " << i;

        print_array_result(currentSupply, currentUsers);

    }

    // Все посчитано

    // ------------------------------------ Расчет стоимости доставки

    QString stringResult = "";

    int cost = 0; // Общая стоимость

    for( int x_supply = 0; x_supply < (currentSupply-1); x_supply += 1 )
    {
        for( int y_users = 0; y_users < (currentUsers-1); y_users += 1 )
        {
            if(array_result[y_users][x_supply] > 0)
            {
                cost += array_result[y_users][x_supply] * array[y_users][x_supply];
                stringResult += QString::number(array_result[y_users][x_supply]) + QString("*") + QString::number(array[y_users][x_supply]);

                if( ( currentSupply-2 == x_supply ) && ( currentUsers-2 == y_users ) )
                {

                    stringResult += QString("=");
                }
                else
                {
                    stringResult += QString("+");
                }

            }
            else
            {
                cost += 0;
            }
        }
    }

    stringResult += QString::number(cost);
    stringResult += QString(" ден. ед.");

    ui->plainTextEdit_result->appendPlainText(stringResult);

    // ------------------------------------

    // array         Исходные данные от пользователя, веса и запасы с потребностями
    // array_result  Результаты вычислений

    // Заполнение полей таблицы для ячеек AB

    for( int x_supply = 0; x_supply < (currentSupply-1); x_supply += 1 )
    {
        for( int y_users = 0; y_users < (currentUsers-1); y_users += 1 )
        {

            QString cellString = QString::number(array[y_users][x_supply]) + QString("  ") + QString::number(array_result[y_users][x_supply]);

            QTableWidgetItem* item = ui->tableWidget->item(y_users,x_supply);

            item->setText( cellString );

            if(array_result[y_users][x_supply] != 0)
            {

                item->setFont(QFont("MS Shell Dlg 2",-1,QFont::Bold));

                item->setBackground(QBrush(QColor(255,170,0,127)));

            }

        }
    }

    // Заполнение ячеек запас/потребность

    // строка потребность

    for( int x_supply = 0; x_supply < (currentSupply - 1); x_supply += 1 )
    {
      if(array_result[currentUsers - 1][x_supply] == -1){

          QString cellstring = QString::number(array[currentUsers - 1][x_supply]) + QString("  ") + QString("нет");

          QTableWidgetItem *item = ui->tableWidget->item(currentUsers-1,x_supply);

          item->setText(cellstring);

      }
    }

    //строка запас

    for( int y_users = 0; y_users < (currentUsers - 1); y_users += 1 )
    {
      if(array_result[y_users][currentSupply - 1] == -1){

          QString cellstring = QString::number(array[y_users][currentSupply - 1]) + QString("  ") + QString("нет");

          QTableWidgetItem *item = ui->tableWidget->item(y_users,currentSupply-1);

          item->setText(cellstring);

      }
    }

    // Рассчет потенциалов

    // array_result[y_users][currentSupply];

    int u_start = 0;
    int v_start = 0;

    x_supply = 0;
    y_users  = 0;

    qDebug() << "Weight array";

    print_array(currentSupply+1,currentUsers+1);

    qDebug() << "-------------- Potencials";

    QString potencialsStringResult;

    for(int i = 0; i<(currentUsers+currentSupply-3); i++){

            qDebug() << "x_supply: " << x_supply << " y_users: " << y_users;

            //u_start = array[x_supply][currentUsers+1];
            //v_start = array [currentSupply+1][y_users];

           /* if( (x_supply == 3) && (y_users == 2))
            {
                volatile int idd = 10;
                idd += 1;
            } */

            v_start = array_result[currentSupply][y_users]; // [4][0]
            u_start = array_result[x_supply][currentUsers]; // [1][4]

            //qDebug() << "v_start: " << v_start << "u_start: " << u_start;
           potencialsStringResult += QString("A") + QString::number(x_supply+1) + QString("B") + QString::number(y_users+1) + QString(":");
           potencialsStringResult += QString("V") + QString::number(y_users+1) + QString("U") + QString::number(x_supply+1) + QString("=") + QString::number(array[x_supply][y_users]) + QString("\r\n");

            if(v_start > 0){
                u_start = array[x_supply][y_users] - v_start;
                array_result[x_supply][currentUsers] = u_start;
                array_result[currentSupply][y_users] = v_start;

                potencialsStringResult += QString::number(v_start) + QString("+(") + QString::number(u_start) + QString(")=") + QString::number(array[x_supply][y_users]) + QString("\r\n");

                y_users += 1;

            }

            else if(u_start > 0){
                v_start = array[x_supply][y_users] - u_start;
                array_result[currentSupply][y_users] = v_start;
                array_result[x_supply][currentUsers] = u_start;

                potencialsStringResult += QString::number(v_start) + QString("+(") + QString::number(u_start) + QString(")=") + QString::number(array[x_supply][y_users]) + QString("\r\n");

                x_supply += 1;


            }

            else if(!(v_start + u_start == array[x_supply][y_users] )){
                v_start = array[x_supply][y_users] - u_start;
                array_result[currentSupply][y_users] = v_start;
                array_result[x_supply][currentUsers] = u_start;

                potencialsStringResult += QString::number(v_start) + QString("+(") + QString::number(u_start) + QString(")=") + QString::number(array[x_supply][y_users]) + QString("\r\n");

                x_supply += 1;
            }





            // potencialsStringResult += QString::number(v_start) + QString("+(") + QString::number(u_start) + QString(")=") + QString::number(array[x_supply][y_users]) + QString("\r\n");

            // print_array_result(currentSupply+1,currentUsers+1);
            //qDebug() << "\n\r" ;

           /* for(){
           ui->plainTextEdit_result->appendPlainText(QString("A") + QString::number(x_supply+1) + QString("B") + QString::number(y_users+1)


            }*/



    }

    ui->plainTextEdit_result->appendPlainText(potencialsStringResult);




}

void MainWindow::on_action_triggered()
{

    QSettings settings("settings.ini", QSettings::IniFormat);

    int currentUsers  = ui->tableWidget->columnCount();
    int currentSupply = ui->tableWidget->rowCount();

    settings.setValue("Current users", currentUsers );
    settings.setValue("Current supply", currentSupply );

    settings.beginWriteArray("array_sources");

    int k = 0;

    for(int currentSupplyIndex = 0; currentSupplyIndex < currentSupply; currentSupplyIndex++)
    {
        for( int currentUsersIndex = 0 ; currentUsersIndex < currentUsers; currentUsersIndex++)
        {
            if( ( currentSupplyIndex == currentSupply-1 ) && (currentUsersIndex == currentUsers-1) )
            {
                // Крайняя, неактивированная ячейка
                continue;
            }

            settings.setArrayIndex(k);

            k += 1;

            settings.setValue("ItemValue", ui->tableWidget->item(currentSupplyIndex, currentUsersIndex)->text());

        }
    }

    settings.endArray();

    settings.sync();

/*
   QFile file("data.txt");
   QMessageBox msgBox;
   if(!file.open(QIODevice::WriteOnly)){
       msgBox.setText(QString("Ошибка сохранения"));
       msgBox.exec()
       return;

   }
   else {
   ui->plainTextEdit_result->appendPlainText("Данные сохранены");
   }
   QTextStream out(&file);

   for(int i=0;i< int N;i++){
        for(int p=0; p < intj; p++){
            out
        }

   }
   }
*/

}


void MainWindow::on_action_2_triggered()
{

    QSettings settings("settings.ini", QSettings::IniFormat);

    int currentUsers  = settings.value("Current users").toInt();  //ui->tableWidget->columnCount();
    int currentSupply = settings.value("Current supply").toInt(); //ui->tableWidget->rowCount();

    ui->tableWidget->setRowCount(currentUsers);
    ui->tableWidget->setColumnCount(currentSupply);

    settings.beginReadArray("array_sources");

    int k = 0;

    for(int currentSupplyIndex = 0; currentSupplyIndex < currentSupply; currentSupplyIndex++)
    {
        for( int currentUsersIndex = 0 ; currentUsersIndex < currentUsers; currentUsersIndex++)
        {
            if( ( currentSupplyIndex == currentSupply-1 ) && (currentUsersIndex == currentUsers-1) )
            {
                // Крайняя, неактивированная ячейка
                continue;
            }

            settings.setArrayIndex(k);

            k += 1;

            QTableWidgetItem* item = ui->tableWidget->item(currentSupplyIndex, currentUsersIndex);
            QString itemText = settings.value("ItemValue").toString();

            if ( item != 0 )
            {
                ui->tableWidget->item(currentSupplyIndex, currentUsersIndex)->setText( itemText );
            }
            else
            {
                item = new QTableWidgetItem();
                item->setText(itemText);
                ui->tableWidget->setItem(currentUsersIndex, currentUsersIndex, item);
            }
        }
    }

    settings.endArray();

    // settings.sync();

}


void MainWindow::on_action_3_triggered()
{

   // ui->tableWidget->clearContents();

    int currentUsers  = ui->tableWidget->columnCount();
    int currentSupply = ui->tableWidget->rowCount();

    for(int i = 0; i < currentSupply; i++)
    {
        for( int j = 0 ; j < currentUsers; j++)
        {
            if( ( i == currentSupply-1 ) && (j == currentUsers-1) )
            {
                // Крайняя, неактивированная ячейка
                continue;
            }

            QTableWidgetItem* item =  ui->tableWidget->item(i,j);

            item->setFont(QFont("MS Shell Dlg 2",-1,QFont::Normal));

            item->setBackground(QBrush(QColor(255,255,255)));

            item->setText("");

            ui->plainTextEdit_result->clear();

        }
    }

}


/*

*/
