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

#include "at_logsengine.h"
#include "testrunner.h"


int main(int argc, char *argv[]) 
{
    bool noGui(false);
    bool noFullGui(false);
    QString combinedOutputFileName;
    for (int i=0; i<argc; i++) {
        if (QString(argv[i]) == "-nogui"){
            noGui = true;
        } else if (QString(argv[i]) == "-nofullgui"){
            noFullGui = true;
        } else if (QString(argv[i]) == "-o" && i + 1 < argc ){
            i++;
            combinedOutputFileName = QString(argv[i]);
        }
    }
    TestRunner testRunner("API_Test_LogsEngine", combinedOutputFileName);
    if ( noGui ){
        CActiveScheduler* sched = new CActiveScheduler;
        CActiveScheduler::Install(sched);
        AT_LogsEngine test;
        testRunner.runTests(test);
        CActiveScheduler::Install(0);
        delete sched;
    } else if ( noFullGui ){
        QCoreApplication app(argc, argv);
        AT_LogsEngine test;
        testRunner.runTests(test);
    } else {
        printf("Running tests...\n");
    
        QApplication app(argc, argv);
        AT_LogsEngine test;
        testRunner.runTests(test);
    }
    testRunner.printResults(); 
    return 0;   
}



 

