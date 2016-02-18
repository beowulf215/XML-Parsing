#ifndef XMLPARSE_H
#define XMLPARSE_H
#include <QObject>
#include <QDebug>
#include <QXmlStreamReader>
#include <QFile>
#include <QDir>
#include <QVector>
#include <QTextCodec>

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
    QString value;
};

struct interface
{
    int attcount;
    QString label;
    QString target;
    QString status_path;
    QString status;
    QString direction;
    QVector<attribute> attributes; //Dynamic array of attribute structures
};

struct process
{
    QString name;
    QString group;
    shape procShape;
    QString status_path;
    QString status;
    interface procInterface;
};

struct host
{
    int processcount;
    QString name;
    QString dns;
    shape hostShape;
    QString status_path;
    QString status;
    interface hostInterface;
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


class XMLParse
{



public:
    XMLParse();
    sys Parse(QString xmlFilePath);

    //FLAG SECTION
    void processSubsystem(sys &sys_1);
    void processHost(sys &sys_1);
    void processInterface();
    void processProc(sys &sys_1);
    void processAtt(sys &sys_1);
    //END FLAG

    //COMMON SECTION
    void processName(sys &sys_1, QXmlStreamReader &xml);
    void processStatus_Path(sys &sys_1, QXmlStreamReader &xml);
    //END COMMON

    //SHAPE SECTION
    void processStyle(sys &sys_1, QXmlStreamReader &xml);
    void processColor(sys &sys_1, QXmlStreamReader &xml);
    void processBorder(sys &sys_1, QXmlStreamReader &xml);
    void processImage_URL(sys &sys_1, QXmlStreamReader &xml);
    //END SHAPE

    //HOST SECTION
    void processDNS(sys &sys_1, QXmlStreamReader &xml);
    //END HOST

    //INTERFACE SECTION
    void processLabel(sys &sys_1, QXmlStreamReader &xml);
    void processTarget(sys &sys_1, QXmlStreamReader &xml);
    void processDirection(sys &sys_1, QXmlStreamReader &xml);
    //END INTERFACE

    //ATTRIBUTE SECTION
    void processKey(sys &sys_1, QXmlStreamReader &xml);
    void processType(sys &sys_1, QXmlStreamReader &xml);
    void processValue(sys &sys_1, QXmlStreamReader &xml);
    //END ATTRIBUTE

    //PROCESS SECTION
    void processGroup(sys &sys_1, QXmlStreamReader &xml);
    //END PROCESS


    void processEndTag(sys &sys_1, QXmlStreamReader &xml);

private:

};



#endif // XMLPARSE_H
