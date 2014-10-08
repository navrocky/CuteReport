/***************************************************************************
 *   This file is part of the CuteReport project                           *
 *   Copyright (C) 2012-2014 by Alexander Mikhalov                         *
 *   alexander.mikhalov@gmail.com                                          *
 *                                                                         *
 **                   GNU General Public License Usage                    **
 *                                                                         *
 *   This library is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation, either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 *                                                                         *
 **                  GNU Lesser General Public License                    **
 *                                                                         *
 *   This library is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License as        *
 *   published by the Free Software Foundation, either version 3 of the    *
 *   License, or (at your option) any later version.                       *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library.                                      *
 *   If not, see <http://www.gnu.org/licenses/>.                           *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 ****************************************************************************/
#include "serializerxml.h"
#include "reportinterface.h"
#include "datasetinterface.h"
#include "pageinterface.h"
#include "iteminterface.h"
#include "bandinterface.h"
#include "printerinterface.h"
#include "rendererinterface.h"
#include "storageinterface.h"
#include "forminterface.h"
#include "dummyband.h"
#include "dummyitem.h"
#include "objectfactory.h"

#include <QtCore>
#include <QDomDocument>
#include <QBrush>

#if QT_VERSION <= 0x050000
static const int firstPropertyNum = 1;
#else
static const int firstPropertyNum = 0;
#endif

static const QString MODULENAME = "SerializerXML";


Q_DECLARE_METATYPE(QPainterPath)

using namespace CuteReport;

SerializerXML::SerializerXML(QObject * parent)
    :SerializerInterface(parent)
{

}


SerializerXML::~SerializerXML()
{

}


SerializerInterface *SerializerXML::createInstance(QObject * parent) const
{
    return new SerializerXML(parent);
}


QString SerializerXML::lastError()
{
    return m_lastError;
}


QByteArray SerializerXML::serialize(const QObject *object, bool *ok)
{
    QDomDocument doc("CuteReport");
    doc.appendChild(objectProperties(object, &doc, object->metaObject()->className()));

    QByteArray ba =  doc.toByteArray();

    if (ok)
        *ok = true;

    return ba;
}


QObject * SerializerXML::deserialize(const QByteArray &data, bool * ok)
{
    QDomDocument doc("CuteReport");

    QString errorStr;
    int line, column;
    if (!doc.setContent(data, &errorStr, &line, &column)) {
        m_lastError = QString("Error in line %1, column %2: %3").arg(line).arg(column).arg(errorStr);
        if (ok)
            *ok = false;
        //        qWarning() << "SerializerXML::deserialize  error:" << m_lastError;
        return 0;
    }

    QDomElement reportElement = doc.firstChildElement();
    QObject * object = objectFromDom(0, reportElement);

    if (ok)
        *ok = true;

    return object;
}


//QDomDocument SerializerXML::reportToDom(const ReportInterface *report)
//{
//    QDomDocument doc("report");
//    doc.appendChild(objectProperties(report, &doc, "report"));
//    doc.createProcessingInstruction("xml", "version=\"2.0\" encoding=\"utf-8\"");

//    return doc;
//}


CuteReport::ReportInterface * SerializerXML::reportFromDom(QDomDocument * doc)
{
    CuteReport::ReportInterface * report;
    QDomElement reportElement = doc->firstChildElement();

    report = dynamic_cast <CuteReport::ReportInterface*> (objectFromDom(0, reportElement));
    return report;
}


QDomElement SerializerXML::objectProperties(const QObject * object, QDomDocument * doc, const QString & objectName)
{
    QList<StringPair> specialAttributes;
    QString objectTypeName = objectType(object, specialAttributes);

    QDomElement d;

    if (objectTypeName.isEmpty()) {
        /** old method */
        d = doc->createElement(objectName.isEmpty() ? object->metaObject()->className() : objectName);
    } else {
        /** new method*/
        d = doc->createElement(objectTypeName);
        //d.setAttribute("className", object->metaObject()->className());
        if (dynamic_cast<const CuteReport::ReportPluginInterface*>(object)) {
            const CuteReport::ReportPluginInterface* module = reinterpret_cast<const CuteReport::ReportPluginInterface*>(object);
            d.setAttribute("moduleName", module->moduleFullName());
            d.setAttribute("moduleVersion", module->moduleVersion());
            d.setAttribute("extends", module->extendsModules().join(", "));
        }
        foreach (const StringPair & attr, specialAttributes)
            d.setAttribute(attr.first, attr.second);
    }

    if (object->metaObject()->propertyCount()) {
        QDomElement pro = doc->createElement("properties");

        for (int p = firstPropertyNum; p < object->metaObject()->propertyCount(); ++p) {
            pro.appendChild(propertyToDom(doc, object ,p));
        }

        d.appendChild(pro);
    }

    if (object->children().size()) {
        QDomElement ch = doc->createElement("children");

        for (int c = 0; c < object->children().size();c++)
            ch.appendChild(objectProperties(object->children()[c], doc));

        d.appendChild(ch);
    }

    return d;
}


QObject * SerializerXML::objectFromDom( QObject * parent, const QDomElement & dom, ReportInterface * report)
{
    QObject * obj = 0;

    if (dom.tagName() == "Report" || dom.tagName() == "CuteReport::ReportInterface") {
        report = reportCore()->createReport();
        obj = report;
        setObjectPropertiesFromDom(obj, dom);
        setObjectChildrenFromDom(obj, dom, report);
        //report->init();
    }

    if (!obj) {

        if (dom.attribute("moduleName").isEmpty()) {
            /** old method */
            obj = createObject(report, dom.tagName(), parent );
        } else {
            /** new method */
            obj = createObject(dom, parent, report );
        }

        if (obj){
            setObjectPropertiesFromDom(obj, dom);
            setObjectChildrenFromDom(obj, dom, report);
            obj->setParent(parent);
        }
    }

    if (!obj) {
        obj = ObjectFactory::instance()->createObject( dom.tagName().toLatin1() );
        if (obj) {
            setObjectPropertiesFromDom(obj, dom);
            setObjectChildrenFromDom(obj, dom, report);
            obj->setParent(parent);
        }
    }

    if (obj) {
        ReportInterface * report = qobject_cast<ReportInterface*>(obj);
        if (report) {
            QList<CuteReport::ReportPluginInterface*> modules = this->findChildren<CuteReport::ReportPluginInterface*>();
            report->init();
        }
    }
    //    else
    //        qWarning() << tr("Can't find plugin for tagname \'%1\'").arg(dom.tagName());

    return obj;
}


void SerializerXML::setObjectPropertiesFromDom(QObject * object, const QDomElement & dom)
{
    for (QDomElement prop = dom.firstChildElement("properties").firstChild().toElement(); !prop.isNull(); prop = prop.nextSibling().toElement()) {
        for (int p = firstPropertyNum; p < object->metaObject()->propertyCount(); ++p) {
            QMetaProperty metaProperty = object->metaObject()->property(p);
            if (metaProperty.name() != prop.tagName().toLatin1())
                continue;

            if (metaProperty.name()[0] != '_')  {   // do not save special properties
                metaProperty.write(object, domToVariant(prop));
                break;
            }

        }
    }
}


void SerializerXML::setObjectChildrenFromDom(QObject * object, const QDomElement & dom, ReportInterface * report)
{
    for (QDomElement ch = dom.firstChildElement("children").firstChildElement();!ch.isNull();ch = ch.nextSiblingElement())
        objectFromDom(object, ch, report);
}


QDomElement SerializerXML::variantToDom(QDomDocument * doc, const QString & name, const QVariant & value)
{
    if (!doc)
        return QDomElement();

    QDomElement dom = doc->createElement(name);
    dom.setAttribute("type", value.typeName());

    QString typeName = QString(value.typeName()).trimmed();

    if (QString("QPainterPath") == typeName) {
        /*	<tag_name type="QPainterPath">
                    <moveTo>x,y</moveTo>
                    <lineTo>x,y</lineTo>
                    <curveTo>x,y</curveTo>
                    <curveToData>x,y</curveToData>
                    <curveToData>x,y</curveToData>
                    ...
                </tag_name>
            */
        QPainterPath p=value.value<QPainterPath>();
        for (int i=0;i<p.elementCount();i++) {
            QDomElement el;
            switch(p.elementAt(i).type) {
            case QPainterPath::MoveToElement:
                el= doc->createElement("moveTo");
                break;
            case QPainterPath::LineToElement:
                el= doc->createElement("lineTo");
                break;
            case QPainterPath::CurveToElement:
                el= doc->createElement("curveTo");
                break;
            case QPainterPath::CurveToDataElement:
                el= doc->createElement("curveToData");
                break;
            }
            el.appendChild(doc->createTextNode(QString("%1,%2").arg(p.elementAt(i).x).arg(p.elementAt(i).y)));
            dom.appendChild(el);
        }
        return dom;
    } else if (QString("CuteReport::Margins") == typeName) {
        Margins margins = value.value<Margins>();
        dom.appendChild(doc->createTextNode(QString("%1,%2,%3,%4").arg(margins.left()).arg(margins.top()).arg(margins.right()).arg(margins.bottom())));
    } else {
        switch (value.type()) {
        case QVariant::Int:	//<tag_name type="Int">string_value</tag_name>
        case QVariant::Double:	//<tag_name type="Double">string_value</tag_name>
        case QVariant::LongLong://<tag_name type="LongLong">string_value</tag_name>
        case QVariant::UInt:	//<tag_name type="UInt">string_value</tag_name>
        case QVariant::ULongLong://<tag_name type="ULongLong">string_value</tag_name>
        case QVariant::Bool:	//<tag_name type="Bool">string_value</tag_name>
        case QVariant::Char:	//<tag_name type="Char">string_value</tag_name>
        case QVariant::Date:	//<tag_name type="Date">string_value</tag_name>
        case QVariant::DateTime://<tag_name type="DateTime">string_value</tag_name>
        case QVariant::Font:	//<tag_name type="Font">string_value</tag_name>
        case QVariant::KeySequence://<tag_name type="KeySequence">string_value</tag_name>
        case QVariant::Time:	//<tag_name type="Time">string_value</tag_name>
        case QVariant::String:	//<tag_name type="Time">string_value</tag_name>
            dom.appendChild(doc->createTextNode(value.toString().replace('\n',"<newline>")));
            break;

        case QVariant::StringList:
            /*	<tag_name type="StringList">
                    <string>value1</string>
                    <string>value2</string>
                    ...
                    <string>valuex</string>

                </tag_name>
            */
            foreach(QString str, value.toStringList())
                dom.appendChild(variantToDom(doc, "string", str));
            break;

        case QVariant::ByteArray://<tag_name type="ByteArray">string_value</tag_name>
            dom.appendChild(doc->createTextNode(value.toByteArray().toBase64()));
            break;

        case QVariant::Color:	//<tag_name type="Color">red,green,blue,alpha</tag_name>
            dom.appendChild(doc->createTextNode(QString("%1,%2,%3,%4").arg(value.value<QColor>().red()).arg(value.value<QColor>().green()).arg(value.value<QColor>().blue()).arg(value.value<QColor>().alpha())));
            break;

        case QVariant::Rect:	//<tag_name type="Rect">x,y,w,h</tag_name>
        case QVariant::RectF:	//<tag_name type="RectF">x,y,w,h</tag_name>
            dom.appendChild(doc->createTextNode(QString("%1,%2,%3,%4").arg(value.toRectF().x()).arg(value.toRectF().y()).arg(value.toRectF().width()).arg(value.toRectF().height())));
            break;

        case QVariant::Transform:	//<tag_name type="Rect">m11,m12,m13,m21,m22,m23,m31,m32,m33</tag_name>
            dom.appendChild(doc->createTextNode(QString("%1,%2,%3,%4,%5,%6,%7,%8,%9").arg(value.value<QTransform>().m11()).arg(value.value<QTransform>().m12()).arg(value.value<QTransform>().m13()).arg(value.value<QTransform>().m21()).arg(value.value<QTransform>().m22()).arg(value.value<QTransform>().m23()).arg(value.value<QTransform>().m31()).arg(value.value<QTransform>().m32()).arg(value.value<QTransform>().m33())));
            break;

        case QVariant::Line:	//<tag_name type="Rect">x1,y1,x2,y2</tag_name>
        case QVariant::LineF:	//<tag_name type="RectF">x1,y1,x2,y2</tag_name>
            dom.appendChild(doc->createTextNode(QString("%1,%2,%3,%4").arg(value.toLineF().x1()).arg(value.toLineF().y1()).arg(value.toLineF().x2()).arg(value.toLineF().y2())));
            break;

        case QVariant::Size:	//<tag_name type="Size">w,h</tag_name>
        case QVariant::SizeF:	//<tag_name type="SizeF">w,h</tag_name>
            dom.appendChild(doc->createTextNode(QString("%1,%2").arg(value.toSizeF().width()).arg(value.toSizeF().height())));
            break;

        case QVariant::Point:	//<tag_name type="Point">x,y</tag_name>
        case QVariant::PointF:	//<tag_name type="PointF">x,y</tag_name>
            dom.appendChild(doc->createTextNode(QString("%1,%2").arg(value.toPointF().x()).arg(value.toPointF().y())));
            break;

        case QVariant::Pixmap:	//<tag_name type="Pixmap">base64png</tag_name>
        {
            QByteArray data;
            QBuffer buffer(&data);
            buffer.open(QBuffer::ReadWrite);
            value.value<QPixmap>().toImage().save(&buffer, "PNG");
            buffer.close();
            dom.appendChild(doc->createTextNode(data.toBase64()));
        }
            break;

        case QVariant::Image:	//<tag_name type="Pixmap">base64png</tag_name>
            return variantToDom(doc, name, QPixmap::fromImage(value.value<QImage>()));
            break;

        case QVariant::Brush:
            /*
            <tag_name type="Brush">
                <color>color_value</color>
                <style>style_value</style>
                <texture type="Pixmap" fromat="QImage::Format">base64image</texture>
                <gradient>
                    <spread>spread_value</spread>
                    <coordinateMode>coordinatemode_value</coordinateMode>
                    <type>type_value</type>

                    <!-- QConicalGradient --!>
                    <angle>angle_value</angle>
                    <center>x,y</center>
                    <!-- QConicalGradient --!>

                    <!-- QRadialGradient --!>
                    <center>x,y</center>
                    <radius>radius_value</radius>
                    <focalPoint>x,y</focalPoint>
                    <!-- QRadialGradient --!>

                    <!-- QLinearGradient --!>
                    <start>x,y</start>
                    <final>x,y</final>
                    <!-- QLinearGradient --!>
                    <stops>
                        <stop>
                            <point>point_value</point>
                            <color>color_value</color>
                        </stop>
                        .....
                        <stop>
                            <point>point_value</point>
                            <color>color_value</color>
                        </stop>
                    </stops>
                </gradient>
            </tag_name>
            */
            dom.appendChild(variantToDom(doc, "color", value.value<QBrush>().color()));
            dom.appendChild(variantToDom(doc, "style", (int)(value.value<QBrush>().style())));
            dom.appendChild(variantToDom(doc, "texture", value.value<QBrush>().texture()));
            if (value.value<QBrush>().gradient() && value.value<QBrush>().gradient()->type()!=QGradient::NoGradient) {
                QDomElement gel=doc->createElement("gradient");
                gel.appendChild(variantToDom(doc, "spread", value.value<QBrush>().gradient()->spread()));
                gel.appendChild(variantToDom(doc, "coordinateMode", value.value<QBrush>().gradient()->coordinateMode()));
                gel.appendChild(variantToDom(doc, "type", value.value<QBrush>().gradient()->type()));
                switch(value.value<QBrush>().gradient()->type())
                {
                case QGradient::LinearGradient:
                    gel.appendChild(variantToDom(doc, "start",reinterpret_cast<const QLinearGradient*>(value.value<QBrush>().gradient())->start()));
                    gel.appendChild(variantToDom(doc, "final",reinterpret_cast<const QLinearGradient*>(value.value<QBrush>().gradient())->finalStop()));
                    break;
                case QGradient::RadialGradient:
                    gel.appendChild(variantToDom(doc, "center",reinterpret_cast<const QRadialGradient*>(value.value<QBrush>().gradient())->center()));
                    gel.appendChild(variantToDom(doc, "radius",reinterpret_cast<const QRadialGradient*>(value.value<QBrush>().gradient())->radius()));
                    gel.appendChild(variantToDom(doc, "focalPoint",reinterpret_cast<const QRadialGradient*>(value.value<QBrush>().gradient())->focalPoint()));
                    break;
                case QGradient::ConicalGradient:
                    gel.appendChild(variantToDom(doc, "angle",reinterpret_cast<const QConicalGradient*>(value.value<QBrush>().gradient())->angle()));
                    gel.appendChild(variantToDom(doc, "center",reinterpret_cast<const QConicalGradient*>(value.value<QBrush>().gradient())->center()));
                    break;
                default:
                    break;
                }
                QDomElement stops=doc->createElement("stops");
                foreach(QGradientStop stop, value.value<QBrush>().gradient()->stops()) {
                    QDomElement stp=doc->createElement("stop");
                    stp.appendChild(variantToDom(doc, "point",stop.first));
                    stp.appendChild(variantToDom(doc, "color", stop.second));
                    stops.appendChild(stp);
                }
                gel.appendChild(stops);
                dom.appendChild(gel);
            }
            break;

        case QVariant::Pen:
            /*
            <tag_name type="Pen">
                <brush type="Brush">
                    <color>color_value</color>
                    <style>style_value</style>
                    <texture type="Pixmap" fromat="QImage::Format">base64image</texture>
                </brush>
                <capStyle>capStyle_value</capStyle>
                <color>color_value</color>
                <isCosmetic>isCosmetic_value</isCosmetic>
                <joinStyle>joinStyle_value</joinStyle>
                <miterLimit>miterLimit_value</miterLimit>
                <style>style_value</style>
                <width>width_value</width>
            </tag_name>
            */
            dom.appendChild(variantToDom(doc, "brush", value.value<QPen>().brush()));
            dom.appendChild(variantToDom(doc, "capStyle", value.value<QPen>().capStyle()));
            dom.appendChild(variantToDom(doc, "color", value.value<QPen>().color()));
            dom.appendChild(variantToDom(doc, "isCosmetic", value.value<QPen>().isCosmetic()));
            dom.appendChild(variantToDom(doc, "joinStyle", value.value<QPen>().joinStyle()));
            dom.appendChild(variantToDom(doc, "miterLimit", value.value<QPen>().miterLimit()));
            dom.appendChild(variantToDom(doc, "style", int(value.value<QPen>().style())));
            dom.appendChild(variantToDom(doc, "width", value.value<QPen>().width()));
            break;

        case QVariant::Map:
            /*	<tag_name type="QVariantMap">
                    <key_1>value</key_1>
                    <key_2>value</key_2>
                    ...
                    <key_x>value</key_x>

                </tag_name>
            */
            // alex: doesnt work if key contains slash
            foreach(QString key, value.toMap().uniqueKeys()) {
                QDomElement keyElement = doc->createElement("key");
                keyElement.setAttribute("name", key);
                foreach(QVariant val, value.toHash().values(key))
                    keyElement.appendChild(variantToDom(doc, "value", val));
                dom.appendChild(keyElement);
            }

        case QVariant::Hash:
            /*	<tag_name type="QVariantHash">
                <key_1>value</key_1>
                <key_2>value</key_2>
                ...
                <key_x>value</key_x>

            </tag_name>
        */
            // alex: doesnt work if key contains slash

            foreach(QString key, value.toHash().uniqueKeys()) {
                QDomElement keyElement = doc->createElement("key");
                keyElement.setAttribute("name", key);
                foreach(QVariant val, value.toHash().values(key))
                    keyElement.appendChild(variantToDom(doc, "value", val));
                dom.appendChild(keyElement);
            }
            break;

        case QVariant::Region:
            /*	<tag_name type="QRegion">
                    <rect>x,y,w,h</rect>
                    <rect>x,y,w,h</rect>
                    ...
                    <rect>x,y,w,h</rect>
                </tag_name>
            */
            foreach(QRect r, value.value<QRegion>().rects())
                dom.appendChild(variantToDom(doc, "rect", r));
            break;

        default:
            if (typeName != "") {
                reportCore()->log(LogWarning, MODULENAME, QString("Type \'%1\' is not implemented. \'%2\' property will be empty").arg(value.typeName()).arg(name));
            }
            break;
        }
    }
    return dom;
}


QVariant SerializerXML::domToVariant(const QDomElement & dom)
{
    QVariant value;
    QStringList rect, size, point, strl;
    QBrush brush;
    QPen pen;
    QVariantMap map;
    QDomElement node;
    QRegion region;

    if (QString("QPainterPath")==dom.attribute("type", "").toLatin1()) {
        QPainterPath p;
        node = dom.firstChildElement();
        QPointF c1,c2;
        int cstate=0;
        for (;!node.isNull();node = node.nextSiblingElement())
        {
            point=node.text().split(',');
            if (node.tagName()=="moveTo")
                p.moveTo(point[0].toDouble(),point[1].toDouble());
            if (node.tagName()=="lineTo")
                p.lineTo(point[0].toDouble(),point[1].toDouble());
            if (node.tagName()=="curveTo")
            {
                c1.setX(point[0].toDouble());
                c1.setY(point[1].toDouble());
                cstate=1;
            }

            if (node.tagName()=="curveToData")
            {
                if (cstate==1)
                {
                    c2.setX(point[0].toDouble());
                    c2.setY(point[1].toDouble());
                    cstate=2;
                }
                else
                {
                    p.cubicTo(c1,c2,QPointF(point[0].toDouble(),point[1].toDouble()));
                    cstate=0;
                }
            }

        }
        value.setValue(p);
        return value;
    }

    if (QString("CuteReport::Margins") == dom.attribute("type", "").toLatin1()) {
        QStringList m = dom.text().split(',');
        if (m.size() == 4)
            value.setValue(Margins(m[0].toDouble(), m[1].toDouble(), m[2].toDouble(), m[3].toDouble()));
        return value;
    }


    switch (QVariant::nameToType(dom.attribute("type", "").toLatin1()))
    {
    case QVariant::Bool:	//<tag_name type="Bool">string_value</tag_name>
        value = QVariant(dom.text()).toBool();
        break;

    case QVariant::Int:	//<tag_name type="Int">string_value</tag_name>
    case QVariant::Char:	//<tag_name type="Char">string_value</tag_name>
        value = dom.text().toInt();
        break;

    case QVariant::Double:	//<tag_name type="Double">string_value</tag_name>
        value = dom.text().toDouble();
        break;

    case QVariant::LongLong://<tag_name type="LongLong">string_value</tag_name>
        value = dom.text().toLongLong();
        break;

    case QVariant::UInt:	//<tag_name type="UInt">string_value</tag_name>
    case QVariant::ULongLong://<tag_name type="ULongLong">string_value</tag_name>
        value = dom.text().toULongLong();
        break;

    case QVariant::Date:	//<tag_name type="Date">string_value</tag_name>
        value = QVariant(dom.text()).toDate();
        break;

    case QVariant::DateTime://<tag_name type="DateTime">string_value</tag_name>
        value = QVariant(dom.text()).toDateTime();
        break;

    case QVariant::Font:	//<tag_name type="Font">string_value</tag_name>
        value = QVariant(dom.text()).value<QFont>();
        break;

    case QVariant::KeySequence://<tag_name type="KeySequence">string_value</tag_name>
        value = QVariant(dom.text()).value<QKeySequence>();
        break;

    case QVariant::Time:	//<tag_name type="Time">string_value</tag_name>
        value = QVariant(dom.text()).toTime();
        break;

    case QVariant::String:	//<tag_name type="Time">string_value</tag_name>
        value = dom.text().replace("<newline>","\n");
        break;

    case QVariant::ByteArray://<tag_name type="ByteArray">string_value</tag_name>
        value = QByteArray::fromBase64(dom.text().toLatin1());
        break;

    case QVariant::Color:	//<tag_name type="Color">red,green,blue,alpha</tag_name>
        rect = dom.text().split(',');
        if (rect.size() == 4)
            value = QColor(rect[0].toInt(), rect[1].toInt(), rect[2].toInt(), rect[3].toInt());
        break;

    case QVariant::Rect:	//<tag_name type="Rect">x,y,w,h</tag_name>
    case QVariant::RectF:	//<tag_name type="RectF">x,y,w,h</tag_name>
        rect = dom.text().split(',');
        if (rect.size() == 4)
            value = QRectF(rect[0].toDouble(), rect[1].toDouble(), rect[2].toDouble(), rect[3].toDouble());
        break;

    case QVariant::Transform:	//<tag_name type="Rect">m11,m12,m21,m22,dx,dy</tag_name>
        rect = dom.text().split(',');
        if (rect.size() == 9)
            value = QTransform(rect[0].toDouble(), rect[1].toDouble(), rect[2].toDouble(), rect[3].toDouble(), rect[4].toDouble(), rect[5].toDouble(), rect[6].toDouble(), rect[7].toDouble(), rect[8].toDouble());
        break;

    case QVariant::Line:	//<tag_name type="Rect">x1,y1,x2,y2</tag_name>
    case QVariant::LineF:	//<tag_name type="RectF">x1,y1,x2,y2</tag_name>
        rect = dom.text().split(',');
        if (rect.size() == 4)
            value = QLineF(rect[0].toDouble(), rect[1].toDouble(), rect[2].toDouble(), rect[3].toDouble());
        break;

    case QVariant::Size:	//<tag_name type="Size">w,h</tag_name>
    case QVariant::SizeF:	//<tag_name type="SizeF">w,h</tag_name>
        size = dom.text().split(',');
        if (size.size() == 2)
            value = QSizeF(size[0].toDouble(), size[1].toDouble());
        break;

    case QVariant::Point:	//<tag_name type="Size">x,y</tag_name>
    case QVariant::PointF:	//<tag_name type="SizeF">x,y</tag_name>
        point = dom.text().split(',');
        if (point.size() == 2)
            value = QPointF(point[0].toDouble(), point[1].toDouble());
        break;

    case QVariant::Pixmap:	//<tag_name type="Pixmap">base64png</tag_name>
    {
        QByteArray data = QByteArray::fromBase64(dom.text().toLatin1());
        QBuffer buffer(&data);
        buffer.open(QBuffer::ReadWrite);
        QImage img;
        img.load(&buffer, "PNG");
        buffer.close();
        value = QPixmap::fromImage(img);
    }
        break;

    case QVariant::Brush:
        /*
            <tag_name type="Brush">
                <color>color_value</color>
                <style>style_value</style>
                <texture type="Pixmap" fromat="QImage::Format">base64image</texture>
                <gradient>
                    <spread>spread_value</spread>
                    <coordinateMode>coordinatemode_value</coordinateMode>
                    <type>type_value</type>

                    <!-- QConicalGradient --!>
                    <angle>angle_value</angle>
                    <center>x,y</center>
                    <!-- QConicalGradient --!>

                    <!-- QRadialGradient --!>
                    <center>x,y</center>
                    <radius>radius_value</radius>
                    <focalPoint>x,y</focalPoint>
                    <!-- QRadialGradient --!>

                    <!-- QLinearGradient --!>
                    <start>x,y</start>
                    <final>x,y</final>
                    <!-- QLinearGradient --!>
                    <stops>
                        <stop>
                            <point>point_value</point>
                            <color>color_value</color>
                        </stop>
                        .....
                        <stop>
                            <point>point_value</point>
                            <color>color_value</color>
                        </stop>
                    </stops>
                </gradient>
            </tag_name>
            */
        brush.setTexture(domToVariant(dom.firstChildElement("texture")).value<QPixmap>());
        brush.setColor(domToVariant(dom.firstChildElement("color")).value<QColor>());
        if (!dom.firstChildElement("gradient").isNull()) {
            QDomElement gel=dom.firstChildElement("gradient");
            QGradient *gr=0;
            switch(domToVariant(gel.firstChildElement("type")).toInt()) {
            case QGradient::LinearGradient:
                gr = new QLinearGradient(domToVariant(gel.firstChildElement("start")).toPointF(), domToVariant(gel.firstChildElement("final")).toPointF());
                break;
            case QGradient::RadialGradient:
                gr = new QRadialGradient(domToVariant(gel.firstChildElement("center")).toPointF(), domToVariant(gel.firstChildElement("radius")).toDouble(), domToVariant(gel.firstChildElement("focalPoint")).toPointF());
                break;
            case QGradient::ConicalGradient:
                gr = new QConicalGradient(domToVariant(gel.firstChildElement("angle")).toPointF(), domToVariant(gel.firstChildElement("center")).toDouble());
                break;
            }
            gr->setCoordinateMode((QGradient::CoordinateMode)domToVariant(gel.firstChildElement("coordinateMode")).toInt());
            gr->setSpread((QGradient::Spread)domToVariant(gel.firstChildElement("spread")).toInt());
            QGradientStops gsps;
            node = gel.firstChildElement("stops").firstChildElement("stop");
            for (;!node.isNull();node = node.nextSiblingElement()) {
                QGradientStop gsp;
                gsp.first=domToVariant(node.firstChildElement("point")).toDouble();
                gsp.second=domToVariant(node.firstChildElement("color")).value<QColor>();
                gsps.push_back(gsp);
            }
            gr->setStops(gsps);
            value = QBrush(*gr);
            delete gr;
        } else {
            brush.setStyle((Qt::BrushStyle)domToVariant(dom.firstChildElement("style")).toInt());
            value = brush;
        }
        break;

    case QVariant::Pen:
        /*
            <tag_name type="Pen">
                <brush type="Brush">
                    <color>color_value</color>
                    <style>style_value</style>
                    <texture type="Pixmap" fromat="QImage::Format">base64image</texture>
                </brush>
                <capStyle>capStyle_value</capStyle>
                <color>color_value</color>
                <isCosmetic>isCosmetic_value</isCosmetic>
                <joinStyle>joinStyle_value</joinStyle>
                <miterLimit>miterLimit_value</miterLimit>
                <style>style_value</style>
                <width>width_value</width>
            </tag_name>
            */
        pen.setCapStyle((Qt::PenCapStyle)domToVariant(dom.firstChildElement("capStyle")).toInt());
        pen.setColor(domToVariant(dom.firstChildElement("color")).value<QColor>());
        pen.setBrush(domToVariant(dom.firstChildElement("brush")).value<QBrush>());
        pen.setCosmetic(domToVariant(dom.firstChildElement("isCosmetic")).toBool());
        pen.setJoinStyle((Qt::PenJoinStyle)domToVariant(dom.firstChildElement("joinStyle")).toInt());
        pen.setMiterLimit(domToVariant(dom.firstChildElement("miterLimit")).toInt());
        pen.setStyle((Qt::PenStyle)domToVariant(dom.firstChildElement("style")).toInt());
        pen.setWidth(domToVariant(dom.firstChildElement("width")).toInt());
        value = pen;
        break;

    case QVariant::Map:
        /*	<tag_name type="QVariantMap">
                    <key_1>value</key_1>
                    <key_2>value</key_2>
                    ...
                    <key_x>value</key_x>

                </tag_name>
            */
        node = dom.firstChildElement();
        for (;!node.isNull();node = node.nextSiblingElement()) {
            QDomElement val = node.firstChildElement();
            for (;!val.isNull();val = val.nextSiblingElement())
                map.insertMulti(node.attribute("name"), domToVariant(val));
        }
        value = map;
        break;

    case QVariant::Hash: {
        /*	<tag_name type="QVariantHash">
                <key_1>value</key_1>
                <key_2>value</key_2>
                ...
                <key_x>value</key_x>

            </tag_name>
        */
        QVariantHash hash;
        node = dom.firstChildElement();
        for (;!node.isNull();node = node.nextSiblingElement()) {
            QDomElement val = node.firstChildElement();
            for (;!val.isNull();val = val.nextSiblingElement())
                hash.insertMulti(node.attribute("name"), domToVariant(val));
        }
        value = hash;
    }
        break;

    case QVariant::Region:
        /*	<tag_name type="QRegion">
                    <rect>x,y,w,h</rect>
                    <rect>x,y,w,h</rect>
                    ...
                    <rect>x,y,w,h</rect>
                </tag_name>
            */
        node = dom.firstChildElement();
        for (;!node.isNull();node = node.nextSiblingElement())
            region+=domToVariant(node).toRect();
        value=region;
        break;

    case QVariant::StringList:
        /*	<tag_name type="StringList">
                    <string>value1</string>
                    <string>value2</string>
                    ...
                    <string>valuex</string>

                </tag_name>
            */
        strl.clear();
        node = dom.firstChildElement();
        for (;!node.isNull();node = node.nextSiblingElement())
            strl.push_back(domToVariant(node).toString());
        value = strl;
        break;

    default:
        break;
    }
    return value;
}


QDomElement SerializerXML::propertyToDom(QDomDocument *doc, const QObject *object, int propertyNum)
{
    QDomElement element;
    QMetaProperty metaProperty = object->metaObject()->property(propertyNum);
    QString metaPropertyName = QString(metaProperty.name()).trimmed();

    // do not save special properties
    if (metaPropertyName[0] == '_')
        return element;

    // workarounds
    if (metaPropertyName == "itemIndexMethod")
        return element;

    int alternateProperyNum = -1;
    for (int i = firstPropertyNum; i < object->metaObject()->propertyCount(); ++i) {
        if (i == propertyNum)
            continue;
        QMetaProperty metaProp = object->metaObject()->property(i);
        if (metaProp.name() == metaProperty.name()) {
            alternateProperyNum = i;
        }
    }

    if (alternateProperyNum == -1) {
        element = variantToDom(doc, metaProperty.name(), metaProperty.read(object));
    } else {
        if (!metaProperty.isDesignable(object))
            return element;

        element = variantToDom(doc, metaProperty.name(), metaProperty.read(object));

        QMetaProperty altMetaProperty = object->metaObject()->property(alternateProperyNum);

        if (altMetaProperty.isEnumType()) {
            int alternateValue = *reinterpret_cast<int*>(altMetaProperty.read(object).data());
            element.setAttribute("alternateValue", alternateValue);
        }
    }

    return element;
}


QObject * SerializerXML::createObject(CuteReport::ReportInterface *report, const QString & classname, QObject * parent) const
{
    const ReportPluginInterface * object = 0;
    if ((object = itemPluginByClassName(classname)))
        return reportCore()->createItemObject(report, object->moduleFullName(), parent ? parent : report);
    if (!object && (object = pagePluginByClassName(classname)))
        return reportCore()->createPageObject(report, object->moduleFullName());
    if (!object && (object = datasetPluginByClassName(classname)))
        return reportCore()->createDatasetObject(report, object->moduleFullName());
    if (!object && (object = printerPluginByClassName(classname)))
        return reportCore()->createPrinterObject(report, object->moduleFullName());
    if (!object && (object = rendererPluginByClassName(classname)))
        return reportCore()->createRendererObject(report, object->moduleFullName());
    if (!object && (object = storagePluginByClassName(classname)))
        return reportCore()->createStorageObject(report, object->moduleFullName());
    if (!object && (object = formPluginByClassName(classname)))
        reportCore()->createFormObject(report, object->moduleFullName());
    return 0;
}



const QObject * SerializerXML::pluginByClassName(const QString &name) const
{
    const QObject * object = itemPluginByClassName(name);
    if (!object)
        object = pagePluginByClassName(name);
    if (!object)
        object = datasetPluginByClassName(name);
    if (!object)
        object = printerPluginByClassName(name);
    if (!object)
        object = rendererPluginByClassName(name);
    if (!object)
        object = storagePluginByClassName(name);
    if (!object)
        object = storagePluginByClassName(name);
    return object;
}


const CuteReport::BaseItemInterface* SerializerXML::itemPluginByClassName(const QString & name) const
{
    foreach(const CuteReport::BaseItemInterface* item, reportCore()->itemModules())
        if (item->metaObject()->className() == name)
            return item;
    return 0;
}


const CuteReport::PageInterface* SerializerXML::pagePluginByClassName(const QString & name) const
{
    foreach(CuteReport::PageInterface* item, reportCore()->pageModules())
        if (item->metaObject()->className() == name)
            return item;
    return 0;
}


const CuteReport::DatasetInterface* SerializerXML::datasetPluginByClassName(const QString & name) const
{
    foreach(CuteReport::DatasetInterface* item, reportCore()->datasetModules())
        if (item->metaObject()->className() == name)
            return item;
    return 0;
}


const CuteReport::PrinterInterface* SerializerXML::printerPluginByClassName(const QString &name) const
{
    foreach(CuteReport::PrinterInterface* item, reportCore()->printerModules())
        if (item->metaObject()->className() == name)
            return item;
    return 0;
}


const CuteReport::RendererInterface* SerializerXML::rendererPluginByClassName(const QString &name) const
{
    foreach(CuteReport::RendererInterface* item, reportCore()->rendererModules())
        if (item->metaObject()->className() == name)
            return item;
    return 0;
}


const CuteReport::StorageInterface* SerializerXML::storagePluginByClassName(const QString &name) const
{
    foreach(CuteReport::StorageInterface* item, reportCore()->storageModules())
        if (item->metaObject()->className() == name)
            return item;
    return 0;
}


const CuteReport::FormInterface *SerializerXML::formPluginByClassName(const QString &name) const
{
    foreach(CuteReport::FormInterface* item, reportCore()->formModules())
        if (item->metaObject()->className() == name)
            return item;
    return 0;
}


QObject * SerializerXML::createObject( const QDomElement & dom, QObject * parent, CuteReport::ReportInterface *report) const
{
    QObject * object = 0;
    QString tagName = dom.tagName();
    QString moduleName = dom.attribute("moduleName");

    //compatibility workaround
    if (!moduleName.contains("::"))
        moduleName.prepend("Standard::");

    // we dont need to process "Report" object here
    if (tagName == "Band") {
        BandInterface * band = dynamic_cast<CuteReport::BandInterface*>(reportCore()->createItemObject(report, moduleName, parent ? parent : report));
        if (!band) {
            QString layoutType = dom.attribute("layoutType");
            QString layoutPriority = dom.attribute("layoutPriority");
            DummyBand * dBand = new CuteReport::DummyBand(parent ? parent : report);
            dBand->setOriginalModuleName(moduleName);
            if (!layoutType.isEmpty())
                dBand->setLayoutType( (BandInterface::LayoutType)layoutType.toInt());
            if (!layoutPriority.isEmpty())
                dBand->setLayoutPriority(layoutPriority.toInt());

            if (report)
                report->setInvalid();
            band = dBand;
            reportCore()->log(LogError, MODULENAME, QString("Band with name \'%1\' is not found").arg(moduleName));
        }
        return band;
    }

    if (tagName == "Item") {
        object = reportCore()->createItemObject(report, moduleName, parent ? parent : report);
        if (!object && !dom.attribute("extends").isEmpty()) {
            foreach (const QString & baseModule, dom.attribute("extends").split(",", QString::SkipEmptyParts)) {
                object = reportCore()->createItemObject(report, baseModule);
                if (object)
                    break;
            }
        }
        if (!object) {
            DummyItem * dItem = new CuteReport::DummyItem(parent ? parent : report);
            dItem->setOriginalModuleName(moduleName);

            if (report)
                report->setInvalid();
            object = dItem;
            reportCore()->log(LogError, MODULENAME, QString("Item with name \'%1\' is not found").arg(moduleName));
        }
        return object;
    } else if (tagName == "Page") {
        // FIXME: compatibility
        if (moduleName == "Standard::Standard")
            moduleName = "Standard::Page";
        object = reportCore()->createPageObject(report, moduleName);
        if (!object && !dom.attribute("extends").isEmpty()) {
            foreach (const QString & baseModule, dom.attribute("extends").split(",", QString::SkipEmptyParts)) {
                object = reportCore()->createPageObject(report, baseModule);
                if (object)
                    return object;
            }
        }
    } else if (tagName == "Dataset") {
        object = reportCore()->createDatasetObject(report, moduleName);
        if (!object && !dom.attribute("extends").isEmpty()) {
            foreach (const QString & baseModule, dom.attribute("extends").split(",", QString::SkipEmptyParts)) {
                object = reportCore()->createDatasetObject(report, baseModule);
                if (object)
                    return object;
            }
        }
    } else if (tagName == "Storage") {
        object = reportCore()->createStorageObject(report, moduleName);
        if (!object && !dom.attribute("extends").isEmpty()) {
            foreach (const QString & baseModule, dom.attribute("extends").split(",", QString::SkipEmptyParts)) {
                object = reportCore()->createStorageObject(report, baseModule);
                if (object)
                    return object;
            }
        }
    } else if (tagName == "Renderer") {
        // FIXME: compatibility
        if (moduleName == "Standard::Standard")
            moduleName = "Standard::Renderer";
        object = reportCore()->createRendererObject(report, moduleName);
        if (!object && !dom.attribute("extends").isEmpty()) {
            foreach (const QString & baseModule, dom.attribute("extends").split(",", QString::SkipEmptyParts)) {
                object = reportCore()->createRendererObject(report, baseModule);
                if (object)
                    return object;
            }
        }
    } else if (tagName == "Printer") {
        // FIXME: compatibility
        if (moduleName == "Standard::Standard")
            moduleName = "Standard::Printer";
        object = reportCore()->createPrinterObject(report, moduleName);
        if (!object && !dom.attribute("extends").isEmpty()) {
            foreach (const QString & baseModule, dom.attribute("extends").split(",", QString::SkipEmptyParts)) {
                object = reportCore()->createPrinterObject(report, baseModule);
                if (object)
                    return object;
            }
        }
    } else if (tagName == "Form") {
        object = reportCore()->createFormObject(report, moduleName);
        if (!object && !dom.attribute("extends").isEmpty()) {
            foreach (const QString & baseModule, dom.attribute("extends").split(",", QString::SkipEmptyParts)) {
                object = reportCore()->createFormObject(report, baseModule);
                if (object)
                    return object;
            }
        }
    }

    return object;
}


QString SerializerXML::objectType(const QObject * object, QList<StringPair> &specialAttributes) const
{
    if (dynamic_cast<const CuteReport::ReportInterface*>(object))
        return "Report";

    if (dynamic_cast<const CuteReport::BandInterface*>(object)) {
        const CuteReport::BandInterface * band = reinterpret_cast<const CuteReport::BandInterface*>(object);
        specialAttributes.append(StringPair("layoutType", QString::number(band->layoutType())));
        specialAttributes.append(StringPair("layoutPriority", QString::number(band->layoutPriority())));
        return "Band";
    }
    if (dynamic_cast<const CuteReport::BaseItemInterface*>(object))
        return "Item";
    if (dynamic_cast<const CuteReport::PageInterface*>(object))
        return "Page";
    if (dynamic_cast<const CuteReport::DatasetInterface*>(object))
        return "Dataset";
    if (dynamic_cast<const CuteReport::StorageInterface*>(object))
        return "Storage";
    if (dynamic_cast<const CuteReport::RendererInterface*>(object))
        return "Renderer";
    if (dynamic_cast<const CuteReport::PrinterInterface*>(object))
        return "Printer";
    if (dynamic_cast<const CuteReport::FormInterface*>(object))
        return "Form";

    return QString();
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(SerializerXML, SerializerXML)
#endif
