/*-----------------------------------------------------------------------------
Name         logfile.h

Purpose      Handles log file, low level, output;

History		 11 Jun 16  AFB	Created
-----------------------------------------------------------------------------*/

#ifndef LOGFILE_H
#define LOGFILE_H

#include <QObject> // ISA - QObject;
#include <QFile> // HASA - QFile object to stream data to;
#include <QDateTime> // USES - QDateTime to timestamp the file;
#include <QDebug>

class LogFile : public QObject
{
    Q_OBJECT
public:
    explicit LogFile(QObject *parent = 0); // Constructor;

    ~LogFile(); // Destructor;

    // Sets the name of the log file and opens the QFile object;
    void setNameAndOpen(const QString& filename);

    // Appends a QString to the log file;
    void append(const QString& str);

    // Appends a std::string to the log file;
    void append(const std::string& str);

private:
    // Object being written to;
    QFile* mFile;

};

#endif // LOGFILE_H
