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
#include "ut_logssystemtimeobserver.h"
#include "logssystemtimeobserver.h"
#include <QtTest/QtTest>
#include <HbExtendedLocale>
#include <bacntf.h> 

void UT_LogsSystemTimeObserver::initTestCase()
{
}

void UT_LogsSystemTimeObserver::cleanupTestCase()
{
}

void UT_LogsSystemTimeObserver::init()
{    
    mTimeObserver = new LogsSystemTimeObserver();
}

void UT_LogsSystemTimeObserver::cleanup()
{
    delete mTimeObserver;
}

void UT_LogsSystemTimeObserver::testConstructor()
{
    QVERIFY( mTimeObserver );
    QVERIFY( mTimeObserver->mEnvChangeNotifier->IsActive() );
}

void UT_LogsSystemTimeObserver::testEnvironmentChangeCallback()
{
    QSignalSpy spy(mTimeObserver, SIGNAL(timeFormatChanged()));
    QCOMPARE( mTimeObserver->EnvironmentChangeCallback(mTimeObserver), 0 );
    QCOMPARE( spy.count(), 0 );
}
