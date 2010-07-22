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

#include "st_logscntfinder.h"
#include "st_logscntfinderqwerty.h"
#include "st_logscntfinderthai.h"

#include "testresultxmlparser.h"


#define STEST_CLASS( tc )\
    tc tc##_instance;\
    QStringList tc##_args( #tc );\
    resultFileName = QString("c:/data/others/") + QString( #tc ) + QString(".xml");\
    tc##_args << "-xml" << "-o" << resultFileName;\
    QTest::qExec(&tc##_instance, tc##_args);\
    parser.parseAndPrintResults(resultFileName,true)
    


int main(int argc, char *argv[]) 
{
    bool promptOnExit(false);
    bool useQApplication(true);
    for (int i=0; i<argc; i++) {
        if (QString(argv[i]) == "-prompt")
            promptOnExit = true;
        else if (QString(argv[i]) == "-noqapp")
            useQApplication = false;
    }
    printf("Running tests...\n");
    
    QApplication* app = 0;
    if ( useQApplication ) 
        app = new QApplication(argc, argv);
    
    TestResultXmlParser parser;
    QString resultFileName;
    
    STEST_CLASS( ST_LogsCntFinder );
    STEST_CLASS( ST_LogsCntFinderThai );
    STEST_CLASS( ST_LogsCntFinderQwerty );
    
    if (promptOnExit) {
        printf("Press any key...\n");
        getchar(); 
    }
    delete app;
    return 0;   
}



 
