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

#include "EditorScaleBiasModule.h"

std::string EditorScaleBiasModule::FACTORY_NAME = "ScaleBias";

EditorScaleBiasModule::EditorScaleBiasModule() : EditorModule(1)
{
}

void EditorScaleBiasModule::fillPropertyGrid (wxPropertyGrid *pg)
{
	pg->Append( wxPropertyCategory(wxT("Source modules")) );
	appendSourceModuleProperty (pg, wxT("Source module"), mSourceModules[0]);
	pg->Append( wxPropertyCategory(wxT("Parameters")) );
	pg->Append( wxFloatProperty(wxT("Scale"), wxPG_LABEL, mModule.getScale()) );
	pg->Append( wxFloatProperty(wxT("Bias"), wxPG_LABEL, mModule.getBias()) );
}

void EditorScaleBiasModule::onPropertyChange (wxPropertyGrid *pg, const wxString &name)
{
	if (name == _("Source module"))
	{
		wxString val = pg->GetPropertyValueAsString (name);
		mSourceModules[0] = val;
	}
	else if (name == _("Scale"))
	{
		double val = pg->GetPropertyValueAsDouble (name);
		mModule.setScale (val);
	}
	else if (name == _("Bias"))
	{
		double val = pg->GetPropertyValueAsDouble (name);
		mModule.setBias (val);
	}
}

bool EditorScaleBiasModule::validate (wxPropertyGrid *pg)
{
	bool valid = true;
	EditorModule *module = NULL;

	module = getSourceModule(0);
	if (module && module->validateTree(this))
	{
		mModule.setSourceModule(0, module->getModule());
	}
	valid = setValid (pg, "Source module", module != NULL && module->validateTree(this) && module->validate(NULL)) && valid;

	return valid;
}

void EditorScaleBiasModule::writeProperties (TiXmlElement *element)
{
	TiXmlElement *prop;

	writeSourceModules (element);

	prop = new TiXmlElement ("Scale");
	prop->SetDoubleAttribute ("value", mModule.getScale());
	element->LinkEndChild (prop);

	prop = new TiXmlElement ("Bias");
	prop->SetDoubleAttribute ("value", mModule.getBias());
	element->LinkEndChild (prop);
}

bool EditorScaleBiasModule::readProperties (TiXmlElement *element)
{
	TiXmlElement *prop;
	double dval;

	if (!readSourceModules (element))
		return false;

	prop = element->FirstChildElement ("Scale");
	if (prop == NULL || prop->QueryDoubleAttribute ("value", &dval) != TIXML_SUCCESS)
		return false;
	mModule.setScale (dval);

	prop = element->FirstChildElement ("Bias");
	if (prop == NULL || prop->QueryDoubleAttribute ("value", &dval) != TIXML_SUCCESS)
		return false;
	mModule.setBias (dval);

	return true;
}
