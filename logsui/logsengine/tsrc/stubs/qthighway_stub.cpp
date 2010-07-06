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

#include <xqservicerequest.h>
#include <xqrequestinfo.h>
#include <QHash>
#include "qthighway_stub_helper.h"

QString qtHighwayStubService;
QString qtHighwayStubMessage;
bool qtHighwayStubRequestBg = false;

void QtHighwayStubHelper::reset()
{
    qtHighwayStubService.clear();
    qtHighwayStubMessage.clear();
    qtHighwayStubRequestBg = false;
}

QString QtHighwayStubHelper::service()
{
    return qtHighwayStubService;
}

QString QtHighwayStubHelper::message()
{
    return qtHighwayStubMessage;
}

bool QtHighwayStubHelper::isRequestBg()
{
    return qtHighwayStubRequestBg;
}

XQServiceRequest::XQServiceRequest(
    const QString& service, const QString& message, const bool& synchronous)
{
    qtHighwayStubService = service;
    qtHighwayStubMessage = message;
}

XQServiceRequest::~XQServiceRequest()
{

}


void XQServiceRequest::setArguments(const QList<QVariant> &arguments)
{
    Q_UNUSED(arguments)
}

QString XQServiceRequest::service() const
{
    return qtHighwayStubService;
}

QString XQServiceRequest::message() const
{
    return qtHighwayStubMessage;
}

bool XQServiceRequest::send(QVariant& retValue) 
{
    return true;
}
void XQServiceRequest::addArg(const QVariant& v)
{

}

void XQServiceRequest::setInfo(const XQRequestInfo &info)
{
    Q_UNUSED(info);
}

XQRequestInfo::XQRequestInfo()
{
}

XQRequestInfo::~XQRequestInfo()
{
    
}

void XQRequestInfo::setBackground(bool on)
{
    qtHighwayStubRequestBg = on;
}

bool XQRequestInfo::isBackground() const
{
    return qtHighwayStubRequestBg;
}


void XQRequestInfo::setForeground(bool on)
{
    Q_UNUSED(on)
}
