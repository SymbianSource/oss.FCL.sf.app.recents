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

#include "logspredictivethai12keytranslator.h"
#include "logslogger.h"

const QChar IgnoreList[] = {'*', '#' };
const int IgnoreCount = 2;


// -----------------------------------------------------------------------------
// LogsPredictiveThai12KeyTranslator::LogsPredictiveThai12KeyTranslator()
// -----------------------------------------------------------------------------
//
LogsPredictiveThai12KeyTranslator::LogsPredictiveThai12KeyTranslator()
    : LogsPredictive12KeyTranslator()
{
    LOGS_QDEBUG( "logs [FINDER] -> LogsPredictiveThai12KeyTranslator::\
LogsPredictiveThai12KeyTranslator()" )
    HbInputLanguage lang = 
            HbInputSettingProxy::instance()->globalInputLanguage();
    ASSERT( lang.language() == QLocale::Thai );
    mKeyMap = HbKeymapFactory::instance()->keymap( lang.language(), 
                                                   lang.variant() );
    
    
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
const QChar LogsPredictiveThai12KeyTranslator::translateChar( 
                                                    const QChar character ) const
{
    QChar keycode;
    if ( !isIgnored( character ) ) {
        keycode = LogsPredictive12KeyTranslator::translateChar( character );
        keycode = !keycode.isNull() && !isIgnored( keycode ) ? 
                    keycode : QChar();
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





