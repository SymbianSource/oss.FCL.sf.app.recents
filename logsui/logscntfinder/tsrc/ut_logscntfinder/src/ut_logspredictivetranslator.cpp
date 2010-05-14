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

#include <hbinputsettingproxy.h>

#include "ut_logspredictivetranslator.h"
#include "logspredictivetranslator.h"

#include <QtTest/QtTest>


void UT_LogsPredictiveTranslator::initTestCase()
{
    mOrigLang = HbInputSettingProxy::instance()->globalInputLanguage();
    
}

void UT_LogsPredictiveTranslator::cleanupTestCase()
{
    HbInputSettingProxy::instance()->setGlobalInputLanguage( mOrigLang );
    
}


void UT_LogsPredictiveTranslator::init()
{
    mTranslator = LogsPredictiveTranslator::instance();
}

void UT_LogsPredictiveTranslator::cleanup()
{
    LogsPredictiveTranslator::deleteInstance();
    HbInputSettingProxy::instance()->setGlobalInputLanguage( mOrigLang );
    
}

void UT_LogsPredictiveTranslator::testConstructor()
{
    QVERIFY( mTranslator );
    QVERIFY( mTranslator->mKeyMap );
    LogsPredictiveTranslator::deleteInstance();
    
    mTranslator = 0;
    HbInputLanguage thai( QLocale::Thai );
    HbInputSettingProxy::instance()->setGlobalInputLanguage( thai );

    mTranslator = LogsPredictiveTranslator::instance();
    QVERIFY( mTranslator );
    QVERIFY( mTranslator->mKeyMap );
    
}


void UT_LogsPredictiveTranslator::testTranslate()
{
    QString test1( "12345" );
    
    QCOMPARE( mTranslator->LogsPredictiveTranslator::translate( test1 ).length(),
              test1.length() );
    
    QCOMPARE( mTranslator->LogsPredictiveTranslator::translate( test1, 2 ).length(),
              2 );
}

void UT_LogsPredictiveTranslator::testStartsWith()
{
    QCOMPARE( mTranslator->startsWith( QString("" ), QString("") ), 0 ); 
    QCOMPARE( mTranslator->startsWith( QString("123" ), QString("") ), 0 );
    QCOMPARE( mTranslator->startsWith( QString("" ), QString("123") ), 0 );
    QCOMPARE( mTranslator->startsWith( QString("123" ), QString("1234") ), 0 ); 
    QCOMPARE( mTranslator->startsWith( QString("123" ), QString("1") ), 1 ); 
    QCOMPARE( mTranslator->startsWith( QString("123" ), QString("123") ), 3 ); 
    QCOMPARE( mTranslator->startsWith( QString("123" ), QString("12"), true ), 2 ); 
    QCOMPARE( mTranslator->startsWith( QString("123" ), QString("12"), false ), 2 );
    
}




