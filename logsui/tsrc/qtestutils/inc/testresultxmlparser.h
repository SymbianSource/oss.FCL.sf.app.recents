/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:
*
*/

#ifndef TESTRESULTXMLPARSER_H
#define TESTRESULTXMLPARSER_H

#include <QXmlDefaultHandler>
#include <hbinputsettingproxy.h>

#define ST_LOGSCNTFINDER_SET_LANGUAGE( lang )\
    HbInputSettingProxy::instance()->setGlobalInputLanguage( HbInputLanguage( lang ) )
    

class TestResultXmlParser : public QXmlDefaultHandler
{
public: // Constructors and destructor
    TestResultXmlParser();
    ~TestResultXmlParser();    
    
public: // From QXmlContentHandler 
    bool startElement(
        const QString& namespaceURI,
        const QString& localName,
        const QString& qName,
        const QXmlAttributes& atts);
    
    bool endElement(
        const QString& namespaceURI,
        const QString& localName,
        const QString& qName);
    
    bool characters(const QString& ch);
       
public: // New functions
    
    int parse(const QString& fileName);
    
    int parseAndPrintResults(
        const QString& fileName,
        bool printDetails=false);
    
    int testCount();
    
    QStringList errors();
    
private: // Data
    int mTestCount;
    QStringList* mErrors;
    bool mParsingIncidentElement;
    bool mParsingDescriptionElement;
    bool mCurrentTestFailed;
    QString mCurrentTestName;
    QString mCurrentTestFile;
    int mCurrentTestFailureLine;
};


#endif // TESTRESULTXMLPARSER_H
