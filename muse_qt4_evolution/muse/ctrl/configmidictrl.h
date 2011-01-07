//=============================================================================
//  MusE
//  Linux Music Editor
//  $Id:$
//
//  Copyright (C) 2002-2006 by Werner Schweer and others
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License version 2.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//=============================================================================

#ifndef __CONFIGMIDICTRL_H__
#define __CONFIGMIDICTRL_H__

#include "ui_configmidictrl.h"

class MidiTrack;

//---------------------------------------------------------
//   ConfigMidiCtrl
//---------------------------------------------------------

class ConfigMidiCtrl : public QDialog, public Ui::ConfigMidiCtrlBase {
      Q_OBJECT

      MidiTrack* track;

   private slots:
      void addClicked();
      void removeClicked();
      void availableSelected(QListWidgetItem*);
      void managedSelected(QListWidgetItem*);
      virtual void done(int);
      void defineClicked();

   public:
      ConfigMidiCtrl(MidiTrack*);
      };

#endif

