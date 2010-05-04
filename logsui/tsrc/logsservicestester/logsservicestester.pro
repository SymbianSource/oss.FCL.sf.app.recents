#                                                                    
# Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).   
# All rights reserved.                                                
# This component and the accompanying materials are made available    
# under the terms of "Eclipse Public License v1.0"    
# which accompanies this distribution, and is available               
# at the URL "http://www.eclipse.org/legal/epl-v10.html".   
#                                                                     
# Initial Contributors:                                               
# Nokia Corporation - initial contribution.                           
#                                                                     
# Contributors:                                                       
#                                                                     
# Description:                                                        
#                                                                     
#

TEMPLATE = app
TARGET = 
DEPENDPATH += .
INCLUDEPATH += .
INCLUDEPATH += $$APP_LAYER_SYSTEMINCLUDE
CONFIG += hb

# Input
SOURCES += main.cpp

symbian: {
    TARGET.UID2 = 0x100039CE
    TARGET.UID3 = 0xEC209DCF
    TARGET.CAPABILITY = CAP_APPLICATION AllFiles
    LIBS += -llogsservices   
}

