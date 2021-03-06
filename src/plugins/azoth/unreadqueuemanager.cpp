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

#include "unreadqueuemanager.h"
#include <QMainWindow>
#include <interfaces/core/icoreproxy.h>
#include "interfaces/iclentry.h"
#include "core.h"
#include "chattabsmanager.h"

namespace LeechCraft
{
namespace Azoth
{
	UnreadQueueManager::UnreadQueueManager (QObject *parent)
	: QObject (parent)
	{
	}

	void UnreadQueueManager::AddMessage (QObject *msg)
	{
		Queue_ << msg;
	}

	void UnreadQueueManager::ShowNext ()
	{
		QObject *msgObj = 0;
		while (!Queue_.isEmpty () && !msgObj)
			msgObj = Queue_.takeFirst ();
		if (!msgObj)
			return;

		IMessage *msg = qobject_cast<IMessage*> (msgObj);
		ICLEntry *entry = qobject_cast<ICLEntry*> (msg->ParentCLEntry ());
		Core::Instance ().GetChatTabsManager ()->OpenChat (entry);

		QMainWindow *mw = Core::Instance ().GetProxy ()->GetMainWindow ();
		mw->show ();
		mw->activateWindow ();
		mw->raise ();
	}

	void UnreadQueueManager::clearMessagesForEntry (QObject *entryObj)
	{
		for (auto i = Queue_.begin (); i != Queue_.end (); )
		{
			if (!*i)
			{
				i = Queue_.erase (i);
				continue;
			}

			IMessage *msg = qobject_cast<IMessage*> (*i);
			if (msg->ParentCLEntry () == entryObj)
			{
				i = Queue_.erase (i);
				continue;
			}

			++i;
		}
	}
}
}
