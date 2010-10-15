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
#include <QLocale>
#include <hbinputkeymap.h>
#include <hbinputsettingproxy.h>
#include <QTextCodec>

#include "logspredictivelatin12keytranslator.h"
#include "logslogger.h"


//mapping char,key(name)
const QChar SpecialMapping[] = {'+', StarKey, '*', StarKey, '#',HashKey };
const int SpecialsCount = 3;
const QChar SpaceSepar(' ');
const QChar ZeroSepar('0');
const int NotAssigned = -1;


// -----------------------------------------------------------------------------
// LogsPredictiveLatin12KeyTranslator::LogsPredictiveLatin12KeyTranslator()
// -----------------------------------------------------------------------------
//
LogsPredictiveLatin12KeyTranslator::LogsPredictiveLatin12KeyTranslator() 
    : LogsPredictive12KeyTranslator( QLocale::English )
{
    LOGS_QDEBUG( "logs [FINDER] -> LogsPredictiveLatin12KeyTranslator::\
LogsPredictiveLatin12KeyTranslator()" )
    
    createSpecialMapping();
    
    LOGS_QDEBUG( "logs [FINDER] <- LogsPredictiveLatin12KeyTranslator::\
LogsPredictiveLatin12KeyTranslator()" )
}


// -----------------------------------------------------------------------------
// LogsPredictiveLatin12KeyTranslator::LogsPredictiveLatin12KeyTranslator()
// -----------------------------------------------------------------------------
//
LogsPredictiveLatin12KeyTranslator::LogsPredictiveLatin12KeyTranslator( 
        const HbInputLanguage& lang )
    : LogsPredictive12KeyTranslator( lang )
{
    LOGS_QDEBUG( "logs [FINDER] -> LogsPredictiveLatin12KeyTranslator::\
LogsPredictiveLatin12KeyTranslator()" )
    createSpecialMapping();
    
    LOGS_QDEBUG( "logs [FINDER] <- LogsPredictiveLatin12KeyTranslator::\
LogsPredictiveLatin12KeyTranslator()" )
}

// -----------------------------------------------------------------------------
// LogsPredictiveLatin12KeyTranslator::createSpecialMapping()
// -----------------------------------------------------------------------------
//
void LogsPredictiveLatin12KeyTranslator::createSpecialMapping()
{
    int index = 0;
    int arraySize = SpecialsCount * 2;
    while( index < arraySize ) {
        QChar character = SpecialMapping[ index++ ];
        QChar keycode = SpecialMapping[ index++ ];
        mSpecialKeyMap[ character ] = keycode;
    }
    
}

// -----------------------------------------------------------------------------
// LogsPredictiveLatin12KeyTranslator::~LogsPredictiveLatin12KeyTranslator()
// -----------------------------------------------------------------------------
//
LogsPredictiveLatin12KeyTranslator::~LogsPredictiveLatin12KeyTranslator()
{
    LOGS_QDEBUG( "logs [FINDER] -> LogsPredictiveLatin12KeyTranslator::\
~LogsPredictiveLatin12KeyTranslator()" )
    mSpecialKeyMap.clear();
    LOGS_QDEBUG( "logs [FINDER] <- LogsPredictiveLatin12KeyTranslator::\
~LogsPredictiveLatin12KeyTranslator()" )
    
}

// -----------------------------------------------------------------------------
// LogsPredictiveLatin12KeyTranslator::patternTokens()
// -----------------------------------------------------------------------------
//
QStringList LogsPredictiveLatin12KeyTranslator::patternTokens( const QString& pattern ) const
{
        
    LOGS_QDEBUG( "logs [FINDER] -> LogsPredictiveLatin12KeyTranslator::\
patternTokens()" )
    LOGS_QDEBUG_2( "logs [FINDER] pattern ", pattern );
    QString car;
    QString cdr;
    
    QStringList target;
    splitPattern( pattern, car, cdr );
    if ( car.length() ) {
        target.append( car );
        if ( cdr.length() ) {
            target.append( cdr );
        }
    }
    LOGS_QDEBUG( "logs [FINDER] <- LogsPredictiveLatin12KeyTranslator::\
patternTokens()" )
    return target;
}

// -----------------------------------------------------------------------------
// LogsPredictiveLatin12KeyTranslator::splitPattern()
// -----------------------------------------------------------------------------
//
void LogsPredictiveLatin12KeyTranslator::splitPattern( const QString& pattern, 
                                                  QString& car, QString& cdr ) const
{
    car = "";
    cdr = "";
    
    QChar current;
    QChar previous;
    int splitStart = NotAssigned;
    int splitEnd = NotAssigned;
    int index = 0;
    while( splitEnd == NotAssigned && index < pattern.length() ) {
        current = pattern[index];
        splitStart = splitStart == NotAssigned &&
                    ( previous != ZeroSepar && previous != QChar() ) && 
                    current == ZeroSepar ? 
                        index : splitStart;
        splitEnd = splitStart != NotAssigned && 
                   previous == ZeroSepar && 
                   current != ZeroSepar ?
                      index : splitEnd;
        previous = current;
        index++;
    }
    
    if ( splitStart != NotAssigned && splitEnd != NotAssigned ) {
        car = pattern.left( splitStart );
        cdr = pattern.right( pattern.length() - splitEnd );  
    } else {
        car = pattern; 
    }
}

// -----------------------------------------------------------------------------
// LogsPredictiveLatin12KeyTranslator::trimPattern()
// -----------------------------------------------------------------------------
//
QString& LogsPredictiveLatin12KeyTranslator::trimPattern( QString& pattern, 
                                                     bool tailOnly ) const
{
    QRegExp lead("^0*");//remove leading zeros
    QRegExp trail("0*$");//remove trailing zeros
    
    if ( pattern.length() ) {
        if ( !tailOnly ) {
            pattern.remove( lead );
        }
        
        pattern.remove( trail );
        
        if( !pattern.length() ) {
            pattern += ZeroSepar;
        }
    }
    return pattern;
}


// -----------------------------------------------------------------------------
// LogsPredictiveLatin12KeyTranslator::hasPatternSeparators()
// -----------------------------------------------------------------------------
//
int LogsPredictiveLatin12KeyTranslator::hasPatternSeparators( 
        const QString& pattern ) const
{
    return pattern.count( ZeroSepar );

}

// -----------------------------------------------------------------------------
// LogsPredictiveLatin12KeyTranslator::translateChar()
// -----------------------------------------------------------------------------
//
const QString LogsPredictiveLatin12KeyTranslator::translateChar( 
                                                    const QChar character,
                                                    bool& ok ) const
{
    ok = true;
    QChar sch = mSpecialKeyMap[ character ];
    QString keycode( sch );
    if ( sch.isNull() ) {
        keycode = LogsPredictive12KeyTranslator::translateChar( character );
        if ( keycode.isEmpty() ) {
            QString decomposed = character.decomposition();
            if (decomposed.isEmpty()) {
                ok = false;
                return keycode;
            }
            return translateChar (decomposed.at(0), ok);
        }
    }
    return keycode;
}



// -----------------------------------------------------------------------------
// LogsPredictiveLatin12KeyTranslator::nameTokens()
// -----------------------------------------------------------------------------
//
QStringList LogsPredictiveLatin12KeyTranslator::nameTokens( 
                                                    const QString& name ) const
{
    return name.split( SpaceSepar, QString::SkipEmptyParts );
}






