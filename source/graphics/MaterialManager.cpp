/* Copyright (C) 2012 Wildfire Games.
 * This file is part of 0 A.D.
 *
 * 0 A.D. is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * 0 A.D. is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with 0 A.D.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "precompiled.h"
#include "lib/ogl.h"
#include "ps/Filesystem.h"
#include "ps/XML/Xeromyces.h"
#include "MaterialManager.h"

CMaterial CMaterialManager::LoadMaterial(const VfsPath& pathname)
{
	if (pathname.empty())
		return CMaterial();

	std::map<VfsPath, CMaterial>::iterator iter = m_Materials.find(pathname);
	if (iter != m_Materials.end())
		return iter->second;

	CXeromyces xeroFile;
	if (xeroFile.Load(g_VFS, pathname) != PSRETURN_OK)
		return CMaterial();

	#define EL(x) int el_##x = xeroFile.GetElementID(#x)
	#define AT(x) int at_##x = xeroFile.GetAttributeID(#x)
	EL(alpha_blending);
	EL(define);
	EL(shader);
	AT(effect);
	AT(name);
	AT(value);
	#undef AT
	#undef EL

	CMaterial material;

	XMBElement root = xeroFile.GetRoot();
	XMBElementList childNodes = root.GetChildNodes();
	
	XERO_ITER_EL(root, node)
	{
		int token = node.GetNodeName();
		XMBAttributeList attrs = node.GetAttributes();
		if (token == el_alpha_blending)
		{
			material.SetUsesAlphaBlending(true);
		}
		else if (token == el_shader)
		{
			material.SetShaderEffect(attrs.GetNamedItem(at_effect));
		}
		else if (token == el_define)
		{
			material.AddShaderDefine(attrs.GetNamedItem(at_name).c_str(), attrs.GetNamedItem(at_value).c_str());
		}
	}

	m_Materials[pathname] = material;
	return material;
}
