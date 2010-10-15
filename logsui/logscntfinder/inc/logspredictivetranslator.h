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

#ifndef LOGSPREDICTIVETRANSLATOR_H
#define LOGSPREDICTIVETRANSLATOR_H

#include <QObject>
#include <QChar>
#include <QList>



class LogsCntToken 
{
public:
    
    const QString& text() const {return mText;}
    const QString& translation() const {return mTranslatedText;}
    
protected:
    
    QString mText;
    QString mTranslatedText;
    
};

typedef QList<LogsCntToken> LogsCntTokenArray;

class LogsCntTokenIterator : public QListIterator<LogsCntToken> 
{
public:            

    inline LogsCntTokenIterator( LogsCntTokenArray& container )
        : QListIterator<LogsCntToken>( container ), mCount( container.count() ) {}
   inline int count() const {return mCount;}
    
private:
    
    int mCount;
    
};


class HbKeymap;
class HbInputLanguage;




/**
 * predictive translator. Singelton
 *
 */
class LogsPredictiveTranslator : public QObject 
{

public: 

    static LogsPredictiveTranslator* instance();
    static void deleteInstance();
    static HbInputLanguage currentLanguage(); 
    
    ~LogsPredictiveTranslator();
    
    virtual const QString preparePattern( const QString& pattern ) const;
    const QString translateText( const QString& text );
    
public: //abstracts
    
    virtual bool match( const QString& pattern, 
                        LogsCntTokenIterator& names ) const = 0;
    
    virtual QStringList patternTokens( const QString& pattern ) const = 0;
    virtual QString& trimPattern( QString& pattern, bool tailOnly = false ) const = 0;
    virtual QStringList nameTokens( const QString& name ) const = 0;
    virtual int hasPatternSeparators( const QString& pattern ) const = 0;
    virtual int mib() const = 0;
    
protected:
    
    virtual const QString translateChar( const QChar character, bool& ok ) const = 0;
    
    explicit LogsPredictiveTranslator( const HbInputLanguage& lang );
    bool encodes( const QString& sniplet );
    const QString translate( const QString& text, bool* ok = 0, int count = -1 ) const;
    inline virtual void* keyMap() const { return (void*)mKeyMap; }
    
private:
    
    const LogsPredictiveTranslator& nameTranslator( const QString& name );
    
protected:
    
    const HbKeymap* mKeyMap;

private:
    
    LogsPredictiveTranslator* mNameTranslator;
    static LogsPredictiveTranslator* mInstance;
    friend class UT_LogsPredictiveTranslator;
    
};

#endif //LOGSPREDICTIVETRANSLATOR_H
