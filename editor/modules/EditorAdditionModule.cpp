// This file is part of the Noise++ Editor.
// Copyright (c) 2008, Urs C. Hanselmann
//
// The Noise++ Editor is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// The Noise++ Editor is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with the Noise++ Editor.  If not, see <http://www.gnu.org/licenses/>.
//

#include "EditorAdditionModule.h"

std::string EditorAdditionModule::FACTORY_NAME = "Addition";

EditorAdditionModule::EditorAdditionModule() : EditorModule(2)
{
}

void EditorAdditionModule::fillPropertyGrid (wxPropertyGrid *pg)
{
	pg->Append( wxPropertyCategory(wxT("Source modules")) );
	appendSourceModuleProperty (pg, wxT("Source module 1"), mSourceModules[0]);
	appendSourceModuleProperty (pg, wxT("Source module 2"), mSourceModules[1]);
}

void EditorAdditionModule::onPropertyChange (wxPropertyGrid *pg, const wxString &name)
{
	if (name == _("Source module 1"))
	{
		wxString val = pg->GetPropertyValueAsString (name);
		mSourceModules[0] = val;
	}
	else if (name == _("Source module 2"))
	{
		wxString val = pg->GetPropertyValueAsString (name);
		mSourceModules[1] = val;
	}
}

bool EditorAdditionModule::validate (wxPropertyGrid *pg)
{
	bool valid = true;
	EditorModule *module = NULL;

	module = getSourceModule(0);
	if (module)
	{
		mModule.setSourceModule(0, module->getModule());
	}
	valid = setValid (pg, "Source module 1", module != NULL && module->validate(NULL)) && valid;
	module = getSourceModule(1);
	if (module)
	{
		mModule.setSourceModule(1, module->getModule());
	}
	valid = setValid (pg, "Source module 2", module != NULL && module->validate(NULL)) && valid;

	return valid;
}

void EditorAdditionModule::writeProperties (TiXmlElement *element)
{
	writeSourceModules (element);
}

bool EditorAdditionModule::readProperties (TiXmlElement *element)
{
	if (!readSourceModules (element))
		return false;

	return true;
}
