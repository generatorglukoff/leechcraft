/**********************************************************************
 * LeechCraft - modular cross-platform feature rich internet client.
 * Copyright (C) 2011 Minh Ngo
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

#ifndef PLAYPAUSEACTION_H
#define PLAYPAUSEACTION_H
#include <QAction>

namespace LeechCraft
{
namespace Potorchu
{
	class PlayPauseAction : public QAction
	{
		Q_OBJECT

		QPair<QIcon, QIcon> PlayPausePair_;
		bool Play_;
	public:
		PlayPauseAction (const QPair<QIcon, QIcon>& playPausePair,
				QObject *parent = 0);
	public slots:
		void handlePause ();
		void handlePlay ();
	private slots:
		void handleTriggered ();
	signals:
		void play ();
		void pause();
	};
}
}

#endif // PLAYPAUSECTION_H