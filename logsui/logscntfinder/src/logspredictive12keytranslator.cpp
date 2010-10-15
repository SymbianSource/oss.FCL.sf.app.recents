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
#include <QRegExp>
#include <hbinputkeymapfactory.h>
#include <hbinputkeymap.h>
#include <hbinputsettingproxy.h>

#include "logspredictive12keytranslator.h"
#include "logslogger.h"


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
// LogsPredictive12KeyTranslator::translateChar()
// -----------------------------------------------------------------------------
//
const QString LogsPredictive12KeyTranslator::translateChar( 
        const QChar character ) const
{
    const HbMappedKey* mappedKey = 0;
    if ( mKeyMap ) {
        mappedKey = mKeyMap->keyForCharacter( HbKeyboardVirtual12Key, character );
        if ( !mappedKey ) {
            mappedKey = mKeyMap->keyForCharacter( HbKeyboardSctPortrait, character );
            return mappedKey ? QString( StarKey ) : QString();
        }
    }
    return mappedKey ? QString( mappedKey->keycode ) : QString();
}

// -----------------------------------------------------------------------------
// LogsPredictive12KeyTranslator::match()
// -----------------------------------------------------------------------------
//
bool LogsPredictive12KeyTranslator::match( 
        const QString& pattern, 
        LogsCntTokenIterator& names ) const
{
    QString modifiedPattern = pattern;
    modifiedPattern = trimPattern( modifiedPattern, true );
    
    bool match = doSimpleMatch( modifiedPattern, names ); 
        
    if (!match && hasPatternSeparators( modifiedPattern ) ) {
        QStringList patternArray = patternTokens( modifiedPattern );
        match = doComplexMatch( patternArray, names );
        if (!match ) {
            for(int i=0;i<patternArray.length();i++ ) {
                trimPattern( patternArray[i] );
            }
            match = doComplexMatch( patternArray, names );
        }
    }
    
    return match;
}

// -----------------------------------------------------------------------------
// LogsPredictive12KeyTranslator::doSimpleMatch()
// -----------------------------------------------------------------------------
//
bool LogsPredictive12KeyTranslator::doSimpleMatch( 
                        const QString& pattern,
                        LogsCntTokenIterator& names ) const
{
    int matchCount = 0;
    names.toFront();
    
    while( names.hasNext() && !matchCount ) {
        matchCount = (int)names.next().translation().startsWith( pattern );
    }

    return matchCount > 0;
}


// -----------------------------------------------------------------------------
// LogsPredictive12KeyTranslator::doComplexMatch()
// -----------------------------------------------------------------------------
//
bool LogsPredictive12KeyTranslator::doComplexMatch( 
                        const QStringList& patternArray,
                        LogsCntTokenIterator& names ) const
{
    const bool zero = false;
    names.toFront();

    int targetMatchCount = patternArray.count();
    int namesCount = names.count();

    //if pattern has more tokens than name(s), it is a missmatch
    if ( namesCount < targetMatchCount ) {
        return false;
    }

    QListIterator<QString> patterns( patternArray );
    QVector<bool> matchVector(targetMatchCount, zero );
    int currentPattern = 0;
    int matchCount = 0;
    bool match = false;
    
    while( names.hasNext() && matchCount < targetMatchCount ) {
        LogsCntToken name = names.next();
        currentPattern = 0;
        patterns.toFront();
        match = false;
        while ( !name.text().isEmpty() && 
                 patterns.hasNext() && !match ) {
            QString pattern = patterns.next();
            //unique match check
            if ( !matchVector.at( currentPattern ) ) {
                match = matchVector[ currentPattern ] 
                      = name.translation().startsWith( pattern );
                matchCount = match ? matchCount+1 : matchCount;
            }
            currentPattern++;
        }
    }
    return matchCount >= targetMatchCount;

    }

