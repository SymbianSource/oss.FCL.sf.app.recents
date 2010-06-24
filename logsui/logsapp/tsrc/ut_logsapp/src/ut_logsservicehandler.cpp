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
#include "ut_logsservicehandler.h"
#include "logsservicehandler.h"
#include "logsservicehandlerold.h"
#include "qthighway_stub_helper.h"
#include "ut_logsviewmanager.h"
#include "logsviewmanager.h"
#include "logsmainwindow.h"
#include "logscomponentrepository.h"
#include "logsmatchesview.h"
#include "logsdefs.h"

//SYSTEM
#include <QtTest/QtTest>

Q_DECLARE_METATYPE(LogsServices::LogsView)

void UT_LogsServiceHandler::initTestCase()
{
}

void UT_LogsServiceHandler::cleanupTestCase()
{
}


void UT_LogsServiceHandler::init()
{
    mMainWindow =  new LogsMainWindow();
    mService = new LogsServiceHandler(*mMainWindow);
    mServiceOld = new LogsServiceHandlerOld(*mMainWindow);
    
    mLogsViewManager = new LogsViewManager(*mMainWindow, *mService, *mServiceOld);
    
}

void UT_LogsServiceHandler::cleanup()
{
    delete mService;
    mService = 0;
    delete mServiceOld;
    mServiceOld = 0;
    delete mMainWindow;
    mMainWindow = 0;
    delete mLogsViewManager;
    mLogsViewManager = 0;
    
    QtHighwayStubHelper::reset();
}

void UT_LogsServiceHandler::testConstructor()
{
    QVERIFY( mService );
    QVERIFY( !mService->mIsAppStartedUsingService );
    QVERIFY( !mService->isStartedUsingService() );

    QVERIFY( mServiceOld );
    QVERIFY( !mServiceOld->mIsAppStartedUsingService );
    QVERIFY( !mServiceOld->isStartedUsingService() );

    
    QtHighwayStubHelper::setIsService(true);
    LogsServiceHandler serviceHandler(*mMainWindow);
    QVERIFY( serviceHandler.mIsAppStartedUsingService );
    QVERIFY( serviceHandler.isStartedUsingService() );
    
    LogsServiceHandlerOld serviceHandlerOld(*mMainWindow);
    QVERIFY( serviceHandlerOld.mIsAppStartedUsingService );
    QVERIFY( serviceHandlerOld.isStartedUsingService() );
}

void UT_LogsServiceHandler::testStart()
{
    qRegisterMetaType< LogsServices::LogsView >("LogsServices::LogsView");
    QSignalSpy spy(mServiceOld, SIGNAL(activateView(LogsServices::LogsView, bool, QString)));
     
    // Wrong view
    QVERIFY( mServiceOld->start( 9999, true ) != 0 );
    QVERIFY( spy.count() == 0 );

    // Correct view
    mServiceOld->mMainWindow.mForeground = false;
    QVERIFY( mServiceOld->start( (int)LogsServices::ViewReceived, true  ) == 0 );
    QVERIFY( spy.count() == 1 );
    LogsServices::LogsView view = 
        qvariant_cast< LogsServices::LogsView >(spy.at(0).at(0));
    QVERIFY( view == LogsServices::ViewReceived );
    QVERIFY( mServiceOld->mMainWindow.mForeground );  
}

void UT_LogsServiceHandler::testStartWithNum()
{
    qRegisterMetaType< LogsServices::LogsView >("LogsServices::LogsView");
    QSignalSpy spy2(mServiceOld, SIGNAL(activateView(QString)));

    mServiceOld->mMainWindow.mForeground = false;
    QVERIFY( mServiceOld->startWithNum( (int)LogsServices::ViewReceived, true,
            QString("+123456")  ) == 0 );
    QVERIFY( spy2.count() == 1 );
    QVERIFY( spy2.at(0).at(0).toString() == QString("+123456"));
    QVERIFY( mServiceOld->mMainWindow.mForeground );  
}

void UT_LogsServiceHandler::testShow()
{
    qRegisterMetaType< LogsServices::LogsView >("LogsServices::LogsView");
    QSignalSpy spy(mService, SIGNAL(activateView(LogsServices::LogsView, bool, QString)));
    QSignalSpy spy2(mService, SIGNAL(activateView(QString)));
    QVariantMap map;

    // Wrong view
    map.insert(logsViewIndexParam, QVariant(9999));
    QVERIFY( mService->show(map) != 0 );
    QVERIFY( spy.count() == 0 );
    QVERIFY( spy2.count() == 0 );
    
    // Correct view, dialpad text is empty
    map.clear();
    map.insert(logsViewIndexParam, QVariant((int)LogsServices::ViewReceived));
    mService->mMainWindow.mForeground = false;
    QVERIFY( mService->show(map) == 0 );
    QVERIFY( spy.count() == 1 );
    QVERIFY( spy2.count() == 0 );
    LogsServices::LogsView view = 
            qvariant_cast< LogsServices::LogsView >(spy.at(0).at(0));
    QVERIFY( view == LogsServices::ViewReceived );
    QVERIFY( mServiceOld->mMainWindow.mForeground );
    
    // Correct view, dialpad text not empty
    spy.clear();
    map.clear();
    map.insert(logsViewIndexParam, QVariant((int)LogsServices::ViewReceived));
    map.insert(logsDialpadTextParam, QVariant(QString("+123456")));
    mService->mMainWindow.mForeground = false;
    QVERIFY( mService->show(map) == 0 );
    QVERIFY( spy.count() == 0 );
    QVERIFY( spy2.count() == 1 );
    QVERIFY( spy2.at(0).at(0).toString() == QString("+123456"));
    QVERIFY( mServiceOld->mMainWindow.mForeground );
    
    // Some completly dummy parameters or no params at all
    spy.clear();
    spy2.clear();
    map.clear();
    map.insert("dummy_param", QVariant(666));
    mService->mMainWindow.mForeground = false;
    QVERIFY( mService->show(map) == 0 );
    QVERIFY( spy.count() == 1 );
    QVERIFY( spy2.count() == 0 );
    view = qvariant_cast< LogsServices::LogsView >(spy.at(0).at(0));
    QVERIFY( view == LogsServices::ViewAll );
    QVERIFY( mServiceOld->mMainWindow.mForeground );
}
