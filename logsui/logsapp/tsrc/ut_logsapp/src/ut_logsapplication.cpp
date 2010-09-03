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

//USER
#include "ut_logsapplication.h"
//#include "hbapplication.h"
#include "logsapplication.h"
#include "qthighway_stub_helper.h"
#include "hbstubs_helper.h"
//SYSTEM
#include <QtTest/QtTest>
#include <hbmainwindow.h>
#include <QKeyEvent>

void UT_LogsApplication::initTestCase()
{

}

void UT_LogsApplication::cleanupTestCase()
{

}


void UT_LogsApplication::init()
{
    int value = qApp->argc();
    mLogsApplication = new LogsApplication(value,qApp->argv());
    
}

void UT_LogsApplication::cleanup()
{
    delete mLogsApplication;
    mLogsApplication = 0;
}

void UT_LogsApplication::testtestLogsAppEngineReady()
{
    QSignalSpy spy( mLogsApplication, SIGNAL(applicationReady()) );
    // Not handled
    QVERIFY( mLogsApplication->mReadCompleted == false );
    QVERIFY( mLogsApplication->mViewReady == false );
    mLogsApplication->testLogsAppEngineReady();
    QVERIFY( spy.count() == 0 );
    
    QVERIFY( mLogsApplication->mReadCompleted == true );
    QVERIFY( mLogsApplication->mViewReady == false );
    // Not handled
    mLogsApplication->testLogsAppEngineReady();
    QVERIFY( spy.count() == 0 );
    
    // Handled
    mLogsApplication->mReadCompleted = false;
    mLogsApplication->mViewReady = true;
    mLogsApplication->testLogsAppEngineReady();
    QVERIFY( spy.count() == 1 );
    
    spy.clear();
    mLogsApplication->mViewReady = false;
    mLogsApplication->mReadCompleted = false;
    mLogsApplication->testLogsAppEngineReady();
    QVERIFY( spy.count() == 0 );
        
    mLogsApplication->testLogsHandleAppViewReady();
    QVERIFY( spy.count() == 1 );
    
}

void UT_LogsApplication::testtestLogsHandleAppViewReady()
{
    QSignalSpy spy( mLogsApplication, SIGNAL(applicationReady()) );
    // Not handled
    QVERIFY( mLogsApplication->mReadCompleted == false );
    QVERIFY( mLogsApplication->mViewReady == false );
    mLogsApplication->testLogsHandleAppViewReady();
    QVERIFY( spy.count() == 0 );
    
    QVERIFY( mLogsApplication->mReadCompleted == false );
    QVERIFY( mLogsApplication->mViewReady == true );
    // Not handled
    mLogsApplication->testLogsHandleAppViewReady();
    QVERIFY( spy.count() == 0 );
    
    // Handled
    mLogsApplication->mViewReady = false;
    mLogsApplication->mReadCompleted = true;
    mLogsApplication->testLogsHandleAppViewReady();
    QVERIFY( spy.count() == 1 );  
    
    spy.clear();
    mLogsApplication->mViewReady = false;
    mLogsApplication->mReadCompleted = false;
    mLogsApplication->testLogsHandleAppViewReady();
    QVERIFY( spy.count() == 0 );
    
    mLogsApplication->testLogsAppEngineReady();
    QVERIFY( spy.count() == 1 );
}

