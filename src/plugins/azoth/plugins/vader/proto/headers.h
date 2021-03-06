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

#ifndef PLUGINS_AZOTH_PLUGINS_VADER_PROTO_HEADERS_H
#define PLUGINS_AZOTH_PLUGINS_VADER_PROTO_HEADERS_H
#include <QByteArray>

namespace LeechCraft
{
namespace Azoth
{
namespace Vader
{
namespace Proto
{
	struct Header
	{
		quint32 Magic_;					//< Magic
		quint32 Proto_;					//< Protocol version
		quint32 Seq_;					//< Sequence number
		quint32 MsgType_;				//< Packet type
		quint32 DataLength_;			//< Data length
		quint32 From_;					//< Sender's address
		quint32 FromPort_;				//< Sender's port
		unsigned char Reserved_ [16];	//< Reserved

		explicit Header (QByteArray&);
		Header (quint32 msgType = 0, quint32 seq = 0);
		QByteArray Serialize () const;
	};

	namespace Packets
	{
		const int Hello = 0x1001;
		const int HelloAck = 0x1002;
		const int LoginAck = 0x1004;
		const int LoginRej = 0x1005;
		const int Ping = 0x1006;
		const int Msg = 0x1008;
		const int MsgAck = 0x1009;
		const int MsgRecv = 0x1011;
		const int MsgStatus = 0x1012;
		const int UserStatus = 0x100F;
		const int Logout = 0x1013;
		const int ConnParams = 0x1014;
		const int UserInfo = 0x1015;
		const int Contact = 0x1019;
		const int ContactAck = 0x101A;
		const int ModifyContact = 0x101B;
		const int ModifyContactAck = 0x101C;
		const int OfflineMsgAck = 0x101D;
		const int DeleteOfflineMsg = 0x101E;
		const int Authorize = 0x1020;
		const int AuthorizeAck = 0x1021;
		const int ChangeStatus = 0x1022;
		const int GetMPOPSession = 0x1024;
		const int MPOPSession = 0x1025;
		const int WPRequest = 0x1029;
		const int AnketaInfo = 0x1028;
		const int MailboxStatus = 0x1033;
		const int ContactList2 = 0x1037;
		const int Login2 = 0x1038;
		const int SMS = 0x1039;
		const int SMSAck = 0x1040;
		const int NewMail = 0x1048;
		const int MicroblogRecv = 0x1063;
		const int MicroblogPost = 0x1064;
	}

	enum MsgFlag
	{
		Offline = 0x00000001,
		NoRecv = 0x00000004,
		Authorize = 0x00000008,
		System = 0x00000040,
		RTF = 0x00000080,
		Contact = 0x00000200,
		Notify = 0x00000400,
		SMS = 0x00000800,
		Multicast = 0x00001000,
		Alarm = 0x00004000,
		CP1251 = 0x00200000,
		Multichat = 0x00400000
	};

	Q_DECLARE_FLAGS (MsgFlags, MsgFlag);
	Q_DECLARE_OPERATORS_FOR_FLAGS (LeechCraft::Azoth::Vader::Proto::MsgFlags);

	const quint32 MsgUserFlagMask = 0x000036A8;

	namespace MessageStatus
	{
		const quint16 Delivered = 0x0000;
		const quint16 RejNoUser = 0x8001;
		const quint16 RejIntErr = 0x8003;
		const quint16 RejLimitExceeded = 0x8004;
		const quint16 RejTooLarge = 0x8005;
		const quint16 RejDenyOffline = 0x8006;
	}

	namespace SMSStatus
	{
		const int OK = 0x01;
		const int ServUnavail = 0x02;
		const int InvalidParams = 0x10000;
	}

	namespace UserState
	{
		const quint32 Offline = 0x00000000;
		const quint32 Online = 0x00000001;
		const quint32 Away = 0x00000002;
		const quint32 Undeterm = 0x00000003;
		const quint32 FlagInvisible = 0x80000000;
	}

	namespace LogoutFlags
	{
		const quint16 NoRelogin = 0x0010;
	}

	enum ContactOpFlag
	{
		None = 0x00000000,
		Removed = 0x00000001,
		Group = 0x00000002,
		Invisible = 0x00000004,
		Visible = 0x00000008,
		Ignore = 0x00000010,
		Shadow = 0x00000020
	};

	Q_DECLARE_FLAGS (ContactOpFlags, ContactOpFlag);
	Q_DECLARE_OPERATORS_FOR_FLAGS (ContactOpFlags);

	namespace ContactAck
	{
		const quint16 Success = 0x0000;
		const quint16 Error = 0x0001;
		const quint16 IntErr = 0x0002;
		const quint16 NoSuchUser = 0x0003;
		const quint16 InvalidInfo = 0x0004;
		const quint16 UserExists = 0x0005;
		const quint8 GroupLimit = 0x6;
	}

	namespace MPOPSession
	{
		const quint8 Fail = 0x00;
		const quint8 Success = 0x01;
	}

	enum WPParams
	{
		User,
		Domain,
		Nickname,
		Firstname,
		Lastname,
		Sex,
		Birthday,
		Date1,
		Date2,
		Online,
		Status,
		CityID,
		Zodiac,
		BirthdayMonth,
		BirthdayDay,
		CountryID,
		MAX
	};

	namespace AnketaInfoStatus
	{
		const quint8 OK = 0x01;
		const quint8 NoUser = 0x00;
		const quint8 DBErr = 0x02;
		const quint8 RateLimit = 0x03;
	}

	namespace CLResponse
	{
		const quint16 OK = 0x0000;
		const quint16 Error = 0x0001;
		const quint16 IntErr = 0x0002;
	}

	enum FeatureFlag
	{
		RTFMessages = 0x0001,
		BaseSmiles = 0x0002,
		AdvancedSmiles = 0x0004,
		ContactsExch = 0x0008,
		Wakeup = 0x0010
	};

	enum BlogStatus
	{
		UpdateBlog = 0x01,
		Music = 0x02,
		BlogReply = 0x04,
		BlogNotify = 0x10
	};

	Q_DECLARE_FLAGS (FeatureFlags, FeatureFlag);
	Q_DECLARE_OPERATORS_FOR_FLAGS (FeatureFlags);
}
}
}
}

#endif
