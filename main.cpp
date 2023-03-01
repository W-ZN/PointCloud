#pragma execution_character_set("utf-8")
#include "mainwindow.h"
#include <QFont>
#include <QTextCodec>
#include <QApplication>
#include <vtkOutputWindow.h>

int main(int argc, char *argv[])
{
    vtkOutputWindow::SetGlobalWarningDisplay(0);
    QApplication a(argc, argv);
    a.setFont(QFont("Microsoft Yahei", 10));

    #if (QT_VERSION <= QT_VERSION_CHECK(5,0,0))
    #if _MSC_VER
        QTextCodec *codec = QTextCodec::codecForName("gbk");
    #else
        QTextCodec *codec = QTextCodec::codecForName("utf-8");
    #endif
        QTextCodec::setCodecForLocale(codec);
        QTextCodec::setCodecForCStrings(codec);
        QTextCodec::setCodecForTr(codec);
    #else
        QTextCodec *codec = QTextCodec::codecForName("utf-8");
        QTextCodec::setCodecForLocale(codec);
    #endif

    MainWindow w;
    w.setWindowTitle("PCL");
    w.setWindowIcon(QIcon(":/logo.ico"));
    w.show();

    return a.exec();
}
