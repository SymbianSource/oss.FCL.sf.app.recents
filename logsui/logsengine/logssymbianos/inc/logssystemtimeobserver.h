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

#ifndef LOGSSYSTEMTIMEOBSERVER_H
#define LOGSSYSTEMTIMEOBSERVER_H

//  INCLUDES
#include <QObject>

// FORWARD DECLARATION
class CEnvironmentChangeNotifier;

// CLASS DECLARATION



/**
 * LogsSystemTimeObserver is used to observe changes in system locale settings
 */
class LogsSystemTimeObserver : public QObject
    {
    Q_OBJECT
    
public:
        
    explicit LogsSystemTimeObserver(QObject* parent = 0);
    
    virtual ~LogsSystemTimeObserver();

signals:

    void timeFormatChanged();

    
private:
    
    static TInt EnvironmentChangeCallback(TAny* aThis);

private: // data
        
    CEnvironmentChangeNotifier* mEnvChangeNotifier;
    
private:    
    friend class UT_LogsSystemTimeObserver;
    };

#endif      // LOGSSYSTEMTIMEOBSERVER_H

// End of File
      

        
       
