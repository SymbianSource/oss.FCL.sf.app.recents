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
#include <hbinputkeymapfactory.h>
#include <hbinputkeymap.h>
#include <hbinputsettingproxy.h>
#include <QTextCodec>

#include "logspredictivethai12keytranslator.h"
#include "logslogger.h"

const QChar IgnoreList[] = {'*', '#' };
const int IgnoreCount = 2;




// -----------------------------------------------------------------------------
// LogsPredictiveThai12KeyTranslator::LogsPredictiveThai12KeyTranslator()
// -----------------------------------------------------------------------------
//
LogsPredictiveThai12KeyTranslator::LogsPredictiveThai12KeyTranslator()
    : LogsPredictive12KeyTranslator( QLocale::Thai )
{
    LOGS_QDEBUG( "logs [FINDER] -> LogsPredictiveThai12KeyTranslator::\
LogsPredictiveThai12KeyTranslator()" )
    LOGS_QDEBUG( "logs [FINDER] <- LogsPredictiveThai12KeyTranslator::\
LogsPredictiveThai12KeyTranslator()" )
}

// -----------------------------------------------------------------------------
// LogsPredictiveThai12KeyTranslator::LogsPredictiveThai12KeyTranslator()
// -----------------------------------------------------------------------------
//
LogsPredictiveThai12KeyTranslator::LogsPredictiveThai12KeyTranslator(
        const HbInputLanguage& lang )
    : LogsPredictive12KeyTranslator( lang )
{
    LOGS_QDEBUG( "logs [FINDER] -> LogsPredictiveThai12KeyTranslator::\
LogsPredictiveThai12KeyTranslator()" )
    ASSERT( lang.language() == QLocale::Thai );
    LOGS_QDEBUG( "logs [FINDER] <- LogsPredictiveThai12KeyTranslator::\
LogsPredictiveThai12KeyTranslator()" )
}

// -----------------------------------------------------------------------------
// LogsPredictiveThai12KeyTranslator::~LogsPredictiveThai12KeyTranslator()
// -----------------------------------------------------------------------------
//
LogsPredictiveThai12KeyTranslator::~LogsPredictiveThai12KeyTranslator()
{
    LOGS_QDEBUG( "logs [FINDER] -> LogsPredictiveThai12KeyTranslator::\
~LogsPredictiveThai12KeyTranslator()" )
    LOGS_QDEBUG( "logs [FINDER] <- LogsPredictiveThai12KeyTranslator::\
~LogsPredictiveThai12KeyTranslator()" )
    
}

// -----------------------------------------------------------------------------
// LogsPredictiveThai12KeyTranslator::translateChar()
// -----------------------------------------------------------------------------
//
const QString LogsPredictiveThai12KeyTranslator::translateChar( 
                                                    const QChar character, 
                                                    bool& ok ) const
{
    ok = true;
    QString keycode;
    if ( !isIgnored( character ) ) {
        keycode = LogsPredictive12KeyTranslator::translateChar( character );
        ok = !keycode.isEmpty();
        keycode = !keycode.isEmpty() && !isIgnored( *keycode.data() ) ? 
                keycode : QString();
    }
    return keycode;
}

// -----------------------------------------------------------------------------
// LogsPredictiveThai12KeyTranslator::nameTokens()
// -----------------------------------------------------------------------------
//
QStringList LogsPredictiveThai12KeyTranslator::nameTokens( 
                                                    const QString& name ) const
{
    return QStringList( name );
}
        

// -----------------------------------------------------------------------------
// LogsPredictiveThai12KeyTranslator::patternTokens()
// -----------------------------------------------------------------------------
//
QStringList LogsPredictiveThai12KeyTranslator::patternTokens( 
                                                const QString& pattern ) const
{
    return QStringList( pattern );
}

// -----------------------------------------------------------------------------
// LogsPredictiveThai12KeyTranslator::hasPatternSeparators()
// -----------------------------------------------------------------------------
//
int LogsPredictiveThai12KeyTranslator::hasPatternSeparators( 
                                                    const QString& pattern ) const
{
    Q_UNUSED( pattern )
    return 0;
}

// -----------------------------------------------------------------------------
// LogsPredictiveThai12KeyTranslator::trimPattern()
// -----------------------------------------------------------------------------
//
QString& LogsPredictiveThai12KeyTranslator::trimPattern( 
                                    QString& pattern, bool tailOnly ) const
{
    Q_UNUSED( tailOnly )
    return pattern;        
}


// -----------------------------------------------------------------------------
// LogsPredictiveThai12KeyTranslator::isIgnored()
// -----------------------------------------------------------------------------
//
bool LogsPredictiveThai12KeyTranslator::isIgnored( const QChar character ) const
{
    bool found = false;
    int index = 0;
    while( index < IgnoreCount && !found ) {
        found = IgnoreList[ index++ ] == character;
    }
    return found;
}
