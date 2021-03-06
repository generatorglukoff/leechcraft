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

#ifndef PLUGINS_AZOTH_PLUGINS_XOOX_TRANSFERMANAGER_H
#define PLUGINS_AZOTH_PLUGINS_XOOX_TRANSFERMANAGER_H
#include <interfaces/itransfermanager.h>

class QXmppTransferManager;
class QXmppTransferJob;

namespace LeechCraft
{
namespace Azoth
{
namespace Xoox
{
	class GlooxAccount;
	class TransferManager : public QObject
						  , public ITransferManager
	{
		Q_OBJECT
		Q_INTERFACES (LeechCraft::Azoth::ITransferManager);

		QXmppTransferManager *Manager_;
		GlooxAccount *Account_;
	public:
		TransferManager (QXmppTransferManager*, GlooxAccount*);

		QObject* SendFile (const QString&, const QString&, const QString&);
		GlooxAccount* GetAccount () const;
	private slots:
		void handleFileReceived (QXmppTransferJob*);
	signals:
		void fileOffered (QObject*);
	};
}
}
}

#endif
