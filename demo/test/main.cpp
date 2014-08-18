#include <QCoreApplication>
#include <QDebug>
#include <QRegExp>

#include <iostream>
//#include "../../src/core/functions.h"
//#include "../../src/core/types.h"

using namespace std;

//static const QString variablesRegExp("([^\\\\]\\$|^\\$)\\{(\\w+)\\}");
static const QString variablesRegExp("(?:^|[^\\\\](?=\\$))\\$\\{(\\w+)\\}");
int variablesRegExpNum = 1;
int variablesRegExpPosDelta = 1;

QSet<QString> findVariables(const QString & str)
{
    QSet<QString> set;

    QRegExp rx(variablesRegExp);
    rx.setMinimal(true);
    int pos = 0;
    while (pos >= 0) {
        pos = rx.indexIn(str, pos);
        if (pos != -1) {
            qDebug() << rx.cap(0) << rx.cap(1) << rx.cap(2) << rx.cap(3);
            set.insert( rx.cap(variablesRegExpNum) );
            pos += rx.cap(0).length();
        }
    }

    return set;
}


QString setVariablesValue(const QString & str, const QVariantHash & values)
{
    QString strOut = str;
    QRegExp rx(variablesRegExp);
    int pos = 0;
    while (pos >= 0) {
        pos = rx.indexIn(strOut, pos);
        if (pos != -1) {
            QString strOrig = rx.cap(0);
            QString varName = rx.cap(variablesRegExpNum);
            QString varValue = values.value(varName).toString();
            strOut.replace(QString("${%1}").arg(varName), varValue);
            pos += strOrig.length();
        }
    }
    return strOut;
}



int main(int /*argc*/, char * /*argv[]*/)
{
    QString str1("${test1}");
    QString str2("\\${test1}");
    QString str3("sadfsdfsdf s sdf sfsdfsdfs${test2}sfsdfsdfsdfsd");
    QString str4("sadfsdfsdf s sdf sfsdfsdfs\\${test2}sfsdfsdfsdfsd");
    qDebug() << str1;
//    qDebug() << str2;
    qDebug() << str3;
//    qDebug() << str4;
    qDebug() << "-----------------------------------";
    findVariables(str1).values();
//    findVariables(str2).values();
    findVariables(str3).values();
//    findVariables(str4).values();

    QVariantHash hash;
    hash.insert("test1", "result1");
    hash.insert("test2", "result2");
    qDebug() << setVariablesValue(str1, hash);
    qDebug() << setVariablesValue(str3, hash);
}
