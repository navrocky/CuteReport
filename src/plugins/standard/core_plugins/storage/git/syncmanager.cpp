/***************************************************************************
 *   This file is part of the CuteReport project                           *
 *   Copyright (C) 2012-2014 by Alexander Mikhalov                         *
 *   alexander.mikhalov@gmail.com                                          *
 *                                                                         *
 **                   GNU General Public License Usage                    **
 *                                                                         *
 *   This library is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation, either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 *                                                                         *
 **                  GNU Lesser General Public License                    **
 *                                                                         *
 *   This library is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License as        *
 *   published by the Free Software Foundation, either version 3 of the    *
 *   License, or (at your option) any later version.                       *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library.                                      *
 *   If not, see <http://www.gnu.org/licenses/>.                           *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 ****************************************************************************/
#include "syncmanager.h"
#include <QtCore>

SyncManager::SyncManager(const QString &gitPath, QObject *parent) :
    QObject(parent),
    m_gitPath(gitPath)
{
}


void SyncManager::setGitPath(const QString &gitPath)
{
    QMutexLocker locker(&m_mutex);
    m_gitPath = gitPath;
}


bool SyncManager::isActive()
{
    QMutexLocker locker(&m_mutex);
    return m_isActive;
}


void SyncManager::setActive(bool b)
{
    QMutexLocker locker(&m_mutex);
    m_isActive = b;
}


bool SyncManager::checkBinary(const QString & gitPath, QString * version)
{
    QProcess git;
    git.start(gitPath, QStringList() << "--version");
    if (!git.waitForStarted()) {
        return false;
    }

    if (!git.waitForFinished()) {
        return false;
    }

    QString output = git.readAll();

    bool result = output.contains(QRegExp("git\\s+version"));
    if (version) {
        QRegExp rx("git\\s+version\\s+(.*)");
        int pos = rx.indexIn(output);
        if (pos != -1) {
             *version = rx.cap(1);
        }
    }

    return result;
}


bool SyncManager::pull(const QString & localDir)
{
    setActive(true);

    QProcess git;
    git.setWorkingDirectory(localDir);
    git.start(m_gitPath, QStringList() << "pull");
    if (!git.waitForStarted()) {
        setActive(false);
        return false;
    }

    if (!git.waitForFinished()) {
        setActive(false);
        return false;
    }

    QTextCodec * codec = QTextCodec::codecForName("UTF-8");

    m_output = codec->toUnicode(git.readAll());
    m_stdError = codec->toUnicode(git.readAllStandardError());
    m_stdOutput = codec->toUnicode(git.readAllStandardOutput());

    setActive(false);
    return !m_output.isEmpty();
}


bool SyncManager::push(const QString & localDir)
{
    setActive(true);

    QProcess git;
    git.setWorkingDirectory(localDir);
    git.start(m_gitPath, QStringList() << "push");
    if (!git.waitForStarted()) {
        setActive(false);
        return false;
    }

    if (!git.waitForFinished()) {
        setActive(false);
        return false;
    }

    m_output = git.readAll();
    m_stdError = git.readAllStandardError();
    m_stdOutput = git.readAllStandardOutput();
    setActive(false);

    return !m_output.isEmpty();
}


bool SyncManager::isLocalRepoExists(const QString & localDir)
{
    setActive(true);

    QDir dir(localDir);
    if (!dir.exists()) {
        setActive(false);
        return false;
    }

    QProcess git;
    git.setWorkingDirectory(localDir);
    git.start(m_gitPath, QStringList() << "status" << localDir);
    if (!git.waitForStarted()) {
        setActive(false);
        return false;
    }

    if (!git.waitForFinished()) {
        setActive(false);
        return false;
    }

    QTextCodec * codec = QTextCodec::codecForName("UTF-8");

    m_output = codec->toUnicode(git.readAll());
    m_stdError = codec->toUnicode(git.readAllStandardError());
    m_stdOutput = codec->toUnicode(git.readAllStandardOutput());

    setActive(true);

    return !m_stdError.contains("fatal");
}


bool SyncManager::clone(const QString &remoteUrl, const QString & localDir )
{
    setActive(true);

    QProcess git;
    git.setWorkingDirectory(localDir);
    git.start(m_gitPath, QStringList()  << "clone" << remoteUrl << localDir);
    if (!git.waitForStarted() || !git.waitForFinished()) {
        setActive(false);
        return false;
    }

    QTextCodec * codec = QTextCodec::codecForName("UTF-8");

    m_output = codec->toUnicode(git.readAll());
    m_stdError = codec->toUnicode(git.readAllStandardError());
    m_stdOutput = codec->toUnicode(git.readAllStandardOutput());

    setActive(false);
    return !m_output.isEmpty();
}


QString SyncManager::stdOutput() const
{
    return m_stdOutput;
}


QString SyncManager::stdError() const
{
    return m_stdError;
}


QString SyncManager::output() const
{
    return m_output;
}


QString SyncManager::fullOutput() const
{
    return m_output + m_stdOutput + m_stdError;
}
