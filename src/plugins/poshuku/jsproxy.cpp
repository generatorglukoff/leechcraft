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

#include "jsproxy.h"
#include <QString>
#include <QVariant>
#include <QTextCodec>
#include <QtDebug>
#include <util/util.h>

namespace LeechCraft
{
namespace Poshuku
{
	JSProxy::JSProxy (QObject *parent)
	: QObject (parent)
	{
	}

	void JSProxy::debug (const QString& string)
	{
		qDebug () << Q_FUNC_INFO << string;
	}

	void JSProxy::warning (const QString& string)
	{
		qWarning () << Q_FUNC_INFO << string;
	}
}
}
