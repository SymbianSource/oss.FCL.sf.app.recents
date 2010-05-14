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
#include <QObject>
#include <QLocale>
#include <QHash>
#include <hbinputsettingproxy.h>
#include <QStringList>

#include "logspredictivetranslator.h"
#include "logspredictivelatin12keytranslator.h"
#include "logspredictivethai12keytranslator.h"
#include "logslogger.h"

LogsPredictiveTranslator* LogsPredictiveTranslator::mInstance = 0;


// -----------------------------------------------------------------------------
// LogsPredictiveTranslator::instance()
// -----------------------------------------------------------------------------
//
LogsPredictiveTranslator* LogsPredictiveTranslator::instance()
{
    if ( !mInstance ) {
        HbInputLanguage lang = 
                HbInputSettingProxy::instance()->globalInputLanguage();
        switch( lang.language() ) {
            case QLocale::Thai:
                mInstance = new LogsPredictiveThai12KeyTranslator();
                break;
            default:
                mInstance = new LogsPredictiveLatin12KeyTranslator();
                break;
            }
    }
    return mInstance;
}

// -----------------------------------------------------------------------------
// LogsPredictiveTranslator::deleteInstance()
// -----------------------------------------------------------------------------
//
void LogsPredictiveTranslator::deleteInstance()
{
    delete mInstance;
    mInstance = 0;
}


// -----------------------------------------------------------------------------
// LogsPredictiveTranslator::LogsPredictiveTranslator()
// -----------------------------------------------------------------------------
//
LogsPredictiveTranslator::LogsPredictiveTranslator()
{
    LOGS_QDEBUG( "logs [FINDER] -> LogsPredictiveTranslator::\
LogsPredictiveTranslator()" )
    LOGS_QDEBUG( "logs [FINDER] <- LogsPredictiveTranslator::\
LogsPredictiveTranslator()" )
}


// -----------------------------------------------------------------------------
// LogsPredictiveTranslator::~LogsPredictiveTranslator()
// -----------------------------------------------------------------------------
//
LogsPredictiveTranslator::~LogsPredictiveTranslator()
{
    LOGS_QDEBUG( "logs [FINDER] -> LogsPredictiveTranslator::\
~LogsPredictiveTranslator()" )
    mInstance = 0;
    mKeyMap = 0;
    LOGS_QDEBUG( "logs [FINDER] <- LogsPredictiveTranslator::\
~LogsPredictiveTranslator()" )
    
}

// -----------------------------------------------------------------------------
// LogsPredictiveTranslator::translate()
// -----------------------------------------------------------------------------
//
const QString LogsPredictiveTranslator::translate( const QString& name, 
                                                   int count ) const
{
    LOGS_QDEBUG( "logs [FINDER] -> LogsPredictiveTranslator::translate()" )
    LOGS_QDEBUG_2( "logs [FINDER] name ", name );
    
    count = count == -1 ? name.length() : count;
    QString result;
    const QChar* content = name.data();
    int index = 0;
    while( index < count ) {
        QChar ch = translateChar( *content++ );
        if ( !ch.isNull() ) {
            result.append( ch );
        }
        index++;
    }
    LOGS_QDEBUG( "logs [FINDER] <- LogsPredictiveTranslator::translate()" )
    return result;
}

// -----------------------------------------------------------------------------
// LogsPredictiveTranslator::startsWith()
// -----------------------------------------------------------------------------
//
int LogsPredictiveTranslator::startsWith( const QString& text, 
                                          const QString& pattern,
                                          bool optimize ) const
{
    LOGS_QDEBUG( "logs [FINDER] -> LogsPredictiveTranslator::startsWith()" )
    //assumed that text has found based on pattern, thus only checking with
    //first char is enough, if mightContainZeroes eq false
    int matchCount = pattern.length();
    if ( text.isEmpty() || matchCount > text.length() ) {
        matchCount = 0;
    } else {
        if ( !optimize ) {
            QString translatedText = translate( text, pattern.length() );
            matchCount = translatedText == pattern ? matchCount : 0; 
        } else {
            matchCount = translateChar( *text.data() ) == *pattern.data() ? 
                         matchCount : 0;
        }
    }
        
    LOGS_QDEBUG_2( "logs [FINDER] matchCount=", matchCount )
    LOGS_QDEBUG( "logs [FINDER] <- LogsPredictiveTranslator::startsWith()" )
    return matchCount;
}

