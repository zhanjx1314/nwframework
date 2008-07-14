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
#ifndef WIN32COMBOBOX_H_
#define WIN32COMBOBOX_H_

#include "ComboBox.h"
#include "Win32Control.h"

//********************************************************************
//
//********************************************************************
class Win32ComboBox : public Win32Control<::ComboBox>
{
public:
    Win32ComboBox ();
    virtual    ~Win32ComboBox()                      { Win32ComboBox::done(); }

    virtual bool          init        (GUI* _gui, const char* _name, GUIControl* _parent);
    virtual void          done        ();

private:
    typedef Win32Control<::ComboBox> Inherited;

    // ComboBox
    virtual void selectItem(int _index);
    virtual int getSelectedItem() const;
    virtual void internalAddItem(const char* _text);
    virtual void internalClearItemsList();

    // Win32ControlData
    virtual bool sendMsg(UINT message, WPARAM wParam, LPARAM lParam);

    bool createWindow(GUIControl* _parent);
};

#endif
