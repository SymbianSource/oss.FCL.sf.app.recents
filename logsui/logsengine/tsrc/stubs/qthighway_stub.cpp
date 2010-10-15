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

#include <xqrequestinfo.h>
#include <xqappmgr.h>
#include <xqaiwrequest.h>
#include <QHash>
#include "qthighway_stub_helper.h"

bool qtHighwayStubRequestBg = false;
bool qtHighwayStubRequestEmbedded = false;

QString qtAiwStubInterface;
QString qtAiwStubOperation;
bool qtAiwStubSynchronous = true;
bool qtAiwFailCreateRequest = false;

void QtHighwayStubHelper::reset()
{
    qtHighwayStubRequestBg = false;
    qtHighwayStubRequestEmbedded = false;
    qtAiwStubInterface.clear();
    qtAiwStubOperation.clear();
    qtAiwStubSynchronous = true;
    qtAiwFailCreateRequest = false;
}

bool QtHighwayStubHelper::isRequestSynchronous()
{
    return qtAiwStubSynchronous;
}

void QtHighwayStubHelper::setFailCreateAiwRequest(bool fail)
{
    qtAiwFailCreateRequest = fail;
}

QString QtHighwayStubHelper::interface()
{
    return qtAiwStubInterface;
}

QString QtHighwayStubHelper::operation()
{
    return qtAiwStubOperation;
}

bool QtHighwayStubHelper::isRequestBg()
{
    return qtHighwayStubRequestBg;
}

bool QtHighwayStubHelper::isRequestEmbedded()
{
    return qtHighwayStubRequestEmbedded;
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
    qtHighwayStubRequestBg = !on;
}

void XQRequestInfo::setEmbedded(bool embedded)
{
    qtHighwayStubRequestEmbedded = embedded;
}


// -----------------------------------------------------------------------------
// XQApplicationManager stubs
// -----------------------------------------------------------------------------
//
XQApplicationManager::XQApplicationManager()
{
}

XQApplicationManager::~XQApplicationManager()
{
}

XQAiwRequest* XQApplicationManager::create( const QString& interface, 
        const QString& operation, bool embedded)
{
    if (!qtAiwFailCreateRequest) {
        qtAiwStubInterface = interface;
        qtAiwStubOperation = operation;
        qtHighwayStubRequestEmbedded = embedded;
        XQAiwInterfaceDescriptor descr;
        return new XQAiwRequest(descr, operation, embedded);
    } else {
        return 0; 
    }
}

XQAiwRequest::XQAiwRequest(const XQAiwInterfaceDescriptor &descriptor, 
        const QString &operation, bool embedded)
{
    Q_UNUSED(descriptor)
    Q_UNUSED(operation)
    Q_UNUSED(embedded)
}

XQAiwRequest::~XQAiwRequest()
{    
}

const QString& XQAiwRequest::operation() const
{
    return qtAiwStubOperation;
}

const XQAiwInterfaceDescriptor& XQAiwRequest::descriptor() const
{
    return XQAiwInterfaceDescriptor();
}

void XQAiwRequest::setArguments(const QList<QVariant> &arguments)
{
    Q_UNUSED(arguments)
}

bool XQAiwRequest::send()
{
    return true;
}

bool XQAiwRequest::send(QVariant &returnValue)
{
    Q_UNUSED(returnValue)
    return true;
}

void XQAiwRequest::setSynchronous(bool synchronous)
{
    qtAiwStubSynchronous = synchronous;
}

void XQAiwRequest::setInfo(const XQRequestInfo &info)
{
    Q_UNUSED(info)
}


XQAiwInterfaceDescriptor::XQAiwInterfaceDescriptor()
{    
}

XQAiwInterfaceDescriptor::~XQAiwInterfaceDescriptor()
{    
}

QString XQAiwInterfaceDescriptor::interfaceName() const
{
    return qtAiwStubInterface;
}
