/*
 * Copyright (C) 2017 Ryan C. Scott <ryan@5pmCasual.com>
 */

#include <QApplication>
#include <QDebug>
#include <QUrl>
#include <QHostInfo>
#include <QProcess>
#include "core/Group.h"
#include "core/Metadata.h"
#include "core/Entry.h"
#include "gui/DatabaseTabWidget.h"

#include "SshKeys.h"

SshKeys::SshKeys(DatabaseTabWidget *parent) : QObject(parent)
{
    // Connect to signals for database
    // TODO:  Connect for modification messages
    connect(parent, SIGNAL(databaseUnlocked(DatabaseWidget*)),
            this, SLOT(onUnlock(DatabaseWidget*)));

    connect(parent, SIGNAL(databaseLocked(DatabaseWidget*)),
            this, SLOT(onLock(DatabaseWidget*)));

    connect(parent, SIGNAL(databaseWithFileClosed(QString)),
            this, SLOT(onClose(QString)));
}

SshKeys::~SshKeys()
{
}

void SshKeys::onUnlock(DatabaseWidget *dbWidget)
{
    qDebug() << "UNLOCKED" << dbWidget->database()->metadata()->name();

    if (Database *db = dbWidget->database()) {
        addKeys(db);
    }
}

void SshKeys::onLock(DatabaseWidget *dbWidget)
{
    qDebug() << "LOCKED" << dbWidget->database()->metadata()->name();

    removeAllKeys();
}

void SshKeys::onClose(QString path)
{
    qDebug() << "CLOSED:" << path;

    removeAllKeys();
}

void SshKeys::addKey(Entry *entry, QString hostname, QProcessEnvironment &env)
{
    auto url = QUrl(entry->url());
    
    if (url.scheme() != "ssh" || url.path() == "")
        return;

    {
        auto hostnameNoTLD = hostname.left(hostname.lastIndexOf('.'));
        auto entryNoTLD = url.host().left(url.host().lastIndexOf('.'));
        
        if (QString::compare(hostnameNoTLD, entryNoTLD, Qt::CaseInsensitive) != 0)
            return;
    }
    
    qDebug() << "SSH: Adding key:" << url.path();

    QProcess add;
    add.setProcessEnvironment(env);
    add.start("ssh-add", QStringList() << url.path());
    add.waitForStarted();
    add.write(entry->password().toUtf8());
    add.write("\n");
    add.closeWriteChannel();
    add.waitForFinished();

    // Add to a list of keys so we're not guessing at the removal
    m_addedKeys << url.path();
}

void SshKeys::addKeys(Database *db)
{
    auto hostname = QHostInfo::localHostName();
    auto env = QProcessEnvironment::systemEnvironment();

    env.insert("SSH_ASKPASS", QCoreApplication::applicationDirPath() + "/ssh-ask-pipe");

    for (auto entry : db->rootGroup()->entriesRecursive()) {
        addKey(entry, hostname, env);
    }
}

void SshKeys::removeAllKeys()
{
    for (auto path : m_addedKeys) {
        QProcess remove;
        remove.start("ssh-add", QStringList() << "-d" << path);
        remove.waitForFinished();
    }

    m_addedKeys.clear();
}
