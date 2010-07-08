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
//USER
#include "logsmainwindow.h"
#include "logsviewmanager.h"
#include "logsservicehandler.h"
#include "logsservicehandlerold.h"
#include "logslogger.h"
#include "logsdefs.h"

//SYSTEM
#include <QObject>
#include <hbmainwindow.h>
#include <hbapplication.h>
#include <hbstyleloader.h>
#include <hbtranslator.h>

int main(int argc, char *argv[])
{
    LOGS_QDEBUG( "logs [UI] -> main()" )
  
    HbApplication app(argc, argv);
    LogsMainWindow window;
    HbTranslator translator("dialer");
    translator.loadCommon();

    HbStyleLoader::registerFilePath(":/logslayouts");
    
    // Create service handler asap so that services are published fast.
    // Servicehandler cannot be created before HbApplication creation.
    // This can cause problem of service request not coming through if
    // HbApplication creation takes long time.
    LogsServiceHandler service;
    LogsServiceHandlerOld serviceOld;
    LogsViewManager viewManager(window, service, serviceOld);
    
    // Don't show window yet as app might be started at background
    int err = app.exec();
    
    HbStyleLoader::unregisterFilePath(":/logslayouts");
    
    LOGS_QDEBUG( "logs [UI] <- main()" )
    return err;
}
