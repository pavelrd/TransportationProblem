#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QDebug"
#include "QLineEdit"
#include "QRegExp"
#include "QMessageBox"
#include <stdlib.h>
#include "ItemDelegate.hpp"

#include <modi.h>

#include <random>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    // ------- Чтение файла настроек, хранящего настройки окна. Установка размеров окна
    //          исходя из настроек в этом файле если эти настройки существуют

    QSettings settings("windowParameters.ini", QSettings::IniFormat);

    int windowWidth = settings.value("WindowWidth").toInt();
    int windowHeight = settings.value("WindowHeight").toInt();

    // Применяем размер окна из настроек только если он там задан не менее чем 500 на 500

    if( ( windowWidth >= 500 ) && (windowHeight >= 500 ) )
    {
        this->resize(windowWidth, windowHeight);
    }

    // ------- Опциональная функция, включает другой вид отрисовки таблицы после расчета

    viewDelegate = ui->tableWidget->itemDelegate();

    // ui->tableWidget->setItemDelegate(new ItemDelegate());


    // ------- Инициализация генератора случайных чисел для возможности автоматического заполения таблицы
    //          в целях отладки одно и то же число
    //         Чтобы последовательность чисел была разная заменить на
    //           srand(time(0))
    //

    srand(100);

    // ------- Соединение сигналов от элементов формы с их обратотчиками

    connect(ui->pushButton_create, SIGNAL(clicked()), this, SLOT(createTable()) );

    connect(ui->pushButton_calculate, SIGNAL(clicked()), this, SLOT(calculate()) );

    connect( ui->checkBox_splitResultShow, SIGNAL(stateChanged(int)), this, SLOT(changeView(int)) );

}

MainWindow::~MainWindow()
{

    // ------- При закрытии окна MainWindow сохраняем его размер в файле настроек windowParameters.ini

    QSettings settings("windowParameters.ini", QSettings::IniFormat);

    settings.setValue("WindowWidth", this->size().width() );
    settings.setValue("WindowHeight", this->size().height() );

    settings.sync();

    // -------

    delete ui;

}

/**
 *
 * @brief Создает таблицу по размерам, заданным пользователем
 *
 */

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

    for(int supplyIndex = currentSupply; supplyIndex<(supply+1); supplyIndex++)
    {
        for(int usersIndex = 0; usersIndex<(users+1); usersIndex++)
        {
            QTableWidgetItem* Item = new QTableWidgetItem("");
            ui->tableWidget->setItem( supplyIndex, usersIndex, Item);
        }
    }

    for(int usersIndex = currentUsers; usersIndex<(users+1); usersIndex++)
    {
        for(int supplyIndex = 0; supplyIndex<(supply+1); supplyIndex++)
        {
            QTableWidgetItem* Item = new QTableWidgetItem("");
            ui->tableWidget->setItem(supplyIndex, usersIndex, Item);
        }
    }

    QTableWidgetItem *blockedItem = ( ui->tableWidget->item(supply, users) );

    blockedItem->setFlags( blockedItem->flags() & (~Qt::ItemIsEnabled) );

    QStringList usersHeaders;

    for( int usersIndex = 1 ; usersIndex <= users; usersIndex++ )
    {
        QString test = QString("B") + QString::number(usersIndex); //;QString(i);

        usersHeaders.append( test );

    }

    QStringList supplyHeaders;

    for( int supplyIndex = 1 ; supplyIndex <= supply; supplyIndex++ )
    {
        QString test = QString("A") + QString::number(supplyIndex); //;QString(i);

        supplyHeaders.append( test );

    }

    usersHeaders.append("Запас");
    supplyHeaders.append("Потребность");

    ui->tableWidget->setHorizontalHeaderLabels(usersHeaders);
    ui->tableWidget->setVerticalHeaderLabels(supplyHeaders);

    qDebug() << "users: " << users << " supply: " << supply;

}

/**
 *
 * @brief Вызывается при нажатии кнопки "Расчёт".
 *         Производит расчет транспортной задачи методом северо западного угла
 *         Выводит результат в таблицу и в текстовом виде
 *
 */

void MainWindow::calculate()
{

    ui->plainTextEdit_result->clear();

    // ------- Проверка пользовательского ввода

    if( isUserInputBad() )
    {
        return;
    }

    // ------- Заполнение массива Array данными из интерфейса(QTableWidget)

    fillArrayFromTableWidgetData();

    // print_array(); // <-- Напечатать массив Array, который только что заполнили в целях отладки

    // ------- Проверка задачи на равновесность

    if( isProblemHasEqualWeight() )
    {
        ui->plainTextEdit_result->appendPlainText(QString("Задача равновесная"));
    }
    else
    {
        ui->plainTextEdit_result->appendPlainText(QString("Задача неравновесная!"));
        return;
    }

    // ------- Очистка массива с результатом result_array

    clearResultArray();

    // ------- Скопировать значения потребностей и запасов из массива array в массив result_array

    copyUsersAndSupplyValuesToResultArray();

    // ------- Первоначальный расчет методом северо-западного угла

    calculateFirstSolutionByNorthwestCornerMethod();

    // ------- Расчет и получение строки со стоимостью доставки

    QString deliveryCostString = calculateDeliveryCost();

    // ------- Отображение строки с расчетом стоимости доставки

    ui->plainTextEdit_result->appendPlainText( deliveryCostString );

    // ------- Отображение результатов расчетов из
    //          массивов array и array_result на tableWidget

    showAllResultsOnTableWidget();

    // ------- Расчет потенциалов

    Potentials potentials = calculatePotentials();

    // ------- Отображение строки с расчетом потенциалов

    ui->plainTextEdit_result->appendPlainText( potentials.calculateString );

    // ------- Оценка незадействованных маршрутов

    ui->plainTextEdit_result->appendPlainText( QString("Поиск оценки незадействованных маршрутов. \n"));

    Estimations estimations;

    estimations = calculateEstimationOfUnusedRoutes( potentials );

    ui->plainTextEdit_result->appendPlainText( estimations.calculateString );

    for( auto item : estimations.est )
    {
        if( item < 0 )
        {

            // Требуется дальнейшая оптимизация

            // ui->plainTextEdit_result->appendPlainText( "Оптимизация..." );

            // Расчет...

            // ------- Подсчет с помощью класса MODI

            MODI modi;

            int currentUsers  = ui->tableWidget->columnCount();
            int currentSupply = ui->tableWidget->rowCount();

            mymatrix matrix;

            for(int currentSupplyIndex = 0; currentSupplyIndex < currentSupply; currentSupplyIndex++)
            {

                vector<float> row;

                for( int currentUsersIndex = 0 ; currentUsersIndex < currentUsers; currentUsersIndex++)
                {

                    row.push_back( array[currentSupplyIndex][currentUsersIndex]  );
                }

                matrix.push_back(row);

            }

            QString value = modi.calc( matrix );

            // -------- Вывод расчета в текстовое поле

            ui->plainTextEdit_result->appendPlainText( value );



            return;
        }
    }

    ui->plainTextEdit_result->appendPlainText( "Текущая стоимость наименьшая, оптимизация не требуется." );

}

/**
 *
 * @brief Сохраняет данные из таблицы в файл настроек "settings.ini"
 *
 */

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

}

/**
 *
 * @brief Загружает данные из файла "settings.ini" в таблицу. Если какие-то строки
 *         и столбцы таблицы еще не созданы то создает их
 *
 */

void MainWindow::on_action_2_triggered()
{

    QSettings settings("settings.ini", QSettings::IniFormat);

    if ( settings.allKeys().isEmpty() )
    {

        QMessageBox msgBox;

        msgBox.setWindowTitle("Ошибка");

        msgBox.setText( QString("Нет сохраненных ранее настроек!") );

        msgBox.exec();

        return;

    }

    int currentUsers  = settings.value("Current users").toInt();  //ui->tableWidget->columnCount();
    int currentSupply = settings.value("Current supply").toInt(); //ui->tableWidget->rowCount();

    if( (currentUsers == 0) || (currentSupply == 0) )
    {

        QMessageBox msgBox;

        msgBox.setWindowTitle("Ошибка");

        msgBox.setText( QString("Неправильный файл настроек, один или оба размера таблицы отсутствуют или нулевые!") );

        msgBox.exec();

        return;

    }

    ui->spinBox_users->setValue(currentUsers-1);
    ui->spinBox_supply->setValue(currentSupply-1);

    createTable();

    // ------------------------------------

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

/**
 *
 * @brief Очистка таблицы
 *
 */

void MainWindow::on_action_3_triggered()
{

   // ui->tableWidget->clearContents();

    int currentUsers  = ui->tableWidget->columnCount();
    int currentSupply = ui->tableWidget->rowCount();

    for(int supplyIndex = 0; supplyIndex < currentSupply; supplyIndex++)
    {
        for( int currentIndex = 0 ; currentIndex < currentUsers; currentIndex++)
        {
            if( ( supplyIndex == currentSupply-1 ) && (currentIndex == currentUsers-1) )
            {
                // Крайняя, неактивированная ячейка
                continue;
            }

            QTableWidgetItem* item =  ui->tableWidget->item(supplyIndex,currentIndex);

            item->setFont(QFont("MS Shell Dlg 2",-1,QFont::Normal));

            item->setBackground(QBrush(QColor(255,255,255)));

            item->setText("");

            ui->plainTextEdit_result->clear();

        }
    }

}


/**
 *
 * @brief Провека на то что в таблицу TableWidget были
 *         введены числа
 *
 * @return true - были введены не числа, false - все в порядке
 *
 */

bool MainWindow::isUserInputBad()
{

    int currentUsers  = ui->tableWidget->columnCount();
    int currentSupply = ui->tableWidget->rowCount();

    for(int supplyIndex = 0; supplyIndex < currentSupply; supplyIndex++)
    {
        for( int usersIndex = 0 ; usersIndex < currentUsers; usersIndex++)
        {

            if( ( supplyIndex == currentSupply-1 ) && (usersIndex == currentUsers-1) )
            {
                // Крайняя, неактивированная ячейка
                continue;
            }

            QTableWidgetItem* item =  ui->tableWidget->item( supplyIndex, usersIndex );

            if( item == 0 )
            {
                // Вывести окно с ошибкой
                qDebug() << QString("Обращение по недопустимому индексу массива - A")+QString::number(supplyIndex+1)+QString(" B")+QString::number(usersIndex+1);

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

                    if( supplyIndex == (currentSupply-1) )
                    {
                        supplyText = QString("Потребность");
                    }
                    else
                    {
                        supplyText = QString("А") + QString::number(supplyIndex+1);
                    }

                    if( usersIndex == (currentUsers-1) )
                    {
                        usersText = QString("Запас");
                    }
                    else
                    {
                        usersText = QString("B") + QString::number(usersIndex+1);
                    }

                    msgBox.setText( QString("Не заполнен или заполнен неправильно элемент в строке: ") + supplyText + QString(" столбце: ") + usersText );

                    msgBox.exec();

                    return false;

                }
                //else
                //{
                    // array[supplyIndex][usersIndex] = text.toInt();
                //}
            }

        }
    }

    return false;

}

/**
 *
 * @brief Заполнение массива array числами из tableWidget
 *
 */

void MainWindow::fillArrayFromTableWidgetData()
{

    int currentUsers  = ui->tableWidget->columnCount();
    int currentSupply = ui->tableWidget->rowCount();

    for(int supplyIndex = 0; supplyIndex < currentSupply; supplyIndex++)
    {
        for( int usersIndex = 0 ; usersIndex < currentUsers; usersIndex++)
        {

            if( ( supplyIndex == currentSupply-1 ) && (usersIndex == currentUsers-1) )
            {
                // Крайняя, неактивированная ячейка
                continue;
            }

            QTableWidgetItem* item =  ui->tableWidget->item( supplyIndex, usersIndex );

            if( item == 0 )
            {
                // Вывести окно с ошибкой
                qDebug() << QString("Обращение по недопустимому индексу массива - A")+QString::number(supplyIndex+1)+QString(" B")+QString::number(usersIndex+1);

            }
            else
            {
                QString text = item->text();
                QRegExp re("\\d*");

                if( ! ( ( text.isEmpty() ) || ( text.isNull() ) || ( !re.exactMatch(text) ) ) )
                {
                    array[supplyIndex][usersIndex] = text.toInt();
                }
            }
        }
    }

}


/**
 *
 * @brief Проверяет равновесная ли задача
 *
 * @return true - если задача равновеная, false - задача не равновесная
 *
 */

bool MainWindow::isProblemHasEqualWeight()
{

    int currentUsers  = ui->tableWidget->columnCount();
    int currentSupply = ui->tableWidget->rowCount();

    int sum_supply = 0;
    int sum_users = 0;

    for (int j = 0; j < currentUsers - 1; j++)
    {
        sum_supply += array[currentSupply - 1][j];
    }

    for (int i = 0; i < currentSupply - 1; i++)
    {
        sum_users += array[i][currentUsers - 1];
    }

    if(sum_supply == sum_users)
    {
        return true;
    }
    else
    {
        return false;
    }

}

/**
 *
 * @brief Заполняет массив array_result нулями
 *
 */

void MainWindow::clearResultArray()
{

    for( auto& rows : array_result )
    {
        for( auto& item : rows )
        {
            item = 0;
        }
    }

}

/**
 *
 * @brief Копирует значения запаса и потребности из
 *         массива array в массив resultArray
 *
 */

void MainWindow::copyUsersAndSupplyValuesToResultArray()
{

    int currentUsers  = ui->tableWidget->columnCount();
    int currentSupply = ui->tableWidget->rowCount();

    for( int i = 0; i < currentUsers-1 ; i++)
    {
        array_result[currentSupply-1][i] = array[currentSupply-1][i];
    }

    for( int i = 0; i < currentSupply-1 ; i++)
    {
        array_result[i][currentUsers-1] = array[i][currentUsers-1];
    }

}

/**
 *
 * @brief Расчет стоимости доставки
 *
 * @return Строка с подробным расчетом стоимости доставки
 *
 */

QString MainWindow::calculateDeliveryCost()
{

    int currentUsers  = ui->tableWidget->columnCount();
    int currentSupply = ui->tableWidget->rowCount();

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

    return stringResult;

}

/**
 *
 * @brief Расчет потенциалов
 *
 * @return Строка с подробным расчетом потенциалов
 *
 */

MainWindow::Potentials MainWindow::calculatePotentials()
{

    qDebug() << "\n------- Start calculatePotentials() \n";

    int currentUsers  = ui->tableWidget->columnCount();
    int currentSupply = ui->tableWidget->rowCount();

    //int u_start = 0;
    //int v_start = 0;

    //int x_supply = 0;
    //int y_users  = 0;

    struct Potentials potentials(currentUsers, currentSupply);

    enum filled
    {
        FILLED = 0,
        UNFILLED
    };

    vector<filled> filledU(currentUsers);
    vector<filled> filledV(currentUsers);

    for( auto& item : potentials.v ) { item = 0; }

    for( auto& item : potentials.u ) { item = 0; }

    for( auto& item : filledU ) { item = UNFILLED; }

    for( auto& item : filledV ) { item = UNFILLED; }

    bool start = true;

    print_array();

    qDebug() << "Cycle " << usedRoute.size();

    potentials.calculateString += QString("Пусть u%1 равно 0 \n").arg(usedRoute[0].users+1);

    for( auto item : usedRoute )
    {

        volatile int currentElement = array[item.users][item.supply];

        qDebug() << currentElement;

        potentials.calculateString += QString("A%1B%2 : ").arg(item.users+1).arg(item.supply+1);

        if( start )
        {

            start = false;

            // Приравниваем начальный потенциал нулю

            potentials.u[ item.users ] = 0;

            potentials.v[ item.supply ] = currentElement - potentials.u[item.users];

            potentials.calculateString += QString("v%1 + u%2 = %3   ").arg(item.supply+1).arg(item.users+1).arg(  potentials.u[ item.users ] + potentials.v[ item.supply ] );

            if( potentials.u[item.users] >= 0 )
            {
                potentials.calculateString += QString("v%1 = %2 - %3 = %4 \r\n").arg(item.supply+1).arg(currentElement).arg(potentials.u[item.users]).arg( currentElement - potentials.u[item.users] );
            }
            else
            {
                potentials.calculateString += QString("v%1 = %2 - (%3) = %4 \r\n").arg(item.supply+1).arg(currentElement).arg(potentials.u[item.users]).arg( currentElement - potentials.u[item.users] );
            }

            // Помечаем заполненными сразу U и V

            filledU[item.users] = FILLED;
            filledV[item.supply] = FILLED;

        }
        else
        {

            if( filledU[item.users] == UNFILLED )
            {

                if( filledV[item.supply] == UNFILLED )
                {

                    potentials.u.clear();
                    potentials.v.clear();

                    return potentials;

                }

                potentials.u[item.users] = currentElement - potentials.v[item.supply];

                filledU[item.users] = FILLED;

                potentials.calculateString += QString("v%1 + u%2 = %3   ").arg(item.supply+1).arg(item.users+1).arg(  potentials.u[ item.users ] + potentials.v[ item.supply ] );

                if( potentials.u[item.users] >= 0 )
                {
                    potentials.calculateString += QString("u%1 = %2 - %3 = %4 \r\n").arg(item.users+1).arg(currentElement).arg(potentials.v[item.supply]).arg( currentElement - potentials.v[item.supply] );
                }
                else
                {
                    potentials.calculateString += QString("u%1 = %2 - (%3) = %4 \r\n").arg(item.users+1).arg(currentElement).arg(potentials.v[item.supply]).arg( currentElement - potentials.v[item.supply]  );
                }

            }
            else
            {
                // Заполняем V

                potentials.v[item.supply] = currentElement - potentials.u[item.users];

                filledV[item.supply] = FILLED;

                potentials.calculateString += QString("v%1 + u%2 = %3   ").arg(item.supply+1).arg(item.users+1).arg(  potentials.u[ item.users ] + potentials.v[ item.supply ] );

                if( potentials.u[item.users] >= 0 )
                {
                    potentials.calculateString += QString("v%1 = %2 - %3 = %4 \r\n").arg(item.supply+1).arg(currentElement).arg(potentials.u[item.users]).arg( currentElement - potentials.u[item.users] );
                }
                else
                {
                    potentials.calculateString += QString("v%1 = %2 - (%3) = %4 \r\n").arg(item.supply+1).arg(currentElement).arg(potentials.u[item.users]).arg( currentElement - potentials.u[item.users] );
                }

            }

        }


    }

    qDebug() << "U values";

    for( auto item : potentials.u )
    {
        qDebug() << item;
    }

    qDebug() << "V values";

    for( auto item : potentials.v )
    {
        qDebug() << item;
    }


    qDebug() << "\n------- End calculatePotentials() \n";


    return potentials;

/*
    QString potencialsStringResult;

    for(int i = 0; i<(currentUsers+currentSupply-3); i++)
    {

        qDebug() << "x_supply: " << x_supply << " y_users: " << y_users;

        //u_start = array[x_supply][currentUsers+1];
        //v_start = array [currentSupply+1][y_users];

        v_start = array_result[currentSupply][y_users]; // [4][0]
        u_start = array_result[x_supply][currentUsers]; // [1][4]

        // qDebug() << "v_start: " << v_start << "u_start: " << u_start;

        //
        // TODO: переделать на строку с аргументами
        //        По примеру: QString status = QString("Processing file %1 of %2: %3").arg(i).arg(total).arg(fileName);
        //

        potencialsStringResult += QString("A") + QString::number(x_supply+1) + QString("B") + QString::number(y_users+1) + QString(":");
        potencialsStringResult += QString("V") + QString::number(y_users+1) + QString("U") + QString::number(x_supply+1) + QString("=") + QString::number(array[x_supply][y_users]) + QString("\r\n");

        if(v_start > 0)
        {

            u_start = array[x_supply][y_users] - v_start;
            array_result[x_supply][currentUsers] = u_start;
            array_result[currentSupply][y_users] = v_start;

            potencialsStringResult += QString::number(v_start) + QString("+(") + QString::number(u_start) + QString(")=") + QString::number(array[x_supply][y_users]) + QString("\r\n");

            y_users += 1;

        }
        else if(u_start > 0)
        {

            v_start = array[x_supply][y_users] - u_start;
            array_result[currentSupply][y_users] = v_start;
            array_result[x_supply][currentUsers] = u_start;

            potencialsStringResult += QString::number(v_start) + QString("+(") + QString::number(u_start) + QString(")=") + QString::number(array[x_supply][y_users]) + QString("\r\n");

            x_supply += 1;


        }
        else if(!(v_start + u_start == array[x_supply][y_users] ))
        {

            v_start = array[x_supply][y_users] - u_start;
            array_result[currentSupply][y_users] = v_start;
            array_result[x_supply][currentUsers] = u_start;

            potencialsStringResult += QString::number(v_start) + QString("+(") + QString::number(u_start) + QString(")=") + QString::number(array[x_supply][y_users]) + QString("\r\n");

            x_supply += 1;
        }

        // potencialsStringResult += QString::number(v_start) + QString("+(") + QString::number(u_start) + QString(")=") + QString::number(array[x_supply][y_users]) + QString("\r\n");

        // print_array_result(currentSupply+1,currentUsers+1);
        //qDebug() << "\n\r" ;

        // for(){
        //ui->plainTextEdit_result->appendPlainText(QString("A") + QString::number(x_supply+1) + QString("B") + QString::number(y_users+1)
        //}

    }
*/

    //qDebug() << "\n------- End calculatePotentials() \n";

    //return potencialsStringResult;

}

/**
 *
 * @brief Отображает результаты расчетов из
 *          массивов array и array_result на tableWidget
 *
 */

void MainWindow::showAllResultsOnTableWidget()
{

    int currentUsers  = ui->tableWidget->columnCount();
    int currentSupply = ui->tableWidget->rowCount();

    // array         Исходные данные от пользователя, веса и запасы с потребностями
    // array_result  Результаты вычислений

    // Заполнение полей таблицы для ячеек AB

    qDebug("TEST PRINT!!!!");

    print_array_result( currentSupply, currentUsers );

    for( int x_supply = 0; x_supply < (currentSupply-1); x_supply += 1 )
    {
        for( int y_users = 0; y_users < (currentUsers-1); y_users += 1 )
        {

            QString cellString = QString::number(array[y_users][x_supply]) + QString("  ") + QString::number(array_result[y_users][x_supply]);

            QTableWidgetItem* item = ui->tableWidget->item(y_users,x_supply);

            item->setText( cellString );


            for( auto elem : usedRoute )
            {

                if( ( elem.users == y_users ) && ( elem.supply == x_supply ) )
                {

                    item->setFont(QFont("MS Shell Dlg 2",-1,QFont::Bold));

                    item->setBackground(QBrush(QColor(255,170,0,127)));

                }

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

}

/**
 *
 * @brief Начальное решение методом северо западного угла
 *
 */

void MainWindow::calculateFirstSolutionByNorthwestCornerMethod()
{

    int currentUsers  = ui->tableWidget->columnCount();
    int currentSupply = ui->tableWidget->rowCount();

    qDebug() << "\n------- Start calculateFirstSolutionByNorthwestCornerMethod() \n";

    qDebug() << "Result array, before any iterations : \n";

    print_array_result(currentSupply, currentUsers);

    int x_supply = 0;
    int y_users = 0;

    usedRoute.clear();

    for(int i = 0; i<(currentUsers+currentSupply-3); i++) // так как нам нужна размерность, которую задает пользовтель - 1
    {

        int supply_calc = array_result[currentSupply-1][x_supply];
        int users_calc  = array_result[y_users][currentUsers-1];

        // ------- отладка

        qDebug() << "\nResult array, iteration: " << i << "\n";

        print_array_result(currentSupply, currentUsers, x_supply, y_users );

        struct index element;

        element.users  = y_users;
        element.supply = x_supply;

        usedRoute.push_back(element);

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

            // ------- отладка

            //qDebug() << "\nResult array, iteration: " << i << "\n";

            //print_array_result(currentSupply, currentUsers, x_supply, y_users-1 );

            qDebug() << "\nTHEN GO DOWN ↓";

            // -------

        }
        else if( users_calc > supply_calc )
        {

           int minvalue = std::min(supply_calc, users_calc);

           array_result[y_users][x_supply] = supply_calc;

           //qDebug() << "Supply calc " << supply_calc;

           array_result[y_users][currentUsers-1]   = users_calc - minvalue;
           array_result[currentSupply-1][x_supply] = -1;

           x_supply += 1;

           // ------- отладка

           //qDebug() << "\nResult array, iteration: " << i << "\n";

           //print_array_result(currentSupply, currentUsers, x_supply-1, y_users );

           qDebug() << "\nTHEN GO RIGHT →";

           // -------

        }
        else if( supply_calc > users_calc )
        {

            int minvalue = std::min(supply_calc, users_calc);

            array_result[y_users][x_supply] = users_calc;

            array_result[y_users][currentUsers-1] = -1;
            array_result[currentSupply-1][x_supply] = supply_calc - minvalue;

            y_users += 1;

            //print_array_result(currentSupply, currentUsers, x_supply, y_users-1 );

            qDebug() << "\nTHEN GO DOWN ↓";

            // -------

        }
        else
        {
            qDebug() << "BAD CONDITION!!!";
            usedRoute.resize(usedRoute.size()-1);
            break;
        }

    }

    for( auto element : usedRoute )
    {
        qDebug() << element.users << " " << element.supply;
    }

    qDebug() << "------- End calculateFirstSolutionByNorthwestCornerMethod()";

}

/**
 *
 *  @brief Заполнение таблицы случайными значениями
 *
 */

void MainWindow::on_action_4_triggered()
{

    int currentUsers  = ui->tableWidget->columnCount();
    int currentSupply = ui->tableWidget->rowCount();

    for(int supplyIndex = 0; supplyIndex < currentSupply; supplyIndex++)
    {
        for( int currentIndex = 0 ; currentIndex < currentUsers; currentIndex++)
        {
            if( ( supplyIndex == currentSupply-1 ) && (currentIndex == currentUsers-1) )
            {
                // Крайняя, неактивированная ячейка
                continue;
            }

            QTableWidgetItem* item =  ui->tableWidget->item(supplyIndex,currentIndex);

            item->setFont(QFont("MS Shell Dlg 2",-1,QFont::Normal));

            item->setBackground(QBrush(QColor(255,255,255)));

            item->setText( QString::number( rand() % 50 ) );

            ui->plainTextEdit_result->clear();

        }
    }

}

/**
 *
 * @brief Выводит значения массива array в отладочный терминал
 *
 */

void MainWindow::print_array()
{

    int y = ui->tableWidget->columnCount();
    int x = ui->tableWidget->rowCount();

    for (int i = 0; i<x; i++) {
        QString rowText = "";
        for(int j = 0; j<y;j++)
        {

            auto array_value = array[i][j];

            // ------- Добавление лишних пробелов для выравнивания

            if( array_value < 10 )
            {
                rowText += "    ";
            }
            else if( array_value < 100 )
            {
                rowText += "   ";
            }
            else if( array_value < 1000 )
            {
                rowText += "  ";
            }

            if( array_value >= 0 )
            {
                rowText += " ";
            }

            rowText += " ";

            // -------

            rowText += QString::number(array_value);

        }
        qDebug() << rowText;
    }

}

/**
 *
 * @brief Выводит значения массива array_result в отладочный терминал
 *
 */

void MainWindow::print_array_result(int x, int y, int highlightX, int highlightY )
{

    for (int i = 0; i<x; i++)
    {
        QString rowText = "";

        for(int j = 0; j<y;j++)
        {

            auto array_value = array_result[i][j];

            // ------- Добавление лишних пробелов для выравнивания

            if( array_value < 10 )
            {
                rowText += "   ";
            }
            else if( array_value < 100 )
            {
                rowText += "  ";
            }

            if( array_value >= 0 )
            {
                rowText += " ";
            }

            // -------

            if( (highlightX == j) && (highlightY == i) )
            {
                rowText += QString("[") + QString::number(array_value) + QString("]");
            }
            else
            {
                rowText += "  ";
                rowText += QString::number(array_value);

            }

        }
        qDebug() << rowText;
    }

}

/**
 *
 * @brief changeView
 *
 * @param state
 *
 */

void MainWindow::changeView(int state)
{

    if( state == Qt::Checked )
    {
         ui->tableWidget->setItemDelegate(new ItemDelegate());
    }
    else
    {
        ui->tableWidget->setItemDelegate(viewDelegate);
    }
}

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

MainWindow::Estimations MainWindow::calculateEstimationOfUnusedRoutes(Potentials potentials)
{

    int currentUsers  = ui->tableWidget->columnCount() - 1;
    int currentSupply = ui->tableWidget->rowCount() - 1;

    QString estimationString = QString("");

    Estimations estimations;

    for(int supplyIndex = 0; supplyIndex < currentSupply; supplyIndex++)
    {
        for( int usersIndex = 0 ; usersIndex < currentUsers; usersIndex++ )
        {

            bool skipCell = false;

            // Проверить нет ли этой ячейки в задействованном маршруте

            for( auto item : usedRoute )
            {
                if( ( item.users == usersIndex ) && ( item.supply == supplyIndex ) )
                {
                    skipCell = true;
                    break;
                }
            }

            // Проверено и задан флаг

            if( skipCell )
            {
                // Пропускаем эту ячейку, потому-что она задействована в существующем маршруте
                continue;
            }

            // Ячейка была незадействована в маршруте, считаем для неё оценку незадейтвованных марштутов

            estimations.calculateString += QString("A%1B%2 : Δ%1%2 = c%1%2 - (u%1 + v%2) = ").arg(usersIndex+1).arg(supplyIndex+1); // arg(supplyIndex+1)

            int summ = array[usersIndex][supplyIndex] - ( potentials.u[usersIndex] + potentials.v[supplyIndex]);

                                                                          // .arg( array[supplyIndex][usersIndex] ).arg( potentials.u[usersIndex] ).arg( potentials.v[supplyIndex] ).arg(summ);

            estimations.calculateString += QString("%1 - (%2 + %3) = %4 \n").arg( array[usersIndex][supplyIndex] ).arg( potentials.u[usersIndex] ).arg( potentials.v[supplyIndex] ).arg(summ);

            estimations.est.push_back(summ);

        }

    }

    return estimations;

}
