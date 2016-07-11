/*-------------------------------------------------------------------
Name         logfile.cpp

Purpose      Handles log file output;

History		 11 Jun 16  AFB	Created
-----------------------------------------------------------------*/

#ifndef LOGFILE_H
#define LOGFILE_H

#include <QObject> // ISA - QObject;
#include <QFile>
#include <QDateTime>
#include <QDebug>

class LogFile : public QObject
{
    Q_OBJECT
public:
    explicit LogFile(QObject *parent = 0);
    ~LogFile();

    // Sets the name of the log file and opens the QFile object;
    void setNameAndOpen(const QString& filename);

    // Appends a QString to the log file;
    void append(const QString& str);

    // Appends a std::string to the log file;
    void append(const std::string& str);

private:
    QFile* mFile;

};

#endif // LOGFILE_H
