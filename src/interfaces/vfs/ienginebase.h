/**********************************************************************
 * LeechCraft - modular cross-platform feature rich internet client.
 * Copyright (C) 2006-2011  Georg Rudoy
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **********************************************************************/

#ifndef INTERFACES_VFS_IENGINEBASE_H
#define INTERFACES_VFS_IENGINEBASE_H
#include <QtPlugin>

class QAbstractFileEngine;

namespace LeechCraft
{
namespace VFS
{
	class IEngineBase
	{
	public:
		virtual ~IEngineBase () {}
		
		virtual QString GetName () const = 0;

		virtual QString GetDescription () const = 0;
		
		virtual QAbstractFileEngine* CreateEngine (const QString&) const = 0;
	};
}
}

Q_DECLARE_INTERFACE (LeechCraft::VFS::IEngineBase,
		"org.Deviant.LeechCraft.VFS.IEngineBase/1.0");

#endif