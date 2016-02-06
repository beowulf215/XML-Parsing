#include "xmlparse.h"


QString xmlFilePath("/home/brian/Desktop/ssviso.xml");

int subcount;
int hostcount;
int proccount;
int attcount;
int activesect; //Shows which area the code is parsing from in the xml
int inter_stat; //Shows which interface is being read in the xml

/* GUIDE FOR ACTIVESECT
 * 0 = Subsystem
 * 1 = Host
 * 2 = Process
 */

/* GUIDE FOR INTER_STAT
 * 0 = Subsystem Interface
 * 1 = Host Interface
 * 2 = Process Interface
 */


XMLParse::XMLParse()
{
    subcount = 0;
    hostcount = 0;
    proccount = 0;
    attcount = 0;
    activesect = 0;
    inter_stat = 0;
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


    sys sys_1; //Initialization of the system data structure.
    qDebug() << "System structure initiated";//DEBUG


    //EXAMPLE FOR HELP

   /*sys_1.subsystems.push_back(subsystem());

   sys_1.subsystems[0].hosts.push_back(host());

   sys_1.subsystems[0].hosts[0].processes.push_back(process());

   sys_1.subsystems[0].hosts[0].processes[0].procInterface.attributes.push_back(attribute());

   sys_1.subsystems[0].hosts[0].processes[0].procInterface.attributes[0].key = "TEST";


    qDebug()<< sys_1.subsystems[0].hosts[0].processes[0].procInterface.attributes[0].key;*/
    //END EXAMPLE



    //Beginning of data structure population from xml

    while (!xml.atEnd() && !xml.hasError())
    {
        QXmlStreamReader::TokenType token = xml.readNext();
        //qDebug() << "Token Reader Initiated";//DEBUG

      if(token == QXmlStreamReader::StartDocument)
        {
           xml.readNext();
           qDebug() << "Start of Document Recognized"; //DEBUG
        }


        if(token == QXmlStreamReader::StartElement)
        {

            //PROGRAM FLAG SECTION: This is for the program to know which section of the XML it is parsing

            if(xml.name() == "subsystems")//Subsystems Parsing Rule
            {
                qDebug() << "Subsystems Recognized";//DEBUG
            }

            if(xml.name() == "subsystem")//Subsystem Parsing Rule
            {
                processSubsystem(sys_1);
            }

            if(xml.name() == "host")//Host Parsing Rule
            {
                processHost(sys_1);
            }

            if(xml.name() == "interface")//Interface Parsing Rule
            {
                processInterface();
            }

            //END PROGRAM FLAG SECTION





            if(xml.name() == "name") //Make cases for all sections for setting name
            {
                processName(sys_1, xml);
            }





            //SHAPE SECTION

            if(xml.name() == "style") //Make cases for all sections for setting style
            {
                processStyle(sys_1, xml);
            }

            if(xml.name() == "color") //Make cases for all sections of setting color
            {
                processColor(sys_1, xml);
            }

            if(xml.name() == "border") //Make cases for all sections of setting border
            {
                processBorder(sys_1, xml);
            }

            if(xml.name() == "image_url") //Make cases for all sections of setting the image_url
            {
                processImage_URL(sys_1, xml);
            }

            //END SHAPE SECTION





            //INTERFACE SECTION


            if(xml.name() == "label") //Make cases for all sections of setting interface label
            {
                processLabel(sys_1, xml);
            }

            if(xml.name() == "target") //Make cases for all sections of setting interface target
            {
                processTarget(sys_1, xml);
            }

            //END INTERFACE SECTION


            else
            {
                //xml.readNext();
            }
        }

        else if (token == QXmlStreamReader::EndElement)
        {
            qDebug() << "FOUND END";
        }

        else
        {
            //xml.readNext();
        }
    }

}


//PROGRAM FLAG SECTION
void XMLParse::processSubsystem(sys &sys_1)
{
    sys_1.subsystems.push_back(subsystem());
    activesect = 0;
    qDebug() << "Subsystem Recognized";//DEBUG
}


void XMLParse::processHost(sys &sys_1)
{
    sys_1.subsystems[subcount].hosts.push_back(host());
    activesect = 1;
    qDebug() << "Host Recognized";//DEBUG
}


void XMLParse::processInterface()
{
    if (activesect == 0)
    {
        inter_stat = 0;
        qDebug() << "Subsystem Interface Recognized";//DEBUG
    }
    else if (activesect == 1)
    {
        inter_stat = 1;
        qDebug() << "Host Interface Recognized";//DEBUG
    }
    else if (activesect == 2)
    {
        inter_stat = 2;
        qDebug() << "Process Interface Recognized";//DEBUG
    }
}

//END PROGRAM FLAG SECTION



void XMLParse::processName(sys &sys_1, QXmlStreamReader &xml)
{
    if (activesect == 0) //Check for subsystem
    {
        sys_1.subsystems[subcount].name = xml.readElementText();
        qDebug() << "Subsystem #" << subcount << " name:" << sys_1.subsystems[subcount].name;
    }
    else if (activesect == 1) //Check for host
    {
        sys_1.subsystems[subcount].hosts[hostcount].name = xml.readElementText();
        qDebug() << "Host #" << hostcount << " name:" << sys_1.subsystems[subcount].hosts[hostcount].name;
    }
}



//SHAPE SECTION
void XMLParse::processStyle(sys &sys_1, QXmlStreamReader &xml)
{
    if (activesect == 0) //Check for subsystem
    {
        sys_1.subsystems[subcount].subsysShape.style = xml.readElementText();
        qDebug() << "Subsystem #" << subcount << " style:" << sys_1.subsystems[subcount].subsysShape.style;
    }
}

void XMLParse::processColor(sys &sys_1, QXmlStreamReader &xml)
{
    if (activesect == 0) //Check for subsystem
    {
        sys_1.subsystems[subcount].subsysShape.color = xml.readElementText();
        qDebug() << "Subsystem #" << subcount << " color:" << sys_1.subsystems[subcount].subsysShape.color;
    }
}

void XMLParse::processBorder(sys &sys_1, QXmlStreamReader &xml)
{
    if (activesect == 0) //Check for subsystem
    {
        sys_1.subsystems[subcount].subsysShape.border = xml.readElementText();
        qDebug() << "Subsystem #" << subcount << " border:" << sys_1.subsystems[subcount].subsysShape.border;
    }
}

void XMLParse::processImage_URL(sys &sys_1, QXmlStreamReader &xml)
{
    if (activesect == 0) //Check for subsystem
    {
        sys_1.subsystems[subcount].subsysShape.image_url = xml.readElementText();
        qDebug() << "Subsystem #" << subcount << " imageURL:" << sys_1.subsystems[subcount].subsysShape.image_url;
    }
}
//END SHAPE SECTION



//INTERFACE SECTION
void XMLParse::processLabel(sys &sys_1, QXmlStreamReader &xml)
{
    if (inter_stat == 0) //Check for subsystem interface
    {
        sys_1.subsystems[subcount].subInterface.label = xml.readElementText();
        qDebug() << "Subsystem #" << subcount << " Interface Label: " << sys_1.subsystems[subcount].subInterface.label;
    }
}

void XMLParse::processTarget(sys &sys_1, QXmlStreamReader &xml)
{
    if (inter_stat == 0) //Check for subsystem interface
    {
        sys_1.subsystems[subcount].subInterface.target = xml.readElementText();
        qDebug() << "Subsystem #" << subcount << " Interface Target: " << sys_1.subsystems[subcount].subInterface.target;
    }
}
