#include "xmlparse.h"

int subcount;
int hostcount;
int proccount;
int attcount;
int activesect; //Shows which area the code is parsing from in the xml
int inter_stat; //Shows which interface is being read in the xml
int infoindex; //Index value for the index data structure for system bookkeeping

bool int_active; //Shows whether or not the xml is currently parsing inside data from an interface

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


XMLParse::XMLParse() //This is only a constructor for the XMLParse class
{
    subcount = 0;
    hostcount = 0;
    proccount = 0;
    attcount = 0;
    activesect = 0;
    inter_stat = 0;
    infoindex = -1; //Set to -1 for once a spot is pushed back on the index array, it will increment and the first value will be on index 0
    int_active = false;
}


sys XMLParse::Parse(QString xmlFilePath) //This function returns a data structure that is populated with information from the SSV Schema xml. The file path to the xml schema will be required to be passed into the function at declaration.

{

    QFile* file = new QFile(xmlFilePath);


    sys sys_1; //Initialization of the system data structure.
    qDebug() << "System structure initiated";//DEBUG


    if(!file->open(QIODevice::ReadOnly | QIODevice::Text)) // This will test if the xml schema is readable. If not, an empty data structure will be returned along with a debug message.
    {
        qDebug() << "Cannot Read XML";
        return sys_1;
    }

    QXmlStreamReader xml(file);




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

            if(xml.name() == "hosts")//Hosts Parsing Rule
            {
                qDebug() << "Hosts Recognized";//DEBUG
            }

            if(xml.name() == "processes")//Processes Parsing Rule
            {
                qDebug() << "Processes Recognized";//DEBUG
            }

            if(xml.name() == "attributes")//Attributes Parsing Rule
            {
                if (inter_stat == 0)//Check for Subsystem Interface
                    qDebug() << "Subsystem Interface Attributes Recognized";//DEBUG
            }

            if(xml.name() == "subsystem")//Subsystem Parsing Rule
            {
                processSubsystem(sys_1);
            }

            if(xml.name() == "host")//Host Parsing Rule
            {
                processHost(sys_1);
            }

            if(xml.name() == "attribute")//Attribute Parsing Rule
            {
                processAtt(sys_1);
            }

            if(xml.name() == "interface")//Interface Parsing Rule
            {
                processInterface(sys_1);
            }

            if(xml.name() == "process")//Process Parsing Rule
            {
                processProc(sys_1);
            }

            //END PROGRAM FLAG SECTION




            //COMMON SECTION: Tags in the xml that are located in different tiers of the SSV

            if(xml.name() == "name") //Check for name tag
            {
                processName(sys_1, xml);
            }

            if(xml.name() == "status_path") //Check for status_path tag
            {
                processStatus_Path(sys_1, xml);
            }
            //END COMMON SECTION


            //SHAPE SECTION: Tags that are synonymous with the shape of the SSV node

            if(xml.name() == "style") //Check for style tag
            {
                processStyle(sys_1, xml);
            }

            if(xml.name() == "color") //Check for color tag
            {
                processColor(sys_1, xml);
            }

            if(xml.name() == "border") //Check for border tag
            {
                processBorder(sys_1, xml);
            }

            if(xml.name() == "image_url") //Check for image_url tag
            {
                processImage_URL(sys_1, xml);
            }
            //END SHAPE SECTION


            //HOST SECTION: Tags that are synonymous with the host sections of the SSV

            if(xml.name() == "dns") //Check for dns tag
            {
                processDNS(sys_1, xml);
            }
            //END HOST SECTION


            //INTERFACE SECTION: Tags that are synonymous with the interface sections of the SSV

            if(xml.name() == "label") //Check for label tag
            {
                processLabel(sys_1, xml);
            }

            if(xml.name() == "target") //Check for target tag
            {
                processTarget(sys_1, xml);
            }

            if(xml.name() == "direction") //Check for direction tag
            {
                processDirection(sys_1, xml);
            }
            //END INTERFACE SECTION


            //ATTRIBUTE SECTION: Tags that are synonymous with the attribute sections of the SSV

            if(xml.name() == "key") //Check for key tag
            {
                processKey(sys_1, xml);
            }

            if(xml.name() == "type") //Check for type tag
            {
                processType(sys_1, xml);
            }

            if(xml.name() == "value") //Check for value tag
            {
                processValue(sys_1, xml);
            }
            //END ATTRIBUTE SECTION


            //PROCESS SECTION: Tags that are synonymous with the process sections of the SSV

            if(xml.name() == "group") //Check for group tag
            {
                processGroup(sys_1, xml);
            }
            //END PROCESS SECTION

        }

        else if (token == QXmlStreamReader::EndElement) //Check for end tag
        {
            processEndTag(sys_1, xml);
        }

    }

    for (int i = 0; i <= infoindex; i++)
    {
        qDebug() << "\nSubindex: " << sys_1.index[i].subindex
                 << " Hostindex: " << sys_1.index[i].hostindex
                 << " Procindex: " << sys_1.index[i].procindex
                 << " Host Name: " << sys_1.index[i].hostname
                 << " Host DNS: " << sys_1.index[i].hostdns
                 << " Process Name: " << sys_1.index[i].procname
                 << " Status_Path: " << sys_1.index[i].statuspath
                 << " Interface Label: " << sys_1.index[i].ifacelabel
                 << " Interface Target: " <<  sys_1.index[i].ifacetarget
                 << " Interface Status_Path: " << sys_1.index[i].ifstatpath
                 << "\n";
    }

    return sys_1; //Returning the populated data structure

}


//BEGINNING OF TAG PROCESSING FUNCTIONS


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
    sys_1.index.push_back(info()); //Establish new entry for index for host
    infoindex++; //Increment index for bookkeeping
}

void XMLParse::processProc(sys &sys_1)
{
    sys_1.subsystems[subcount].hosts[hostcount].processes.push_back(process());
    activesect = 2;
    qDebug() << "Process Recognized";//DEBUG
    sys_1.index.push_back(info()); //Establish new entry for index for process
    infoindex++; //Increment index for bookkeeping
}


void XMLParse::processInterface(sys &sys_1)
{
    int_active = true;

    if (activesect == 0)
    {
        inter_stat = 0;
        qDebug() << "Subsystem Interface Recognized";//DEBUG
        sys_1.index.push_back(info()); //Establish new entry for index for subsystem
        infoindex++; //Increment index for bookkeeping
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

void XMLParse::processAtt(sys &sys_1)
{
    if (inter_stat == 0)
    {
        sys_1.subsystems[subcount].subInterface.attributes.push_back(attribute());
        qDebug() << "Subsystem Interface Attribute Recognized";//DEBUG
    }

    if (inter_stat == 1)
    {
        sys_1.subsystems[subcount].hosts[hostcount].hostInterface.attributes.push_back(attribute());
        qDebug() << "Host Interface Attribute Recognized";//DEBUG
    }

    if (inter_stat == 2)
    {
        sys_1.subsystems[subcount].hosts[hostcount].processes[proccount].procInterface.attributes.push_back(attribute());
        qDebug() << "Process Interface Attribute Recognized";//DEBUG
    }
}
//END PROGRAM FLAG SECTION


//COMMON SECTION

void XMLParse::processName(sys &sys_1, QXmlStreamReader &xml)
{
    if (activesect == 0) //Check for subsystem
    {
        sys_1.subsystems[subcount].name = xml.readElementText();
        qDebug() << "Subsystem #" << subcount << " name:" << sys_1.subsystems[subcount].name;//DEBUG
    }
    else if (activesect == 1) //Check for host
    {
        sys_1.subsystems[subcount].hosts[hostcount].name = xml.readElementText();
        qDebug() << "Host #" << hostcount << " name:" << sys_1.subsystems[subcount].hosts[hostcount].name;//DEBUG

        //Index entry for host
        sys_1.index[infoindex].subindex = subcount;
        sys_1.index[infoindex].hostindex = hostcount;
        sys_1.index[infoindex].procindex = -1; //The -1 signifies that the index is not catorizing a process at the moment
        sys_1.index[infoindex].hostname = sys_1.subsystems[subcount].hosts[hostcount].name;
        //End of index entry

    }
    else if (activesect == 2) //Check for process
    {
        sys_1.subsystems[subcount].hosts[hostcount].processes[proccount].name = xml.readElementText();
        qDebug() << "Process #" << proccount << " name:" << sys_1.subsystems[subcount].hosts[hostcount].processes[proccount].name;//DEBUG
        //Index entry for host
        sys_1.index[infoindex].subindex = subcount;
        sys_1.index[infoindex].hostindex = hostcount;
        sys_1.index[infoindex].procindex = proccount;
        sys_1.index[infoindex].procname = sys_1.subsystems[subcount].hosts[hostcount].processes[proccount].name;
        //End of index entry
    }
}

void XMLParse::processStatus_Path(sys &sys_1, QXmlStreamReader &xml)
{
    if (int_active && inter_stat == 0) //Case for checking for an active interface within a subsystem
    {
        sys_1.subsystems[subcount].subInterface.status_path = xml.readElementText();
        qDebug() << "Subsystem #" << subcount << "Interface Status Path:" << sys_1.subsystems[subcount].subInterface.status_path;//DEBUG
        sys_1.index[infoindex].ifstatpath = sys_1.subsystems[subcount].subInterface.status_path;
    }

    if (!int_active && activesect == 1) // Case for checking for a host
    {
        sys_1.subsystems[subcount].hosts[hostcount].status_path = xml.readElementText();
        qDebug() << "Host #" << hostcount << " Status Path:" << sys_1.subsystems[subcount].hosts[hostcount].status_path;
        sys_1.index[infoindex].statuspath = sys_1.subsystems[subcount].hosts[hostcount].status_path;
    }

    if (int_active && activesect == 1) // Case for checking a host's interface
    {
        sys_1.subsystems[subcount].hosts[hostcount].hostInterface.status_path = xml.readElementText();
        qDebug() << "Host #" << hostcount << " Interface Status Path:" << sys_1.subsystems[subcount].hosts[hostcount].hostInterface.status_path;//DEBUG
        sys_1.index[infoindex].ifstatpath = sys_1.subsystems[subcount].hosts[hostcount].hostInterface.status_path;
    }

    if (!int_active && activesect == 2) //Case for checking for a process
    {
        sys_1.subsystems[subcount].hosts[hostcount].processes[proccount].status_path = xml.readElementText();
        qDebug() << "Process #" << proccount << " Process Status Path for Host #" << hostcount << ": " << sys_1.subsystems[subcount].hosts[hostcount].processes[proccount].status_path;//DEBUG
        sys_1.index[infoindex].statuspath = sys_1.subsystems[subcount].hosts[hostcount].processes[proccount].status_path;
    }

    if (int_active && activesect == 2) //Case for checking for an active interface within a subsystem
    {
        sys_1.subsystems[subcount].hosts[hostcount].processes[proccount].procInterface.status_path = xml.readElementText();
        qDebug() << "Process #" << proccount << " Process Status Path for Host #" << hostcount << " for Process Interface: " << sys_1.subsystems[subcount].hosts[hostcount].processes[proccount].procInterface.status_path;//DEBUG
        sys_1.index[infoindex].ifstatpath = sys_1.subsystems[subcount].hosts[hostcount].processes[proccount].procInterface.status_path;
    }
}
//END COMMON SECTION


//SHAPE SECTION

void XMLParse::processStyle(sys &sys_1, QXmlStreamReader &xml)
{
    if (activesect == 0) //Check for subsystem
    {
        sys_1.subsystems[subcount].subsysShape.style = xml.readElementText();
        qDebug() << "Subsystem #" << subcount << " style:" << sys_1.subsystems[subcount].subsysShape.style;//DEBUG
    }

    if (activesect == 1) //Check for host
    {
        sys_1.subsystems[subcount].hosts[hostcount].hostShape.style = xml.readElementText();
        qDebug() << "Host #" << hostcount << " Style:" << sys_1.subsystems[subcount].hosts[hostcount].hostShape.style;//DEBUG
    }

    if (activesect == 2) //Check for process
    {
        sys_1.subsystems[subcount].hosts[hostcount].processes[proccount].procShape.style = xml.readElementText();
        qDebug() << "Process #" << proccount << " Style:" << sys_1.subsystems[subcount].hosts[hostcount].processes[proccount].procShape.style;//DEBUG
    }
}

void XMLParse::processColor(sys &sys_1, QXmlStreamReader &xml)
{
    if (activesect == 0) //Check for subsystem
    {
        sys_1.subsystems[subcount].subsysShape.color = xml.readElementText();
        qDebug() << "Subsystem #" << subcount << " color:" << sys_1.subsystems[subcount].subsysShape.color;//DEBUG
    }

    if (activesect == 1) //Check for host
    {
        sys_1.subsystems[subcount].hosts[hostcount].hostShape.color = xml.readElementText();
        qDebug() << "Host #" << hostcount << " Color:" << sys_1.subsystems[subcount].hosts[hostcount].hostShape.color;//DEBUG
    }

    if (activesect == 2) //Check for process
    {
        sys_1.subsystems[subcount].hosts[hostcount].processes[proccount].procShape.color = xml.readElementText();
        qDebug() << "Process #" << proccount << " Color:" << sys_1.subsystems[subcount].hosts[hostcount].processes[proccount].procShape.color;//DEBUG
    }
}

void XMLParse::processBorder(sys &sys_1, QXmlStreamReader &xml)
{
    if (activesect == 0) //Check for subsystem
    {
        sys_1.subsystems[subcount].subsysShape.border = xml.readElementText();
        qDebug() << "Subsystem #" << subcount << " border:" << sys_1.subsystems[subcount].subsysShape.border;//DEBUG
    }

    if (activesect == 1) //Check for host
    {
        sys_1.subsystems[subcount].hosts[hostcount].hostShape.border = xml.readElementText();
        qDebug() << "Host #" << hostcount << " border:" << sys_1.subsystems[subcount].hosts[hostcount].hostShape.border;//DEBUG
    }

    if (activesect == 2) //Check for process
    {
        sys_1.subsystems[subcount].hosts[hostcount].processes[proccount].procShape.border = xml.readElementText();
        qDebug() << "Process #" << proccount << " border:" << sys_1.subsystems[subcount].hosts[hostcount].processes[proccount].procShape.border;//DEBUG
    }
}

void XMLParse::processImage_URL(sys &sys_1, QXmlStreamReader &xml)
{
    if (activesect == 0) //Check for subsystem
    {
        sys_1.subsystems[subcount].subsysShape.image_url = xml.readElementText();
        qDebug() << "Subsystem #" << subcount << " imageURL:" << sys_1.subsystems[subcount].subsysShape.image_url;//DEBUG
    }

    if (activesect == 1) //Check for host
    {
        sys_1.subsystems[subcount].hosts[hostcount].hostShape.image_url = xml.readElementText();
        qDebug() << "Host #" << hostcount << " imageURL:" << sys_1.subsystems[subcount].hosts[hostcount].hostShape.image_url;//DEBUG
    }

    if (activesect == 2) //Check for process
    {
        sys_1.subsystems[subcount].hosts[hostcount].processes[proccount].procShape.image_url = xml.readElementText();
        qDebug() << "Process #" << proccount << " imageURL:" << sys_1.subsystems[subcount].hosts[hostcount].processes[proccount].procShape.image_url;//DEBUG
    }
}
//END SHAPE SECTION



//HOST SECTION

void XMLParse::processDNS(sys &sys_1, QXmlStreamReader &xml)
{
    sys_1.subsystems[subcount].hosts[hostcount].dns = xml.readElementText();
    qDebug() << "Host #" << hostcount << " dns:" << sys_1.subsystems[subcount].hosts[hostcount].dns;//DEBUG
    //Index entry for host
    sys_1.index[infoindex].subindex = subcount;
    sys_1.index[infoindex].hostindex = hostcount;
    sys_1.index[infoindex].procindex = -1; //The -1 signifies that the index is not catagorizing a process at the moment
    sys_1.index[infoindex].hostdns = sys_1.subsystems[subcount].hosts[hostcount].dns;
    //End of index entry
}
//END HOST SECTION


//INTERFACE SECTION

void XMLParse::processLabel(sys &sys_1, QXmlStreamReader &xml)
{
    if (inter_stat == 0) //Check for subsystem interface
    {
        sys_1.subsystems[subcount].subInterface.label = xml.readElementText();
        qDebug() << "Subsystem #" << subcount << " Interface Label: " << sys_1.subsystems[subcount].subInterface.label;//DEBUG

        //Start of interface label indexing
        sys_1.index[infoindex].ifacelabel = sys_1.subsystems[subcount].subInterface.label;
        sys_1.index[infoindex].subindex = subcount;
        sys_1.index[infoindex].hostindex = -1;//The -1 signifies that the index is not catagorizing a host at the moment
        sys_1.index[infoindex].procindex = -1;//The -1 signifies that the index is not catagorizing a process at the moment
        //End of interface label indexing
    }

    if (inter_stat == 1) //Check for host interface
    {
        sys_1.subsystems[subcount].hosts[hostcount].hostInterface.label = xml.readElementText();
        qDebug() << "Host #" << hostcount << " Interface Label: " << sys_1.subsystems[subcount].hosts[hostcount].hostInterface.label;//DEBUG

        //Start of interface label indexing
        sys_1.index[infoindex].ifacelabel = sys_1.subsystems[subcount].hosts[hostcount].hostInterface.label;
        //End of interface label indexing
    }

    if (inter_stat == 2) //Check for process interface
    {
        sys_1.subsystems[subcount].hosts[hostcount].processes[proccount].procInterface.label = xml.readElementText();
        qDebug() << "Process #" << proccount << " Interface Label: " << sys_1.subsystems[subcount].hosts[hostcount].processes[proccount].procInterface.label;//DEBUG

        //Start of interface label indexing
        sys_1.index[infoindex].ifacelabel = sys_1.subsystems[subcount].hosts[hostcount].processes[proccount].procInterface.label;
        //End of interface label indexing
    }
}

void XMLParse::processTarget(sys &sys_1, QXmlStreamReader &xml)
{
    if (inter_stat == 0) //Check for subsystem interface
    {
        sys_1.subsystems[subcount].subInterface.target = xml.readElementText();
        qDebug() << "Subsystem #" << subcount << " Interface Target: " << sys_1.subsystems[subcount].subInterface.target;//DEBUG

        //Start of interface target indexing for subsystem
        sys_1.index[infoindex].ifacetarget = sys_1.subsystems[subcount].subInterface.target;
        //End of interface target indexing
    }

    if (inter_stat == 1) //Check for host interface
    {
        sys_1.subsystems[subcount].hosts[hostcount].hostInterface.target = xml.readElementText();
        qDebug() << "Host #" << hostcount << " Interface Target: " << sys_1.subsystems[subcount].hosts[hostcount].hostInterface.target;//DEBUG

        //Start of interface target indexing for host
        sys_1.index[infoindex].ifacetarget = sys_1.subsystems[subcount].hosts[hostcount].hostInterface.target;
        //End of interface target indexing
    }

    if (inter_stat == 2) //Check for process interface
    {
        sys_1.subsystems[subcount].hosts[hostcount].processes[proccount].procInterface.target= xml.readElementText();
        qDebug() << "Process #" << proccount << " Interface Target: " << sys_1.subsystems[subcount].hosts[hostcount].processes[proccount].procInterface.target;//DEBUG

        //Start of interface target indexing for process
        sys_1.index[infoindex].ifacetarget = sys_1.subsystems[subcount].hosts[hostcount].processes[proccount].procInterface.target;
        //End of interface target indexing
    }
}

void XMLParse::processDirection(sys &sys_1, QXmlStreamReader &xml)
{
    if (inter_stat == 0) //Check for subsystem interface
    {
        sys_1.subsystems[subcount].subInterface.direction = xml.readElementText();
        qDebug() << "Subsystem #" << subcount << "Interface Direction: " << sys_1.subsystems[subcount].subInterface.direction;//DEBUG
    }

    if (inter_stat == 1) //Check for host interface
    {
        sys_1.subsystems[subcount].hosts[hostcount].hostInterface.direction = xml.readElementText();
        qDebug() << "Host #" << hostcount << "Interface Direction: " << sys_1.subsystems[subcount].hosts[hostcount].hostInterface.direction;//DEBUG
    }

    if (inter_stat == 2) //Check for process interface
    {
        sys_1.subsystems[subcount].hosts[hostcount].processes[proccount].procInterface.direction = xml.readElementText();
        qDebug() << "Process #" << proccount << "Interface Direction: " << sys_1.subsystems[subcount].hosts[hostcount].processes[proccount].procInterface.direction;//DEBUG
    }
}
//END INTERFACE SECTION


//ATTRIBUTE SECTION

void XMLParse::processKey(sys &sys_1, QXmlStreamReader &xml)
{
    if (inter_stat == 0) //Check for subsystem interface
    {
        sys_1.subsystems[subcount].subInterface.attributes[attcount].key = xml.readElementText();
        qDebug() << "Key for attribute " << attcount << " for subsystem #" << subcount << ": " << sys_1.subsystems[subcount].subInterface.attributes[attcount].key;
    }

    if (inter_stat == 1) //Check for host interface
    {
        sys_1.subsystems[subcount].hosts[hostcount].hostInterface.attributes[attcount].key = xml.readElementText();
        qDebug() << "Key for attribute " << attcount << " for host #" << hostcount << ": " << sys_1.subsystems[subcount].hosts[hostcount].hostInterface.attributes[attcount].key;
    }

    if (inter_stat == 2) //Check for process interface
    {
        sys_1.subsystems[subcount].hosts[hostcount].processes[proccount].procInterface.attributes[attcount].key = xml.readElementText();
        qDebug() << "Key for attribute " << attcount << " for process #" << proccount << ": " << sys_1.subsystems[subcount].hosts[hostcount].processes[proccount].procInterface.attributes[attcount].key;
    }
}

void XMLParse::processType(sys &sys_1, QXmlStreamReader &xml)
{
    if (inter_stat == 0) //Check for subsystem interface
    {
        sys_1.subsystems[subcount].subInterface.attributes[attcount].type = xml.readElementText();
        qDebug() << "Type for attribute " << attcount << " for subsystem #" << subcount << ": " << sys_1.subsystems[subcount].subInterface.attributes[attcount].type;
    }

    if (inter_stat == 1) //Check for host interface
    {
        sys_1.subsystems[subcount].hosts[hostcount].hostInterface.attributes[attcount].type = xml.readElementText();
        qDebug() << "Type for attribute " << attcount << " for host #" << hostcount << ": " << sys_1.subsystems[subcount].hosts[hostcount].hostInterface.attributes[attcount].type;
    }

    if (inter_stat == 2) //Check for process interface
    {
        sys_1.subsystems[subcount].hosts[hostcount].processes[proccount].procInterface.attributes[attcount].type = xml.readElementText();
        qDebug() << "Type for attribute " << attcount << " for process #" << proccount << ": " << sys_1.subsystems[subcount].hosts[hostcount].processes[proccount].procInterface.attributes[attcount].type;
    }
}

void XMLParse::processValue(sys &sys_1, QXmlStreamReader &xml)
{
    if (inter_stat == 0) //Check for subsystem interface
    {
        sys_1.subsystems[subcount].subInterface.attributes[attcount].value = xml.readElementText();
        qDebug() << "Value for attribute " << attcount << " for subsystem #" << subcount << ": " << sys_1.subsystems[subcount].subInterface.attributes[attcount].value;
    }

    if (inter_stat == 1) //Check for host interface
    {
        sys_1.subsystems[subcount].hosts[hostcount].hostInterface.attributes[attcount].value = xml.readElementText();
        qDebug() << "Value for attribute " << attcount << " for host #" << hostcount << ": " << sys_1.subsystems[subcount].hosts[hostcount].hostInterface.attributes[attcount].value;
    }

    if (inter_stat == 2) //Check for process interface
    {
        sys_1.subsystems[subcount].hosts[hostcount].processes[proccount].procInterface.attributes[attcount].value = xml.readElementText();
        qDebug() << "Value for attribute " << attcount << " for process #" << proccount << ": " << sys_1.subsystems[subcount].hosts[hostcount].processes[proccount].procInterface.attributes[attcount].value;
    }
}
//END ATTRIBUTE SECTION


//PROCESS SECTION

void XMLParse::processGroup(sys &sys_1, QXmlStreamReader &xml)
{
    sys_1.subsystems[subcount].hosts[hostcount].processes[proccount].group = xml.readElementText();
    qDebug() << "Group for Process "<< proccount << " for host " << hostcount << ": " << sys_1.subsystems[subcount].hosts[hostcount].processes[proccount].group;
}
//END PROCESS SECTION


//END TAG SECTION
void XMLParse::processEndTag(sys &sys_1, QXmlStreamReader &xml) //This is the code to handle the end tags of the xml file and to increase the appropriate counters
{
    if (xml.name() == "attributes")
    {
        attcount = 0;
        qDebug() << "Attribute count reset!"; //DEBUG
    }

    if (xml.name() == "attribute")
    {
        if (inter_stat == 0) //Check for subsystem interface
        {
            sys_1.subsystems[subcount].subInterface.attcount++;
            attcount++;
            qDebug() << "Subsystem Attribute #" << attcount-1 << " complete! Starting Subsystem Attribute#" << attcount; //DEBUG
        }

        if (inter_stat == 1) //Check for host interface
        {
            sys_1.subsystems[subcount].hosts[hostcount].hostInterface.attcount++;
            attcount++;
            qDebug() << "Host Attribute #" << attcount-1 << " complete! Starting Host Attribute#" << attcount; //DEBUG
        }

        if (inter_stat == 2) //Check for process interface
        {
            sys_1.subsystems[subcount].hosts[hostcount].processes[proccount].procInterface.attcount++;
            attcount++;
            qDebug() << "Process Attribute #" << attcount-1 << " complete! Starting Process Attribute#" << attcount; //DEBUG
        }
    }

    if (xml.name() == "subsystem")
    {
        sys_1.subsyscnt++;
        subcount++;
        qDebug() << "Subsystem #" << subcount-1 << " complete! Starting Subsystem #" << subcount; //DEBUG
    }

    if (xml.name() == "host")
    {
        sys_1.subsystems[subcount].hostcount++;
        hostcount++;
        qDebug() << "Host #" << hostcount-1 << " complete! Starting Host #" << hostcount; //DEBUG
    }

    if (xml.name() == "hosts")
    {
        hostcount = 0;
        qDebug() << "Host count reset!"; //DEBUG
    }

    if (xml.name() == "process")
    {
        sys_1.subsystems[subcount].hosts[hostcount].processcount++;
        proccount++;
        qDebug() << "Process #" << proccount-1 << " complete! Starting Process #" << proccount; //DEBUG
    }

    if (xml.name() == "processes")
    {
        proccount = 0;
        qDebug() << "Process count reset"; //DEBUG
    }

    if (xml.name() == "interface")
    {
        int_active = false;
        qDebug() << "Interface no longer active";
    }
}
