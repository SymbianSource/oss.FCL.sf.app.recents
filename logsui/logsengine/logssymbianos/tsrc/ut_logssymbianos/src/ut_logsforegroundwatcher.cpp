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
#include "ut_logsforegroundwatcher.h"
#include "logsforegroundwatcher.h"

#include <QtTest/QtTest>
#include <coemain.h>

CCoeEnv* testEnv = 0;

CCoeEnv* CCoeEnv::Static()
{
    testEnv = new CCoeEnv();
    return testEnv;
}

CCoeEnv::CCoeEnv() : CActive(EPriorityNormal)
{
    
}

CCoeEnv::~CCoeEnv()
{
}

void CCoeEnv::AddForegroundObserverL(MCoeForegroundObserver& /*aForegroundObserver*/)
{
    
}

void UT_LogsForegroundWatcher::initTestCase()
{
}

void UT_LogsForegroundWatcher::cleanupTestCase()
{
    delete testEnv;
    testEnv = 0;
}

void UT_LogsForegroundWatcher::init()
{
    mWatcher = new LogsForegroundWatcher();
}

void UT_LogsForegroundWatcher::cleanup()
{
    delete mWatcher;
}

void UT_LogsForegroundWatcher::testConstructor()
{
    QVERIFY( mWatcher );
}

void UT_LogsForegroundWatcher::testHandleLosingForeground()
{
    QSignalSpy spyGaining( mWatcher, SIGNAL(gainingForeground()) );
    QSignalSpy spyLosing( mWatcher, SIGNAL(losingForeground()) );
    mWatcher->HandleLosingForeground();
    QVERIFY( spyGaining.count() == 0 );
    QVERIFY( spyLosing.count() == 1 );
}

void UT_LogsForegroundWatcher::testHandleGainingForeground()
{
    QSignalSpy spyGaining( mWatcher, SIGNAL(gainingForeground()) );
    QSignalSpy spyLosing( mWatcher, SIGNAL(losingForeground()) );
    mWatcher->HandleGainingForeground();
    QVERIFY( spyGaining.count() == 1 );
    QVERIFY( spyLosing.count() == 0 );
}

