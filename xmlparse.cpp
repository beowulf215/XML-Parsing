#include "xmlparse.h"




QString xmlFilePath("/home/brian/Desktop/ssv.xml");

int subcount = 0;
int hostcount = 0;
int proccount = 0;
int attcount = 0;
int activesect = 0; //Shows which area the code is parsing from in the xml

/* GUIDE FOR ACTIVESECT
 * 0 = Subsystem
 * 1 = Host
 * 2 = Interface
 * 3 = Process
 */


XMLParse::XMLParse()
{



}


void XMLParse::Parse() //In the end, change so that it returns a populated system data structure to the main program. You can pass in the filepath when done.

{

    QFile* file = new QFile(xmlFilePath);


    if(!file->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Cannot Read XML";
        return;
    }

    QXmlStreamReader xml(file);

    xml.readNext();
    xml.readNext();
    xml.readNext();


    sys sys_1; //Initialization of the system data structure.



    //EXAMPLE FOR HELP



   /*sys_1.subsystems.push_back(subsystem());

   sys_1.subsystems[0].hosts.push_back(host());

   sys_1.subsystems[0].hosts[0].processes.push_back(process());

   sys_1.subsystems[0].hosts[0].processes[0].procInterface.attributes.push_back(attribute());

   sys_1.subsystems[0].hosts[0].processes[0].procInterface.attributes[0].key = "THANK GOD";


    qDebug()<< sys_1.subsystems[0].hosts[0].processes[0].procInterface.attributes[0].key;*/

    //Beginning of data structure population from xml

    while (!xml.atEnd())
    {
        if(xml.isStartElement())
        {
            if(xml.name() == "subsystems")
                xml.readNext();

            if(xml.name() == "subsystem")
            {
                sys_1.subsystems.push_back(subsystem());
                activesect = 0;
                xml.readNext();
            }

            if(xml.name() == "name") //Make cases for all sections
            {
                if (activesect == 0) //Check for subsystem
                {
                    sys_1.subsystems[subcount].name = xml.readElementText();
                    qDebug() << sys_1.subsystems[subcount].name;
                }
            }
            else
            {
                xml.readNext();
            }
        }
        else
        {
            xml.readNext();
        }
    }







}
