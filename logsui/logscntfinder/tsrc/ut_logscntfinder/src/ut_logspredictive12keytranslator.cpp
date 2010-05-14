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
#include "ut_logspredictive12keytranslator.h"
#include "logspredictive12keytranslator.h"

#include <hbinputsettingproxy.h>
#include <hbinputkeymapfactory.h>

#include <QtTest/QtTest>
#include <QSignalSpy>

class LogsPredictive12KeyTranslator_mock : public LogsPredictive12KeyTranslator
{
public:
    
    inline LogsPredictive12KeyTranslator_mock() 
        :LogsPredictive12KeyTranslator() 
    {  mKeyMap = 
       HbKeymapFactory::instance()->keymap( QLocale::English, 
                                            QLocale::AnyCountry );
    }
    
    inline QStringList nameTokens( const QString& /*name*/ ) const
            {return QStringList();}

};


void UT_LogsPredictive12KeyTranslator::initTestCase()
{
    mOrigLang = HbInputSettingProxy::instance()->globalInputLanguage();
    
}

void UT_LogsPredictive12KeyTranslator::cleanupTestCase()
{
    HbInputSettingProxy::instance()->setGlobalInputLanguage( mOrigLang );
    
}


void UT_LogsPredictive12KeyTranslator::init()
{
    mTranslator = new LogsPredictive12KeyTranslator_mock();
}

void UT_LogsPredictive12KeyTranslator::cleanup()
{
    delete mTranslator;
    mTranslator = 0;
    HbInputSettingProxy::instance()->setGlobalInputLanguage( mOrigLang );
    
}

void UT_LogsPredictive12KeyTranslator::testConstructor()
{
    QVERIFY( mTranslator );
    QVERIFY( mTranslator->mKeyMap );
}

void UT_LogsPredictive12KeyTranslator::testPatternTokens()
{
    QCOMPARE( mTranslator->patternTokens( QString("122" ) ).length(), 1 );
    QCOMPARE( mTranslator->patternTokens( QString("122" ) )[0], QString("122" ) );
    QCOMPARE( mTranslator->patternTokens( QString("102" ) ).length(), 2 );
    QCOMPARE( mTranslator->patternTokens( QString("102" ) )[0], QString("1" ) );
    QCOMPARE( mTranslator->patternTokens( QString("102" ) )[1], QString("2" ) );
    QCOMPARE( mTranslator->patternTokens( QString("0010200" ) ).length(), 2 );
    QCOMPARE( mTranslator->patternTokens( QString("0010200" ) )[0], QString("001" ) );
    QCOMPARE( mTranslator->patternTokens( QString("0010200" ) )[1], QString("200" ) );
    QCOMPARE( mTranslator->patternTokens( QString("0" ) ).length(), 1 );
    QCOMPARE( mTranslator->patternTokens( QString("0" ) )[0], QString("0" ) );
    QCOMPARE( mTranslator->patternTokens( QString("000" ) ).length(), 1 );
    QCOMPARE( mTranslator->patternTokens( QString("000" ) )[0], QString("0" ) );
    QCOMPARE( mTranslator->patternTokens( QString("001" ) ).length(), 1 );
    QCOMPARE( mTranslator->patternTokens( QString("001" ) )[0], QString("1" ) );
    
}

void UT_LogsPredictive12KeyTranslator::testHasPatternSeparators()
{
    QVERIFY( !mTranslator->hasPatternSeparators( QString("122" ) ) );
    QVERIFY( mTranslator->hasPatternSeparators( QString("1022" ) ) );
    QCOMPARE( mTranslator->hasPatternSeparators( QString("0010200" ) ), 5 );
    QCOMPARE( mTranslator->hasPatternSeparators( QString("0000" ) ), 4 );
    QCOMPARE( mTranslator->hasPatternSeparators( QString("345345" ) ), 0 );
    
}

void UT_LogsPredictive12KeyTranslator::testTranslateChar()
{
    //latin
    QCOMPARE( mTranslator->translateChar( QChar('0') ), QChar('0') );
    QCOMPARE( mTranslator->translateChar( QChar(0x4E0F) ), QChar() );
    
}


