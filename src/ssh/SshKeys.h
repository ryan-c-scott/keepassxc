/*
 * Copyright (C) 2017 Ryan C. Scott <ryan@5pmCasual.com>
 */

#pragma once

#include <QObject>
#include <QList>
#include <QProcess>

class Database;
class Entry;
class DatabaseTabWidget;
class DatabaseWidget;

class SshKeys : public QObject
{
    Q_OBJECT;

    QStringList m_addedKeys;
    
    void addKey(Entry *entry, QString hostname, QProcessEnvironment &env);
    void addKeys(Database *db);
    void removeAllKeys();

private Q_SLOTS:
    void onUnlock(DatabaseWidget *dbWidget);
    void onLock(DatabaseWidget *dbWidget);
    void onClose(QString path);
    
public:
    explicit SshKeys(DatabaseTabWidget *parent = 0);
    ~SshKeys();
};
