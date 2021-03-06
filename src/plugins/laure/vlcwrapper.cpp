/**********************************************************************
 * LeechCraft - modular cross-platform feature rich internet client.
 * Copyright (C) 2011  Minh Ngo
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

#include "vlcwrapper.h"
#include <QString>
#include <QTime>
#include <QDebug>
#include <QTimer>
#include <QFile>
#include <QUrl>
#include <util/util.h>

namespace LeechCraft
{
namespace Laure
{
	const char * const vlc_args[] = {
		 "-I", "dummy"         // Don't use any interface
         ,"--ignore-config"      // Don't use VLC's config
         ,"--verbose=-1"
         ,"--quiet"
	};

	namespace
	{
		void ListEventCallback (const libvlc_event_t *event, void *data)
		{
			VLCWrapper *wrapper = static_cast<VLCWrapper*> (data);
			switch (event->type)
			{
			case libvlc_MediaListPlayerNextItemSet:
				wrapper->handledHasPlayed ();
				wrapper->handleNextItemSet ();
				break;
			}
		}
	}

	QVariantMap MediaMeta::ToVariantMap () const
	{
		QVariantMap map;
		map ["Artist"] = Artist_;
		map ["Album"] = Album_;
		map ["Title"] = Title_;
		map ["Genre"] = Genre_;
		map ["Date"] = Date_;
		map ["TrackNumber"] = TrackNumber_;
		map ["Length"] = Length_;
		return map;
	}

	VLCWrapper::VLCWrapper (QObject* parent)
	: QObject (parent)
	, CurrentItem_ (-1)
	{
		Instance_ = libvlc_instance_ptr (libvlc_new (sizeof (vlc_args)
				/ sizeof (vlc_args[0]), vlc_args), libvlc_release);
		LPlayer_ = libvlc_media_list_player_ptr (libvlc_media_list_player_new (Instance_.get ()),
				libvlc_media_list_player_release);
		Player_ = libvlc_media_player_ptr (libvlc_media_player_new (Instance_.get ()),
				libvlc_media_player_release);
		List_ = libvlc_media_list_ptr (libvlc_media_list_new (Instance_.get ()),
				libvlc_media_list_release);

		libvlc_media_list_player_set_media_player (LPlayer_.get (), Player_.get ());
		libvlc_media_list_player_set_media_list (LPlayer_.get (), List_.get ());

		auto listEventManager = libvlc_media_list_player_event_manager (LPlayer_.get ());
		libvlc_event_attach (listEventManager, libvlc_MediaListPlayerNextItemSet,
				     ListEventCallback, this);
	}

	void VLCWrapper::handleNextItemSet ()
	{
		auto list = List_.get ();
		int index = libvlc_media_list_index_of_item (list,
				libvlc_media_player_get_media (Player_.get ()));

		const MediaMeta& meta = GetItemMeta (index);
		emit gotEntity (Util::MakeNotification ("Laure",
				tr ("%1 - %2").arg (meta.Artist_).arg (meta.Title_),
					PInfo_));

		CurrentItem_ = index;
		emit (itemPlayed (CurrentItem_));
		QTimer::singleShot (5000, this, SLOT (nowPlaying ()));
	}

	void VLCWrapper::handledHasPlayed ()
	{
	}

	MediaMeta VLCWrapper::GetItemMeta (int row, const QString& location) const
	{
		MediaMeta meta;
		auto m = libvlc_media_list_item_at_index (List_.get (), row);
		if (!m)
			return meta;

		if (!QUrl (location).scheme ().isEmpty ())
		{
			meta.Artist_ = tr ("Internet stream");
			meta.Title_ = location;
			return meta;
		}

		libvlc_media_parse (m);

		meta.Artist_ = libvlc_media_get_meta (m, libvlc_meta_Artist);
		meta.Album_ = libvlc_media_get_meta (m, libvlc_meta_Album);
		meta.Title_ = libvlc_media_get_meta (m, libvlc_meta_Title);
		meta.Genre_ = libvlc_media_get_meta (m, libvlc_meta_Genre);
		meta.Date_ = libvlc_media_get_meta (m, libvlc_meta_Date);

		meta.TrackNumber_ = QString (libvlc_media_get_meta (m,
						libvlc_meta_TrackNumber))
				.toInt ();
		return meta;
	}

	int VLCWrapper::RowCount () const
	{
		return libvlc_media_list_count (List_.get ());
	}

	int VLCWrapper::GetCurrentIndex () const
	{
		return CurrentItem_;
	}

	bool VLCWrapper::IsPlaying () const
	{
		return libvlc_media_list_player_is_playing (LPlayer_.get ());
	}

	int VLCWrapper::GetVolume () const
	{
		return libvlc_audio_get_volume (Player_.get ());
	}

	float VLCWrapper::GetMediaPosition () const
	{
		return IsPlaying () ?
				libvlc_media_player_get_position (Player_.get ()) :
				-1;
	}

	int VLCWrapper::GetTime () const
	{
		return libvlc_media_player_get_time (Player_.get ());
	}

	int VLCWrapper::GetLength () const
	{
		return libvlc_media_player_get_length (Player_.get ());
	}

	void VLCWrapper::setWindow (WId winId)
	{
		libvlc_media_player_t *m = Player_.get ();
		int time = libvlc_media_player_get_time (m);
		libvlc_media_player_stop (m);
#ifdef Q_OS_WIN32
		libvlc_media_player_set_hwnd(m, winId);
#else
		libvlc_media_player_set_xwindow (m, winId);
#endif
		libvlc_media_player_play (m);
		libvlc_media_player_set_time (m, time);
	}

	bool VLCWrapper::removeRow (int pos)
	{
		return !libvlc_media_list_remove_index (List_.get (), pos);
	}

	void VLCWrapper::addRow (const QString& location)
	{
		auto m = libvlc_media_new_path (Instance_.get (),
				location.toAscii ());

		if (!libvlc_media_list_add_media (List_.get (), m))
			emit itemAdded (GetItemMeta (RowCount () - 1, location), location);
		else
			libvlc_media_release (m);
	}

	void VLCWrapper::setPlaybackMode (PlaybackMode mode)
	{
		auto list = LPlayer_.get ();
		switch (mode)
		{
		case PlaybackModeDefault:
			libvlc_media_list_player_set_playback_mode (list,
					libvlc_playback_mode_default);
			break;
		case PlaybackModeLoop:
			libvlc_media_list_player_set_playback_mode (list,
					libvlc_playback_mode_loop);
			break;
		case PlaybackModeRepeat:
			libvlc_media_list_player_set_playback_mode (list,
					libvlc_playback_mode_repeat);
			break;
		}
	}

	void VLCWrapper::setMeta (libvlc_meta_t type, const QString& value, int index)
	{
		auto m = libvlc_media_list_item_at_index (List_.get (), index);
		libvlc_media_set_meta (m, type, value.toAscii ());
		libvlc_media_save_meta (m);
	}

	void VLCWrapper::playItem (int val)
	{
		const int count = RowCount ();
		if (val == count)
			CurrentItem_ = 0;
		else if (val == -1)
			CurrentItem_ = count - 1;
		else
			CurrentItem_ = val;

		libvlc_media_list_player_play_item_at_index (LPlayer_.get (),
				CurrentItem_);
	}

	void VLCWrapper::nowPlaying ()
	{
		MediaMeta meta = GetItemMeta (CurrentItem_);
		meta.Length_ = libvlc_media_player_get_length (Player_.get ())
				/ 1000;

		Entity scrobbleEntity;
		scrobbleEntity.Additional_ = meta.ToVariantMap ();
		scrobbleEntity.Mime_ = "x-leechcraft/now-playing-track-info";
		emit gotEntity (scrobbleEntity);
	}

	void VLCWrapper::stop ()
	{
		libvlc_media_player_stop (Player_.get ());
	}

	void VLCWrapper::pause ()
	{
		libvlc_media_player_pause (Player_.get ());
	}

	void VLCWrapper::play ()
	{
		libvlc_media_player_play (Player_.get ());
		if (!IsPlaying ())
			emit paused ();
	}

	void VLCWrapper::next ()
	{
		libvlc_media_list_player_next (LPlayer_.get ());
	}

	void VLCWrapper::prev ()
	{
		libvlc_media_list_player_previous (LPlayer_.get ());
	}

	void VLCWrapper::setVolume (int vol)
	{
		libvlc_audio_set_volume (Player_.get (), vol);
	}

	void VLCWrapper::setPosition (float pos)
	{
		libvlc_media_player_set_position (Player_.get (), pos);
	}
}
}
