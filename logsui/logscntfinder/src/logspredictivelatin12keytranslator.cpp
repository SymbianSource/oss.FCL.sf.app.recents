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

#include "logspredictivelatin12keytranslator.h"
#include "logslogger.h"

//mapping char,key(name)
const QChar SpecialMapping[] = {'+', '*', '*', '*','#','#'};
const int SpecialsCount = 3;
const QChar SpaceSepar(' ');


// -----------------------------------------------------------------------------
// LogsPredictiveLatin12KeyTranslator::LogsPredictiveLatin12KeyTranslator()
// -----------------------------------------------------------------------------
//
LogsPredictiveLatin12KeyTranslator::LogsPredictiveLatin12KeyTranslator()
    : LogsPredictive12KeyTranslator()
{
    LOGS_QDEBUG( "logs [FINDER] -> LogsPredictiveLatin12KeyTranslator::\
LogsPredictiveLatin12KeyTranslator()" )
    HbInputLanguage lang = 
            HbInputSettingProxy::instance()->globalInputLanguage();
    mKeyMap = HbKeymapFactory::instance()->keymap( lang.language(), 
                                                   lang.variant() );
    
    int index = 0;
    int arraySize = SpecialsCount * 2;
    while( index < arraySize ) {
        QChar character = SpecialMapping[ index++ ];
        QChar keycode = SpecialMapping[ index++ ];
        mSpecialKeyMap[ character ] = keycode;
    }
    
    LOGS_QDEBUG( "logs [FINDER] <- LogsPredictiveLatin12KeyTranslator::\
LogsPredictiveLatin12KeyTranslator()" )
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
// LogsPredictiveLatin12KeyTranslator::translateChar()
// -----------------------------------------------------------------------------
//
const QChar LogsPredictiveLatin12KeyTranslator::translateChar( 
                                                    const QChar character ) const
{
    QChar keycode = mSpecialKeyMap[ character ]; 
    if ( keycode.isNull() ) {
        keycode = LogsPredictive12KeyTranslator::translateChar( character );
        if ( keycode.isNull() ) {
            QString decomposed = character.decomposition();
            if (decomposed.isEmpty()) {
                return keycode;
            }
            return translateChar (decomposed.at(0));
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
        
