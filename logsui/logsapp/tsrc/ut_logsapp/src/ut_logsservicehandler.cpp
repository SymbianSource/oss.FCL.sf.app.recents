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
    mService = new LogsServiceHandler();
    mServiceOld = new LogsServiceHandlerOld();
    
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
    LogsServiceHandler serviceHandler;
    QVERIFY( serviceHandler.mIsAppStartedUsingService );
    QVERIFY( serviceHandler.isStartedUsingService() );
    
    LogsServiceHandlerOld serviceHandlerOld;
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
    QVERIFY( mServiceOld->start( (int)LogsServices::ViewReceived, true  ) == 0 );
    QVERIFY( spy.count() == 1 );
    LogsServices::LogsView view = 
        qvariant_cast< LogsServices::LogsView >(spy.at(0).at(0));
    QVERIFY( view == LogsServices::ViewReceived );
}

void UT_LogsServiceHandler::testStartWithNum()
{
    qRegisterMetaType< LogsServices::LogsView >("LogsServices::LogsView");
    QSignalSpy spy2(mServiceOld, SIGNAL(activateView(QString)));

    QVERIFY( mServiceOld->startWithNum( (int)LogsServices::ViewReceived, true,
            QString("+123456")  ) == 0 );
    QVERIFY( spy2.count() == 1 );
    QVERIFY( spy2.at(0).at(0).toString() == QString("+123456"));
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
    map.insert(logsShowDialpadParam, QVariant(true));
    QVERIFY( mService->show(map) == 0 );
    QVERIFY( spy.count() == 1 );
    QVERIFY( spy2.count() == 0 );
    LogsServices::LogsView view = 
            qvariant_cast< LogsServices::LogsView >(spy.at(0).at(0));
    QCOMPARE( view, LogsServices::ViewReceived );
    QCOMPARE( spy.at(0).at(1).toBool(), true );
    QCOMPARE( spy.at(0).at(2).toString(), QString(""));
    
    // Correct view, dialpad text not empty
    spy.clear();
    map.clear();
    map.insert(logsViewIndexParam, QVariant((int)LogsServices::ViewReceived));
    map.insert(logsDialpadTextParam, QVariant(QString("+123456")));
    QVERIFY( mService->show(map) == 0 );
    QVERIFY( spy.count() == 0 );
    QVERIFY( spy2.count() == 1 );
    QCOMPARE( spy2.at(0).at(0).toString(), QString("+123456"));
    
    // Some completly dummy parameters or no params at all
    spy.clear();
    spy2.clear();
    map.clear();
    map.insert("dummy_param", QVariant(666));
    QVERIFY( mService->show(map) == 0 );
    QVERIFY( spy.count() == 1 );
    QVERIFY( spy2.count() == 0 );
    view = qvariant_cast< LogsServices::LogsView >(spy.at(0).at(0));
    QCOMPARE( view, LogsServices::ViewAll );
    QCOMPARE( spy.at(0).at(1).toBool(), false );
    QCOMPARE( spy.at(0).at(2).toString(), QString(""));    
    
    // Both new and deprecated params present, new params will be used
    spy.clear();
    map.clear();
    map.insert(logsViewIndexParam, QVariant((int)LogsServices::ViewReceived));
    map.insert(logsShowDialpadParam, QVariant(false));
    map.insert(logsDialpadTextParam, QVariant(QString("+123456")));
    
    map.insert(logsViewIndexParamNew, QVariant((int)LogsServices::ViewCalled));
    map.insert(logsShowDialpadParamNew, QVariant(true));
    map.insert(logsDialpadTextParamNew, QVariant(QString("")));
    QVERIFY( mService->show(map) == 0 );
    QVERIFY( spy.count() == 1 );
    QVERIFY( spy2.count() == 0 );
    view = qvariant_cast< LogsServices::LogsView >(spy.at(0).at(0));
    QCOMPARE( view, LogsServices::ViewCalled );
    QCOMPARE( spy.at(0).at(1).toBool(), true );
    QCOMPARE( spy.at(0).at(2).toString(), QString(""));
}
