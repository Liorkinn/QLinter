#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QDebug>
#include "xmlparser.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDateTime>
#include <QFileInfo>
#include <QFile>

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
    void createUI( const QStringList &headers );

    void checkAlias ( QString driver, QString user, QString pwd );

    void checkMain ( int row,int column );

    void colorGreen ( int row,int column );

    void colorRed ( int row, int column );

private:
    std::vector<QDomElement>dbworker;

    XmlParser xml;

    QSqlDatabase db;
};
#endif // MAINWINDOW_H
