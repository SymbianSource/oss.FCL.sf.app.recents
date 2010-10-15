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
#include "logspredictivelatin12keytranslator.h"
#include "logspredictivethai12keytranslator.h"

#include <QtTest/QtTest>
#include "qtcontacts_stubs.h"


void UT_LogsPredictiveTranslator::initTestCase()
{
    
}

void UT_LogsPredictiveTranslator::cleanupTestCase()
{
    LOGS_CNTFINDER_SET_PHONE_LANGUAGE( QLocale::English );    
}


void UT_LogsPredictiveTranslator::init()
{
    mTranslator = LogsPredictiveTranslator::instance();
}

void UT_LogsPredictiveTranslator::cleanup()
{
    LogsPredictiveTranslator::deleteInstance();
    LOGS_CNTFINDER_SET_PHONE_LANGUAGE( QLocale::English );    
    
}

void UT_LogsPredictiveTranslator::testConstructor()
{
    QVERIFY( mTranslator );
    QCOMPARE( mTranslator->mib(), MIBenumLatin );
    QVERIFY( mTranslator->mKeyMap );
    QVERIFY( mTranslator->mNameTranslator == 0 );
    QVERIFY( LogsPredictiveTranslator::mInstance != 0 );
    LogsPredictiveTranslator::deleteInstance();
    QVERIFY( LogsPredictiveTranslator::mInstance == 0 );
    
    mTranslator = 0;
    LOGS_CNTFINDER_SET_PHONE_LANGUAGE( QLocale::Thai );    

    mTranslator = LogsPredictiveTranslator::instance();
    QVERIFY( mTranslator );
    QCOMPARE( mTranslator->mib(), MIBenumThai );
    //temp off QVERIFY( mTranslator->mKeyMap );
    QVERIFY( mTranslator->mNameTranslator == 0 );
    QVERIFY( LogsPredictiveTranslator::mInstance != 0 );
    
    LogsPredictiveTranslator::deleteInstance();
    QVERIFY( LogsPredictiveTranslator::mInstance == 0 );
    mTranslator = 0;

    
}

void UT_LogsPredictiveTranslator::testTranslate()
{
    QString test1( "12345" );
    bool ok;
    
    QCOMPARE( mTranslator->LogsPredictiveTranslator::translate( test1 ).length(),
              test1.length() );
    
    QCOMPARE( mTranslator->LogsPredictiveTranslator::translate( test1, &ok, 2 ).length(),
              2 );
    QVERIFY( ok );
    
    QString test2("abc");
    test2.append(QChar(9));
    test2.append("efghijklmn");
    
    QCOMPARE( mTranslator->LogsPredictiveTranslator::translate( test2, &ok ).length(),
              3);
              
    QVERIFY( !ok );
    
}

void UT_LogsPredictiveTranslator::testPreparePattern()
{
    QString test1( "%12345" );
    
    QCOMPARE( mTranslator->LogsPredictiveTranslator::preparePattern( test1 ).length(),
              test1.length() );
    
}


void UT_LogsPredictiveTranslator::testTranslateText()
{
    QString test1( "%12345" );
    
    QCOMPARE( mTranslator->translateText( test1 ).length(), test1.length() );
    QVERIFY( !mTranslator->mNameTranslator );
    QCOMPARE( mTranslator->mib(), MIBenumLatin );
    
    //china
    QString uni;
    uni.append(QChar(0x0219));
    uni.append(QChar(0x4E0F));
    
    mTranslator->translateText( uni );
    QVERIFY( !mTranslator->mNameTranslator );
    QCOMPARE( mTranslator->mib(), MIBenumLatin );
    

    
}

void UT_LogsPredictiveTranslator::testNameTranslator()
{
    QString test1( "12345" );
    QCOMPARE( mTranslator->mib(), MIBenumLatin );
    QVERIFY( !mTranslator->mNameTranslator );
    QCOMPARE( mTranslator->nameTranslator( test1 ).mib(), MIBenumLatin );
    QVERIFY( mTranslator->mNameTranslator );

    //china
    QString uni;
    uni.append(QChar(0x0219));
    uni.append(QChar(0x4E0F));
    QCOMPARE( mTranslator->nameTranslator( uni ).mib(), MIBenumLatin );
    QVERIFY( !mTranslator->mNameTranslator );
    QCOMPARE( mTranslator->mib(), MIBenumLatin );
    
    QEXPECT_FAIL("", "No proper Thai keymap yet", Abort );
    
    //text is thai, input lang latin
    const int ucsize = 9;
    //                           1      2      3      4      5     6      7      8     9
    const QChar thaiName1[] = {0x0E01,0x0E06,0x0E0A,0x0E0E,0x0E14,0x0E19,0x0E1E,0x0E23,0x0E2A };
    QCOMPARE( mTranslator->nameTranslator( QString( thaiName1,ucsize ) ).mib(), MIBenumThai );
    QVERIFY( mTranslator->mNameTranslator );
    QCOMPARE( mTranslator->mNameTranslator->mib(), MIBenumThai );
    QCOMPARE( mTranslator->mib(), MIBenumLatin );
    
    QCOMPARE( mTranslator->nameTranslator( QString( thaiName1,ucsize ) ).mib(), MIBenumThai );
    QVERIFY( mTranslator->mNameTranslator );
    QCOMPARE( mTranslator->mNameTranslator->mib(), MIBenumThai );
    QCOMPARE( mTranslator->mib(), MIBenumLatin );
    
}




