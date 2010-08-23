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
#include "ut_logsmainwindow.h"
#include "logsmainwindow.h"
#include "qthighway_stub_helper.h"
#include "hbstubs_helper.h"

//SYSTEM
#include <QtTest/QtTest>
#include <hbmainwindow.h>
#include <QKeyEvent>

void UT_LogsMainWindow::initTestCase()
{

}

void UT_LogsMainWindow::cleanupTestCase()
{

}


void UT_LogsMainWindow::init()
{
    mMainWindow = new LogsMainWindow;
}

void UT_LogsMainWindow::cleanup()
{
    delete mMainWindow;
    mMainWindow = 0;
}

void UT_LogsMainWindow::testKeyPressEvent()
{
    QSignalSpy spy( mMainWindow, SIGNAL(callKeyPressed()) );
    // Not handled
    QKeyEvent event( QEvent::KeyPress, Qt::Key_Hangup, Qt::NoModifier);
    mMainWindow->keyPressEvent( &event );
    QVERIFY( spy.count() == 0 );
    
    // Handled
    QKeyEvent event2( QEvent::KeyPress, Qt::Key_Yes, Qt::NoModifier);
    mMainWindow->keyPressEvent( &event2 );
    QVERIFY( spy.count() == 1 );
    
}

void UT_LogsMainWindow::testSendAppToBackground()
{
    QtHighwayStubHelper::reset();
    mMainWindow->sendAppToBackground();
    QVERIFY( QtHighwayStubHelper::utilToBackground() );  
}

void UT_LogsMainWindow::testBringAppToForeground()
{
    QtHighwayStubHelper::reset();
    HbStubHelper::reset();
    mMainWindow->bringAppToForeground();
    QVERIFY( HbStubHelper::isWidgetRaised() ); 
    
    // Subsequent call raise tries to raise regardless of current state
    HbStubHelper::reset();
    mMainWindow->bringAppToForeground();
    QVERIFY( HbStubHelper::isWidgetRaised() ); 
}
 
void UT_LogsMainWindow::testEventFilter()
{    
    QSignalSpy spy( mMainWindow, SIGNAL(localeChanged()) );
    QSignalSpy foregroundSpy( mMainWindow, SIGNAL(appGainedForeground()) );

    //Event we are not interested in
    QEvent event(QEvent::Show);
    QVERIFY( !mMainWindow->eventFilter(this, &event) );
    QVERIFY( !mMainWindow->isForeground() );
    QVERIFY( !mMainWindow->mLocaleChanged );
    QCOMPARE( foregroundSpy.count(), 0 );
   
    //Coming foreground, locale not changed
    QEvent eventFg(QEvent::ApplicationActivate);
    QVERIFY( !mMainWindow->eventFilter(this, &eventFg) );
    QVERIFY( mMainWindow->isForeground() );
    QVERIFY( !mMainWindow->mLocaleChanged );
    QCOMPARE( foregroundSpy.count(), 1 );
    foregroundSpy.clear();
    
    //LocaleChange event on FG
    QEvent eventLocale(QEvent::LocaleChange);
    QVERIFY( !mMainWindow->eventFilter(this, &eventLocale) );
    QVERIFY( mMainWindow->isForeground() );
    QVERIFY( !mMainWindow->mLocaleChanged );
    QVERIFY( spy.count() == 1 );
    QCOMPARE( foregroundSpy.count(), 0 );
    spy.clear();
    
    //Going background
    QEvent eventBg(QEvent::ApplicationDeactivate);
    QVERIFY( !mMainWindow->eventFilter(this, &eventBg) );
    QVERIFY( !mMainWindow->isForeground() );
    QVERIFY( !mMainWindow->mLocaleChanged );
    QCOMPARE( foregroundSpy.count(), 0 );
    
    //LocaleChange event on BG
    QVERIFY( !mMainWindow->eventFilter(this, &eventLocale) );
    QVERIFY( !mMainWindow->isForeground() );
    QVERIFY( mMainWindow->mLocaleChanged );
    QVERIFY( spy.count() == 0 );

    //Coming foreground after locale cange event
    QVERIFY( !mMainWindow->eventFilter(this, &eventFg) );
    QVERIFY( mMainWindow->isForeground() );
    QVERIFY( !mMainWindow->mLocaleChanged );
    QVERIFY( spy.count() == 1 );
}
