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

#include "sidebar.h"
#include <QIcon>
#include <QAction>
#include <QMainWindow>
#include <QStatusBar>
#include <interfaces/imwproxy.h>
#include <interfaces/core/icoreproxy.h>
#include <interfaces/core/ipluginsmanager.h>
#include <interfaces/iactionsexporter.h>
#include <interfaces/ihavetabs.h>

namespace LeechCraft
{
namespace Sidebar
{
	void Plugin::Init (ICoreProxy_ptr proxy)
	{
		Proxy_ = proxy;

		Bar_ = new QToolBar (tr ("Sidebar"));
		Bar_->setFloatable (false);
		Bar_->setMovable (false);
		Bar_->setIconSize (QSize (48, 48));

		SepAfterPlugins_ = Bar_->addSeparator ();
		SepAfterQL_ = Bar_->addSeparator ();

		Proxy_->GetMWProxy ()->AddToolbar (Bar_, Qt::LeftToolBarArea);
		Proxy_->GetMainWindow ()->statusBar ()->hide ();
	}

	void Plugin::SecondInit ()
	{
		auto hasTabs = Proxy_->GetPluginsManager ()->
				GetAllCastableRoots<IHaveTabs*> ();
		Q_FOREACH (QObject *ihtObj, hasTabs)
		{
			IHaveTabs *iht = qobject_cast<IHaveTabs*> (ihtObj);

			Q_FOREACH (const TabClassInfo& tc, iht->GetTabClasses ())
			{
				if (!(tc.Features_ & TabFeature::TFOpenableByRequest) ||
						(tc.Features_ & TabFeature::TFSingle))
					continue;

				if (tc.Icon_.isNull ())
					continue;

				QAction *act = new QAction (tc.Icon_,
						tc.VisibleName_, this);
				act->setProperty ("Sidebar/Object",
						QVariant::fromValue<QObject*> (ihtObj));
				act->setProperty ("Sidebar/TabClass", tc.TabClass_);
				connect (act,
						SIGNAL (triggered (bool)),
						this,
						SLOT (openNewTab ()));

				Bar_->insertAction (SepAfterPlugins_, act);
			}
		}
	}

	QByteArray Plugin::GetUniqueID () const
	{
		return "org.LeechCraft.Sidebar";
	}

	void Plugin::Release ()
	{
	}

	QString Plugin::GetName () const
	{
		return "Sidebar";
	}

	QString Plugin::GetInfo () const
	{
		return tr ("A nice sidebar with quick launch area, tabs and tray-like area.");
	}

	QIcon Plugin::GetIcon () const
	{
		return QIcon ();
	}

	QSet<QByteArray> Plugin::GetPluginClasses () const
	{
		QSet<QByteArray> result;
		result << "org.LeechCraft.Core.Plugins/1.0";
		return result;
	}

	void Plugin::hookGonnaFillQuickLaunch (IHookProxy_ptr proxy)
	{
		proxy->CancelDefault ();

		auto exporters = Proxy_->GetPluginsManager ()->
				GetAllCastableTo<IActionsExporter*> ();

		Q_FOREACH (IActionsExporter *exp, exporters)
		{
			QList<QAction*> actions = exp->GetActions (AEPQuickLaunch);
			if (actions.isEmpty ())
				continue;

			Q_FOREACH (QAction *action, actions)
			{
				Proxy_->RegisterSkinnable (action);
				Bar_->insertAction (SepAfterQL_, action);
			}
		}
	}

	void Plugin::openNewTab ()
	{
		QObject *pluginObj = sender ()->
				property ("Sidebar/Object").value<QObject*> ();
		const QByteArray& tc = sender ()->
				property ("Sidebar/TabClass").toByteArray ();

		IHaveTabs *iht = qobject_cast<IHaveTabs*> (pluginObj);
		iht->TabOpenRequested (tc);
	}
}
}

Q_EXPORT_PLUGIN2 (leechcraft_sidebar, LeechCraft::Sidebar::Plugin);