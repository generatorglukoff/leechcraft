/**********************************************************************
 * LeechCraft - modular cross-platform feature rich internet client.
 * Copyright (C) 2011  Minh Ngo
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

#include "lastfmsubmitter.h"
#include <QCryptographicHash>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <lastfm/Track>
#include <lastfm.h>

namespace LeechCraft
{
namespace Lastfmscrobble
{
	const QString ScrobblingSite_ = "http://ws.audioscrobbler.com/2.0/";
	
	MediaMeta::MediaMeta (const QMap<QString, QVariant>& tagMap)
	: Artist_ (tagMap ["Artist"].toString ())
	, Album_ (tagMap ["Album"].toString ())
	, Title_ (tagMap ["Title"].toString ())
	, Genre_ (tagMap ["Genre"].toString ())
	, Date_ (tagMap ["Date"].toString ())
	, TrackNumber_ (tagMap ["TrackNumber"].toInt ())
	, Length_ (tagMap ["Length"].toInt ())
	{
	}
	
	namespace
	{
		QString AuthToken (const QString& username, const QString& password)
		{
			const QString& passHash = QCryptographicHash::hash (password.toAscii (),
					QCryptographicHash::Md5).toHex ();
			return QCryptographicHash::hash ((username + passHash).toAscii (),
					QCryptographicHash::Md5).toHex ();
		}
		
		QString ApiSig (const QString& api_key, const QString& authToken,
				const QString& method, const QString& username,
				const QString& secret)
		{
			const QString& str = QString ("api_key%1authToken%2method%3username%4%5")
					.arg (api_key)
					.arg (authToken)
					.arg (method)
					.arg (username)
					.arg (secret);
			return QCryptographicHash::hash (str.toAscii (),
					QCryptographicHash::Md5).toHex ();
		}
	}

	bool LastFMSubmitter::IsConnected () const
	{
		return static_cast<bool> (Scrobbler_);
	}
	
	void LastFMSubmitter::SetPassword (const QString& password)
	{
		Password_ = password;
	}
	
	void LastFMSubmitter::SetUsername (const QString& username)
	{
		lastfm::ws::Username = username;
	}

	LastFMSubmitter::LastFMSubmitter (QObject *parent)
	: QObject (parent)
	{
		lastfm::ws::ApiKey = "be076efd1c241366f27fde6fd024e567";
		lastfm::ws::SharedSecret = "8352aead3be59ab319cd4e578d374843";
	}
	
	void LastFMSubmitter::Init (QNetworkAccessManager *manager)
	{
		const QString& authToken = AuthToken (lastfm::ws::Username,
				Password_);
		
		const QString& api_sig = ApiSig (lastfm::ws::ApiKey, authToken,
				"auth.getMobileSession", lastfm::ws::Username,
				lastfm::ws::SharedSecret);
		const QString& url = QString ("%1?method=%2&username=%3&authToken=%4&api_key=%5&api_sig=%6")
				.arg (ScrobblingSite_)
				.arg ("auth.getMobileSession")
				.arg (lastfm::ws::Username)
				.arg (authToken)
				.arg (lastfm::ws::ApiKey)
				.arg (api_sig);

		QNetworkReply *reply = manager->get (QNetworkRequest (QUrl (url)));
		connect (reply,
				SIGNAL (finished ()),
				this,
				SLOT (getSessionKey ()));
	}
		
	void LastFMSubmitter::getSessionKey ()
	{
		QNetworkReply *reply = qobject_cast<QNetworkReply*> (sender ());
		
		if (!reply)
			return;
		
		QDomDocument doc;
		doc.setContent (QString::fromUtf8 (reply->readAll ()));
		reply->deleteLater ();
		const auto& domList = doc.documentElement ()
				.elementsByTagName ("key");
		if (!domList.size ())
			return;
		
		lastfm::ws::SessionKey = domList.at (0).toElement ().text ();
			
		Scrobbler_.reset (new lastfm::Audioscrobbler ("tst"));
		
		connect (Scrobbler_.get (),
				SIGNAL (status (int)),
				this,
				SIGNAL (status (int)));
	}
		
	void LastFMSubmitter::sendTrack (const MediaMeta& info)
	{
		if (!IsConnected ())
			return;
		
		Scrobbler_->submit ();
		lastfm::Track track;
		lastfm::MutableTrack mutableTrack (track);
		mutableTrack.setTitle (info.Title_);
		mutableTrack.setAlbum (info.Album_);
		mutableTrack.setArtist (info.Artist_);
		mutableTrack.stamp ();
		mutableTrack.setSource (lastfm::Track::Player);
		mutableTrack.setDuration (info.Length_);
		mutableTrack.setTrackNumber (info.TrackNumber_);
		Scrobbler_->nowPlaying (track);
		Scrobbler_->cache (track);
	}
	
	void LastFMSubmitter::submit ()
	{
		if (!IsConnected ())
			return;
		
		Scrobbler_->submit ();
	}
}
}
