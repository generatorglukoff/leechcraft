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

#ifndef PLUGINS_LIZNOO_LIZNOO_H
#define PLUGINS_LIZNOO_LIZNOO_H
#include <QObject>
#include <interfaces/iinfo.h>
#include <interfaces/iactionsexporter.h>
#include "batteryinfo.h"

namespace LeechCraft
{
namespace Liznoo
{
	class DBusThread;

	class Plugin : public QObject
				 , public IInfo
				 , public IActionsExporter
	{
		Q_OBJECT
		Q_INTERFACES (IInfo IActionsExporter)

		ICoreProxy_ptr Proxy_;
		DBusThread *Thread_;
		QMap<QString, QAction*> Battery2Action_;
	public:
		void Init (ICoreProxy_ptr);
		void SecondInit ();
		QByteArray GetUniqueID () const;
		void Release ();
		QString GetName () const;
		QString GetInfo () const;
		QIcon GetIcon () const;

		QList<QAction*> GetActions (ActionsEmbedPlace) const;
	private slots:
		void handleBatteryInfo (Liznoo::BatteryInfo);
		void handleThreadStarted ();
	signals:
		void gotActions (QList<QAction*>, LeechCraft::ActionsEmbedPlace);
	};
}
}

#endif