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
#include "ut_logspredictivethai12keytranslator.h"
#include "logspredictivethai12keytranslator.h"

#include <hbinputsettingproxy.h>

#include <QtTest/QtTest>
#include <QSignalSpy>


void UT_LogsPredictiveThai12KeyTranslator::initTestCase()
{
    mOrigLang = HbInputSettingProxy::instance()->globalInputLanguage();
}

void UT_LogsPredictiveThai12KeyTranslator::cleanupTestCase()
{
    
    HbInputSettingProxy::instance()->setGlobalInputLanguage( mOrigLang );
    
}


void UT_LogsPredictiveThai12KeyTranslator::init()
{
    
    HbInputLanguage thai( QLocale::Thai );
    
    HbInputSettingProxy::instance()->setGlobalInputLanguage( thai );
    mTranslator = new LogsPredictiveThai12KeyTranslator();
    
}

void UT_LogsPredictiveThai12KeyTranslator::cleanup()
{
    HbInputSettingProxy::instance()->setGlobalInputLanguage( mOrigLang );
    delete mTranslator;
    mTranslator = 0;
}

void UT_LogsPredictiveThai12KeyTranslator::testConstructor()
{
    QVERIFY( mTranslator );
    
}

void UT_LogsPredictiveThai12KeyTranslator::testTranslate()
{
    QCOMPARE( mTranslator->translate( QString( "0123456789" ) ), QString( "0123456789" ) );
    QCOMPARE( mTranslator->translate( QString( "Nokia" ) ), QString( "" ) );
    QCOMPARE( mTranslator->translate( QString( "*0123##456*789*" ) ), QString( "0123456789" ) );
    
    const int ucsize = 9;
    //                           1      2      3      4      5     6      7      8     9
    const QChar thaiName1[] = {0x0E01,0x0E06,0x0E0A,0x0E0E,0x0E14,0x0E19,0x0E1E,0x0E23,0x0E2A };
    QCOMPARE( mTranslator->translate( QString( thaiName1,ucsize ) ), QString( "123456789" ) );
    
    //                          1       2      *      4      5     6      #      8     9
    const QChar thaiName2[] = {0x0E01,0x0E06,0x0E2F,0x0E0E,0x0E14,0x0E19,0x0E30,0x0E23,0x0E2A };
    QCOMPARE( mTranslator->translate( QString( thaiName2,ucsize ) ), QString( "1245689" ) );
    
            
}

void UT_LogsPredictiveThai12KeyTranslator::testTranslateChar()
{

    QCOMPARE( mTranslator->translateChar( QChar('0') ), QChar('0') );
    QCOMPARE( mTranslator->translateChar( QChar('1') ), QChar('1') );
    QCOMPARE( mTranslator->translateChar( QChar('N') ), QChar() );
    QCOMPARE( mTranslator->translateChar( QChar('*') ), QChar() );
    QCOMPARE( mTranslator->translateChar( QChar('#') ), QChar() );
    QCOMPARE( mTranslator->translateChar( QChar(0x0E01) ), QChar('1') );
    QCOMPARE( mTranslator->translateChar( QChar(0x0E06) ), QChar('2') );
    QCOMPARE( mTranslator->translateChar( QChar(0x0E0A) ), QChar('3') );
    QCOMPARE( mTranslator->translateChar( QChar(0x0E0E) ), QChar('4') );
    QCOMPARE( mTranslator->translateChar( QChar(0x0E14) ), QChar('5') );
    QCOMPARE( mTranslator->translateChar( QChar(0x0E19) ), QChar('6') );
    QCOMPARE( mTranslator->translateChar( QChar(0x0E1E) ), QChar('7') );
    QCOMPARE( mTranslator->translateChar( QChar(0x0E23) ), QChar('8') );
    QCOMPARE( mTranslator->translateChar( QChar(0x0E2A) ), QChar('9') );
    QCOMPARE( mTranslator->translateChar( QChar(0x0E2F) ), QChar() );
    QCOMPARE( mTranslator->translateChar( QChar(0x0E30) ), QChar() );
    
}


