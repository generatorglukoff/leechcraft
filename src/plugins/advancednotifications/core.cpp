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

#include "core.h"
#include "notificationruleswidget.h"

namespace LeechCraft
{
namespace AdvancedNotifications
{
	Core::Core ()
	: NRW_ (0)
	{
	}
	
	Core& Core::Instance ()
	{
		static Core c;
		return c;
	}
	
	ICoreProxy_ptr Core::GetProxy () const
	{
		return Proxy_;
	}
	
	void Core::SetProxy (ICoreProxy_ptr proxy)
	{
		Proxy_ = proxy;
	}
	
	NotificationRulesWidget* Core::GetNRW ()
	{
		if (!NRW_)
			NRW_ = new NotificationRulesWidget;
		return NRW_;
	}
}
}