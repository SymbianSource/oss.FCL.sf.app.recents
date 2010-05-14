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
LogsPredictive12KeyTranslator::LogsPredictive12KeyTranslator() 
    : LogsPredictiveTranslator()
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
    QStringList target = pattern.split( ZeroSepar, QString::SkipEmptyParts );
    if ( target.length() > 1 ) {
        QString& first = target[0];
        QString& last = target[target.length()-1];
        padWithZeros( first, pattern, 0 );
        padWithZeros( last, pattern, last.length() );
    } else if ( target.length() == 0 ) {
        target.append( ZeroSepar );
    }
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

