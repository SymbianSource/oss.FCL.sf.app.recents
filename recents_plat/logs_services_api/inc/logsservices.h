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
#ifndef LOGSSERVICES_H
#define LOGSSERVICES_H

#include <QString>
#include <logsexport.h> 

/**
* Logs application services
* @deprecated  Use XQApplicationManager instead. 
*  Service name: "logs",
*  Interface name: "com.nokia.symbian.ILogsView"
*  Operation signature: "show(QVariantMap)" 
*  Operation parameters: 
*     "view_index": int, 0 - all recent, 1 - received, 2 - called, 3 - missed
*     "show_dialpad": bool, if true view is shown with dialpad opened on top
*     "dialpad_text": QString, number to be prefilled into dialpad
*     
*  Example usage:
*  XQApplicationManager appMgr;
*  QScopedPointer<XQAiwRequest> request(appMgr.create(QString("logs"),
*                  QString("com.nokia.symbian.ILogsView"), 
*                  QString("show(QVariantMap)"), false));   
*  if (!request.isNull()) {
*      int retValue = -1; 
*      QList<QVariant> arglist;
*      QVariantMap map;
*      map.insert("view_index", QVariant(0));
*      map.insert("show_dialpad", QVariant(true));
*      map.insert("dialpad_text", QVariant(QString()));
*      arglist.append(QVariant(map));
*      request->setArguments(arglist);
*      QVariant ret(retValue);        
*      request->send(ret);
*  }
*
*/
class LogsServices
{
    public:
    
    //Deprecated
    enum LogsView{
        ViewAll,
        ViewReceived,
        ViewCalled,
        ViewMissed
    };
    /**
     * Requests to start Logs application. If Logs is already running,
     * activates the requested view.
     * 
     * @deprecated Use "com.nokia.symbian.ILogsView" interface 
     * and "show(int,bool,QString)" operation instead
     *
     * @param activatedView   The view to be activated in Logs.
     * @param showDialpad     Specifies whether dialpad should be shown on top
     *                        of the activated view
     * @param number          Predefined number for dialpad
     * @return 0 if started/activated succesfully
     */
    LOGSSERVICES_EXPORT static int start(
        LogsView activatedView, bool showDialpad, const QString& number = QString());

};


#endif  //LOGSSERVICES_H
