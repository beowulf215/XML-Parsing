#ifndef XMLPARSE_H
#define XMLPARSE_H
#include <QObject>
#include <QDebug>
#include <QXmlStreamReader>
#include <QFile>
#include <QDir>
#include <QVector>
#include <QTextCodec>

class XMLParse
{

    //Data Structures for the System Status Viewer
    struct shape
    {
        QString style;
        QString color;
        QString border;
        QString image_url;
    };

    struct attribute
    {
        QString key;
        QString type;
    };

    struct interface
    {
        int attcount;
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
        int processcount;
        QString name;
        QString dns;
        shape hostShape;
        QVector<process> processes; //Dynamic array of process structures
    };

    struct subsystem
    {
        int hostcount;
        QString name;
        shape subsysShape;
        interface subInterface;
        QVector<host> hosts; //Dynamic array of host structures
    };

    struct sys
    {
        int subsyscnt;
        QVector<subsystem> subsystems; //Dynamic array of subsystem structures
    };



public:
    XMLParse();
    void Parse();
    void processSubsystem(sys &sys_1);
    void processHost(sys &sys_1);
    void processInterface();

    void processName(sys &sys_1, QXmlStreamReader &xml);

    void processStyle(sys &sys_1, QXmlStreamReader &xml);
    void processColor(sys &sys_1, QXmlStreamReader &xml);
    void processBorder(sys &sys_1, QXmlStreamReader &xml);
    void processImage_URL(sys &sys_1, QXmlStreamReader &xml);

    void processLabel(sys &sys_1, QXmlStreamReader &xml);
    void processTarget(sys &sys_1, QXmlStreamReader &xml);

private:

};



#endif // XMLPARSE_H
