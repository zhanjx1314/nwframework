/*
 *       This file is part of NWFramework.
 *       Copyright (c) InCrew Software and Others.
 *       (See the AUTHORS file in the root of this distribution.)
 *
 *       NWFramework is free software; you can redistribute it and/or modify
 *       it under the terms of the GNU General Public License as published by
 *       the Free Software Foundation; either version 2 of the License, or
 *       (at your option) any later version.
 *
 *       NWFramework is distributed in the hope that it will be useful,
 *       but WITHOUT ANY WARRANTY; without even the implied warranty of
 *       MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *       GNU General Public License for more details.
 * 
 *       You should have received a copy of the GNU General Public License
 *       along with NWFramework; if not, write to the Free Software
 *       Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
 */
#ifndef GAEDITBOX_H_
#define GAEDITBOX_H_

#include "GUIAdapterCreator.h"
#include "EditBox.h"
#include "NWIP.h"

//********************************************************************
//
//********************************************************************
class GAEditBoxString : public GUIAdapter<::EditBox,std::string>, public EditBoxEventHandler
{
    virtual bool initAdapter();
    virtual void editBoxModified(::EditBox* _editBox);
    virtual void update(const char * _contextName, const char * _objName);
};

//********************************************************************
//
//********************************************************************
class GAEditBoxInt : public GUIAdapter<::EditBox,int>, public EditBoxEventHandler
{
    virtual bool initAdapter();
    virtual void editBoxModified(::EditBox* _editBox);
    virtual void update(const char * _contextName, const char * _objName);
};

//********************************************************************
//
//********************************************************************
class GAEditBoxNWIP : public GUIAdapter<::EditBox,NWIP>, public EditBoxEventHandler
{
    virtual bool initAdapter();
    virtual void editBoxModified(::EditBox* _editBox);
    virtual void update(const char * _contextName, const char * _objName);
};


#endif
