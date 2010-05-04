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

#ifndef LOGSCENREPEDITORVIEW_H
#define LOGSCENREPEDITORVIEW_H

#include <hbview.h>

class HbRadioButtonList;
class CRepository;

class LogsCenrepEditorView : public HbView
{
    Q_OBJECT

public:
    LogsCenrepEditorView ();
    virtual ~LogsCenrepEditorView ();

private slots:

    void saveSettings();
    
private:
    

private:
    HbRadioButtonList* mList;
    CRepository* mRepository;
};

#endif // LOGSCENREPEDITORVIEW_H
