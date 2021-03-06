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

#include "accountthread.h"
#include <QTimer>
#include "account.h"
#include "accountthreadworker.h"
#include "core.h"

namespace LeechCraft
{
namespace Snails
{
	AccountThread::AccountThread (Account *parent)
	: A_ (parent)
	{
	}

	AccountThreadWorker* AccountThread::GetWorker () const
	{
		return W_;
	}

	void AccountThread::run ()
	{
		W_ = new AccountThreadWorker (A_);

		ConnectSignals ();

		QThread::run ();
		delete W_;
	}

	void AccountThread::ConnectSignals ()
	{
		connect (W_,
				SIGNAL (gotMsgHeaders (QList<Message_ptr>)),
				A_,
				SLOT (handleMsgHeaders (QList<Message_ptr>)));
		connect (W_,
				SIGNAL (gotProgressListener (ProgressListener_g_ptr)),
				A_,
				SIGNAL (gotProgressListener (ProgressListener_g_ptr)));
		connect (W_,
				SIGNAL (messageBodyFetched (Message_ptr)),
				A_,
				SLOT (handleMessageBodyFetched (Message_ptr)));
		connect (W_,
				SIGNAL (gotUpdatedMessages (QList<Message_ptr>)),
				A_,
				SLOT (handleGotUpdatedMessages (QList<Message_ptr>)));
		connect (W_,
				SIGNAL (gotOtherMessages (QList<QByteArray>, QStringList)),
				A_,
				SLOT (handleGotOtherMessages (QList<QByteArray>, QStringList)));
		connect (W_,
				SIGNAL (gotFolders (QList<QStringList>)),
				A_,
				SLOT (handleGotFolders (QList<QStringList>)));
		connect (W_,
				SIGNAL (gotEntity (LeechCraft::Entity)),
				&Core::Instance (),
				SIGNAL (gotEntity (LeechCraft::Entity)));
	}
}
}
