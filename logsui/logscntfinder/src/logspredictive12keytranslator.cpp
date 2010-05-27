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
#include <hbinputkeymapfactory.h>
#include <hbinputkeymap.h>
#include <hbinputsettingproxy.h>

#include "logspredictive12keytranslator.h"
#include "logslogger.h"

const QChar ZeroSepar('0');


// -----------------------------------------------------------------------------
// LogsPredictive12KeyTranslator::LogsPredictive12KeyTranslator()
// -----------------------------------------------------------------------------
//
LogsPredictive12KeyTranslator::LogsPredictive12KeyTranslator( 
                                                    const HbInputLanguage& lang ) 
    : LogsPredictiveTranslator( lang )
{
    LOGS_QDEBUG( "logs [FINDER] -> LogsPredictive12KeyTranslator::\
LogsPredictive12KeyTranslator()" )
    LOGS_QDEBUG( "logs [FINDER] <- LogsPredictive12KeyTranslator::\
LogsPredictive12KeyTranslator()" )

}

// -----------------------------------------------------------------------------
// LogsPredictive12KeyTranslator::~LogsPredictive12KeyTranslator()
// -----------------------------------------------------------------------------
//
LogsPredictive12KeyTranslator::~LogsPredictive12KeyTranslator()
{
    LOGS_QDEBUG( "logs [FINDER] -> LogsPredictive12KeyTranslator::\
~LogsPredictive12KeyTranslator()" )
    LOGS_QDEBUG( "logs [FINDER] <- LogsPredictive12KeyTranslator::\
~LogsPredictive12KeyTranslator()" )
    
}

// -----------------------------------------------------------------------------
// LogsPredictive12KeyTranslator::LogsPredictive12KeyTranslator()
// -----------------------------------------------------------------------------
//
QStringList LogsPredictive12KeyTranslator::patternTokens( const QString& pattern ) const
{
    LOGS_QDEBUG( "logs [FINDER] -> LogsPredictive12KeyTranslator::\
patternTokens()" )
    LOGS_QDEBUG_2( "logs [FINDER] pattern ", pattern );

    QStringList target = pattern.split( ZeroSepar, QString::SkipEmptyParts );
    if ( target.length() > 1 ) {
        LOGS_QDEBUG( "logs [FINDER] has separator(s) " )
        QString& first = target[0];
        QString& last = target[target.length()-1];
        padWithZeros( first, pattern, 0 );
        padWithZeros( last, pattern, last.length() );
    } else if ( target.length() == 1 && //0280 -> 028
                pattern[pattern.length()-1] == ZeroSepar ) {
        LOGS_QDEBUG( "logs [FINDER] no separators, trailing zero(s) " )
        QString& first = target[0];
        padWithZeros( first, pattern, 0 );
    } else if ( target.length() == 0 ) {
        LOGS_QDEBUG( "logs [FINDER] only separators " )
        target.append( ZeroSepar );
    }
    LOGS_QDEBUG( "logs [FINDER] <- LogsPredictive12KeyTranslator::\
patternTokens()" )
    return target;
}

// -----------------------------------------------------------------------------
// LogsPredictive12KeyTranslator::hasPatternSeparators()
// -----------------------------------------------------------------------------
//
int LogsPredictive12KeyTranslator::hasPatternSeparators( 
        const QString& pattern ) const
{
    return pattern.count( ZeroSepar );

}

// -----------------------------------------------------------------------------
// LogsPredictive12KeyTranslator::translateChar()
// -----------------------------------------------------------------------------
//
const QChar LogsPredictive12KeyTranslator::translateChar( 
        const QChar character ) const
{
    const HbMappedKey* mappedKey = 
            mKeyMap->keyForCharacter( HbKeyboardVirtual12Key, character );
    return mappedKey ? mappedKey->keycode : QChar();
}



// -----------------------------------------------------------------------------
// LogsPredictive12KeyTranslator::padWithLeadingZeros()
// -----------------------------------------------------------------------------
//
void LogsPredictive12KeyTranslator::padWithZeros( QString& token, 
                                     const QString& source, int padIndex ) const
{
    const QChar* content = source.data();
    int index = !padIndex ? 0 : source.length()-1;
            
    while( index >= 0 && index < source.length() ) {
        if ( content[ index ] == ZeroSepar ) {
            token.insert( padIndex, ZeroSepar );
            index = !padIndex ? index+1 : index-1;
        } else {
            index = -1;
        }
    }
}

