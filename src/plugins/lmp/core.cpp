#include "core.h"
#include <QUrl>
#include <VideoWidget>

Core::Core ()
: TotalTimeAvailable_ (false)
, VideoWidget_ (0)
{
}

Core& Core::Instance ()
{
	static Core core;
	return core;
}

void Core::Release ()
{
	MediaObject_.reset ();
}

void Core::Reinitialize (const QString& entity)
{
	TotalTimeAvailable_ = false;

	MediaObject_.reset (new Phonon::MediaObject (this));
	MediaObject_->setTickInterval (100);
	connect (MediaObject_.get (),
			SIGNAL (totalTimeChanged (qint64)),
			this,
			SLOT (totalTimeChanged ()));
	connect (MediaObject_.get (),
			SIGNAL (tick (qint64)),
			this,
			SLOT (updateState ()));
	connect (MediaObject_.get (),
			SIGNAL (stateChanged (Phonon::State,
					Phonon::State)),
			this,
			SLOT (updateState ()));

	MediaObject_->setCurrentSource (entity);

	qreal oldVolume = 1;
	if (AudioOutput_.get ())
		oldVolume = AudioOutput_->volume ();
	AudioOutput_.reset (new Phonon::AudioOutput (Phonon::MusicCategory, this));
	AudioOutput_->setVolume (oldVolume);

	Phonon::createPath (Core::Instance ().GetMediaObject (),
			VideoWidget_);
	Phonon::createPath (Core::Instance ().GetMediaObject (),
			AudioOutput_.get ());
}

Phonon::MediaObject* Core::GetMediaObject () const
{
	return MediaObject_.get ();
}

void Core::SetVideoWidget (Phonon::VideoWidget *widget)
{
	VideoWidget_ = widget;
}

void Core::changeVolume (int volume)
{
	if (!AudioOutput_.get ())
		return;

	AudioOutput_->setVolume (static_cast<qreal> (volume) / 100);
}

void Core::play ()
{
	if (MediaObject_.get ())
		MediaObject_->play ();
}

void Core::pause ()
{
	if (MediaObject_.get ())
		MediaObject_->pause ();
}

void Core::setSource (const QString& filename)
{
	MediaObject_->setCurrentSource (filename);
}

void Core::updateState ()
{
	QString result;
	switch (MediaObject_->state ())
	{
		case Phonon::LoadingState:
			result = tr ("Initializing");
			break;
		case Phonon::StoppedState:
			result = tr ("Stopped");
			break;
		case Phonon::PlayingState:
			result = tr ("Playing");
			break;
		case Phonon::BufferingState:
			result = tr ("Buffering");
			break;
		case Phonon::PausedState:
			result = tr ("Paused");
			break;
		case Phonon::ErrorState:
			result = tr ("Error");
			break;
	}
	if (MediaObject_->state () == Phonon::ErrorState)
		result += tr (" (%1)").arg (MediaObject_->errorString ()); 
	result += tr (" [");
	result += QString::number (static_cast<double> (MediaObject_->
				currentTime ())/1000., 'f', 1);
	if (TotalTimeAvailable_)
	{
		result += tr ("/");
		result += QString::number (static_cast<double> (MediaObject_->
					totalTime ())/1000., 'f', 1);
	}
	result += tr ("]");

	result += tr (" borrowing data from ");
	Phonon::MediaSource source = MediaObject_->currentSource ();
	switch (source.type ())
	{
		case Phonon::MediaSource::Invalid:
			result += tr ("nowhere");
			break;
		case Phonon::MediaSource::LocalFile:
			result += source.fileName ();
			break;
		case Phonon::MediaSource::Url:
			result += source.url ().toString ();
			break;
		case Phonon::MediaSource::Disc:
			result += source.deviceName ();
			switch (source.discType ())
			{
				case Phonon::Cd:
					result += tr (" (CD)");
					break;
				case Phonon::Dvd:
					result += tr (" (DVD)");
					break;
				case Phonon::Vcd:
					result += tr (" (VCD)");
					break;
				default:
					result += tr (" (Unknown disc type)");
					break;
			}
			break;
		case Phonon::MediaSource::Stream:
			result += tr ("stream");
			break;
	}

	emit stateUpdated (result);
}

void Core::totalTimeChanged ()
{
	TotalTimeAvailable_ = true;
}

