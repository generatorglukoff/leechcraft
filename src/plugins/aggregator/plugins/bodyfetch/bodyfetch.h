/**********************************************************************
 * LeechCraft - modular cross-platform feature rich internet client.
 * Copyright (C) 2006-2012  Georg Rudoy
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

#pragma once

#include <QObject>
#include <QUrl>
#include <QDir>
#include <interfaces/iinfo.h>
#include <interfaces/iplugin2.h>
#include <interfaces/aggregator/item.h>
#include <interfaces/core/ihookproxy.h>

namespace LeechCraft
{
namespace Aggregator
{
class Item;

namespace BodyFetch
{
	class WorkerObject;

	class Plugin : public QObject
				 , public IInfo
				 , public IPlugin2
	{
		Q_OBJECT
		Q_INTERFACES (IInfo IPlugin2)

		ICoreProxy_ptr Proxy_;
		QDir StorageDir_;
		WorkerObject *WO_;
		QHash<int, QPair<QUrl, QString> > Jobs_;
		QHash<int, QString> ContentsCache_;
		QSet<quint64> FetchedItems_;
	public:
		void Init (ICoreProxy_ptr);
		void SecondInit ();
		QByteArray GetUniqueID () const;
		void Release ();
		QString GetName () const;
		QString GetInfo () const;
		QIcon GetIcon () const;

		QSet<QByteArray> GetPluginClasses () const;
	public slots:
		void hookItemLoad (LeechCraft::IHookProxy_ptr proxy,
				Item*);
		void hookGotNewItems (LeechCraft::IHookProxy_ptr proxy,
				QVariantList items);
	private slots:
		void handleDownload (QUrl);
		void handleJobFinished (int);
		void handleBodyFetched (quint64);
	signals:
		void downloadFinished (QUrl, QString);

		void delegateEntity (const LeechCraft::Entity&, int*, QObject**);
	};
}
}
}
