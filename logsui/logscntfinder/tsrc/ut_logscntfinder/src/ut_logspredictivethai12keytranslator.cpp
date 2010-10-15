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
}

void UT_LogsPredictiveThai12KeyTranslator::cleanupTestCase()
{
    
    
}


void UT_LogsPredictiveThai12KeyTranslator::init()
{
    
    HbInputLanguage thai( QLocale::Thai );
    mTranslator = new LogsPredictiveThai12KeyTranslator( thai );
    
}

void UT_LogsPredictiveThai12KeyTranslator::cleanup()
{
    delete mTranslator;
    mTranslator = 0;
}

void UT_LogsPredictiveThai12KeyTranslator::testConstructor()
{
    QVERIFY( mTranslator );
    
}

void UT_LogsPredictiveThai12KeyTranslator::testTranslateText()
{
    QEXPECT_FAIL("", "No proper Thai keymap yet", Abort );
    QCOMPARE( mTranslator->translateText( QString( "0123456789" ) ), QString( "0123456789" ) );
    //finds name latin name translator
    QCOMPARE( mTranslator->translateText( QString( "Nokia" ) ), QString( "66542" ) );
    
    const int ucsize = 9;
    //                           1      2      3      4      5     6      7      8     9
    const QChar thaiName1[] = {0x0E01,0x0E06,0x0E0A,0x0E0E,0x0E14,0x0E19,0x0E1E,0x0E23,0x0E2A };
    QCOMPARE( mTranslator->translateText( QString( thaiName1,ucsize ) ), QString( "123456789" ) );
    
    //                          1       2      *      4      5     6      #      8     9
    const QChar thaiName2[] = {0x0E01,0x0E06,0x0E2F,0x0E0E,0x0E14,0x0E19,0x0E30,0x0E23,0x0E2A };
    QCOMPARE( mTranslator->translateText( QString( thaiName2,ucsize ) ), QString( "1245689" ) );
    
            
}

void UT_LogsPredictiveThai12KeyTranslator::testPreparePattern()
{
    QEXPECT_FAIL("", "No proper Thai keymap yet", Abort );
    
    QCOMPARE( mTranslator->preparePattern( QString( "0123456789" ) ), QString( "0123456789" ) );
    QCOMPARE( mTranslator->preparePattern( QString( "*0123##456*789*" ) ), QString( "0123456789" ) );
            
}

void UT_LogsPredictiveThai12KeyTranslator::testTranslateChar()
{
    QEXPECT_FAIL("", "No proper Thai keymap yet", Abort );
    bool ok;
    
    QCOMPARE( mTranslator->translateChar( QChar('0'), ok ), QString('0') );
    QVERIFY( ok );
    QCOMPARE( mTranslator->translateChar( QChar('1'), ok ), QString('1') );
    QVERIFY( ok );
    QCOMPARE( mTranslator->translateChar( QChar('N'), ok ), QString() );
    QVERIFY( !ok );
    QCOMPARE( mTranslator->translateChar( QChar('*'), ok ), QString() );
    QVERIFY( ok );
    QCOMPARE( mTranslator->translateChar( QChar('#'), ok ), QString() );
    QVERIFY( ok );
    QCOMPARE( mTranslator->translateChar( QChar(0x0E01), ok ), QString('1') );
    QVERIFY( ok );
    QCOMPARE( mTranslator->translateChar( QChar(0x0E06), ok ), QString('2') );
    QCOMPARE( mTranslator->translateChar( QChar(0x0E0A), ok ), QString('3') );
    QCOMPARE( mTranslator->translateChar( QChar(0x0E0E), ok ), QString('4') );
    QCOMPARE( mTranslator->translateChar( QChar(0x0E14), ok ), QString('5') );
    QCOMPARE( mTranslator->translateChar( QChar(0x0E19), ok ), QString('6') );
    QCOMPARE( mTranslator->translateChar( QChar(0x0E1E), ok ), QString('7') );
    QCOMPARE( mTranslator->translateChar( QChar(0x0E23), ok ), QString('8') );
    QCOMPARE( mTranslator->translateChar( QChar(0x0E2A), ok ), QString('9') );
    QCOMPARE( mTranslator->translateChar( QChar(0x0E2F), ok ), QString() );
    QVERIFY( ok );
    QCOMPARE( mTranslator->translateChar( QChar(0x0E30), ok ), QString() );
    QVERIFY( ok );
    
}


void UT_LogsPredictiveThai12KeyTranslator::testEncodes()
{
    QCOMPARE( mTranslator->mib(), MIBenumThai );
    
    //china
    QString uni1;
    uni1.append(QChar(0x4E0E));
    uni1.append(QChar(0x4E0F));
    QVERIFY( !mTranslator->encodes( uni1) );
    
    QVERIFY( !mTranslator->encodes( QString( "Zulu" ) ) );
    
    QEXPECT_FAIL("", "No proper Thai keymap yet", Abort );
    //thai
    const int ucsize = 9;
    const QChar thaiName2[] = {0x0E01,0x0E06,0x0E2F,0x0E0E,0x0E14,0x0E19,0x0E30,0x0E23,0x0E2A };
    
    QVERIFY( mTranslator->encodes( QString( thaiName2,ucsize ) ) );
    

}


