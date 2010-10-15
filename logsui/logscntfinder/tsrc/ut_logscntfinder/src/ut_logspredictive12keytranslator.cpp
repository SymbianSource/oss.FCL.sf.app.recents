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
#include "qtcontacts_stubs.h"

class LogsPredictive12KeyTranslator_mock : public LogsPredictive12KeyTranslator
{
public:
    
    inline LogsPredictive12KeyTranslator_mock() 
        :LogsPredictive12KeyTranslator( QLocale::English ) 
    {  
    }
    
    inline QStringList nameTokens( const QString& name ) const
        {return QStringList(name);}
    inline const QString translateChar( const QChar /*character*/, bool& /*ok*/ ) const
        {return QString();}
    inline bool encodes( const QString& /*sniplet*/ )
        {return true;}
    inline int mib() const 
        {return 0;}
    inline QString& trimPattern( QString& pattern, bool /*tailOnly*/ ) const
        {return pattern;}
    inline int hasPatternSeparators( const QString& pattern ) const
        {return pattern.length();}
    inline QStringList patternTokens( const QString& pattern ) const
        {return QStringList( pattern );}
    
    

};


void UT_LogsPredictive12KeyTranslator::initTestCase()
{
    
}

void UT_LogsPredictive12KeyTranslator::cleanupTestCase()
{
    LOGS_CNTFINDER_SET_PHONE_LANGUAGE( QLocale::English );    
}


void UT_LogsPredictive12KeyTranslator::init()
{
    mTranslator = new LogsPredictive12KeyTranslator_mock();
}

void UT_LogsPredictive12KeyTranslator::cleanup()
{
    delete mTranslator;
    mTranslator = 0;
    
}

void UT_LogsPredictive12KeyTranslator::testConstructor()
{
    QVERIFY( mTranslator );
    QVERIFY( mTranslator->mKeyMap );
}


void UT_LogsPredictive12KeyTranslator::testTranslateChar()
{
    //latin
    QCOMPARE( mTranslator->translateChar( QChar('0') ), QString('0') );
    QCOMPARE( mTranslator->translateChar( QChar('%') ), QString('*') );
    QCOMPARE( mTranslator->translateChar( QChar(0x4E0F) ), QString() );
    mTranslator->mKeyMap = 0;
    QCOMPARE( mTranslator->translateChar( QChar('0') ), QString() );
    
}


