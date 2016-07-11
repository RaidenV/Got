/*-------------------------------------------------------------------
Name         logfile.cpp

Purpose      Handles log file output;

History		 11 Jun 16  AFB	Created
-----------------------------------------------------------------*/

#include "logfile.h"

/*----------------------------------------------------------------------------

Name		LogFile

Purpose		Construct the log file object;

History		11 Jun 16  AFB	Created
----------------------------------------------------------------------------*/
LogFile::LogFile(QObject *parent) : QObject(parent)
{
    mFile = new QFile(this);
}

/*----------------------------------------------------------------------------

Name		~LogFile

Purpose		Log file destructor;

History		11 Jun 16  AFB	Created
----------------------------------------------------------------------------*/
LogFile::~LogFile()
{
}

/*----------------------------------------------------------------------------

Name		setNameAndOpen

Purpose		Set the file name and open/test for successful open;

Inputs      filename        QString of the file name to be used as the log;

History		11 Jun 16  AFB	Created
----------------------------------------------------------------------------*/
void LogFile::setNameAndOpen(const QString &filename)
{
    // Get date/time of file creation/opening;
    QString date;
    QDateTime dT = QDateTime::currentDateTime();
    date = dT.toString("ddd MMMM d yy");

    mFile->setFileName(filename);

    // Test for successful opening;
    if (!mFile->open
            (QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
    {
        qDebug() << "Error opening file";
    }

    // Output date/time to the log file;
    QTextStream out(mFile);
    out << "\n\r" << date;
    date = dT.toString("hh:mm:ss.zzz");
    out << '\r' << date << '\r';
}

/*----------------------------------------------------------------------------

Name		append

Purpose		Append a QString to the log file;

Inputs      str        QString of the data to be output to the file;

History		11 Jun 16  AFB	Created
----------------------------------------------------------------------------*/
void LogFile::append(const QString& str)
{
    QTextStream out(mFile);
    out << str;
}

/*----------------------------------------------------------------------------

Name		append

Purpose		Append a std::string to the log file;

Inputs      str        std::string of the data to be output to the file;

History		11 Jun 16  AFB	Created
----------------------------------------------------------------------------*/
void LogFile::append(const std::string& str)
{
    QTextStream out(mFile);
    // Qt classes only work with Qt type variables.  Convert the std::string
    // to a QString;
    QString qStr(str.c_str());
    out << qStr;
}

