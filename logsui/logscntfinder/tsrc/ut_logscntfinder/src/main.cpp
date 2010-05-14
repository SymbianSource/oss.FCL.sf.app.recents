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


#include <QtTest/QtTest>

#include "ut_logscntfinder.h"
#include "ut_logspredictivetranslator.h"
#include "ut_logspredictive12keytranslator.h"
#include "ut_logspredictivelatin12keytranslator.h"
#include "ut_logspredictivethai12keytranslator.h"
#include "ut_logscntentry.h"

#include "testresultxmlparser.h"


#define UTEST_CLASS( tc )\
    tc tc##_instance;\
    QStringList tc##_args( #tc );\
    resultFileName = QString("c:/") + QString( #tc ) + QString(".xml");\
    tc##_args << "-xml" << "-o" << resultFileName;\
    QTest::qExec(&tc##_instance, tc##_args);\
    parser.parseAndPrintResults(resultFileName,true)
    


int main(int argc, char *argv[]) 
{
    bool promptOnExit(true);
    bool useQApplication(true);
    for (int i=0; i<argc; i++) {
        if (QString(argv[i]) == "-noprompt")
            promptOnExit = false;
        else if (QString(argv[i]) == "-noqapp")
            useQApplication = false;
    }
    printf("Running tests...\n");
    
    QApplication* app = 0;
    if ( useQApplication ) 
        app = new QApplication(argc, argv);
    
    TestResultXmlParser parser;
    QString resultFileName;
    
    UTEST_CLASS( UT_LogsPredictiveTranslator );
    UTEST_CLASS( UT_LogsPredictive12KeyTranslator );
    UTEST_CLASS( UT_LogsPredictiveLatin12KeyTranslator );
    UTEST_CLASS( UT_LogsPredictiveThai12KeyTranslator );
    UTEST_CLASS( UT_LogsCntEntry );
    UTEST_CLASS( UT_LogsCntFinder );
    
    if (promptOnExit) {
        printf("Press any key...\n");
        getchar(); 
    }
    delete app;
    return 0;   
}



 
