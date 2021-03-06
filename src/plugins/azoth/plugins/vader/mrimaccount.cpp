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

#include "mrimaccount.h"
#include <QDataStream>
#include <QUrl>
#include <util/util.h>
#include <interfaces/iproxyobject.h>
#include "proto/connection.h"
#include "proto/message.h"
#include "mrimprotocol.h"
#include "mrimaccountconfigwidget.h"
#include "mrimbuddy.h"
#include "mrimmessage.h"
#include "core.h"
#include "groupmanager.h"
#include "vaderutil.h"

namespace LeechCraft
{
namespace Azoth
{
namespace Vader
{
	MRIMAccount::MRIMAccount (const QString& name, MRIMProtocol *proto)
	: QObject (proto)
	, Proto_ (proto)
	, Name_ (name)
	, Conn_ (new Proto::Connection (this))
	, GM_ (new GroupManager (this))
	{
		connect (Conn_,
				SIGNAL (gotContacts (QList<Proto::ContactInfo>)),
				this,
				SLOT (handleGotContacts (QList<Proto::ContactInfo>)));
		connect (Conn_,
				SIGNAL (userStatusChanged (Proto::ContactInfo)),
				this,
				SLOT (handleUserStatusChanged (Proto::ContactInfo)));
		connect (Conn_,
				SIGNAL (gotAuthRequest (QString, QString)),
				this,
				SLOT (handleGotAuthRequest (QString, QString)));
		connect (Conn_,
				SIGNAL (gotAuthAck (QString)),
				this,
				SLOT (handleGotAuthAck (QString)));
		connect (Conn_,
				SIGNAL (gotMessage (Proto::Message)),
				this,
				SLOT (handleGotMessage (Proto::Message)));
		connect (Conn_,
				SIGNAL (gotOfflineMessage (Proto::Message)),
				this,
				SLOT (handleGotMessage (Proto::Message)));
		connect (Conn_,
				SIGNAL (gotAttentionRequest (QString, QString)),
				this,
				SLOT (handleGotAttentionRequest (QString, QString)));
		connect (Conn_,
				SIGNAL (statusChanged (EntryStatus)),
				this,
				SLOT (handleOurStatusChanged (EntryStatus)));
		connect (Conn_,
				SIGNAL (contactAdded (quint32, quint32)),
				this,
				SLOT (handleContactAdded (quint32, quint32)));
		connect (Conn_,
				SIGNAL (gotUserTune (QString, QString)),
				this,
				SLOT (handleGotUserTune (QString, QString)));
		connect (Conn_,
				SIGNAL (userStartedTyping (QString)),
				this,
				SLOT (handleUserStartedTyping (QString)));
		connect (Conn_,
				SIGNAL (userStoppedTyping (QString)),
				this,
				SLOT (handleUserStoppedTyping (QString)));
		connect (Conn_,
				SIGNAL (gotNewMail (QString, QString)),
				this,
				SLOT (handleGotNewMail (QString, QString)));
		connect (Conn_,
				SIGNAL (gotPOPKey (QString)),
				this,
				SLOT (handleGotPOPKey (QString)));

		QAction *mb = new QAction (tr ("Open mailbox..."), this);
		connect (mb,
				SIGNAL (triggered ()),
				this,
				SLOT (handleOpenMailbox ()));
		Actions_ << mb;
		Actions_ << VaderUtil::GetBuddyServices (this, SLOT (handleServices ()));

		const QString& ua = "LeechCraft Azoth " + Core::Instance ()
				.GetCoreProxy ()->GetVersion ();
		Conn_->SetUA (ua);
	}

	void MRIMAccount::FillConfig (MRIMAccountConfigWidget *w)
	{
		Login_ = w->GetLogin ();

		const QString& pass = w->GetPassword ();
		if (!pass.isEmpty ())
			Core::Instance ().GetProxy ()->SetPassword (pass, this);
	}

	Proto::Connection* MRIMAccount::GetConnection () const
	{
		return Conn_;
	}

	GroupManager* MRIMAccount::GetGroupManager () const
	{
		return GM_;
	}

	void MRIMAccount::SetTypingState (const QString& to, ChatPartState state)
	{
		Conn_->SetTypingState (to, state == CPSComposing);
	}

	QObject* MRIMAccount::GetObject ()
	{
		return this;
	}

	QObject* MRIMAccount::GetParentProtocol () const
	{
		return Proto_;
	}

	IAccount::AccountFeatures MRIMAccount::GetAccountFeatures () const
	{
		return FHasConfigurationDialog;
	}

	QList<QObject*> MRIMAccount::GetCLEntries ()
	{
		QList<QObject*> result;
		Q_FOREACH (auto b, Buddies_.values ())
			result << b;
		return result;
	}

	QString MRIMAccount::GetAccountName () const
	{
		return Name_;
	}

	QString MRIMAccount::GetOurNick () const
	{
		return Login_.split ('@', QString::SkipEmptyParts).value (0);
	}

	void MRIMAccount::RenameAccount (const QString& name)
	{
		Name_ = name;
	}

	QByteArray MRIMAccount::GetAccountID () const
	{
		return Proto_->GetProtocolID () + "_" + Login_.toUtf8 ();
	}

	QList<QAction*> MRIMAccount::GetActions () const
	{
		return Actions_;
	}

	void MRIMAccount::QueryInfo (const QString&)
	{
	}

	void MRIMAccount::OpenConfigurationDialog ()
	{
	}

	EntryStatus MRIMAccount::GetState () const
	{
		return Status_;
	}

	void MRIMAccount::ChangeState (const EntryStatus& status)
	{
		if (!Conn_->IsConnected ())
		{
			const QString& pass = Core::Instance ().GetProxy ()->GetAccountPassword (this);
			Conn_->SetCredentials (Login_, pass);
		}

		Conn_->SetState (status);
	}

	void MRIMAccount::Synchronize ()
	{
	}

	void MRIMAccount::Authorize (QObject *obj)
	{
		qDebug () << Q_FUNC_INFO << GetAccountName ();
		MRIMBuddy *buddy = qobject_cast<MRIMBuddy*> (obj);
		if (!buddy)
		{
			qWarning () << Q_FUNC_INFO
					<< "wrong object"
					<< obj;
			return;
		}

		const QString& id = buddy->GetHumanReadableID ();
		Conn_->Authorize (id);

		buddy->SetAuthorized (true);

		if (!Buddies_.contains (id))
		{
			Buddies_ [id] = buddy;
			Conn_->AddContact (0, id, buddy->GetEntryName ());
			Conn_->RequestAuth (id, QString ());
		}
	}

	void MRIMAccount::DenyAuth (QObject *obj)
	{
		qDebug () << Q_FUNC_INFO << GetAccountName ();
		MRIMBuddy *buddy = qobject_cast<MRIMBuddy*> (obj);
		if (!buddy)
		{
			qWarning () << Q_FUNC_INFO
					<< "wrong object"
					<< obj;
			return;
		}

		emit removedCLItems (QList<QObject*> () << buddy);
		Buddies_.value (buddy->GetHumanReadableID (), buddy)->deleteLater ();
		Buddies_.remove (buddy->GetHumanReadableID ());
	}

	void MRIMAccount::RequestAuth (const QString& email,
			const QString& msg, const QString& name, const QStringList& groups)
	{
		qDebug () << Q_FUNC_INFO << GetAccountName () << email;
		if (!Buddies_.contains (email))
		{
			const quint32 group = 0;
			const quint32 seqId = Conn_->AddContact (group, email, name);
			PendingAdditions_ [seqId] = { -1, group, Proto::UserState::Offline,
					email, name, QString (), QString (), 0, QString () };
			Conn_->Authorize (email);
		}
		else
			Conn_->RequestAuth (email, msg);
	}

	void MRIMAccount::RemoveEntry (QObject *obj)
	{
		MRIMBuddy *buddy = qobject_cast<MRIMBuddy*> (obj);
		if (!buddy)
		{
			qWarning () << Q_FUNC_INFO
					<< "wrong object"
					<< obj;
			return;
		}

		const qint64 id = buddy->GetID ();
		if (id < 0)
		{
			qWarning () << Q_FUNC_INFO
					<< "cannot remove buddy with negative ID";
			return;
		}

		Buddies_.take (buddy->GetHumanReadableID ())->deleteLater ();
		emit removedCLItems (QList<QObject*> () << buddy);

		Conn_->RemoveContact (id, buddy->GetHumanReadableID (), buddy->GetEntryName ());
	}

	QObject* MRIMAccount::GetTransferManager () const
	{
		return 0;
	}

	void MRIMAccount::PublishTune (const QMap<QString, QVariant>& data)
	{
		QString string;
		auto sa = [&string, &data] (const QString& name, const QString& sep)
			{
				if (!data [name].toString ().isEmpty ())
				{
					string += sep;
					string += data [name].toString ();
				}
			};
		string += data ["artist"].toString ();
		sa ("title", QString::fromUtf8 (" — "));
		sa ("source", " " + tr ("from") + " ");
		sa ("length", ", ");

		Conn_->PublishTune (string);
	}

	QByteArray MRIMAccount::Serialize () const
	{
		QByteArray result;
		QDataStream str (&result, QIODevice::WriteOnly);
		str << static_cast<quint8> (1)
			<< Name_
			<< Login_;

		return result;
	}

	MRIMAccount* MRIMAccount::Deserialize (const QByteArray& ba, MRIMProtocol *proto)
	{
		QDataStream str (ba);
		quint8 ver = 0;
		str >> ver;
		if (ver != 1)
		{
			qWarning () << Q_FUNC_INFO
					<< "unknown version"
					<< ver;
			return 0;
		}

		QString name;
		str >> name;
		MRIMAccount *result = new MRIMAccount (name, proto);
		str >> result->Login_;
		return result;
	}

	MRIMBuddy* MRIMAccount::GetBuddy (const Proto::ContactInfo& info)
	{
		if (Buddies_.contains (info.Email_))
			return Buddies_ [info.Email_];

		MRIMBuddy *buddy = new MRIMBuddy (info, this);
		Buddies_ [info.Email_] = buddy;
		emit gotCLItems (QList<QObject*> () << buddy);
		return buddy;
	}

	void MRIMAccount::handleGotContacts (const QList<Proto::ContactInfo>& contacts)
	{
		QList<QObject*> objs;
		Q_FOREACH (const Proto::ContactInfo& contact, contacts)
		{
			qDebug () << Q_FUNC_INFO << GetAccountName () << contact.Email_ << contact.Alias_ << contact.ContactID_ << contact.UA_ << contact.Features_;
			MRIMBuddy *buddy = new MRIMBuddy (contact, this);
			buddy->SetGroup (GM_->GetGroup (contact.GroupNumber_));
			objs << buddy;
			Buddies_ [contact.Email_] = buddy;
		}

		emit gotCLItems (objs);
	}

	void MRIMAccount::handleUserStatusChanged (const Proto::ContactInfo& status)
	{
		MRIMBuddy *buddy = Buddies_ [status.Email_];
		if (!buddy)
		{
			qWarning () << Q_FUNC_INFO
					<< GetAccountName ()
					<< "unknown buddy"
					<< status.Email_;
			return;
		}

		qDebug () << Q_FUNC_INFO << GetAccountName () << status.Email_;

		auto info = buddy->GetInfo ();
		info.Features_ = status.Features_;
		info.StatusDesc_ = status.StatusDesc_;
		info.StatusID_ = status.StatusID_;
		info.StatusTitle_ = status.StatusTitle_;
		info.UA_ = status.UA_;
		buddy->UpdateInfo (info);
	}

	void MRIMAccount::handleContactAdded (quint32 seq, quint32 id)
	{
		qDebug () << Q_FUNC_INFO << GetAccountName () << id;
		if (!PendingAdditions_.contains (seq))
			return;

		Proto::ContactInfo info = PendingAdditions_.take (seq);
		info.ContactID_ = id;

		handleGotContacts (QList<Proto::ContactInfo> () << info);
	}

	void MRIMAccount::handleGotAuthRequest (const QString& from, const QString& msg)
	{
		qDebug () << Q_FUNC_INFO << GetAccountName () << from;
		if (Buddies_.contains (from) &&
				Buddies_ [from]->IsAuthorized ())
			return;

		Proto::ContactInfo info = {-1, 0, Proto::UserState::Online,
				from, from, QString (), QString (), 0, QString () };

		MRIMBuddy *buddy = new MRIMBuddy (info, this);
		buddy->SetAuthorized (false);
		emit gotCLItems (QList<QObject*> () << buddy);
		emit authorizationRequested (buddy, msg);
	}

	void MRIMAccount::handleGotAuthAck (const QString& from)
	{
		qDebug () << Q_FUNC_INFO << GetAccountName () << from;
		if (!Buddies_.contains (from))
		{
			qWarning () << Q_FUNC_INFO
					<< "unknown buddy"
					<< from;
			return;
		}

		emit itemGrantedSubscription (Buddies_ [from], QString ());
	}

	void MRIMAccount::handleGotMessage (const Proto::Message& msg)
	{
		auto buddy = Buddies_ [msg.From_];
		if (!buddy)
		{
			qWarning () << Q_FUNC_INFO
					<< "incoming message from unknown buddy"
					<< msg.From_
					<< msg.Text_
					<< msg.DT_;
			return;
		}

		MRIMMessage *obj = new MRIMMessage (IMessage::DIn, IMessage::MTChatMessage, buddy);
		obj->SetBody (msg.Text_);
		obj->SetDateTime (msg.DT_);
		buddy->HandleMessage (obj);
	}

	void MRIMAccount::handleGotAttentionRequest (const QString& from, const QString& msg)
	{
		auto buddy = Buddies_ [from];
		if (!buddy)
		{
			qWarning () << Q_FUNC_INFO
					<< "from unknown buddy"
					<< from;
			return;
		}

		buddy->HandleAttention (msg);
	}

	void MRIMAccount::handleOurStatusChanged (const EntryStatus& status)
	{
		Status_ = status;
		emit statusChanged (status);
	}

	void MRIMAccount::handleGotUserTune (const QString& from, const QString& tune)
	{
		auto buddy = Buddies_ [from];
		if (!buddy)
		{
			qWarning () << Q_FUNC_INFO
					<< "from unknown buddy"
					<< from;
			return;
		}

		buddy->HandleTune (tune);
	}

	void MRIMAccount::handleUserStartedTyping (const QString& from)
	{
		auto buddy = Buddies_ [from];
		if (!buddy)
		{
			qWarning () << Q_FUNC_INFO
					<< "from unknown buddy"
					<< from;
			return;
		}

		buddy->HandleCPS (CPSComposing);
	}

	void MRIMAccount::handleUserStoppedTyping (const QString& from)
	{
		auto buddy = Buddies_ [from];
		if (!buddy)
		{
			qWarning () << Q_FUNC_INFO
					<< "from unknown buddy"
					<< from;
			return;
		}

		buddy->HandleCPS (CPSPaused);
	}

	void MRIMAccount::handleGotNewMail (const QString& from, const QString& subj)
	{
		const Entity& e = Util::MakeNotification (Login_,
				tr ("New mail from %1: %2.")
					.arg (from)
					.arg (subj),
				PInfo_);
		Core::Instance ().SendEntity (e);
	}

	void MRIMAccount::handleGotPOPKey (const QString& key)
	{
		qDebug () << Q_FUNC_INFO << key;
		const QString& str = QString ("http://win.mail.ru/cgi-bin/auth?Login=%1&agent=%2")
				.arg (Login_)
				.arg (key);
		const Entity& e = Util::MakeEntity (QUrl (str),
				QString (),
				static_cast<LeechCraft::TaskParameters> (OnlyHandle | FromUserInitiated));
		Core::Instance ().SendEntity (e);
	}

	void MRIMAccount::handleOpenMailbox ()
	{
		Conn_->RequestPOPKey ();
	}

	void MRIMAccount::handleServices ()
	{
		const QString& url = sender ()->property ("URL").toString ();
		const QString& subst = VaderUtil::SubstituteNameDomain (url, Login_);
		qDebug () << Q_FUNC_INFO << subst << url << Login_;
		const Entity& e = Util::MakeEntity (QUrl (subst),
				QString (),
				static_cast<LeechCraft::TaskParameters> (OnlyHandle | FromUserInitiated));
		Core::Instance ().SendEntity (e);
	}
}
}
}
