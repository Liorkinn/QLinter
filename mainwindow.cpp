#include "mainwindow.h"
#include "ui_mainwindow.h"

#define PATH_DATABASE "/home/liora/Рабочий стол/lin/dbsettings.xml"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    if (xml.open(PATH_DATABASE)) {          ///< Открываем database.xml, заносим в std::vector элементы из файла. К данным можно будет обращаться по определённому тегу при помощи attributes. Заполняем comboBox именами баз данных.
            qDebug() << "[Information] Идёт чтение database.xml... Данные успешно получены!";
                dbworker = xml.ListElements(xml.getRoot(), "cluster");
        }else
        {
            QMessageBox::critical(0, "Ошибка открытия файла", "Файл не открыт! Проверьте права доступа database.xml, убедитесь\nчто файл расположен по пути /mnt/settings/,\nа также проверьте правильность содержимого xml файла. Закрытие приложения!");
            qDebug() << "[Error] Файл не открыт! Проверьте права доступа database.xml, убедитесь\nчто файл расположен по пути /mnt/settings/,\nа также проверьте правильность содержимого xml файла."; ///< Если файл не открыт - пользователю показывается всплывающее окно с ошибкой.
        }
    ui->DBtable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->DBtable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->DBtable->setSelectionMode(QAbstractItemView::NoSelection);

    this->createUI(QStringList()  << ( "Имя БД" )
                                  << ( "Статус кластера" )
                                  << ( "Статус сервера 1" )
                                  << ( "Статус сервера 2" ));
}

MainWindow::~MainWindow()
{
    delete ui;
}




void MainWindow::createUI(const QStringList &headers)
{
        ui->DBtable->setColumnCount                                 ( 4 );
        ui->DBtable->setShowGrid                                    ( true );
        ui->DBtable->setHorizontalHeaderLabels                      ( headers );
        ui->DBtable->horizontalHeader()->setStretchLastSection      ( true );

        for(unsigned int i = 0; i < dbworker.size(); i++)
        {
            ui->DBtable->insertRow(i);
            ui->DBtable->setItem(i,0, new QTableWidgetItem(dbworker[i].attribute("alias")));
        }
        ui->DBtable->horizontalHeader()->resizeSection( 0, 175 );
        ui->DBtable->horizontalHeader()->resizeSection( 1, 175 );

    checkAlias( "QLINTER", "SYSTEM", "MANAGER" );
}

void MainWindow::checkAlias(QString driver, QString user, QString pwd)
{
    for(unsigned int i = 0; i < dbworker.size(); i++){
        db = QSqlDatabase::addDatabase  (driver);
        db.setUserName                  (user);
        db.setPassword                  (pwd);
        db.setDatabaseName              (dbworker[i].attribute( "alias" ));

        if(!db.open()){
            ui->DBtable->setItem(i,1, new QTableWidgetItem( "Алиас не запущен." ));
            colorRed(i, 0); colorRed(i, 1);
        }else{
            ui->DBtable->setItem(i,1, new QTableWidgetItem( "Алиас запущен." ));
            checkMain(i,2);
            colorGreen(i, 0); colorGreen(i, 1);
        }

    }
}

void MainWindow::checkMain(int row, int column)
{
    QString
        state{};

      QFileInfo
        stateOne( dbworker[row].attribute( "path1" ) ),
        stateTwo( dbworker[row].attribute( "path2" ) );

      QFile
        file1( stateOne.filePath() ),
        file2( stateTwo.filePath() );


      if(stateOne.lastModified() < stateTwo.lastModified() && file2.open(QIODevice::ReadOnly)){
          state = file2.readAll();
          file2.close();

      }else if(stateOne.lastModified() > stateTwo.lastModified() && file1.open(QIODevice::ReadOnly)){
          state = file1.readAll();
          file1.close();
      }

    ui->DBtable->setItem(row, column, new QTableWidgetItem(state.mid(0, state.indexOf("."))));
}

void MainWindow::colorGreen(int row, int column)
{
    ui->DBtable->item(row, column)->setBackground(Qt::green);
}

void MainWindow::colorRed(int row, int column)
{
    ui->DBtable->item(row, column)->setBackground(Qt::red);

}

