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

#ifndef QTCONTACTSDB_H
#define QTCONTACTSDB_H

#include <QObject>
#include <QStringList>

class ContactQueryResults : public QObject
{
    Q_OBJECT
    
public: 

    static ContactQueryResults* instance();
    static void deleteInstance();
    
    ~ContactQueryResults();
    
    void set( int count, QString fn = QString("John"),
                         QString ln = QString("Malkovich") );
    void reset();
    const QString& firstNameAt( int index ) const;
    const QString& lastNameAt( int index ) const;
    int contacts() const;
    inline void allResultsAdded() {emit resultsAdded();}
    
signals:
    
    void resultsAdded(); 
    
private:

    inline explicit ContactQueryResults() {}
    

private:
    
    static ContactQueryResults* mInstance;
    QStringList mDB;
    
    
};

#endif //QTCONTACTSDB_H


