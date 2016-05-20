#include "var.h"
#include <QFile>
#include <QTextStream>

Var::Var()
{
    //Column=0;
}

Var::Var(QString json)
{
    json = json.replace("'", "\"");
    importFromJsonString(json);
}

Var::Var(QJsonDocument parser)
{
    importFromJsonDocument(parser);
}

Var::Var(QString string, QString name)
{
    setParam("string",name);
    String=string;
}

Var::Var(QMap<QString, QString> map,QString name)
{
    setParam("map",name);
    for(QMap<QString, QString>::const_iterator i = map.constBegin();i != map.constEnd();i++)
        Map.insert(i.key(),Var(i.value(),i.key()));
}
Var::Var(QList <QString> list,QString name, QString itemName)
{
    setParam("list",name);
    for(int i=0;i<list.size();i++)
        List<<Var(list[i],itemName);
}

void Var::importFromXml(QString filePath)
{
    QFile* file = new QFile(filePath);
	if(!file->open(QIODevice::ReadOnly))
    {
        qDebug()<<"Var::Var(QString filePath) - path is incorrect"<<filePath;
    }
	QXmlStreamReader xml(file);
	fromXML(xml);
	file->close();
}

void Var::setParam(QString type, QString name)
{
    setType(type);
    setName(name);
}
void Var::setType(QString type)
{
    typeList<<"map"<<"list"<<"string";
	if(typeList.contains(type)) Type=type;
	else qDebug()<<"Var::setType; type "<<type<<" is unsupported!";
}
void Var::setName(QString name)
{
    Name=name;
}
QString Var::getType() const
{
    return Type;
}
QString Var::getName() const
{
    return Name;
}

void Var::setString(QString string)
{
	String = string;
}

QString Var::getString() const
{
	if(getType()=="string")
		return String;
	else
		qDebug()<<"Var::getString() - var is not string. Name:"<<Name<<" Type:"<<Type;
	return QString("");
}

QList <Var> Var::getList() const
{
    return List;
}
QMap <QString, Var> Var::getMap() const
{
    return Map;
}

void Var::listToMap(QString key)
{
    if(Type == "list")
    {
        for(int i=0;i<List.size();i++)
        {
            if(List[i].contains(key))
            {
                Map.insert(List[i][key].toString(),List[i]);
            }
            else
            {
                qDebug()<<"Var::listToMap - not all items has key: "<<key;
            }
        }
        List.clear();
        Type = "map";
    }
    else
    {
        qDebug()<<"Var::listToMap - type is not list; Name = "<<Name<<"Type = "<<Type;
    }
}

void Var::insert(QString name,Var var)
{
	if(Type=="map")
    {
        var.setName(name);
		Map.insert(name,var);
    }
	else
		qDebug()<<"Var::insert - type is not map; Name = "<<Name<<"Type = "<<Type;
}
void Var::insert(Var var)
{
	if(Type=="map")
    {
        Map.insert(var.getName(),var);
    }
	else
		qDebug()<<"Var::insert - type is not map; Name = "<<Name<<"Type = "<<Type;
}
void Var::insert(QString name,QString string)
{
	Var varStr= Var(string,name);
	insert(name,varStr);
}
void Var::remove(QString name)
{
	if(Type=="map")
    {
        Map.remove(name);
    }
	else
		qDebug()<<"Var::remove - type is not map; Name = "<<Name<<"Type = "<<Type;
}

void Var::remove(int ind)
{
	if(Type=="list")
    {
        List.removeAt(ind);
    }
	else
		qDebug()<<"Var::remove - type is not list; Name = "<<Name<<"Type = "<<Type;
}

Var Var::value(QString name)
{
	if(Type=="map")
		return Map.value(name);
	else
    {
        qDebug()<<"Var::value - type is not map; Name = "<<Name<<"Type = "<<Type;
		return Var();
    }
}

void Var::append(Var var)
{
	if(Type=="list")
    {
		if(var.getType()=="list")
        {
			for(int i=0;i<var.size();i++)
				List.append(var[i]);
        }
		else
        {
			List.append(var);
        }
    }
	else
		qDebug()<<"Var::append - type is not list; Name = "<<Name<<"Type = "<<Type;
}

void Var::appendNR(Var var)
{
	if(Type=="list")
    {
        int f=0;
		for(int i=0;i<List.size();i++)
			if(List[i]==var)
				f=1;
		if(f==0)
			List.append(var);
    }
	else
		qDebug()<<"Var::append - type is not list; Name = "<<Name<<"Type = "<<Type;
}

void Var::appendNRinc(Var var,QString incKey)
{
	if(Type=="list")
    {
        int f=0;
		for(int i=0;i<List.size();i++)
			if(List[i]==var)
            {
                f=1;
				Var incVar=List[i];
				incVar.pval(incKey)->inc();
				List[i]=incVar;
            }
		if(f==0)
			List.append(var);
    }
	else
		qDebug()<<"Var::append - type is not list; Name = "<<Name<<"Type = "<<Type;
}

void Var::inc()
{
	if(getType()=="string"&&String.toInt()>=0)
    {
        String=QString::number(String.toInt()+1);
    }
}

QString Var::toXML()
{
	QString xml="";
	if(getType()=="string")
    {
        xml+="<"+getName()+">"+getString()+"</"+getName()+">";
    }
	if(getType()=="map")
    {
        xml+="<"+getName()+">";
		for(QMap<QString, Var>::const_iterator i = Map.constBegin();i != Map.constEnd();i++)
        {
            Var el=i.value();
			xml+=el.toXML();
        }
		xml+="</"+getName()+">";
    }
	if(getType()=="list")
    {
        xml+="<"+getName()+">";
		for(int i=0;i<List.size();i++)
			xml+=List[i].toXML();
		xml+="</"+getName()+">";
    }
	return xml;
}

Var Var::fromXML(QXmlStreamReader& xml)
{
	while (xml.tokenType() != QXmlStreamReader::StartElement && !xml.atEnd() && !xml.hasError())
    {
		xml.readNext();
    }
	QString root = xml.name().toString();
	setParam("map",root);
	while (!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == root) && !xml.atEnd() && !xml.hasError())
    {
        xml.readNext();
		if (xml.tokenType() == QXmlStreamReader::StartElement)
        {
            Var el;
			el.fromXML(xml);
			if(getType()=="map")
            {
                if(!Map.contains(el.getName()))
                {
                    Map.insert(el.getName(),el);
                }
				else
                {
                    setType("list");
					List<<Map.value(el.getName());
                }
            }
			if(getType()=="list")
            {
                List<<el;
            }
        }
		if (xml.tokenType() == QXmlStreamReader::Characters)
        {
            QString text=xml.text().toString();
			if(!text.startsWith('\n'))
            {
                setType("string");
				String = text;
            }
        }
    }
	return *this;
}

void Var::print(int level)
{
	char spaceStr[100];
	for(int i=0;i<level*4;i++)
    {
		if(i%4==0) spaceStr[i]='|';
		else spaceStr[i]=' ';
    }
		
	spaceStr[level*4]=0;
	
	if(getType()=="string")
    {
        //qDebug()<<spaceStr<<"type = "<<getType()<<" | name = "<<getName()<<" | column = "<<Column;
		qDebug()<<spaceStr<<"String = "<<String;
    }
	if(getType()=="map")
    {
		qDebug()<<spaceStr<<"type = "<<getType()<<" | name = "<<getName();
		spaceStr[level*4]='|';
		spaceStr[level*4+1]=0;
		for(QMap<QString, Var>::const_iterator i = Map.constBegin();i != Map.constEnd();i++)
        {
			qDebug()<<spaceStr<< i.key() << " : ";
			Var el=i.value();
			el.print(level+1);
        }
		spaceStr[level*4]='+';
		spaceStr[level*4+1]='-';
		spaceStr[level*4+2]=0;
		qDebug()<<spaceStr<<"map "<<getName()<<"end";
        //if(Column!=0)qDebug()<<Column<<" - column in map!!!!<<<<<<<<<<<<<<";
    }
	if(getType()=="list")
    {
        qDebug()<<spaceStr<<"type = "<<getType()<<" | name = "<<getName();
		for(int i=0;i<List.size();i++)
        {
            qDebug()<<spaceStr<<i<<" : ";
			List[i].print(level+1);
        }
		qDebug()<<spaceStr<<"list "<<getName()<<"end";
        //if(Column!=0)qDebug()<<Column<<" - column in list!!!!<<<<<<<<<<<<<<";
    }
	if(getType()=="string"&&getType()=="map"&&getType()=="list")
		qDebug()<<"type is not valid!<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<";
}

bool Var::operator == (QString string)
{
	if(getString()==string)return true;
	return false;
}

bool Var::operator == (Var var) const
{
	if(Type==var.getType())
    {
		if(Type=="string")
			if(String==var.getString())
				return true;
		if(Type=="list")
			if(List.size()==var.getList().size())
            {
				for(int i=0;i<List.size();i++)
					if(List[i]!=var.getList()[i])
						return false;
				return true;
            }
		if(Type=="map")
			if(Map.size()==var.getMap().size())
            {
                for(QMap<QString, Var>::const_iterator i = Map.constBegin();i != Map.constEnd();i++)
					if(i.value()!=var.getMap().value(i.key()))
						return false;
				return true;
            }
    }
	return false;
}

bool Var::operator !=(Var var) const
{
	if(*this==var)return false;
	return true;
}

bool Var::operator !=(QString string) const
{
	if(String!=string)return true;
	return false;
}

int Var::toInt() const
{
    return getString().toInt();
}

QString Var::toString() const
{
    return getString();
}

QVariant Var::toQVar() const
{
    return QVariant(getString());
}

int Var::size() const
{
	if(Type=="string")return String.size();
	if(Type=="map")return Map.size();
	if(Type=="list")return List.size();
	return 0;
}

Var Var::operator [] (int i)
{
	if(Type=="list")return List[i];
	qDebug()<<"Var operator [] (int i) - Name:"<<getName()<<" Type:"<<getType()<<" type is not list";
	return Var();
}

Var Var::operator [] (QString key)
{
	if(Type=="map")return Map.value(key);
	qDebug()<<"Var operator [] (QString key) - Name:"<<getName()<<" Type:"<<getType()<<" type is not map";
	return Var();
}

Var* Var::pval(int i)
{
	if(Type=="list")return &List[i];
	qDebug()<<"Var::pval(int i) - Name:"<<getName()<<" Type:"<<getType()<<" type is not list";
	return new Var();
}

Var* Var::pval(QString key)
{
	if(Type=="map")return &(Map[key]);
	qDebug()<<"Var* Var::pval(QString key) - Name:"<<getName()<<" Type:"<<getType()<<" type is not map";
	return new Var();
}


void Var::operator =(QString string)
{
	setType("string");
	String = string;
}

void Var::operator +=(QString string)
{
	setType("string");
	String += string;
}

bool Var::contains(QString key)
{
	if(getType()=="map")
		if(Map.contains(key))return true;
	return false;
}

int Var::contains(Var var)
{
	if(getType()=="list")
    {
		for(int i=0;i<List.size();i++)
			if(List[i]==var)return i;
    }
	return -1;
}

void Var::clear()
{
	List.clear();
	Map.clear();
	String.clear();
	Type="";
	Name="";
}

void Var::showInBrouser(QString fileName)
{
//	QFile file(fileName);
//	  if(file.open(QIODevice::WriteOnly))
//	      {
//	      file.write(toXML().toUtf8());
//	      file.close();
//	      }
//		QUrl url = QUrl(fileName);
//	  QDesktopServices::openUrl(url);
}

Var* Var::findByName(QString name)
{
    return new Var();
}

Var* Var::find(QString string)
{
    return new Var();
}
Var* Var::find(Var var)
{
    return new Var();
}

void Var::exportToXml(QString path)
{
	QFile file(path);
	if(file.open(QIODevice::WriteOnly))
    {
        file.write(toXML().toUtf8());
        file.close();
    }
}

void Var::converMapToList()
{
	if(getType()!="map")
    {
        //qDebug()<<"Var::converMapToList() type is not map! name ="<<name<<"type ="<<type;
		return;
    }
	if(Map.size()!=1)
    {
        qDebug()<<"Var::converMapToList() map size != 1; name ="<<getName()<<"type ="<<getType();
		return;
    }
	List.append(Map.value(Map.keys()[0]));
	setType("list");
}

Var Var::toLine(QString name, int column) const
{
    Var line;line.setParam("map",getName());
    column=0;
    int columnSize=0;
	
	QString prefix;
	if(name=="~")prefix="";
	else 
    {
        if(name.size()==0)prefix=getName();
		else prefix=name+"~"+getName();
    }
	
	if(getType()=="map")
    {
        for(QMap<QString, Var>::const_iterator i = Map.constBegin();i != Map.constEnd();i++)
        {
            Var linePart=Map.value(i.key()).toLine(prefix,column);
			line.unite(linePart);
            column+=columnSize;
            columnSize+=columnSize;
        }
    }
	if(getType()=="list")
    {
		for(int i=0;i<List.size();i++)
        {
            Var linePart=List[i].toLine(prefix+"#"+QString::number(i),column);
			line.unite(linePart);
            column+=columnSize;
        }
    }
	if(getType()=="string")
    {
        line.insert(prefix,String);
        //line.pval(prefix)->Column=column;
        columnSize++;
    }
	return line;
}

void Var::unite(Var var)
{
	if(getType()!="map")
    {
        qDebug()<<"Var::unite current var is not a map!";
		return;
    }
	if(var.getType()!="map")
    {
        qDebug()<<"Var::unite passed var is not a map!";
		return;
    }
	Map.unite(var.Map);
}

void Var::removeEmpty()
{
	if(getType()=="list")
    {
        for(int i=0;i<List.size();i++)
        {
            if(List[i].size()==0)
            {
                remove(i);
				i--;
            }
			else
				pval(i)->removeEmpty();
        }
    }
	if(getType()=="map")
    {
        int k=0;
		int size=Map.size();
		for(QMap<QString, Var>::const_iterator i = Map.constBegin();i != Map.constEnd();++i)
        {
            if(k>=size)break;
			if(Map.value(i.key()).size()==0)
            {
                remove(i.key());
            }
			else
				pval(i.key())->removeEmpty();
			k++;
        }
    }
}

Var Var::fromQVariant(QVariant qvar)
{
    Var var;
	if(qvar.type()==QVariant::List)
    {
        QVariantList qlist = qvar.toList();
		var.setType("list");
		for(int i=0;i<qlist.size();i++)
			var.append(fromQVariant(qlist[i]));
    }
	if(qvar.type()==QVariant::Map)
    {
        QVariantMap qmap = qvar.toMap();
		var.setType("map");
		for(QVariantMap::const_iterator i = qmap.constBegin();i != qmap.constEnd();i++)
			var.insert(i.key(),fromQVariant(i.value()));
    }
	if(qvar.type()==QVariant::String)
    {
        var.setType("string");
		var.setString(qvar.toString());
    }
	if(qvar.type()==QVariant::Int)
    {
        var.setType("string");
		var.setString(QString::number(qvar.toInt()));
    }
	if(qvar.type()==QVariant::Double)
    {
        var.setType("string");
		var.setString(QString::number(qvar.toDouble()));
    }
	return var;
	}

void Var::importFromJsonFile(QString filePath)
{
	QFile* file = new QFile(filePath);
	if(!file->open(QIODevice::ReadOnly))
    {
        qDebug()<<"Var::Var(QString filePath) - path is incorrect"<<filePath;
    }
	QString json = file->readAll();
	QJsonDocument parser = QJsonDocument::fromJson(json.toUtf8());
	Var var = fromQVariant(parser.toVariant());
	setType(var.getType());
	if(var.getType()=="list")
		List=var.List;		
	if(var.getType()=="map")
		Map=var.Map;		
	if(var.getType()=="string")
		String=var.String;		
}

void Var::importFromJsonDocument(QJsonDocument parser)
{
    //QJsonDocument parser = QJsonDocument::fromJson(json.toUtf8());
    Var var = fromQVariant(parser.toVariant());
    setType(var.getType());
    if(var.getType()=="list")
        List=var.List;
    if(var.getType()=="map")
        Map=var.Map;
    if(var.getType()=="string")
        String=var.String;
}

void Var::importFromJsonString(QString json)
{
        QJsonDocument parser = QJsonDocument::fromJson(json.toUtf8());
        Var var = fromQVariant(parser.toVariant());
        setType(var.getType());
        if(var.getType()=="list")
            List=var.List;
        if(var.getType()=="map")
            Map=var.Map;
        if(var.getType()=="string")
            String=var.String;
}

QJsonValue Var::exportToJson()
{
    QJsonValue res;
	if(getType()=="string")
    {
		res = QJsonValue(String);
    }
	if(getType()=="map")
    {
        QJsonObject obj;
		for(QMap<QString, Var>::const_iterator i = Map.constBegin();i != Map.constEnd();i++)
        {
            Var el=i.value();
			obj.insert(i.key(),el.exportToJson());
        }
		res= QJsonValue(obj);
    }
	if(getType()=="list")
    {
        QJsonArray arr;
		for(int i=0;i<List.size();i++)
			arr.append(List[i].exportToJson());
		res = QJsonValue(arr);
    }
	return res;
}

QString Var::exportToJsonString()
{
	QJsonDocument doc;
	if(getType()=="string")
		return this->exportToJson().toString();
	if(getType()=="map")
		doc=QJsonDocument(this->exportToJson().toObject());
	if(getType()=="list")
		doc=QJsonDocument(this->exportToJson().toArray());
	return doc.toJson();
}

QString Var::toParamString()
{
	if(Type!="map")
    {
        qDebug()<<"toParamString: type is not map!";
		return "";
    }
	QString string="";
	for(QMap<QString, Var>::const_iterator i = Map.constBegin();i != Map.constEnd();i++)
		{Var el=i.value();
		string+=i.key()+"=";
		string+=el.toString();
		if(i+1 != Map.constEnd())
				string+="&";
    }
	return string;
}

//===========!!!!!!!!!!!!!!!!============================
void Var::loadJson(QString url, Var* params, QString method)
{
    QNetworkAccessManager* m_pQnaManager;
    m_pQnaManager = new QNetworkAccessManager();

    QString sep = "?";
    if(url.contains("?")) sep="&";
    url+=sep+params->toParamString();

    qDebug()<<url;

    QNetworkReply *r = m_pQnaManager->get(QNetworkRequest(QUrl(url)));
    QEventLoop eLoop;
    QObject::connect(m_pQnaManager, SIGNAL(finished(QNetworkReply*)), &eLoop, SLOT(quit()));
    eLoop.exec();
    if(r->error() != QNetworkReply::NoError)
    {
        qDebug() << " [!]ERROR: " << r->errorString();
    }
    else
    {
        importFromJsonString(r->readAll());
        print();
    }
    delete m_pQnaManager;
}
