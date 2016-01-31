#ifndef XMLPARSE_H
#define XMLPARSE_H
#include <QObject>
#include <QDebug>
#include <QXmlStreamReader>
#include <QFile>
#include <QDir>
#include <QVector>

class XMLParse
{

    //Data Structures for the System Status Viewer
    struct shape
    {
        QString style;
        QString color;
        QString border;
    };

    struct attribute
    {
        QString key;
        QString type;
    };

    struct interface
    {
        QString label;
        QString target;
        QString status_path;
        QString direction;
        QVector<attribute> attributes; //Dynamic array of attribute structures
    };

    struct process
    {
        QString name;
        QString group;
        shape procShape;
        QString status_path;
        interface procInterface;
    };

    struct host
    {
        QString name;
        QString dns;
        shape hostShape;
        QVector<process> processes; //Dynamic array of process structures
    };

    struct subsystem
    {
        QString name;
        shape subsysShape;
        interface subInterface;
        QVector<host> hosts; //Dynamic array of host structures
    };

    struct sys
    {
        QVector<subsystem> subsystems; //Dynamic array of subsystem structures
    };



public:
    XMLParse();
    void Parse();


private:

};



#endif // XMLPARSE_H
